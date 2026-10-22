#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QStyleFactory>
#include <QTextCodec>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    memset(&m_stDevList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));
    m_pcMyCamera = NULL;
    m_bGrabbing = false;
    m_hWnd = (void*)ui->widgetDisplay->winId();
    memset(&m_stImageInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));
    m_hGrabThread = NULL;
    m_bThreadState = false;
    m_bOpenDevice = false;
    m_bOpenLens = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ch:显示错误信息 | en:Show error message
void MainWindow::ShowErrorMsg(QString csMessage, unsigned int nErrorNum)
{
    QString errorMsg = csMessage;
    if (nErrorNum != 0)
    {
        QString TempMsg;
        TempMsg.sprintf(": Error = %x: ", nErrorNum);
        errorMsg += TempMsg;
    }

    switch(nErrorNum)
    {
    case MV_E_HANDLE:           errorMsg += "Error or invalid handle ";                                         break;
    case MV_E_SUPPORT:          errorMsg += "Not supported function ";                                          break;
    case MV_E_BUFOVER:          errorMsg += "Cache is full ";                                                   break;
    case MV_E_CALLORDER:        errorMsg += "Function calling order error ";                                    break;
    case MV_E_PARAMETER:        errorMsg += "Incorrect parameter ";                                             break;
    case MV_E_RESOURCE:         errorMsg += "Applying resource failed ";                                        break;
    case MV_E_NODATA:           errorMsg += "No data ";                                                         break;
    case MV_E_PRECONDITION:     errorMsg += "Precondition error, or running environment changed ";              break;
    case MV_E_VERSION:          errorMsg += "Version mismatches ";                                              break;
    case MV_E_NOENOUGH_BUF:     errorMsg += "Insufficient memory ";                                             break;
    case MV_E_ABNORMAL_IMAGE:   errorMsg += "Abnormal image, maybe incomplete image because of lost packet ";   break;
    case MV_E_UNKNOW:           errorMsg += "Unknown error ";                                                   break;
    case MV_E_GC_GENERIC:       errorMsg += "General error ";                                                   break;
    case MV_E_GC_ACCESS:        errorMsg += "Node accessing condition error ";                                  break;
    case MV_E_ACCESS_DENIED:	errorMsg += "No permission ";                                                   break;
    case MV_E_BUSY:             errorMsg += "Device is busy, or network disconnected ";                         break;
    case MV_E_NETER:            errorMsg += "Network error ";                                                   break;
    case MV_E_LIQUIDLENS_CMD_NOT_SUPPORT:          errorMsg += "Liquid lens Unsupported command ";              break;
    case MV_E_LIQUIDLENS_COMMANDTIMEOUT:           errorMsg += "Liquid lens command timeout ";                  break;
    case MV_E_LIQUIDLENS_OFFLINE:                  errorMsg += "Liquid lens offline ";                          break;
    case MV_E_LIQUIDLENS_CHECKSUM_ERROR:           errorMsg += "Liquid lens checksum error ";                   break;
    case MV_E_LIQUIDLENS_REGISTER_NOT_EXIST:       errorMsg += "Liquid lens register does not exist ";          break;
    case MV_E_LIQUIDLENS_PERMISSION_DENIED:        errorMsg += "Liquid lens permission denied ";                break;
    case MV_E_LIQUIDLENS_PACKET_FORMAT_ERROR:      errorMsg += "Liquid lens packet format error ";              break;
    case MV_E_LIQUIDLENS_DATA_FOAMAT_ERROR:        errorMsg += "Liquid lens data format error ";                break;
    case MV_E_LIQUIDLENS_DATA_OUT_RANGE:           errorMsg += "Liquid lens data out of range ";                break;
    case MV_E_LIQUIDLENS_WRITE_DATA_LENGTH_ERROR:  errorMsg += "Liquid lens Write data length does not match register length ";   break;
    case MV_E_LIQUIDLENS_AF_IMAGE_ABNORMAL:     errorMsg += "Liquid lens Image abnormal during focusing ";      break;
    case MV_E_LIQUIDLENS_DEVICE_BUSY:           errorMsg += "Liquid lens device busy ";                         break;
    case MV_E_LIQUIDLENS_DATA_INCORRECT_ORDER:  errorMsg += "Liquid lens data incorrect order ";                break;
    case MV_E_LIQUIDLENS_RUN_COND_NOT_MET:      errorMsg += "Liquid lens run condition not met ";               break;
    case MV_E_LIQUIDLENS_ACK_DATA_LENGTH_ERROR: errorMsg += "Liquid lens Serial port response data length error ";  break;
    case MV_E_LIQUIDLENS_TRIGGER_MODE_NOT_OPEN: errorMsg += "Liquid lens Trigger mode not opened ";             break;
    case MV_E_LIQUIDLENS_NOT_SOFT_TRIGGER_MODE: errorMsg += "english Not soft trigger mode";                    break;
    case MV_E_LIQUIDLENS_DEVICE_NOT_GRABBING:   errorMsg += " Device not in grabbing state";                    break;
    case MV_E_LIQUIDLENS_STRATEGY_NOT_ONEBYONE: errorMsg += "Stream strategy not supported";                    break;
    case MV_E_LIQUIDLENS_AF_IMAGE_LOST:         errorMsg += "Image missing or abnormal count during focusing";  break;
    case MV_E_LIQUIDLENS_AF_NOT_CONVERGED:      errorMsg += "Reached maximum iteration count during focusing but convergence condition not met"; break;
    case MV_E_LIQUIDLENS_SERIAL_PORT_PARAMS_FAIL: errorMsg += "Serial port parameter configuration failed";     break;
    case MV_E_LIQUIDLENS_INIT_FAILED:           errorMsg += "Lens initialization failed";                       break;
    case MV_E_LIQUIDLENS_TASK_EXECUTING:        errorMsg += "Device is busy, a task is currently executing";    break;
    case MV_E_LIQUIDLENS_UNDEFINED_ERROR:       errorMsg += "Liquid lens undefined error ";                     break;
    }

    QMessageBox::information(NULL, "PROMPT", errorMsg);
}

// ch:取流线程 | en:Grabbing thread
void* GrabThread(void* pUser)
{
    if (pUser)
    {
        MainWindow* pCam = (MainWindow*)pUser;

        pCam->GrabThreadProcess();

        return 0;
    }

    return NULL;
}


void MainWindow::GrabThreadProcess()
{
    MV_FRAME_OUT stImageInfo;
    memset(&stImageInfo, 0, sizeof(MV_FRAME_OUT));

    MV_DISPLAY_FRAME_INFO stDisplayInfo;
    int nRet = MV_OK;

    while(m_bThreadState)
    {
        nRet = m_pcMyCamera->GetImageBuffer(&stImageInfo, 1000);
        if (nRet == MV_OK)
        {
            memset(&stDisplayInfo, 0, sizeof(MV_DISPLAY_FRAME_INFO));

            stDisplayInfo.hWnd = m_hWnd;
            stDisplayInfo.pData = stImageInfo.pBufAddr;
            stDisplayInfo.nDataLen = stImageInfo.stFrameInfo.nFrameLenEx;
            stDisplayInfo.nWidth = stImageInfo.stFrameInfo.nExtendWidth;
            stDisplayInfo.nHeight = stImageInfo.stFrameInfo.nExtendHeight;
            stDisplayInfo.enPixelType = stImageInfo.stFrameInfo.enPixelType;
            m_pcMyCamera->DisplayOneFrame(&stDisplayInfo);

            m_pcMyCamera->FreeImageBuffer(&stImageInfo);
        }
    }


   return ;

}

// ch:按钮使能 | en:Enable control
void MainWindow::EnableControls(bool bIsCameraReady)
{
    ui->bnOpen->setEnabled(m_bOpenDevice ? false : (bIsCameraReady ? true : false));
    ui->bnClose->setEnabled((m_bOpenDevice && bIsCameraReady) ? true : false);
    ui->bnOpenLiquidLens->setEnabled((m_bOpenLens && bIsCameraReady) ? false : (m_bOpenDevice ? true : false));
    ui->bnCloseLiquidLens->setEnabled(m_bOpenLens ? true : false);
    ui->bnExeAutoFocus->setEnabled(m_bOpenLens ? true : false);
    ui->bnSetFocalPower->setEnabled(m_bOpenLens ? true : false);

    ui->tbStartFocalPower->setEnabled(m_bOpenLens ? true : false);
    ui->tbEndFocalPower->setEnabled(m_bOpenLens ? true : false);
    ui->tbCoarseStep->setEnabled(m_bOpenLens ? true : false);
    ui->tblFocalPower->setEnabled(m_bOpenLens ? true : false);
    ui->bnRefresh->setEnabled(m_bOpenLens ? true : false);


}

void MainWindow::on_bnEnum_clicked()
{
    ui->ComboDevices->clear();
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
	ui->ComboDevices->setStyle(QStyleFactory::create("Windows"));
    // ch:枚举子网内所有设备 | en:Enumerate all devices within subnet
    memset(&m_stDevList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));
    int nRet = CMvCamera::EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE | MV_GENTL_CAMERALINK_DEVICE | MV_GENTL_CXP_DEVICE | MV_GENTL_XOF_DEVICE, &m_stDevList);
    if (MV_OK != nRet)
    {
        return;
    }

    // ch:将值加入到信息列表框中并显示出来 | en:Add value to the information list box and display
    for (unsigned int i = 0; i < m_stDevList.nDeviceNum; i++)
    {

        MV_CC_DEVICE_INFO* pDeviceInfo = m_stDevList.pDeviceInfo[i];
        if (NULL == pDeviceInfo)
        {
            continue;
        }
		char strUserName[256] = {0};
        if (pDeviceInfo->nTLayerType == MV_GIGE_DEVICE)
        {
            int nIp1 = ((m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0xff000000) >> 24);
            int nIp2 = ((m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0x00ff0000) >> 16);
            int nIp3 = ((m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0x0000ff00) >> 8);
            int nIp4 = (m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0x000000ff);

            if (strcmp("", (char*)pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName) != 0)
            {
                snprintf(strUserName, 256, "[%d]GigE:   %s (%s) (%d.%d.%d.%d)", i, pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName,
                         pDeviceInfo->SpecialInfo.stGigEInfo.chSerialNumber, nIp1, nIp2, nIp3, nIp4);
            }
            else
            {
                snprintf(strUserName, 256, "[%d]GigE:   %s (%s) (%d.%d.%d.%d)", i, pDeviceInfo->SpecialInfo.stGigEInfo.chModelName,
                         pDeviceInfo->SpecialInfo.stGigEInfo.chSerialNumber, nIp1, nIp2, nIp3, nIp4);
            }
        }
        else if (pDeviceInfo->nTLayerType == MV_USB_DEVICE)
        {
            if (strcmp("", (char*)pDeviceInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName) != 0)
            {
                snprintf(strUserName, 256, "[%d]UsbV3:  %s (%s)", i, pDeviceInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName,
                         pDeviceInfo->SpecialInfo.stUsb3VInfo.chSerialNumber);
            }
            else
            {
                snprintf(strUserName, 256, "[%d]UsbV3:  %s (%s)", i, pDeviceInfo->SpecialInfo.stUsb3VInfo.chModelName,
                         pDeviceInfo->SpecialInfo.stUsb3VInfo.chSerialNumber);
            }
        }
		else if (pDeviceInfo->nTLayerType == MV_GENTL_CAMERALINK_DEVICE)
        {
            if (strcmp("", (char*)pDeviceInfo->SpecialInfo.stCMLInfo.chUserDefinedName) != 0)
            {
                snprintf(strUserName, 256, "[%d]CML:  %s (%s)", i,pDeviceInfo->SpecialInfo.stCMLInfo.chUserDefinedName,
                    pDeviceInfo->SpecialInfo.stCMLInfo.chSerialNumber);
            }
            else
            {
                snprintf(strUserName, 256, "[%d]CML:  %s (%s)", i,pDeviceInfo->SpecialInfo.stCMLInfo.chModelName,
                    pDeviceInfo->SpecialInfo.stCMLInfo.chSerialNumber);
            }
        }
        else if (pDeviceInfo->nTLayerType == MV_GENTL_CXP_DEVICE)
        {
            if (strcmp("", (char*)pDeviceInfo->SpecialInfo.stCXPInfo.chUserDefinedName) != 0)
            {
                snprintf(strUserName, 256, "[%d]CXP:  %s (%s)", i,pDeviceInfo->SpecialInfo.stCXPInfo.chUserDefinedName,
                    pDeviceInfo->SpecialInfo.stCXPInfo.chSerialNumber);
            }
            else
            {
                snprintf(strUserName, 256, "[%d]CXP:  %s (%s)", i,pDeviceInfo->SpecialInfo.stCXPInfo.chModelName,
                    pDeviceInfo->SpecialInfo.stCXPInfo.chSerialNumber);
            }
        }
        else if (pDeviceInfo->nTLayerType == MV_GENTL_XOF_DEVICE)
        {
            if (strcmp("", (char*)pDeviceInfo->SpecialInfo.stXoFInfo.chUserDefinedName) != 0)
            {
                snprintf(strUserName, 256, "[%d]XOF:  %s (%s)", i,pDeviceInfo->SpecialInfo.stXoFInfo.chUserDefinedName,
                    pDeviceInfo->SpecialInfo.stXoFInfo.chSerialNumber);
            }
            else
            {
                snprintf(strUserName, 256, "[%d]XOF:  %s (%s)",i, pDeviceInfo->SpecialInfo.stXoFInfo.chModelName,
                    pDeviceInfo->SpecialInfo.stXoFInfo.chSerialNumber);
            }
        }
        else
        {
            ShowErrorMsg("Unknown device enumerated", 0);
        }
        ui->ComboDevices->addItem(QString::fromLocal8Bit(strUserName));
    }

    if (0 == m_stDevList.nDeviceNum)
    {
        ShowErrorMsg("No device", 0);
        return;
    }
    ui->ComboDevices->setCurrentIndex(0);
}

void MainWindow::on_bnOpen_clicked()
{
    int nIndex = ui->ComboDevices->currentIndex();
    if ((nIndex < 0) | (nIndex >= MV_MAX_DEVICE_NUM))
    {
        ShowErrorMsg("Please select device", 0);
        return;
    }

    // ch:由设备信息创建设备实例 | en:Device instance created by device information
    if (NULL == m_stDevList.pDeviceInfo[nIndex])
    {
        ShowErrorMsg("Device does not exist", 0);
        return;
    }

    if(m_pcMyCamera == NULL)
    {
        m_pcMyCamera = new (std::nothrow) CMvCamera;
        if (NULL == m_pcMyCamera)
        {
            ShowErrorMsg("new CMvCamera Instance failed", MV_E_RESOURCE);
            return;
        }
    }

    int nRet = MV_OK;

    do
    {
        nRet = m_pcMyCamera->Open(m_stDevList.pDeviceInfo[nIndex]);
        if (MV_OK != nRet)
        {
            ShowErrorMsg("Open Fail", nRet);
            break;
        }

        // ch:探测网络最佳包大小(只对GigE相机有效) | en:Detection network optimal package size(It only works for the GigE camera)
        if (m_stDevList.pDeviceInfo[nIndex]->nTLayerType == MV_GIGE_DEVICE)
        {
            unsigned int nPacketSize = 0;
            nRet = m_pcMyCamera->GetOptimalPacketSize(&nPacketSize);
            if (nRet == MV_OK)
            {
                nRet = m_pcMyCamera->SetIntValue("GevSCPSPacketSize",nPacketSize);
                if(nRet != MV_OK)
                {
                    ShowErrorMsg("Warning: Set Packet Size fail!", nRet);
                }
            }
            else
            {
                ShowErrorMsg("Warning: Get Packet Size fail!", nRet);
            }
        }

        // ch:设置图像缓存个数,保证自动对焦时能获取到图像
        // en: Set the number of the internal image cache nodes in SDK, to ensure images can be captured during auto focus
        nRet = m_pcMyCamera->SetImageNodeNum(5);
        if (MV_OK != nRet)
        {
            ShowErrorMsg("Set image node number fail", nRet);
            break;
        }

        //ch:开始采集 | en : Start grabbing
        memset(&m_stImageInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));
        m_bThreadState = true;

        pthread_t threadID;
        if (pthread_create(&threadID, NULL, GrabThread, this) == 0)
        {
            m_hGrabThread = (void*)threadID;
        } else
        {
            m_hGrabThread = NULL;
        }

        if (NULL == m_hGrabThread)
        {
            nRet = MV_E_RESOURCE;
            ShowErrorMsg("Create thread fail", nRet);
            break;
        }

        nRet = m_pcMyCamera->StartGrabbing();
        if (MV_OK != nRet)
        {
            ShowErrorMsg("Start grabbing fail", nRet);
            break;
        }

        m_bOpenDevice = true;
        EnableControls(true);

    }while (0);


    if (MV_OK!=nRet)
    {
        m_bThreadState = false;
        delete m_pcMyCamera;
        m_pcMyCamera = NULL;
        return;
    }

    return;
}

void MainWindow::on_bnClose_clicked()
{
    m_bThreadState = false;

    if (m_hGrabThread)
    {
        pthread_join((pthread_t)m_hGrabThread, NULL);
        m_hGrabThread = NULL;
    }

    if (m_pcMyCamera)
    {
        if (m_bOpenLens)
        {
            m_pcMyCamera->CloseLiquidLens();
        }
        m_pcMyCamera->StopGrabbing();
        m_pcMyCamera->Close();
        delete m_pcMyCamera;
        m_pcMyCamera = NULL;
    }

    m_bOpenLens = false;
    m_bOpenDevice = false;

    EnableControls(true);

}


int MainWindow::GetFocalPowerInfo()
{
    int nCurFocalPower = 0;
    int nRet = m_pcMyCamera->GetFocalPower(&nCurFocalPower);
    if (MV_OK != nRet)
    {
        return nRet;
    }
    else
    {
        ui->tblFocalPowerCurrent->setText(QString("%1").arg(nCurFocalPower));
    }
    return MV_OK;
}

void MainWindow::on_bnOpenLiquidLens_clicked()
{
    if (false == m_bOpenDevice || true == m_bOpenLens || NULL == m_pcMyCamera)
    {
        return;
    }
    MV_CC_LIQUIDLENS_INFO stLensInfo ;
    memset(&stLensInfo, 0, sizeof(MV_CC_LIQUIDLENS_INFO));

    int nRet = m_pcMyCamera->OpenLiquidLens(&stLensInfo);
    if (MV_OK != nRet)
    {
        ShowErrorMsg("Open Liquid Lens fail", nRet);
        return;
    }

    ui->tbLensModel->setText((char*)stLensInfo.chModelName);
    ui->tbLensFirmware->setText((char*)stLensInfo.chFirmwareVersion);
    ui->tbLensStableTime->setText(QString("%1").arg(stLensInfo.nLensStableTime));
    ui->tbFocalPowerMax->setText(QString("%1").arg(stLensInfo.nMaxFocalPower));
    ui->tbFocalPowerMin->setText(QString("%1").arg(stLensInfo.nMinFocalPower));


    nRet = GetFocalPowerInfo();
    if (MV_OK!=nRet)
    {
        ShowErrorMsg("Liquid Lens get focal power info fail", nRet);
        return;
    }

    unsigned int nTimeMs = 0;
    nRet = m_pcMyCamera->GetLensTriggerDelayTime(&nTimeMs);
    if (MV_OK != nRet)
    {
        ShowErrorMsg("Liquid Lens get trigger dalay time fail", nRet);
        return;
    }

    ui->tbLensTriggerDelayTime->setText(QString("%1").arg(nTimeMs));

    m_bOpenLens = true;
    EnableControls(true);
}

void MainWindow::on_bnCloseLiquidLens_clicked()
{
    if (false == m_bOpenDevice || false == m_bOpenLens || NULL == m_pcMyCamera)
    {
        return;
    }

    int nRet = m_pcMyCamera->CloseLiquidLens();
    if (MV_OK != nRet)
    {
        ShowErrorMsg("Close liquid lens fail", nRet);
    }


    m_bOpenLens = false;
    EnableControls(true);
}

void MainWindow::on_bnRefresh_clicked()
{
    int nRet = GetFocalPowerInfo();
    if (nRet != MV_OK)
    {
        ShowErrorMsg("Liquid Lens get focal power fail", nRet);
    }
}

void MainWindow::on_bnSetFocalPower_clicked()
{
    int nInputFPValue =  ui->tblFocalPower->text().toInt();
    int nRet = m_pcMyCamera->SetFocalPower(nInputFPValue);
    if (nRet != MV_OK)
    {
        ShowErrorMsg("Set focal power fail", nRet);
    }

    return;
}

void MainWindow::on_bnExeAutoFocus_clicked()
{
    MV_CC_LIQUIDLENS_AUTOFOCUS_PARAM stAutoFocusParam;
    memset(&stAutoFocusParam, 0, sizeof(MV_CC_LIQUIDLENS_AUTOFOCUS_PARAM));

    stAutoFocusParam.nFocalPowerStart = ui->tbStartFocalPower->text().toInt();
    stAutoFocusParam.nFocalPowerEnd = ui->tbEndFocalPower->text().toInt();
    stAutoFocusParam.nCoarseSteps = ui->tbCoarseStep->text().toInt();
    stAutoFocusParam.nRoiEnable = 0;

    int nResultFocalPower = 0 ;
    int nRet = m_pcMyCamera->AutoFocus(&stAutoFocusParam, &nResultFocalPower);
    if (nRet != MV_OK)
    {
        ShowErrorMsg("Liquid len execute auto focus fail", nRet);
        return;
    }
    else
    {
        ui->tblResultFP->setText(QString("%1").arg(nResultFocalPower));
    }

    return;
}
