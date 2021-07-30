/********************************************************************************
** Form generated from reading UI file 'inoutservicepanel.ui'
**
** Created: Fri Sep 13 11:22:47 2019
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INOUTSERVICEPANEL_H
#define UI_INOUTSERVICEPANEL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_InOutServicePanel
{
public:
    QFrame *frame;
    QLabel *serviceSetupDiagnosticLabel;
    QPushButton *exitButton;
    QFrame *frame_2;
    QFrame *frame_xray_signals;
    QLabel *label_34;
    QRadioButton *radioButtonRxPushButton;
    QRadioButton *radioButtonXrayEnable;
    QRadioButton *radioButtonGridDisable;
    QRadioButton *radioButtonXrayCompleted;
    QRadioButton *radioButtonXrayOn;
    QRadioButton *radioButtonDetectorOn;
    QRadioButton *radioButtonDetectorReady;
    QRadioButton *radioButtonDetectorExpWin;
    QRadioButton *radioButtonDetectorPrep;
    QRadioButton *radioButtonClosedDoor;
    QFrame *frame_cpu_signals;
    QLabel *label_36;
    QRadioButton *radioButtonMasterEnable;
    QRadioButton *radioButtonLoader;
    QRadioButton *radioButtonLamp1;
    QRadioButton *radioButtonLamp2;
    QRadioButton *radioButtonMainsOn;
    QRadioButton *radioButtonPushLamp;
    QRadioButton *radioButtonBuzzerOn;
    QRadioButton *radioButtonBuzzerMode;
    QRadioButton *radioButtonBuzzerMode_2;
    QRadioButton *radioButtonBuzzerMode_3;
    QFrame *frame_compressor;
    QLabel *label_33;
    QRadioButton *radioButtonCompressorEna;
    QRadioButton *radioButtonCompressorUp;
    QRadioButton *radioButtonCompressorDown;
    QRadioButton *radioButtonCompressorUnlk;
    QFrame *frame_carm;
    QLabel *label_32;
    QRadioButton *radioButtonRotEna;
    QRadioButton *radioButtonLiftUp;
    QRadioButton *radioButtonLiftDown;
    QRadioButton *radioButtonLiftSwRot;
    QRadioButton *radioButtonLiftEna;
    QFrame *frame_carm_2;
    QLabel *label_35;
    QRadioButton *radioButtonRotEna_2;
    QRadioButton *radioButtonLiftUp_2;
    QRadioButton *radioButtonLiftEna_2;
    QRadioButton *radioButtonRotEna_4;
    QFrame *frame_carm_3;
    QLabel *label_38;
    QRadioButton *radioButtonRotEna_5;
    QRadioButton *radioButtonLiftUp_4;
    QRadioButton *radioButtonLiftEna_4;
    QRadioButton *radioButtonRotEna_6;
    QRadioButton *radioButtonLiftUp_5;
    QRadioButton *radioButtonLiftUp_6;
    QLabel *label_39;
    QFrame *frame_carm_4;
    QLabel *label_40;
    QRadioButton *radioButtonRotEna_7;
    QRadioButton *radioButtonLiftUp_7;
    QRadioButton *radioButtonLiftEna_5;
    QRadioButton *radioButtonRotEna_8;
    QRadioButton *radioButtonLiftUp_8;
    QRadioButton *radioButtonLiftUp_9;
    QLabel *label_41;

    void setupUi(QWidget *InOutServicePanel)
    {
        if (InOutServicePanel->objectName().isEmpty())
            InOutServicePanel->setObjectName(QString::fromUtf8("InOutServicePanel"));
        InOutServicePanel->resize(800, 480);
        InOutServicePanel->setStyleSheet(QString::fromUtf8("background-color:rgb(0,0,0,0);\n"
"background-image:url(://Service_Package/Service_Package/package_window.png);\n"
"border: 0px;"));
        frame = new QFrame(InOutServicePanel);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(0, 0, 800, 480));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        serviceSetupDiagnosticLabel = new QLabel(frame);
        serviceSetupDiagnosticLabel->setObjectName(QString::fromUtf8("serviceSetupDiagnosticLabel"));
        serviceSetupDiagnosticLabel->setGeometry(QRect(115, 10, 616, 41));
        QFont font;
        font.setFamily(QString::fromUtf8("Bitstream Charter"));
        font.setPointSize(24);
        font.setBold(true);
        font.setItalic(true);
        font.setWeight(75);
        serviceSetupDiagnosticLabel->setFont(font);
        serviceSetupDiagnosticLabel->setStyleSheet(QString::fromUtf8("background-image:url(/transparent.png);\n"
"color: rgb(248, 248, 54);"));
        serviceSetupDiagnosticLabel->setAlignment(Qt::AlignCenter);
        exitButton = new QPushButton(frame);
        exitButton->setObjectName(QString::fromUtf8("exitButton"));
        exitButton->setGeometry(QRect(0, 0, 111, 111));
        frame_2 = new QFrame(frame);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setGeometry(QRect(645, 280, 121, 121));
        frame_2->setStyleSheet(QString::fromUtf8("background-color:rgb(0,0,0,0);\n"
"background-image:url(://Service_IO/Service_IO/in_out_icon.png);\n"
"border: 0px;"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        frame_xray_signals = new QFrame(frame);
        frame_xray_signals->setObjectName(QString::fromUtf8("frame_xray_signals"));
        frame_xray_signals->setGeometry(QRect(15, 110, 216, 186));
        frame_xray_signals->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(74, 74, 74,50);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:10px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        frame_xray_signals->setFrameShape(QFrame::StyledPanel);
        frame_xray_signals->setFrameShadow(QFrame::Raised);
        label_34 = new QLabel(frame_xray_signals);
        label_34->setObjectName(QString::fromUtf8("label_34"));
        label_34->setGeometry(QRect(15, 0, 136, 26));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Bitstream Charter"));
        font1.setPointSize(13);
        font1.setBold(true);
        font1.setWeight(75);
        label_34->setFont(font1);
        label_34->setStyleSheet(QString::fromUtf8("color: rgb(205, 255, 211);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonRxPushButton = new QRadioButton(frame_xray_signals);
        radioButtonRxPushButton->setObjectName(QString::fromUtf8("radioButtonRxPushButton"));
        radioButtonRxPushButton->setGeometry(QRect(10, 25, 196, 19));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Bitstream Charter"));
        font2.setPointSize(12);
        font2.setBold(true);
        font2.setWeight(75);
        radioButtonRxPushButton->setFont(font2);
        radioButtonRxPushButton->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonRxPushButton->setCheckable(true);
        radioButtonRxPushButton->setChecked(false);
        radioButtonRxPushButton->setAutoExclusive(false);
        radioButtonXrayEnable = new QRadioButton(frame_xray_signals);
        radioButtonXrayEnable->setObjectName(QString::fromUtf8("radioButtonXrayEnable"));
        radioButtonXrayEnable->setGeometry(QRect(10, 40, 196, 19));
        radioButtonXrayEnable->setFont(font2);
        radioButtonXrayEnable->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonXrayEnable->setAutoExclusive(false);
        radioButtonGridDisable = new QRadioButton(frame_xray_signals);
        radioButtonGridDisable->setObjectName(QString::fromUtf8("radioButtonGridDisable"));
        radioButtonGridDisable->setGeometry(QRect(10, 55, 196, 19));
        radioButtonGridDisable->setFont(font2);
        radioButtonGridDisable->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonGridDisable->setAutoExclusive(false);
        radioButtonXrayCompleted = new QRadioButton(frame_xray_signals);
        radioButtonXrayCompleted->setObjectName(QString::fromUtf8("radioButtonXrayCompleted"));
        radioButtonXrayCompleted->setGeometry(QRect(10, 70, 201, 19));
        radioButtonXrayCompleted->setFont(font2);
        radioButtonXrayCompleted->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonXrayCompleted->setAutoExclusive(false);
        radioButtonXrayOn = new QRadioButton(frame_xray_signals);
        radioButtonXrayOn->setObjectName(QString::fromUtf8("radioButtonXrayOn"));
        radioButtonXrayOn->setGeometry(QRect(10, 85, 201, 19));
        radioButtonXrayOn->setFont(font2);
        radioButtonXrayOn->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonXrayOn->setAutoExclusive(false);
        radioButtonDetectorOn = new QRadioButton(frame_xray_signals);
        radioButtonDetectorOn->setObjectName(QString::fromUtf8("radioButtonDetectorOn"));
        radioButtonDetectorOn->setGeometry(QRect(10, 100, 166, 19));
        radioButtonDetectorOn->setFont(font2);
        radioButtonDetectorOn->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonDetectorOn->setAutoExclusive(false);
        radioButtonDetectorReady = new QRadioButton(frame_xray_signals);
        radioButtonDetectorReady->setObjectName(QString::fromUtf8("radioButtonDetectorReady"));
        radioButtonDetectorReady->setGeometry(QRect(10, 115, 166, 19));
        radioButtonDetectorReady->setFont(font2);
        radioButtonDetectorReady->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonDetectorReady->setAutoExclusive(false);
        radioButtonDetectorExpWin = new QRadioButton(frame_xray_signals);
        radioButtonDetectorExpWin->setObjectName(QString::fromUtf8("radioButtonDetectorExpWin"));
        radioButtonDetectorExpWin->setGeometry(QRect(10, 130, 201, 19));
        radioButtonDetectorExpWin->setFont(font2);
        radioButtonDetectorExpWin->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonDetectorExpWin->setAutoExclusive(false);
        radioButtonDetectorPrep = new QRadioButton(frame_xray_signals);
        radioButtonDetectorPrep->setObjectName(QString::fromUtf8("radioButtonDetectorPrep"));
        radioButtonDetectorPrep->setGeometry(QRect(10, 145, 166, 19));
        radioButtonDetectorPrep->setFont(font2);
        radioButtonDetectorPrep->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonDetectorPrep->setAutoExclusive(false);
        radioButtonClosedDoor = new QRadioButton(frame_xray_signals);
        radioButtonClosedDoor->setObjectName(QString::fromUtf8("radioButtonClosedDoor"));
        radioButtonClosedDoor->setGeometry(QRect(10, 160, 166, 19));
        radioButtonClosedDoor->setFont(font2);
        radioButtonClosedDoor->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonClosedDoor->setAutoExclusive(false);
        frame_cpu_signals = new QFrame(frame);
        frame_cpu_signals->setObjectName(QString::fromUtf8("frame_cpu_signals"));
        frame_cpu_signals->setGeometry(QRect(245, 75, 201, 186));
        frame_cpu_signals->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(74, 74, 74,50);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:10px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        frame_cpu_signals->setFrameShape(QFrame::StyledPanel);
        frame_cpu_signals->setFrameShadow(QFrame::Raised);
        label_36 = new QLabel(frame_cpu_signals);
        label_36->setObjectName(QString::fromUtf8("label_36"));
        label_36->setGeometry(QRect(15, 0, 136, 26));
        label_36->setFont(font1);
        label_36->setStyleSheet(QString::fromUtf8("color: rgb(205, 255, 211);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonMasterEnable = new QRadioButton(frame_cpu_signals);
        radioButtonMasterEnable->setObjectName(QString::fromUtf8("radioButtonMasterEnable"));
        radioButtonMasterEnable->setGeometry(QRect(10, 25, 176, 19));
        radioButtonMasterEnable->setFont(font2);
        radioButtonMasterEnable->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonMasterEnable->setAutoExclusive(false);
        radioButtonLoader = new QRadioButton(frame_cpu_signals);
        radioButtonLoader->setObjectName(QString::fromUtf8("radioButtonLoader"));
        radioButtonLoader->setGeometry(QRect(10, 55, 181, 19));
        radioButtonLoader->setFont(font2);
        radioButtonLoader->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonLoader->setAutoExclusive(false);
        radioButtonLamp1 = new QRadioButton(frame_cpu_signals);
        radioButtonLamp1->setObjectName(QString::fromUtf8("radioButtonLamp1"));
        radioButtonLamp1->setGeometry(QRect(10, 70, 166, 19));
        radioButtonLamp1->setFont(font2);
        radioButtonLamp1->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonLamp1->setAutoExclusive(false);
        radioButtonLamp2 = new QRadioButton(frame_cpu_signals);
        radioButtonLamp2->setObjectName(QString::fromUtf8("radioButtonLamp2"));
        radioButtonLamp2->setGeometry(QRect(10, 85, 166, 19));
        radioButtonLamp2->setFont(font2);
        radioButtonLamp2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonLamp2->setAutoExclusive(false);
        radioButtonMainsOn = new QRadioButton(frame_cpu_signals);
        radioButtonMainsOn->setObjectName(QString::fromUtf8("radioButtonMainsOn"));
        radioButtonMainsOn->setGeometry(QRect(10, 40, 151, 19));
        radioButtonMainsOn->setFont(font2);
        radioButtonMainsOn->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonMainsOn->setAutoExclusive(false);
        radioButtonPushLamp = new QRadioButton(frame_cpu_signals);
        radioButtonPushLamp->setObjectName(QString::fromUtf8("radioButtonPushLamp"));
        radioButtonPushLamp->setGeometry(QRect(10, 100, 166, 19));
        radioButtonPushLamp->setFont(font2);
        radioButtonPushLamp->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonPushLamp->setAutoExclusive(false);
        radioButtonBuzzerOn = new QRadioButton(frame_cpu_signals);
        radioButtonBuzzerOn->setObjectName(QString::fromUtf8("radioButtonBuzzerOn"));
        radioButtonBuzzerOn->setGeometry(QRect(10, 115, 166, 19));
        radioButtonBuzzerOn->setFont(font2);
        radioButtonBuzzerOn->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonBuzzerOn->setAutoExclusive(false);
        radioButtonBuzzerMode = new QRadioButton(frame_cpu_signals);
        radioButtonBuzzerMode->setObjectName(QString::fromUtf8("radioButtonBuzzerMode"));
        radioButtonBuzzerMode->setGeometry(QRect(10, 130, 166, 19));
        radioButtonBuzzerMode->setFont(font2);
        radioButtonBuzzerMode->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonBuzzerMode->setAutoExclusive(false);
        radioButtonBuzzerMode_2 = new QRadioButton(frame_cpu_signals);
        radioButtonBuzzerMode_2->setObjectName(QString::fromUtf8("radioButtonBuzzerMode_2"));
        radioButtonBuzzerMode_2->setGeometry(QRect(10, 145, 166, 19));
        radioButtonBuzzerMode_2->setFont(font2);
        radioButtonBuzzerMode_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonBuzzerMode_2->setAutoExclusive(false);
        radioButtonBuzzerMode_3 = new QRadioButton(frame_cpu_signals);
        radioButtonBuzzerMode_3->setObjectName(QString::fromUtf8("radioButtonBuzzerMode_3"));
        radioButtonBuzzerMode_3->setGeometry(QRect(10, 160, 166, 19));
        radioButtonBuzzerMode_3->setFont(font2);
        radioButtonBuzzerMode_3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonBuzzerMode_3->setAutoExclusive(false);
        frame_compressor = new QFrame(frame);
        frame_compressor->setObjectName(QString::fromUtf8("frame_compressor"));
        frame_compressor->setGeometry(QRect(15, 305, 216, 111));
        frame_compressor->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(74, 74, 74,50);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:10px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        frame_compressor->setFrameShape(QFrame::StyledPanel);
        frame_compressor->setFrameShadow(QFrame::Raised);
        label_33 = new QLabel(frame_compressor);
        label_33->setObjectName(QString::fromUtf8("label_33"));
        label_33->setGeometry(QRect(15, 0, 131, 26));
        label_33->setFont(font1);
        label_33->setStyleSheet(QString::fromUtf8("color: rgb(205, 255, 211);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonCompressorEna = new QRadioButton(frame_compressor);
        radioButtonCompressorEna->setObjectName(QString::fromUtf8("radioButtonCompressorEna"));
        radioButtonCompressorEna->setGeometry(QRect(10, 25, 191, 19));
        radioButtonCompressorEna->setFont(font2);
        radioButtonCompressorEna->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonCompressorEna->setAutoExclusive(false);
        radioButtonCompressorUp = new QRadioButton(frame_compressor);
        radioButtonCompressorUp->setObjectName(QString::fromUtf8("radioButtonCompressorUp"));
        radioButtonCompressorUp->setGeometry(QRect(10, 43, 191, 19));
        radioButtonCompressorUp->setFont(font2);
        radioButtonCompressorUp->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonCompressorUp->setAutoExclusive(false);
        radioButtonCompressorDown = new QRadioButton(frame_compressor);
        radioButtonCompressorDown->setObjectName(QString::fromUtf8("radioButtonCompressorDown"));
        radioButtonCompressorDown->setGeometry(QRect(10, 62, 196, 19));
        radioButtonCompressorDown->setFont(font2);
        radioButtonCompressorDown->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonCompressorDown->setAutoExclusive(false);
        radioButtonCompressorUnlk = new QRadioButton(frame_compressor);
        radioButtonCompressorUnlk->setObjectName(QString::fromUtf8("radioButtonCompressorUnlk"));
        radioButtonCompressorUnlk->setGeometry(QRect(10, 80, 191, 19));
        radioButtonCompressorUnlk->setFont(font2);
        radioButtonCompressorUnlk->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonCompressorUnlk->setAutoExclusive(false);
        frame_carm = new QFrame(frame);
        frame_carm->setObjectName(QString::fromUtf8("frame_carm"));
        frame_carm->setGeometry(QRect(245, 270, 201, 146));
        frame_carm->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(74, 74, 74,50);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:10px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        frame_carm->setFrameShape(QFrame::StyledPanel);
        frame_carm->setFrameShadow(QFrame::Raised);
        label_32 = new QLabel(frame_carm);
        label_32->setObjectName(QString::fromUtf8("label_32"));
        label_32->setGeometry(QRect(15, 0, 131, 26));
        label_32->setFont(font1);
        label_32->setStyleSheet(QString::fromUtf8("color: rgb(205, 255, 211);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
"\n"
""));
        radioButtonRotEna = new QRadioButton(frame_carm);
        radioButtonRotEna->setObjectName(QString::fromUtf8("radioButtonRotEna"));
        radioButtonRotEna->setGeometry(QRect(10, 35, 131, 19));
        radioButtonRotEna->setFont(font2);
        radioButtonRotEna->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonRotEna->setAutoExclusive(false);
        radioButtonLiftUp = new QRadioButton(frame_carm);
        radioButtonLiftUp->setObjectName(QString::fromUtf8("radioButtonLiftUp"));
        radioButtonLiftUp->setGeometry(QRect(10, 75, 91, 19));
        radioButtonLiftUp->setFont(font2);
        radioButtonLiftUp->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonLiftUp->setAutoExclusive(false);
        radioButtonLiftDown = new QRadioButton(frame_carm);
        radioButtonLiftDown->setObjectName(QString::fromUtf8("radioButtonLiftDown"));
        radioButtonLiftDown->setGeometry(QRect(10, 95, 136, 19));
        radioButtonLiftDown->setFont(font2);
        radioButtonLiftDown->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonLiftDown->setAutoExclusive(false);
        radioButtonLiftSwRot = new QRadioButton(frame_carm);
        radioButtonLiftSwRot->setObjectName(QString::fromUtf8("radioButtonLiftSwRot"));
        radioButtonLiftSwRot->setGeometry(QRect(10, 115, 161, 19));
        radioButtonLiftSwRot->setFont(font2);
        radioButtonLiftSwRot->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonLiftSwRot->setAutoExclusive(false);
        radioButtonLiftEna = new QRadioButton(frame_carm);
        radioButtonLiftEna->setObjectName(QString::fromUtf8("radioButtonLiftEna"));
        radioButtonLiftEna->setGeometry(QRect(10, 55, 121, 19));
        radioButtonLiftEna->setFont(font2);
        radioButtonLiftEna->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonLiftEna->setAutoExclusive(false);
        frame_carm_2 = new QFrame(frame);
        frame_carm_2->setObjectName(QString::fromUtf8("frame_carm_2"));
        frame_carm_2->setGeometry(QRect(460, 270, 156, 146));
        frame_carm_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(74, 74, 74,50);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:10px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        frame_carm_2->setFrameShape(QFrame::StyledPanel);
        frame_carm_2->setFrameShadow(QFrame::Raised);
        label_35 = new QLabel(frame_carm_2);
        label_35->setObjectName(QString::fromUtf8("label_35"));
        label_35->setGeometry(QRect(15, 0, 111, 26));
        label_35->setFont(font1);
        label_35->setStyleSheet(QString::fromUtf8("color: rgb(205, 255, 211);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonRotEna_2 = new QRadioButton(frame_carm_2);
        radioButtonRotEna_2->setObjectName(QString::fromUtf8("radioButtonRotEna_2"));
        radioButtonRotEna_2->setGeometry(QRect(10, 25, 131, 19));
        radioButtonRotEna_2->setFont(font2);
        radioButtonRotEna_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonRotEna_2->setAutoExclusive(false);
        radioButtonLiftUp_2 = new QRadioButton(frame_carm_2);
        radioButtonLiftUp_2->setObjectName(QString::fromUtf8("radioButtonLiftUp_2"));
        radioButtonLiftUp_2->setGeometry(QRect(10, 40, 106, 19));
        radioButtonLiftUp_2->setFont(font2);
        radioButtonLiftUp_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonLiftUp_2->setAutoExclusive(false);
        radioButtonLiftEna_2 = new QRadioButton(frame_carm_2);
        radioButtonLiftEna_2->setObjectName(QString::fromUtf8("radioButtonLiftEna_2"));
        radioButtonLiftEna_2->setGeometry(QRect(10, 55, 121, 19));
        radioButtonLiftEna_2->setFont(font2);
        radioButtonLiftEna_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonLiftEna_2->setAutoExclusive(false);
        radioButtonRotEna_4 = new QRadioButton(frame_carm_2);
        radioButtonRotEna_4->setObjectName(QString::fromUtf8("radioButtonRotEna_4"));
        radioButtonRotEna_4->setGeometry(QRect(10, 75, 131, 19));
        radioButtonRotEna_4->setFont(font2);
        radioButtonRotEna_4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonRotEna_4->setAutoExclusive(false);
        frame_carm_3 = new QFrame(frame);
        frame_carm_3->setObjectName(QString::fromUtf8("frame_carm_3"));
        frame_carm_3->setGeometry(QRect(460, 75, 156, 186));
        frame_carm_3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(74, 74, 74,50);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:10px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        frame_carm_3->setFrameShape(QFrame::StyledPanel);
        frame_carm_3->setFrameShadow(QFrame::Raised);
        label_38 = new QLabel(frame_carm_3);
        label_38->setObjectName(QString::fromUtf8("label_38"));
        label_38->setGeometry(QRect(15, 0, 131, 26));
        label_38->setFont(font1);
        label_38->setStyleSheet(QString::fromUtf8("color: rgb(205, 255, 211);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonRotEna_5 = new QRadioButton(frame_carm_3);
        radioButtonRotEna_5->setObjectName(QString::fromUtf8("radioButtonRotEna_5"));
        radioButtonRotEna_5->setGeometry(QRect(10, 50, 131, 19));
        radioButtonRotEna_5->setFont(font2);
        radioButtonRotEna_5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonRotEna_5->setAutoExclusive(false);
        radioButtonLiftUp_4 = new QRadioButton(frame_carm_3);
        radioButtonLiftUp_4->setObjectName(QString::fromUtf8("radioButtonLiftUp_4"));
        radioButtonLiftUp_4->setGeometry(QRect(10, 80, 106, 19));
        radioButtonLiftUp_4->setFont(font2);
        radioButtonLiftUp_4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonLiftUp_4->setAutoExclusive(false);
        radioButtonLiftEna_4 = new QRadioButton(frame_carm_3);
        radioButtonLiftEna_4->setObjectName(QString::fromUtf8("radioButtonLiftEna_4"));
        radioButtonLiftEna_4->setGeometry(QRect(10, 65, 121, 19));
        radioButtonLiftEna_4->setFont(font2);
        radioButtonLiftEna_4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonLiftEna_4->setAutoExclusive(false);
        radioButtonRotEna_6 = new QRadioButton(frame_carm_3);
        radioButtonRotEna_6->setObjectName(QString::fromUtf8("radioButtonRotEna_6"));
        radioButtonRotEna_6->setGeometry(QRect(10, 130, 131, 19));
        radioButtonRotEna_6->setFont(font2);
        radioButtonRotEna_6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonRotEna_6->setAutoExclusive(false);
        radioButtonLiftUp_5 = new QRadioButton(frame_carm_3);
        radioButtonLiftUp_5->setObjectName(QString::fromUtf8("radioButtonLiftUp_5"));
        radioButtonLiftUp_5->setGeometry(QRect(10, 95, 106, 19));
        radioButtonLiftUp_5->setFont(font2);
        radioButtonLiftUp_5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonLiftUp_5->setAutoExclusive(false);
        radioButtonLiftUp_6 = new QRadioButton(frame_carm_3);
        radioButtonLiftUp_6->setObjectName(QString::fromUtf8("radioButtonLiftUp_6"));
        radioButtonLiftUp_6->setGeometry(QRect(10, 115, 106, 19));
        radioButtonLiftUp_6->setFont(font2);
        radioButtonLiftUp_6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonLiftUp_6->setAutoExclusive(false);
        label_39 = new QLabel(frame_carm_3);
        label_39->setObjectName(QString::fromUtf8("label_39"));
        label_39->setGeometry(QRect(15, 155, 131, 26));
        label_39->setFont(font2);
        label_39->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        frame_carm_4 = new QFrame(frame);
        frame_carm_4->setObjectName(QString::fromUtf8("frame_carm_4"));
        frame_carm_4->setGeometry(QRect(630, 75, 156, 186));
        frame_carm_4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(74, 74, 74,50);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:10px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        frame_carm_4->setFrameShape(QFrame::StyledPanel);
        frame_carm_4->setFrameShadow(QFrame::Raised);
        label_40 = new QLabel(frame_carm_4);
        label_40->setObjectName(QString::fromUtf8("label_40"));
        label_40->setGeometry(QRect(15, 0, 131, 26));
        label_40->setFont(font1);
        label_40->setStyleSheet(QString::fromUtf8("color: rgb(205, 255, 211);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonRotEna_7 = new QRadioButton(frame_carm_4);
        radioButtonRotEna_7->setObjectName(QString::fromUtf8("radioButtonRotEna_7"));
        radioButtonRotEna_7->setGeometry(QRect(10, 50, 131, 19));
        radioButtonRotEna_7->setFont(font2);
        radioButtonRotEna_7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonRotEna_7->setAutoExclusive(false);
        radioButtonLiftUp_7 = new QRadioButton(frame_carm_4);
        radioButtonLiftUp_7->setObjectName(QString::fromUtf8("radioButtonLiftUp_7"));
        radioButtonLiftUp_7->setGeometry(QRect(10, 80, 106, 19));
        radioButtonLiftUp_7->setFont(font2);
        radioButtonLiftUp_7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonLiftUp_7->setAutoExclusive(false);
        radioButtonLiftEna_5 = new QRadioButton(frame_carm_4);
        radioButtonLiftEna_5->setObjectName(QString::fromUtf8("radioButtonLiftEna_5"));
        radioButtonLiftEna_5->setGeometry(QRect(10, 65, 121, 19));
        radioButtonLiftEna_5->setFont(font2);
        radioButtonLiftEna_5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonLiftEna_5->setAutoExclusive(false);
        radioButtonRotEna_8 = new QRadioButton(frame_carm_4);
        radioButtonRotEna_8->setObjectName(QString::fromUtf8("radioButtonRotEna_8"));
        radioButtonRotEna_8->setGeometry(QRect(10, 130, 131, 19));
        radioButtonRotEna_8->setFont(font2);
        radioButtonRotEna_8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonRotEna_8->setAutoExclusive(false);
        radioButtonLiftUp_8 = new QRadioButton(frame_carm_4);
        radioButtonLiftUp_8->setObjectName(QString::fromUtf8("radioButtonLiftUp_8"));
        radioButtonLiftUp_8->setGeometry(QRect(10, 95, 106, 19));
        radioButtonLiftUp_8->setFont(font2);
        radioButtonLiftUp_8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonLiftUp_8->setAutoExclusive(false);
        radioButtonLiftUp_9 = new QRadioButton(frame_carm_4);
        radioButtonLiftUp_9->setObjectName(QString::fromUtf8("radioButtonLiftUp_9"));
        radioButtonLiftUp_9->setGeometry(QRect(10, 115, 106, 19));
        radioButtonLiftUp_9->setFont(font2);
        radioButtonLiftUp_9->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        radioButtonLiftUp_9->setAutoExclusive(false);
        label_41 = new QLabel(frame_carm_4);
        label_41->setObjectName(QString::fromUtf8("label_41"));
        label_41->setGeometry(QRect(15, 155, 131, 26));
        label_41->setFont(font2);
        label_41->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));

        retranslateUi(InOutServicePanel);

        QMetaObject::connectSlotsByName(InOutServicePanel);
    } // setupUi

    void retranslateUi(QWidget *InOutServicePanel)
    {
        InOutServicePanel->setWindowTitle(QApplication::translate("InOutServicePanel", "Form", 0, QApplication::UnicodeUTF8));
        serviceSetupDiagnosticLabel->setText(QApplication::translate("InOutServicePanel", "Service Panel/IO panel", 0, QApplication::UnicodeUTF8));
        exitButton->setText(QString());
        label_34->setText(QApplication::translate("InOutServicePanel", "XRAY-SIGNALS", 0, QApplication::UnicodeUTF8));
        radioButtonRxPushButton->setText(QApplication::translate("InOutServicePanel", "RX PUSH BUTTON", 0, QApplication::UnicodeUTF8));
        radioButtonXrayEnable->setText(QApplication::translate("InOutServicePanel", "XRAY ENABLE", 0, QApplication::UnicodeUTF8));
        radioButtonGridDisable->setText(QApplication::translate("InOutServicePanel", "XRAY GRID DISABLE", 0, QApplication::UnicodeUTF8));
        radioButtonXrayCompleted->setText(QApplication::translate("InOutServicePanel", "XRAY COMPLETED", 0, QApplication::UnicodeUTF8));
        radioButtonXrayOn->setText(QApplication::translate("InOutServicePanel", "XRAY ON (SYNC GRID)", 0, QApplication::UnicodeUTF8));
        radioButtonDetectorOn->setText(QApplication::translate("InOutServicePanel", "DETECTOR ON", 0, QApplication::UnicodeUTF8));
        radioButtonDetectorReady->setText(QApplication::translate("InOutServicePanel", "DETECTOR READY", 0, QApplication::UnicodeUTF8));
        radioButtonDetectorExpWin->setText(QApplication::translate("InOutServicePanel", "DETECTOR EXP-WIN", 0, QApplication::UnicodeUTF8));
        radioButtonDetectorPrep->setText(QApplication::translate("InOutServicePanel", "DETECTOR PREP", 0, QApplication::UnicodeUTF8));
        radioButtonClosedDoor->setText(QApplication::translate("InOutServicePanel", "CLOSED DOOR", 0, QApplication::UnicodeUTF8));
        label_36->setText(QApplication::translate("InOutServicePanel", "CPU SIGNALS", 0, QApplication::UnicodeUTF8));
        radioButtonMasterEnable->setText(QApplication::translate("InOutServicePanel", "MASTER ENABLE", 0, QApplication::UnicodeUTF8));
        radioButtonLoader->setText(QApplication::translate("InOutServicePanel", "LOADER ACTIVATED", 0, QApplication::UnicodeUTF8));
        radioButtonLamp1->setText(QApplication::translate("InOutServicePanel", "LAMP XRAY 1", 0, QApplication::UnicodeUTF8));
        radioButtonLamp2->setText(QApplication::translate("InOutServicePanel", "LAMP XRAY 2", 0, QApplication::UnicodeUTF8));
        radioButtonMainsOn->setText(QApplication::translate("InOutServicePanel", "MAINS ON", 0, QApplication::UnicodeUTF8));
        radioButtonPushLamp->setText(QApplication::translate("InOutServicePanel", "XRAY PUSH LED", 0, QApplication::UnicodeUTF8));
        radioButtonBuzzerOn->setText(QApplication::translate("InOutServicePanel", "BUZZER ON", 0, QApplication::UnicodeUTF8));
        radioButtonBuzzerMode->setText(QApplication::translate("InOutServicePanel", "BUZZER MODE", 0, QApplication::UnicodeUTF8));
        radioButtonBuzzerMode_2->setText(QApplication::translate("InOutServicePanel", "UPS-TEST", 0, QApplication::UnicodeUTF8));
        radioButtonBuzzerMode_3->setText(QApplication::translate("InOutServicePanel", "UPS-DISCHARGE", 0, QApplication::UnicodeUTF8));
        label_33->setText(QApplication::translate("InOutServicePanel", "COMPRESSOR", 0, QApplication::UnicodeUTF8));
        radioButtonCompressorEna->setText(QApplication::translate("InOutServicePanel", "COMPRESSOR ENA", 0, QApplication::UnicodeUTF8));
        radioButtonCompressorUp->setText(QApplication::translate("InOutServicePanel", "COMPRESSOR UP", 0, QApplication::UnicodeUTF8));
        radioButtonCompressorDown->setText(QApplication::translate("InOutServicePanel", "COMPRESSOR DOWN", 0, QApplication::UnicodeUTF8));
        radioButtonCompressorUnlk->setText(QApplication::translate("InOutServicePanel", "COMPRESSOR UNLK", 0, QApplication::UnicodeUTF8));
        label_32->setText(QApplication::translate("InOutServicePanel", "LENZE", 0, QApplication::UnicodeUTF8));
        radioButtonRotEna->setText(QApplication::translate("InOutServicePanel", "ROT-ENA", 0, QApplication::UnicodeUTF8));
        radioButtonLiftUp->setText(QApplication::translate("InOutServicePanel", "LIFT UP", 0, QApplication::UnicodeUTF8));
        radioButtonLiftDown->setText(QApplication::translate("InOutServicePanel", "LIFT DOWN", 0, QApplication::UnicodeUTF8));
        radioButtonLiftSwRot->setText(QApplication::translate("InOutServicePanel", "LIFT SW ROT", 0, QApplication::UnicodeUTF8));
        radioButtonLiftEna->setText(QApplication::translate("InOutServicePanel", "LIFT-ENA", 0, QApplication::UnicodeUTF8));
        label_35->setText(QApplication::translate("InOutServicePanel", "ROTATION", 0, QApplication::UnicodeUTF8));
        radioButtonRotEna_2->setText(QApplication::translate("InOutServicePanel", "ROT-ENA", 0, QApplication::UnicodeUTF8));
        radioButtonLiftUp_2->setText(QApplication::translate("InOutServicePanel", "ROT-CCW", 0, QApplication::UnicodeUTF8));
        radioButtonLiftEna_2->setText(QApplication::translate("InOutServicePanel", "ROT-CW", 0, QApplication::UnicodeUTF8));
        radioButtonRotEna_4->setText(QApplication::translate("InOutServicePanel", "SYNT-ENA", 0, QApplication::UnicodeUTF8));
        label_38->setText(QApplication::translate("InOutServicePanel", "ROT DRIVER IO", 0, QApplication::UnicodeUTF8));
        radioButtonRotEna_5->setText(QApplication::translate("InOutServicePanel", "INPUT 1", 0, QApplication::UnicodeUTF8));
        radioButtonLiftUp_4->setText(QApplication::translate("InOutServicePanel", "INPUT 3", 0, QApplication::UnicodeUTF8));
        radioButtonLiftEna_4->setText(QApplication::translate("InOutServicePanel", "INPUT 2", 0, QApplication::UnicodeUTF8));
        radioButtonRotEna_6->setText(QApplication::translate("InOutServicePanel", "OUTPUT 2", 0, QApplication::UnicodeUTF8));
        radioButtonLiftUp_5->setText(QApplication::translate("InOutServicePanel", "INPUT 4", 0, QApplication::UnicodeUTF8));
        radioButtonLiftUp_6->setText(QApplication::translate("InOutServicePanel", "OUTPUT 1", 0, QApplication::UnicodeUTF8));
        label_39->setText(QApplication::translate("InOutServicePanel", "ANALOG: ---", 0, QApplication::UnicodeUTF8));
        label_40->setText(QApplication::translate("InOutServicePanel", "SYNT DRIVER IO", 0, QApplication::UnicodeUTF8));
        radioButtonRotEna_7->setText(QApplication::translate("InOutServicePanel", "INPUT 1", 0, QApplication::UnicodeUTF8));
        radioButtonLiftUp_7->setText(QApplication::translate("InOutServicePanel", "INPUT 3", 0, QApplication::UnicodeUTF8));
        radioButtonLiftEna_5->setText(QApplication::translate("InOutServicePanel", "INPUT 2", 0, QApplication::UnicodeUTF8));
        radioButtonRotEna_8->setText(QApplication::translate("InOutServicePanel", "OUTPUT 2", 0, QApplication::UnicodeUTF8));
        radioButtonLiftUp_8->setText(QApplication::translate("InOutServicePanel", "INPUT 4", 0, QApplication::UnicodeUTF8));
        radioButtonLiftUp_9->setText(QApplication::translate("InOutServicePanel", "OUTPUT 1", 0, QApplication::UnicodeUTF8));
        label_41->setText(QApplication::translate("InOutServicePanel", "ANALOG: ---", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class InOutServicePanel: public Ui_InOutServicePanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INOUTSERVICEPANEL_H
