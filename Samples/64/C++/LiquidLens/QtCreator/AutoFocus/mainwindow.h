#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "MvCamera.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void GrabThreadProcess(); // ch:取流线程 | en:Grab thread

private:
    void ShowErrorMsg(QString csMessage, unsigned int nErrorNum); // ch:显示错误信息窗口 | en: Show the window of error message
    int GetFocalPowerInfo();                  // ch:获取镜头的光焦度值 | en: Get focal power value
    void EnableControls(bool bIsCameraReady); // ch:按钮使能 | en:Enable control
private slots:

    void on_bnEnum_clicked();           // ch:查找设备 | en:Find Devices

    void on_bnOpen_clicked();           // ch:打开设备 | en:Open Devices

    void on_bnClose_clicked();          // ch:关闭设备 | en:Close Devices

    void on_bnOpenLiquidLens_clicked();  // ch:打开镜头 | en:Open lens

    void on_bnCloseLiquidLens_clicked(); // ch:关闭镜头 | en:Close lens

    void on_bnRefresh_clicked();         // ch:刷新当前光焦度值 | en: Refresh current focal power

    void on_bnSetFocalPower_clicked();   // ch:设置光焦度 | en: Set focal power

    void on_bnExeAutoFocus_clicked();    // ch:执行自动对焦 | en: Execute auto focus

private:
    Ui::MainWindow *ui;

    void *m_hWnd;                          // ch:显示窗口句柄 | en:The Handle of Display Window

    MV_CC_DEVICE_INFO_LIST  m_stDevList;   // ch:设备信息链表 | en:The list of device info
    CMvCamera*              m_pcMyCamera;  // ch:相机类设备实例 | en:The instance of CMvCamera
    bool                    m_bGrabbing;   // ch:是否开始抓图 | en:The flag of Grabbing
    MV_FRAME_OUT_INFO_EX    m_stImageInfo; // ch:图像信息 | en:Image Info
    void*                   m_hGrabThread;              // ch:取流线程句柄 | en:Grab thread handle
    bool                    m_bThreadState;             // ch:取流线程状态 | en:Grab thread state
    bool                    m_bOpenDevice;              // ch:是否打开设备 | en:Whether to open device
    bool                    m_bOpenLens;                // ch:是否开始镜头 | en:Whether to open lens
};

#endif // MAINWINDOW_H
