/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QComboBox *ComboDevices;
    QWidget *widgetDisplay;
    QVBoxLayout *verticalLayout;
    QPushButton *bnEnum;
    QGridLayout *gridLayout;
    QPushButton *bnOpen;
    QPushButton *bnClose;
    QRadioButton *bnContinuesMode;
    QRadioButton *bnTriggerMode;
    QPushButton *bnStart;
    QPushButton *bnStop;
    QCheckBox *cbSoftTrigger;
    QPushButton *bnTriggerExec;
    QLabel *lExposure;
    QLineEdit *tbExposure;
    QLabel *lGain;
    QLineEdit *tbGain;
    QLabel *lFrameRate;
    QLineEdit *tbFrameRate;
    QPushButton *bnGetParam;
    QPushButton *bnSetParam;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(728, 426);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        ComboDevices = new QComboBox(centralWidget);
        ComboDevices->setObjectName(QString::fromUtf8("ComboDevices"));

        verticalLayout_2->addWidget(ComboDevices);

        widgetDisplay = new QWidget(centralWidget);
        widgetDisplay->setObjectName(QString::fromUtf8("widgetDisplay"));

        verticalLayout_2->addWidget(widgetDisplay);

        verticalLayout_2->setStretch(0, 1);
        verticalLayout_2->setStretch(1, 10);

        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(10);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(10, 10, 10, -1);
        bnEnum = new QPushButton(centralWidget);
        bnEnum->setObjectName(QString::fromUtf8("bnEnum"));

        verticalLayout->addWidget(bnEnum);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(10);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(10, -1, 10, 0);
        bnOpen = new QPushButton(centralWidget);
        bnOpen->setObjectName(QString::fromUtf8("bnOpen"));

        gridLayout->addWidget(bnOpen, 0, 0, 1, 1);

        bnClose = new QPushButton(centralWidget);
        bnClose->setObjectName(QString::fromUtf8("bnClose"));
        bnClose->setEnabled(false);

        gridLayout->addWidget(bnClose, 0, 1, 1, 1);

        bnContinuesMode = new QRadioButton(centralWidget);
        bnContinuesMode->setObjectName(QString::fromUtf8("bnContinuesMode"));
        bnContinuesMode->setEnabled(false);

        gridLayout->addWidget(bnContinuesMode, 1, 0, 1, 1);

        bnTriggerMode = new QRadioButton(centralWidget);
        bnTriggerMode->setObjectName(QString::fromUtf8("bnTriggerMode"));
        bnTriggerMode->setEnabled(false);

        gridLayout->addWidget(bnTriggerMode, 1, 1, 1, 1);

        bnStart = new QPushButton(centralWidget);
        bnStart->setObjectName(QString::fromUtf8("bnStart"));
        bnStart->setEnabled(false);

        gridLayout->addWidget(bnStart, 2, 0, 1, 1);

        bnStop = new QPushButton(centralWidget);
        bnStop->setObjectName(QString::fromUtf8("bnStop"));
        bnStop->setEnabled(false);

        gridLayout->addWidget(bnStop, 2, 1, 1, 1);

        cbSoftTrigger = new QCheckBox(centralWidget);
        cbSoftTrigger->setObjectName(QString::fromUtf8("cbSoftTrigger"));
        cbSoftTrigger->setEnabled(false);

        gridLayout->addWidget(cbSoftTrigger, 3, 0, 1, 1);

        bnTriggerExec = new QPushButton(centralWidget);
        bnTriggerExec->setObjectName(QString::fromUtf8("bnTriggerExec"));
        bnTriggerExec->setEnabled(false);

        gridLayout->addWidget(bnTriggerExec, 3, 1, 1, 1);

        lExposure = new QLabel(centralWidget);
        lExposure->setObjectName(QString::fromUtf8("lExposure"));

        gridLayout->addWidget(lExposure, 4, 0, 1, 1);

        tbExposure = new QLineEdit(centralWidget);
        tbExposure->setObjectName(QString::fromUtf8("tbExposure"));
        tbExposure->setEnabled(false);

        gridLayout->addWidget(tbExposure, 4, 1, 1, 1);

        lGain = new QLabel(centralWidget);
        lGain->setObjectName(QString::fromUtf8("lGain"));

        gridLayout->addWidget(lGain, 5, 0, 1, 1);

        tbGain = new QLineEdit(centralWidget);
        tbGain->setObjectName(QString::fromUtf8("tbGain"));
        tbGain->setEnabled(false);

        gridLayout->addWidget(tbGain, 5, 1, 1, 1);

        lFrameRate = new QLabel(centralWidget);
        lFrameRate->setObjectName(QString::fromUtf8("lFrameRate"));

        gridLayout->addWidget(lFrameRate, 6, 0, 1, 1);

        tbFrameRate = new QLineEdit(centralWidget);
        tbFrameRate->setObjectName(QString::fromUtf8("tbFrameRate"));
        tbFrameRate->setEnabled(false);

        gridLayout->addWidget(tbFrameRate, 6, 1, 1, 1);

        bnGetParam = new QPushButton(centralWidget);
        bnGetParam->setObjectName(QString::fromUtf8("bnGetParam"));
        bnGetParam->setEnabled(false);

        gridLayout->addWidget(bnGetParam, 7, 0, 1, 1);

        bnSetParam = new QPushButton(centralWidget);
        bnSetParam->setObjectName(QString::fromUtf8("bnSetParam"));
        bnSetParam->setEnabled(false);

        gridLayout->addWidget(bnSetParam, 7, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        verticalLayout->setStretch(0, 1);
        verticalLayout->setStretch(1, 10);

        horizontalLayout->addLayout(verticalLayout);

        horizontalLayout->setStretch(0, 10);
        horizontalLayout->setStretch(1, 1);
        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        QWidget::setTabOrder(bnEnum, ComboDevices);
        QWidget::setTabOrder(ComboDevices, bnOpen);
        QWidget::setTabOrder(bnOpen, bnClose);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "BasicDemo", nullptr));
        bnEnum->setText(QApplication::translate("MainWindow", "\346\237\245\346\211\276\350\256\276\345\244\207", nullptr));
        bnOpen->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200\350\256\276\345\244\207", nullptr));
        bnClose->setText(QApplication::translate("MainWindow", "\345\205\263\351\227\255\350\256\276\345\244\207", nullptr));
        bnContinuesMode->setText(QApplication::translate("MainWindow", "\350\277\236\347\273\255\346\250\241\345\274\217", nullptr));
        bnTriggerMode->setText(QApplication::translate("MainWindow", "\350\247\246\345\217\221\346\250\241\345\274\217", nullptr));
        bnStart->setText(QApplication::translate("MainWindow", "\345\274\200\345\247\213\351\207\207\351\233\206", nullptr));
        bnStop->setText(QApplication::translate("MainWindow", "\345\201\234\346\255\242\351\207\207\351\233\206", nullptr));
        cbSoftTrigger->setText(QApplication::translate("MainWindow", "\350\275\257\350\247\246\345\217\221", nullptr));
        bnTriggerExec->setText(QApplication::translate("MainWindow", "\350\275\257\350\247\246\345\217\221\344\270\200\346\254\241", nullptr));
        lExposure->setText(QApplication::translate("MainWindow", "\346\233\235\345\205\211", nullptr));
        lGain->setText(QApplication::translate("MainWindow", "\345\242\236\347\233\212", nullptr));
        lFrameRate->setText(QApplication::translate("MainWindow", "\345\270\247\347\216\207", nullptr));
        bnGetParam->setText(QApplication::translate("MainWindow", "\350\216\267\345\217\226\345\217\202\346\225\260", nullptr));
        bnSetParam->setText(QApplication::translate("MainWindow", "\350\256\276\347\275\256\345\217\202\346\225\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
