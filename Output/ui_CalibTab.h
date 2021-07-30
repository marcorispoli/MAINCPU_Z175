/********************************************************************************
** Form generated from reading UI file 'CalibTab.ui'
**
** Created: Fri Sep 13 11:22:47 2019
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CALIBTAB_H
#define UI_CALIBTAB_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QFrame *frame4;
    QFrame *frameCalibTab;
    QFrame *frameCalibKV;
    QFrame *frameCalibTilt;
    QFrame *calibTiltFrameCalibrate;
    QLabel *calibPadSequenceLabelDown_8;
    QFrame *calibTiltFrameInit;
    QPushButton *calibrateTiltNextButton;
    QLabel *calibPadSequenceLabelDown_9;
    QLabel *calibTiltPcb249TrxData;
    QLabel *calibTiltPcb204TrxData;
    QLabel *calibPadSequenceLabelDown_10;
    QLabel *calibPadSequenceLabelDown_11;
    QLabel *calibPadSequenceLabelDown_12;
    QLabel *calibTiltArmData;
    QLabel *calibPadSequenceLabelDown_13;
    QFrame *frameCalibFiltro;
    QFrame *frameCalibHV;
    QFrame *frame_13;
    QFrame *calibHVframeMainPower;
    QLabel *label_48;
    QLabel *calibHVmainPower;
    QLabel *label_50;
    QFrame *calibHVframeWarnVcc;
    QFrame *frame_22;
    QLabel *labelMonitorMas_2;
    QLineEdit *calibHVeditHV;
    QFrame *frame_23;
    QLabel *labelMonitorMas_3;
    QPushButton *calibHVCalibrateButton;
    QFrame *frameCalib0;
    QFrame *frameCalibPad;
    QPushButton *calibPadNextButton;
    QFrame *calibPadFrameUp;
    QLabel *calibPadCalibratedPosUp;
    QLineEdit *calibPadEditUp;
    QLabel *calibPadSequenceLabelUp;
    QFrame *calibPadFrameDwn;
    QLabel *calibPadCalibratedPosDwn;
    QLabel *calibPadSequenceLabelDown;
    QLineEdit *calibPadEditDwn;
    QFrame *calibPadFrameCalibrate;
    QLabel *calibPadSequenceLabelDown_3;
    QFrame *calibPadFrameStore;
    QLabel *calibPadSequenceLabelDown_4;
    QPushButton *calibPadCancelButton;
    QFrame *calibPadFrameInit;
    QLabel *calibPadCalibratedPosInit;
    QLabel *calibPadSequenceLabelDown_2;
    QFrame *frameCalibCompressor;
    QPushButton *calibComprNextButton;
    QFrame *calibComprFrameF1;
    QLineEdit *calibComprEditF1;
    QLabel *calibComprRawData;
    QFrame *calibComprFrameOffset;
    QLabel *calibPadSequenceLabelDown_5;
    QFrame *calibComprFrameCalibrate;
    QLabel *calibPadSequenceLabelDown_6;
    QFrame *calibComprFrameStore;
    QLabel *calibPadSequenceLabelDown_7;
    QPushButton *calibComprCancelButton;
    QFrame *calibComprFrameInit;
    QLabel *calibComprCalibratedComprInit;
    QLabel *calibComprSequenceLabelIntro;
    QFrame *calibComprFrameF2;
    QLineEdit *calibComprEditF2;
    QPushButton *calibPadTabButton;
    QPushButton *calibComprTabButton;
    QPushButton *calibHVTabButton;
    QPushButton *calibTiltTabButton;
    QPushButton *calibKvTabButton;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QString::fromUtf8("Form"));
        Form->resize(480, 640);
        Form->setStyleSheet(QString::fromUtf8("background-image: url(://fold_win_bkg.png);"));
        frame4 = new QFrame(Form);
        frame4->setObjectName(QString::fromUtf8("frame4"));
        frame4->setGeometry(QRect(0, 60, 481, 521));
        frame4->setStyleSheet(QString::fromUtf8("background-color:rgb(201,201,201,0);\n"
"border: 0px;\n"
"background-image:url(:/transparent.png);\n"
""));
        frame4->setFrameShape(QFrame::StyledPanel);
        frame4->setFrameShadow(QFrame::Raised);
        frameCalibTab = new QFrame(frame4);
        frameCalibTab->setObjectName(QString::fromUtf8("frameCalibTab"));
        frameCalibTab->setGeometry(QRect(10, 50, 461, 401));
        frameCalibTab->setStyleSheet(QString::fromUtf8("background-image:url(/transparent.png);\n"
"\n"
"border: 1px solid  rgb(255, 255, 153);\n"
"border-radius:20px;\n"
""));
        frameCalibTab->setFrameShape(QFrame::StyledPanel);
        frameCalibTab->setFrameShadow(QFrame::Raised);
        frameCalibKV = new QFrame(frameCalibTab);
        frameCalibKV->setObjectName(QString::fromUtf8("frameCalibKV"));
        frameCalibKV->setGeometry(QRect(20, 5, 426, 391));
        frameCalibKV->setStyleSheet(QString::fromUtf8("background-color:rgb(255,255,153,30);\n"
"border: 0px;\n"
"background-image:url(:/transparent.png);\n"
""));
        frameCalibKV->setFrameShape(QFrame::StyledPanel);
        frameCalibKV->setFrameShadow(QFrame::Raised);
        frameCalibTilt = new QFrame(frameCalibKV);
        frameCalibTilt->setObjectName(QString::fromUtf8("frameCalibTilt"));
        frameCalibTilt->setGeometry(QRect(110, 325, 41, 31));
        frameCalibTilt->setStyleSheet(QString::fromUtf8("background-color:rgb(255,255,153,30);\n"
"border: 0px;\n"
"background-image:url(:/transparent.png);\n"
""));
        frameCalibTilt->setFrameShape(QFrame::StyledPanel);
        frameCalibTilt->setFrameShadow(QFrame::Raised);
        calibTiltFrameCalibrate = new QFrame(frameCalibTilt);
        calibTiltFrameCalibrate->setObjectName(QString::fromUtf8("calibTiltFrameCalibrate"));
        calibTiltFrameCalibrate->setGeometry(QRect(210, 180, 26, 21));
        calibTiltFrameCalibrate->setStyleSheet(QString::fromUtf8("border-image:url(:/compr_dwn.png);\n"
"background-color: rgba(255, 255, 255,0);\n"
"border-image:url(:/calibrate.png);\n"
""));
        calibTiltFrameCalibrate->setFrameShape(QFrame::StyledPanel);
        calibTiltFrameCalibrate->setFrameShadow(QFrame::Raised);
        calibPadSequenceLabelDown_8 = new QLabel(calibTiltFrameCalibrate);
        calibPadSequenceLabelDown_8->setObjectName(QString::fromUtf8("calibPadSequenceLabelDown_8"));
        calibPadSequenceLabelDown_8->setGeometry(QRect(90, 295, 176, 41));
        QFont font;
        font.setFamily(QString::fromUtf8("DejaVu Sans"));
        font.setPointSize(12);
        font.setBold(true);
        font.setItalic(false);
        font.setWeight(75);
        calibPadSequenceLabelDown_8->setFont(font);
        calibPadSequenceLabelDown_8->setStyleSheet(QString::fromUtf8("background-image:url(:/transparent.png);\n"
"border-image:url(:/transparent.png);\n"
"\n"
"background-color: rgb(255,255,255,0);\n"
"border-color: rgb(255, 255, 153);\n"
"color: rgb(255,0,0);\n"
""));
        calibPadSequenceLabelDown_8->setAlignment(Qt::AlignCenter);
        calibTiltFrameInit = new QFrame(frameCalibTilt);
        calibTiltFrameInit->setObjectName(QString::fromUtf8("calibTiltFrameInit"));
        calibTiltFrameInit->setGeometry(QRect(145, 175, 31, 41));
        calibTiltFrameInit->setStyleSheet(QString::fromUtf8("\n"
"background-color: rgba(255, 255, 255,0);\n"
"border-image:url(:/calib_tilt.png);\n"
""));
        calibTiltFrameInit->setFrameShape(QFrame::StyledPanel);
        calibTiltFrameInit->setFrameShadow(QFrame::Raised);
        calibrateTiltNextButton = new QPushButton(calibTiltFrameInit);
        calibrateTiltNextButton->setObjectName(QString::fromUtf8("calibrateTiltNextButton"));
        calibrateTiltNextButton->setGeometry(QRect(245, 160, 81, 71));
        calibrateTiltNextButton->setFocusPolicy(Qt::NoFocus);
        calibrateTiltNextButton->setStyleSheet(QString::fromUtf8("background-color: rgb(79, 79, 79,0);\n"
"border-image:url(:/play.png);\n"
"border-color: rgb(255, 255, 153);\n"
"color: rgb(255, 255, 153);\n"
"font: 80 16pt \"DejaVu Sans\";\n"
"border-radius:10px;\n"
""));
        calibPadSequenceLabelDown_9 = new QLabel(calibTiltFrameInit);
        calibPadSequenceLabelDown_9->setObjectName(QString::fromUtf8("calibPadSequenceLabelDown_9"));
        calibPadSequenceLabelDown_9->setGeometry(QRect(240, 125, 106, 46));
        QFont font1;
        font1.setFamily(QString::fromUtf8("DejaVu Sans"));
        font1.setPointSize(14);
        font1.setBold(true);
        font1.setItalic(false);
        font1.setWeight(75);
        calibPadSequenceLabelDown_9->setFont(font1);
        calibPadSequenceLabelDown_9->setStyleSheet(QString::fromUtf8("background-image:url(:/transparent.png);\n"
"border-image:url(:/transparent.png);\n"
"\n"
"background-color: rgb(255,255,255,0);\n"
"border-color: rgb(255, 255, 153);\n"
"color: rgb(255,0,0);\n"
""));
        calibPadSequenceLabelDown_9->setAlignment(Qt::AlignCenter);
        calibTiltPcb249TrxData = new QLabel(calibTiltFrameInit);
        calibTiltPcb249TrxData->setObjectName(QString::fromUtf8("calibTiltPcb249TrxData"));
        calibTiltPcb249TrxData->setGeometry(QRect(135, 15, 46, 16));
        QFont font2;
        font2.setFamily(QString::fromUtf8("DejaVu Sans"));
        font2.setPointSize(10);
        font2.setBold(true);
        font2.setItalic(false);
        font2.setWeight(75);
        calibTiltPcb249TrxData->setFont(font2);
        calibTiltPcb249TrxData->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255,0);\n"
"\n"
"border-color: rgb(255, 255, 153);\n"
"color: rgb(0,0,0);\n"
"border-image:url(:/transparent.png);\n"
"background-image:url(:/transparent.png);"));
        calibTiltPcb249TrxData->setAlignment(Qt::AlignCenter);
        calibTiltPcb204TrxData = new QLabel(calibTiltFrameInit);
        calibTiltPcb204TrxData->setObjectName(QString::fromUtf8("calibTiltPcb204TrxData"));
        calibTiltPcb204TrxData->setGeometry(QRect(135, 35, 46, 16));
        calibTiltPcb204TrxData->setFont(font2);
        calibTiltPcb204TrxData->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255,0);\n"
"\n"
"border-color: rgb(255, 255, 153);\n"
"color: rgb(0,0,0);\n"
"border-image:url(:/transparent.png);\n"
"background-image:url(:/transparent.png);"));
        calibTiltPcb204TrxData->setAlignment(Qt::AlignCenter);
        calibPadSequenceLabelDown_10 = new QLabel(calibTiltFrameInit);
        calibPadSequenceLabelDown_10->setObjectName(QString::fromUtf8("calibPadSequenceLabelDown_10"));
        calibPadSequenceLabelDown_10->setGeometry(QRect(185, 15, 66, 16));
        QFont font3;
        font3.setFamily(QString::fromUtf8("DejaVu Sans"));
        font3.setPointSize(8);
        font3.setBold(true);
        font3.setItalic(false);
        font3.setWeight(75);
        calibPadSequenceLabelDown_10->setFont(font3);
        calibPadSequenceLabelDown_10->setStyleSheet(QString::fromUtf8("background-image:url(:/transparent.png);\n"
"border-image:url(:/transparent.png);\n"
"\n"
"background-color: rgb(255,255,255,0);\n"
"border-color: rgb(255, 255, 153);\n"
"color: rgb(255,0,0);\n"
""));
        calibPadSequenceLabelDown_10->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        calibPadSequenceLabelDown_11 = new QLabel(calibTiltFrameInit);
        calibPadSequenceLabelDown_11->setObjectName(QString::fromUtf8("calibPadSequenceLabelDown_11"));
        calibPadSequenceLabelDown_11->setGeometry(QRect(185, 35, 66, 16));
        calibPadSequenceLabelDown_11->setFont(font3);
        calibPadSequenceLabelDown_11->setStyleSheet(QString::fromUtf8("background-image:url(:/transparent.png);\n"
"border-image:url(:/transparent.png);\n"
"\n"
"background-color: rgb(255,255,255,0);\n"
"border-color: rgb(255, 255, 153);\n"
"color: rgb(255,0,0);\n"
""));
        calibPadSequenceLabelDown_11->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        calibPadSequenceLabelDown_12 = new QLabel(calibTiltFrameInit);
        calibPadSequenceLabelDown_12->setObjectName(QString::fromUtf8("calibPadSequenceLabelDown_12"));
        calibPadSequenceLabelDown_12->setGeometry(QRect(95, 35, 36, 16));
        calibPadSequenceLabelDown_12->setFont(font3);
        calibPadSequenceLabelDown_12->setStyleSheet(QString::fromUtf8("background-image:url(:/transparent.png);\n"
"border-image:url(:/transparent.png);\n"
"\n"
"background-color: rgb(255,255,255,0);\n"
"border-color: rgb(255, 255, 153);\n"
"color: rgb(255,0,0);\n"
""));
        calibPadSequenceLabelDown_12->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        calibTiltArmData = new QLabel(calibTiltFrameInit);
        calibTiltArmData->setObjectName(QString::fromUtf8("calibTiltArmData"));
        calibTiltArmData->setGeometry(QRect(150, 320, 46, 16));
        calibTiltArmData->setFont(font2);
        calibTiltArmData->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255,0);\n"
"\n"
"border-color: rgb(255, 255, 153);\n"
"color: rgb(0,0,0);\n"
"border-image:url(:/transparent.png);\n"
"background-image:url(:/transparent.png);"));
        calibTiltArmData->setAlignment(Qt::AlignCenter);
        calibPadSequenceLabelDown_13 = new QLabel(calibTiltFrameInit);
        calibPadSequenceLabelDown_13->setObjectName(QString::fromUtf8("calibPadSequenceLabelDown_13"));
        calibPadSequenceLabelDown_13->setGeometry(QRect(95, 320, 36, 16));
        calibPadSequenceLabelDown_13->setFont(font3);
        calibPadSequenceLabelDown_13->setStyleSheet(QString::fromUtf8("background-image:url(:/transparent.png);\n"
"border-image:url(:/transparent.png);\n"
"\n"
"background-color: rgb(255,255,255,0);\n"
"border-color: rgb(255, 255, 153);\n"
"color: rgb(255,0,0);\n"
""));
        calibPadSequenceLabelDown_13->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        frameCalibFiltro = new QFrame(frameCalibKV);
        frameCalibFiltro->setObjectName(QString::fromUtf8("frameCalibFiltro"));
        frameCalibFiltro->setGeometry(QRect(30, 25, 340, 340));
        frameCalibFiltro->setStyleSheet(QString::fromUtf8("background-color:rgb(255,255,153,30);\n"
"border: 0px;\n"
"background-image:url(:/transparent.png);\n"
"border-image:url(:/Filtro1.png);"));
        frameCalibFiltro->setFrameShape(QFrame::StyledPanel);
        frameCalibFiltro->setFrameShadow(QFrame::Raised);
        frameCalibHV = new QFrame(frameCalibTab);
        frameCalibHV->setObjectName(QString::fromUtf8("frameCalibHV"));
        frameCalibHV->setGeometry(QRect(50, 370, 21, 21));
        frameCalibHV->setStyleSheet(QString::fromUtf8("background-color:rgb(255,255,153,30);\n"
"border: 0px;\n"
"background-image:url(:/transparent.png);\n"
""));
        frameCalibHV->setFrameShape(QFrame::StyledPanel);
        frameCalibHV->setFrameShadow(QFrame::Raised);
        frame_13 = new QFrame(frameCalibHV);
        frame_13->setObjectName(QString::fromUtf8("frame_13"));
        frame_13->setGeometry(QRect(90, 20, 231, 91));
        frame_13->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(74, 74, 74,30);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:10px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        frame_13->setFrameShape(QFrame::StyledPanel);
        frame_13->setFrameShadow(QFrame::Raised);
        calibHVframeMainPower = new QFrame(frame_13);
        calibHVframeMainPower->setObjectName(QString::fromUtf8("calibHVframeMainPower"));
        calibHVframeMainPower->setGeometry(QRect(25, 30, 61, 51));
        calibHVframeMainPower->setStyleSheet(QString::fromUtf8("background-color:rgba(255,255,255,0);\n"
"border-image:url(:/power_g.png);\n"
"border:0px;\n"
"border-radius:0px;\n"
""));
        calibHVframeMainPower->setFrameShape(QFrame::StyledPanel);
        calibHVframeMainPower->setFrameShadow(QFrame::Raised);
        label_48 = new QLabel(frame_13);
        label_48->setObjectName(QString::fromUtf8("label_48"));
        label_48->setGeometry(QRect(10, 0, 211, 26));
        QFont font4;
        font4.setFamily(QString::fromUtf8("Bitstream Charter"));
        font4.setPointSize(14);
        font4.setBold(true);
        font4.setWeight(75);
        label_48->setFont(font4);
        label_48->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 153);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        label_48->setAlignment(Qt::AlignCenter);
        calibHVmainPower = new QLabel(frame_13);
        calibHVmainPower->setObjectName(QString::fromUtf8("calibHVmainPower"));
        calibHVmainPower->setGeometry(QRect(135, 30, 76, 51));
        QFont font5;
        font5.setFamily(QString::fromUtf8("Bitstream Charter"));
        font5.setPointSize(18);
        font5.setBold(true);
        font5.setItalic(false);
        font5.setWeight(75);
        font5.setStrikeOut(false);
        calibHVmainPower->setFont(font5);
        calibHVmainPower->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,30);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:5px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        calibHVmainPower->setAlignment(Qt::AlignCenter);
        label_50 = new QLabel(frame_13);
        label_50->setObjectName(QString::fromUtf8("label_50"));
        label_50->setGeometry(QRect(100, 40, 31, 26));
        label_50->setFont(font4);
        label_50->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        calibHVframeWarnVcc = new QFrame(frame_13);
        calibHVframeWarnVcc->setObjectName(QString::fromUtf8("calibHVframeWarnVcc"));
        calibHVframeWarnVcc->setGeometry(QRect(200, 25, 21, 21));
        calibHVframeWarnVcc->setStyleSheet(QString::fromUtf8("\n"
"background-color:rgba(255,255,255,0);\n"
"border-image:url(:/warning.png);\n"
"border:0px;\n"
"border-radius:0px;\n"
"\n"
""));
        calibHVframeWarnVcc->setFrameShape(QFrame::StyledPanel);
        calibHVframeWarnVcc->setFrameShadow(QFrame::Raised);
        frame_22 = new QFrame(frameCalibHV);
        frame_22->setObjectName(QString::fromUtf8("frame_22"));
        frame_22->setGeometry(QRect(90, 125, 231, 101));
        frame_22->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(74, 74, 74,30);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:10px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        frame_22->setFrameShape(QFrame::StyledPanel);
        frame_22->setFrameShadow(QFrame::Raised);
        labelMonitorMas_2 = new QLabel(frame_22);
        labelMonitorMas_2->setObjectName(QString::fromUtf8("labelMonitorMas_2"));
        labelMonitorMas_2->setGeometry(QRect(5, 5, 221, 26));
        labelMonitorMas_2->setFont(font4);
        labelMonitorMas_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 153);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        labelMonitorMas_2->setAlignment(Qt::AlignCenter);
        calibHVeditHV = new QLineEdit(frame_22);
        calibHVeditHV->setObjectName(QString::fromUtf8("calibHVeditHV"));
        calibHVeditHV->setGeometry(QRect(60, 40, 116, 46));
        QFont font6;
        font6.setFamily(QString::fromUtf8("DejaVu Sans"));
        font6.setPointSize(18);
        font6.setBold(false);
        font6.setItalic(false);
        font6.setWeight(9);
        calibHVeditHV->setFont(font6);
        calibHVeditHV->setFocusPolicy(Qt::NoFocus);
        calibHVeditHV->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(79, 79, 79);\n"
"border-left-color: rgb(255, 255, 153);\n"
"border-bottom-color: rgb(255, 255, 153);\n"
"border-top-color: rgb(255, 255, 153);\n"
"font: 75 18pt \"DejaVu Sans\";"));
        calibHVeditHV->setAlignment(Qt::AlignCenter);
        frame_23 = new QFrame(frameCalibHV);
        frame_23->setObjectName(QString::fromUtf8("frame_23"));
        frame_23->setGeometry(QRect(90, 240, 231, 101));
        frame_23->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(74, 74, 74,30);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:10px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        frame_23->setFrameShape(QFrame::StyledPanel);
        frame_23->setFrameShadow(QFrame::Raised);
        labelMonitorMas_3 = new QLabel(frame_23);
        labelMonitorMas_3->setObjectName(QString::fromUtf8("labelMonitorMas_3"));
        labelMonitorMas_3->setGeometry(QRect(10, 5, 211, 26));
        labelMonitorMas_3->setFont(font4);
        labelMonitorMas_3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 153);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        labelMonitorMas_3->setAlignment(Qt::AlignCenter);
        calibHVCalibrateButton = new QPushButton(frame_23);
        calibHVCalibrateButton->setObjectName(QString::fromUtf8("calibHVCalibrateButton"));
        calibHVCalibrateButton->setGeometry(QRect(55, 40, 121, 51));
        calibHVCalibrateButton->setStyleSheet(QString::fromUtf8("background-color: rgb(79, 79, 79);\n"
"border-color: rgb(255, 255, 153);\n"
"color: rgb(255, 255, 153);\n"
"font: 80 16pt \"DejaVu Sans\";\n"
""));
        frameCalib0 = new QFrame(frameCalibTab);
        frameCalib0->setObjectName(QString::fromUtf8("frameCalib0"));
        frameCalib0->setGeometry(QRect(165, 350, 46, 36));
        frameCalib0->setStyleSheet(QString::fromUtf8("border-image:url(:/calib.png);"));
        frameCalib0->setFrameShape(QFrame::StyledPanel);
        frameCalib0->setFrameShadow(QFrame::Raised);
        frameCalibPad = new QFrame(frameCalibTab);
        frameCalibPad->setObjectName(QString::fromUtf8("frameCalibPad"));
        frameCalibPad->setGeometry(QRect(80, 370, 26, 21));
        frameCalibPad->setStyleSheet(QString::fromUtf8("background-color:rgb(255,255,153,30);\n"
"border: 0px;\n"
"background-image:url(:/transparent.png);\n"
""));
        frameCalibPad->setFrameShape(QFrame::StyledPanel);
        frameCalibPad->setFrameShadow(QFrame::Raised);
        calibPadNextButton = new QPushButton(frameCalibPad);
        calibPadNextButton->setObjectName(QString::fromUtf8("calibPadNextButton"));
        calibPadNextButton->setGeometry(QRect(90, 305, 81, 71));
        calibPadNextButton->setFocusPolicy(Qt::NoFocus);
        calibPadNextButton->setStyleSheet(QString::fromUtf8("background-color: rgb(79, 79, 79,0);\n"
"border-image:url(:/play.png);\n"
"border-color: rgb(255, 255, 153);\n"
"color: rgb(255, 255, 153);\n"
"font: 80 16pt \"DejaVu Sans\";\n"
"border-radius:10px;\n"
""));
        calibPadFrameUp = new QFrame(frameCalibPad);
        calibPadFrameUp->setObjectName(QString::fromUtf8("calibPadFrameUp"));
        calibPadFrameUp->setGeometry(QRect(80, 50, 31, 21));
        calibPadFrameUp->setStyleSheet(QString::fromUtf8("border-image:url(:/compr_up.png);\n"
"background-color: rgba(255, 255, 255,0);\n"
"background-image:url(:/transparent.png);\n"
""));
        calibPadFrameUp->setFrameShape(QFrame::StyledPanel);
        calibPadFrameUp->setFrameShadow(QFrame::Raised);
        calibPadCalibratedPosUp = new QLabel(calibPadFrameUp);
        calibPadCalibratedPosUp->setObjectName(QString::fromUtf8("calibPadCalibratedPosUp"));
        calibPadCalibratedPosUp->setGeometry(QRect(185, 145, 56, 16));
        calibPadCalibratedPosUp->setFont(font);
        calibPadCalibratedPosUp->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255,0);\n"
"\n"
"border-color: rgb(255, 255, 153);\n"
"color: rgb(255,255,153);\n"
"border-image:url(:/transparent.png);\n"
"background-image:url(:/transparent.png);"));
        calibPadCalibratedPosUp->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        calibPadEditUp = new QLineEdit(calibPadFrameUp);
        calibPadEditUp->setObjectName(QString::fromUtf8("calibPadEditUp"));
        calibPadEditUp->setGeometry(QRect(145, 165, 96, 46));
        QFont font7;
        font7.setFamily(QString::fromUtf8("DejaVu Sans"));
        font7.setPointSize(22);
        font7.setBold(false);
        font7.setItalic(false);
        font7.setWeight(9);
        calibPadEditUp->setFont(font7);
        calibPadEditUp->setFocusPolicy(Qt::NoFocus);
        calibPadEditUp->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"background-image:url(:/transparent.png);\n"
"border-image:url(:/transparent.png);\n"
"\n"
"color: rgb(79, 79, 79);\n"
"border-left-color: rgb(255, 255, 153);\n"
"border-bottom-color: rgb(255, 255, 153);\n"
"font: 75 22pt \"DejaVu Sans\";\n"
"border-radius:10px;"));
        calibPadEditUp->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        calibPadSequenceLabelUp = new QLabel(calibPadFrameUp);
        calibPadSequenceLabelUp->setObjectName(QString::fromUtf8("calibPadSequenceLabelUp"));
        calibPadSequenceLabelUp->setGeometry(QRect(55, 0, 66, 31));
        calibPadSequenceLabelUp->setFont(font1);
        calibPadSequenceLabelUp->setStyleSheet(QString::fromUtf8("background-image:url(:/transparent.png);\n"
"border-image:url(:/transparent.png);\n"
"\n"
"background-color: rgb(255,255,255,0);\n"
"border-color: rgb(255, 255, 153);\n"
"color: rgb(255,0,0);\n"
""));
        calibPadSequenceLabelUp->setAlignment(Qt::AlignCenter);
        calibPadFrameDwn = new QFrame(frameCalibPad);
        calibPadFrameDwn->setObjectName(QString::fromUtf8("calibPadFrameDwn"));
        calibPadFrameDwn->setGeometry(QRect(130, 40, 26, 36));
        calibPadFrameDwn->setStyleSheet(QString::fromUtf8("border-image:url(:/compr_dwn.png);\n"
"background-color: rgba(255, 255, 255,0);\n"
"background-image:url(:/transparent.png);\n"
""));
        calibPadFrameDwn->setFrameShape(QFrame::StyledPanel);
        calibPadFrameDwn->setFrameShadow(QFrame::Raised);
        calibPadCalibratedPosDwn = new QLabel(calibPadFrameDwn);
        calibPadCalibratedPosDwn->setObjectName(QString::fromUtf8("calibPadCalibratedPosDwn"));
        calibPadCalibratedPosDwn->setGeometry(QRect(190, 200, 56, 16));
        calibPadCalibratedPosDwn->setFont(font);
        calibPadCalibratedPosDwn->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255,0);\n"
"\n"
"border-color: rgb(255, 255, 153);\n"
"color: rgb(255,255,153);\n"
"border-image:url(:/transparent.png);\n"
"background-image:url(:/transparent.png);"));
        calibPadCalibratedPosDwn->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        calibPadSequenceLabelDown = new QLabel(calibPadFrameDwn);
        calibPadSequenceLabelDown->setObjectName(QString::fromUtf8("calibPadSequenceLabelDown"));
        calibPadSequenceLabelDown->setGeometry(QRect(25, 90, 86, 31));
        calibPadSequenceLabelDown->setFont(font1);
        calibPadSequenceLabelDown->setStyleSheet(QString::fromUtf8("background-image:url(:/transparent.png);\n"
"border-image:url(:/transparent.png);\n"
"\n"
"background-color: rgb(255,255,255,0);\n"
"border-color: rgb(255, 255, 153);\n"
"color: rgb(255,0,0);\n"
""));
        calibPadSequenceLabelDown->setAlignment(Qt::AlignCenter);
        calibPadEditDwn = new QLineEdit(calibPadFrameDwn);
        calibPadEditDwn->setObjectName(QString::fromUtf8("calibPadEditDwn"));
        calibPadEditDwn->setGeometry(QRect(230, 20, 96, 46));
        calibPadEditDwn->setFont(font7);
        calibPadEditDwn->setFocusPolicy(Qt::NoFocus);
        calibPadEditDwn->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"background-image:url(:/transparent.png);\n"
"border-image:url(:/transparent.png);\n"
"\n"
"color: rgb(79, 79, 79);\n"
"border-left-color: rgb(255, 255, 153);\n"
"border-bottom-color: rgb(255, 255, 153);\n"
"font: 75 22pt \"DejaVu Sans\";\n"
"border-radius:10px;"));
        calibPadEditDwn->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        calibPadFrameCalibrate = new QFrame(frameCalibPad);
        calibPadFrameCalibrate->setObjectName(QString::fromUtf8("calibPadFrameCalibrate"));
        calibPadFrameCalibrate->setGeometry(QRect(175, 50, 31, 21));
        calibPadFrameCalibrate->setStyleSheet(QString::fromUtf8("border-image:url(:/compr_dwn.png);\n"
"background-color: rgba(255, 255, 255,0);\n"
"border-image:url(:/calibrate.png);\n"
""));
        calibPadFrameCalibrate->setFrameShape(QFrame::StyledPanel);
        calibPadFrameCalibrate->setFrameShadow(QFrame::Raised);
        calibPadSequenceLabelDown_3 = new QLabel(calibPadFrameCalibrate);
        calibPadSequenceLabelDown_3->setObjectName(QString::fromUtf8("calibPadSequenceLabelDown_3"));
        calibPadSequenceLabelDown_3->setGeometry(QRect(5, 250, 111, 41));
        calibPadSequenceLabelDown_3->setFont(font);
        calibPadSequenceLabelDown_3->setStyleSheet(QString::fromUtf8("background-image:url(:/transparent.png);\n"
"border-image:url(:/transparent.png);\n"
"\n"
"background-color: rgb(255,255,255,0);\n"
"border-color: rgb(255, 255, 153);\n"
"color: rgb(255,0,0);\n"
""));
        calibPadSequenceLabelDown_3->setAlignment(Qt::AlignCenter);
        calibPadFrameStore = new QFrame(frameCalibPad);
        calibPadFrameStore->setObjectName(QString::fromUtf8("calibPadFrameStore"));
        calibPadFrameStore->setGeometry(QRect(220, 50, 36, 31));
        calibPadFrameStore->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255,0);\n"
"border-image:url(:/store.png);\n"
""));
        calibPadFrameStore->setFrameShape(QFrame::StyledPanel);
        calibPadFrameStore->setFrameShadow(QFrame::Raised);
        calibPadSequenceLabelDown_4 = new QLabel(calibPadFrameStore);
        calibPadSequenceLabelDown_4->setObjectName(QString::fromUtf8("calibPadSequenceLabelDown_4"));
        calibPadSequenceLabelDown_4->setGeometry(QRect(5, 210, 91, 36));
        calibPadSequenceLabelDown_4->setFont(font);
        calibPadSequenceLabelDown_4->setStyleSheet(QString::fromUtf8("background-image:url(:/transparent.png);\n"
"border-image:url(:/transparent.png);\n"
"\n"
"background-color: rgb(255,255,255,0);\n"
"border-color: rgb(255, 255, 153);\n"
"color: rgb(255,0,0);\n"
""));
        calibPadSequenceLabelDown_4->setAlignment(Qt::AlignCenter);
        calibPadCancelButton = new QPushButton(frameCalibPad);
        calibPadCancelButton->setObjectName(QString::fromUtf8("calibPadCancelButton"));
        calibPadCancelButton->setGeometry(QRect(250, 305, 81, 71));
        calibPadCancelButton->setFocusPolicy(Qt::NoFocus);
        calibPadCancelButton->setStyleSheet(QString::fromUtf8("background-color: rgb(79, 79, 79,0);\n"
"border-image:url(:/stop.png);\n"
"border-color: rgb(255, 255, 153);\n"
"color: rgb(255, 255, 153);\n"
"font: 80 16pt \"DejaVu Sans\";\n"
"border-radius:10px;\n"
""));
        calibPadFrameInit = new QFrame(frameCalibPad);
        calibPadFrameInit->setObjectName(QString::fromUtf8("calibPadFrameInit"));
        calibPadFrameInit->setGeometry(QRect(35, 45, 36, 31));
        calibPadFrameInit->setStyleSheet(QString::fromUtf8("\n"
"background-color: rgba(255, 255, 255,0);\n"
"border-image:url(:/intro_calib_pad2.png);\n"
""));
        calibPadFrameInit->setFrameShape(QFrame::StyledPanel);
        calibPadFrameInit->setFrameShadow(QFrame::Raised);
        calibPadCalibratedPosInit = new QLabel(calibPadFrameInit);
        calibPadCalibratedPosInit->setObjectName(QString::fromUtf8("calibPadCalibratedPosInit"));
        calibPadCalibratedPosInit->setGeometry(QRect(235, 125, 56, 16));
        calibPadCalibratedPosInit->setFont(font);
        calibPadCalibratedPosInit->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255,0);\n"
"\n"
"border-color: rgb(255, 255, 153);\n"
"color: rgb(255,255,153);\n"
"border-image:url(:/transparent.png);\n"
"background-image:url(:/transparent.png);"));
        calibPadCalibratedPosInit->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        calibPadSequenceLabelDown_2 = new QLabel(calibPadFrameInit);
        calibPadSequenceLabelDown_2->setObjectName(QString::fromUtf8("calibPadSequenceLabelDown_2"));
        calibPadSequenceLabelDown_2->setGeometry(QRect(30, 240, 261, 51));
        calibPadSequenceLabelDown_2->setFont(font);
        calibPadSequenceLabelDown_2->setStyleSheet(QString::fromUtf8("background-image:url(:/transparent.png);\n"
"border-image:url(:/transparent.png);\n"
"\n"
"background-color: rgb(255,255,255,0);\n"
"border-color: rgb(255, 255, 153);\n"
"color: rgb(255,0,0);\n"
""));
        calibPadSequenceLabelDown_2->setAlignment(Qt::AlignCenter);
        frameCalibCompressor = new QFrame(frameCalibTab);
        frameCalibCompressor->setObjectName(QString::fromUtf8("frameCalibCompressor"));
        frameCalibCompressor->setGeometry(QRect(225, 355, 26, 36));
        frameCalibCompressor->setStyleSheet(QString::fromUtf8("background-color:rgb(255,255,153,30);\n"
"border: 0px;\n"
"background-image:url(:/transparent.png);\n"
""));
        frameCalibCompressor->setFrameShape(QFrame::StyledPanel);
        frameCalibCompressor->setFrameShadow(QFrame::Raised);
        calibComprNextButton = new QPushButton(frameCalibCompressor);
        calibComprNextButton->setObjectName(QString::fromUtf8("calibComprNextButton"));
        calibComprNextButton->setGeometry(QRect(90, 305, 81, 71));
        calibComprNextButton->setFocusPolicy(Qt::NoFocus);
        calibComprNextButton->setStyleSheet(QString::fromUtf8("background-color: rgb(79, 79, 79,0);\n"
"border-image:url(:/play.png);\n"
"border-color: rgb(255, 255, 153);\n"
"color: rgb(255, 255, 153);\n"
"font: 80 16pt \"DejaVu Sans\";\n"
"border-radius:10px;\n"
""));
        calibComprFrameF1 = new QFrame(frameCalibCompressor);
        calibComprFrameF1->setObjectName(QString::fromUtf8("calibComprFrameF1"));
        calibComprFrameF1->setGeometry(QRect(155, 65, 31, 26));
        calibComprFrameF1->setStyleSheet(QString::fromUtf8("\n"
"background-color: rgba(255, 255, 255,0);\n"
"background-image:url(:/transparent.png);\n"
"border-image:url(:/calib_compr_f1.png);"));
        calibComprFrameF1->setFrameShape(QFrame::StyledPanel);
        calibComprFrameF1->setFrameShadow(QFrame::Raised);
        calibComprEditF1 = new QLineEdit(calibComprFrameF1);
        calibComprEditF1->setObjectName(QString::fromUtf8("calibComprEditF1"));
        calibComprEditF1->setGeometry(QRect(200, 105, 96, 46));
        calibComprEditF1->setFont(font7);
        calibComprEditF1->setFocusPolicy(Qt::NoFocus);
        calibComprEditF1->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"background-image:url(:/transparent.png);\n"
"border-image:url(:/transparent.png);\n"
"\n"
"color: rgb(79, 79, 79);\n"
"border-left-color: rgb(255, 255, 153);\n"
"border-bottom-color: rgb(255, 255, 153);\n"
"font: 75 22pt \"DejaVu Sans\";\n"
"border-radius:10px;"));
        calibComprEditF1->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        calibComprRawData = new QLabel(calibComprFrameF1);
        calibComprRawData->setObjectName(QString::fromUtf8("calibComprRawData"));
        calibComprRawData->setGeometry(QRect(132, 175, 31, 16));
        calibComprRawData->setFont(font2);
        calibComprRawData->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255,0);\n"
"\n"
"border-color: rgb(255, 255, 153);\n"
"color: rgb(0,0,0);\n"
"border-image:url(:/transparent.png);\n"
"background-image:url(:/transparent.png);"));
        calibComprRawData->setAlignment(Qt::AlignCenter);
        calibComprFrameOffset = new QFrame(frameCalibCompressor);
        calibComprFrameOffset->setObjectName(QString::fromUtf8("calibComprFrameOffset"));
        calibComprFrameOffset->setGeometry(QRect(115, 70, 16, 16));
        calibComprFrameOffset->setStyleSheet(QString::fromUtf8("\n"
"background-color: rgba(255, 255, 255,0);\n"
"background-image:url(:/transparent.png);\n"
"border-image:url(:/calib_compr_offset.png);\n"
""));
        calibComprFrameOffset->setFrameShape(QFrame::StyledPanel);
        calibComprFrameOffset->setFrameShadow(QFrame::Raised);
        calibPadSequenceLabelDown_5 = new QLabel(calibComprFrameOffset);
        calibPadSequenceLabelDown_5->setObjectName(QString::fromUtf8("calibPadSequenceLabelDown_5"));
        calibPadSequenceLabelDown_5->setGeometry(QRect(5, 240, 146, 46));
        calibPadSequenceLabelDown_5->setFont(font1);
        calibPadSequenceLabelDown_5->setStyleSheet(QString::fromUtf8("background-image:url(:/transparent.png);\n"
"border-image:url(:/transparent.png);\n"
"\n"
"background-color: rgb(255,255,255,0);\n"
"border-color: rgb(255, 255, 153);\n"
"color: rgb(255,0,0);\n"
""));
        calibPadSequenceLabelDown_5->setAlignment(Qt::AlignCenter);
        calibComprFrameCalibrate = new QFrame(frameCalibCompressor);
        calibComprFrameCalibrate->setObjectName(QString::fromUtf8("calibComprFrameCalibrate"));
        calibComprFrameCalibrate->setGeometry(QRect(270, 65, 31, 21));
        calibComprFrameCalibrate->setStyleSheet(QString::fromUtf8("border-image:url(:/compr_dwn.png);\n"
"background-color: rgba(255, 255, 255,0);\n"
"border-image:url(:/calibrate.png);\n"
""));
        calibComprFrameCalibrate->setFrameShape(QFrame::StyledPanel);
        calibComprFrameCalibrate->setFrameShadow(QFrame::Raised);
        calibPadSequenceLabelDown_6 = new QLabel(calibComprFrameCalibrate);
        calibPadSequenceLabelDown_6->setObjectName(QString::fromUtf8("calibPadSequenceLabelDown_6"));
        calibPadSequenceLabelDown_6->setGeometry(QRect(5, 250, 111, 41));
        calibPadSequenceLabelDown_6->setFont(font);
        calibPadSequenceLabelDown_6->setStyleSheet(QString::fromUtf8("background-image:url(:/transparent.png);\n"
"border-image:url(:/transparent.png);\n"
"\n"
"background-color: rgb(255,255,255,0);\n"
"border-color: rgb(255, 255, 153);\n"
"color: rgb(255,0,0);\n"
""));
        calibPadSequenceLabelDown_6->setAlignment(Qt::AlignCenter);
        calibComprFrameStore = new QFrame(frameCalibCompressor);
        calibComprFrameStore->setObjectName(QString::fromUtf8("calibComprFrameStore"));
        calibComprFrameStore->setGeometry(QRect(280, 105, 16, 21));
        calibComprFrameStore->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255,0);\n"
"border-image:url(:/store.png);\n"
""));
        calibComprFrameStore->setFrameShape(QFrame::StyledPanel);
        calibComprFrameStore->setFrameShadow(QFrame::Raised);
        calibPadSequenceLabelDown_7 = new QLabel(calibComprFrameStore);
        calibPadSequenceLabelDown_7->setObjectName(QString::fromUtf8("calibPadSequenceLabelDown_7"));
        calibPadSequenceLabelDown_7->setGeometry(QRect(5, 210, 91, 36));
        calibPadSequenceLabelDown_7->setFont(font);
        calibPadSequenceLabelDown_7->setStyleSheet(QString::fromUtf8("background-image:url(:/transparent.png);\n"
"border-image:url(:/transparent.png);\n"
"\n"
"background-color: rgb(255,255,255,0);\n"
"border-color: rgb(255, 255, 153);\n"
"color: rgb(255,0,0);\n"
""));
        calibPadSequenceLabelDown_7->setAlignment(Qt::AlignCenter);
        calibComprCancelButton = new QPushButton(frameCalibCompressor);
        calibComprCancelButton->setObjectName(QString::fromUtf8("calibComprCancelButton"));
        calibComprCancelButton->setGeometry(QRect(250, 305, 81, 71));
        calibComprCancelButton->setFocusPolicy(Qt::NoFocus);
        calibComprCancelButton->setStyleSheet(QString::fromUtf8("background-color: rgb(79, 79, 79,0);\n"
"border-image:url(:/stop.png);\n"
"border-color: rgb(255, 255, 153);\n"
"color: rgb(255, 255, 153);\n"
"font: 80 16pt \"DejaVu Sans\";\n"
"border-radius:10px;\n"
""));
        calibComprFrameInit = new QFrame(frameCalibCompressor);
        calibComprFrameInit->setObjectName(QString::fromUtf8("calibComprFrameInit"));
        calibComprFrameInit->setGeometry(QRect(60, 65, 21, 36));
        calibComprFrameInit->setStyleSheet(QString::fromUtf8("\n"
"background-color: rgba(255, 255, 255,0);\n"
"border-image:url(:/calib_compr_intro.png);\n"
""));
        calibComprFrameInit->setFrameShape(QFrame::StyledPanel);
        calibComprFrameInit->setFrameShadow(QFrame::Raised);
        calibComprCalibratedComprInit = new QLabel(calibComprFrameInit);
        calibComprCalibratedComprInit->setObjectName(QString::fromUtf8("calibComprCalibratedComprInit"));
        calibComprCalibratedComprInit->setGeometry(QRect(135, 130, 56, 16));
        calibComprCalibratedComprInit->setFont(font);
        calibComprCalibratedComprInit->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255,0);\n"
"\n"
"border-color: rgb(255, 255, 153);\n"
"color: rgb(0,0,0);\n"
"border-image:url(:/transparent.png);\n"
"background-image:url(:/transparent.png);"));
        calibComprCalibratedComprInit->setAlignment(Qt::AlignCenter);
        calibComprSequenceLabelIntro = new QLabel(calibComprFrameInit);
        calibComprSequenceLabelIntro->setObjectName(QString::fromUtf8("calibComprSequenceLabelIntro"));
        calibComprSequenceLabelIntro->setGeometry(QRect(30, 240, 261, 51));
        calibComprSequenceLabelIntro->setFont(font);
        calibComprSequenceLabelIntro->setStyleSheet(QString::fromUtf8("background-image:url(:/transparent.png);\n"
"border-image:url(:/transparent.png);\n"
"\n"
"background-color: rgb(255,255,255,0);\n"
"border-color: rgb(255, 255, 153);\n"
"color: rgb(255,0,0);\n"
""));
        calibComprSequenceLabelIntro->setAlignment(Qt::AlignCenter);
        calibComprFrameF2 = new QFrame(frameCalibCompressor);
        calibComprFrameF2->setObjectName(QString::fromUtf8("calibComprFrameF2"));
        calibComprFrameF2->setGeometry(QRect(225, 55, 31, 36));
        calibComprFrameF2->setStyleSheet(QString::fromUtf8("\n"
"background-color: rgba(255, 255, 255,0);\n"
"background-image:url(:/transparent.png);\n"
"border-image:url(:/calib_compr_f2.png);"));
        calibComprFrameF2->setFrameShape(QFrame::StyledPanel);
        calibComprFrameF2->setFrameShadow(QFrame::Raised);
        calibComprEditF2 = new QLineEdit(calibComprFrameF2);
        calibComprEditF2->setObjectName(QString::fromUtf8("calibComprEditF2"));
        calibComprEditF2->setGeometry(QRect(220, 105, 96, 46));
        calibComprEditF2->setFont(font7);
        calibComprEditF2->setFocusPolicy(Qt::NoFocus);
        calibComprEditF2->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"background-image:url(:/transparent.png);\n"
"border-image:url(:/transparent.png);\n"
"\n"
"color: rgb(79, 79, 79);\n"
"border-left-color: rgb(255, 255, 153);\n"
"border-bottom-color: rgb(255, 255, 153);\n"
"font: 75 22pt \"DejaVu Sans\";\n"
"border-radius:10px;"));
        calibComprEditF2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        calibPadTabButton = new QPushButton(frame4);
        calibPadTabButton->setObjectName(QString::fromUtf8("calibPadTabButton"));
        calibPadTabButton->setGeometry(QRect(30, 450, 91, 31));
        calibPadTabButton->setStyleSheet(QString::fromUtf8("border-image:url(:/fold_off2_butt.png);\n"
"border: 0px;\n"
"outline: 0px;\n"
"color: rgb(255, 255, 153);\n"
"font: 80 14pt \"DejaVu Sans\";\n"
""));
        calibComprTabButton = new QPushButton(frame4);
        calibComprTabButton->setObjectName(QString::fromUtf8("calibComprTabButton"));
        calibComprTabButton->setGeometry(QRect(112, 450, 91, 31));
        calibComprTabButton->setStyleSheet(QString::fromUtf8("border-image:url(:/fold_off2_butt.png);\n"
"border: 0px;\n"
"outline: 0px;\n"
"color: rgb(255, 255, 153);\n"
"font: 80 14pt \"DejaVu Sans\";\n"
""));
        calibHVTabButton = new QPushButton(frame4);
        calibHVTabButton->setObjectName(QString::fromUtf8("calibHVTabButton"));
        calibHVTabButton->setGeometry(QRect(195, 450, 91, 31));
        calibHVTabButton->setStyleSheet(QString::fromUtf8("border-image:url(:/fold_off2_butt.png);\n"
"border: 0px;\n"
"outline: 0px;\n"
"color: rgb(255, 255, 153);\n"
"font: 80 14pt \"DejaVu Sans\";\n"
""));
        calibTiltTabButton = new QPushButton(frame4);
        calibTiltTabButton->setObjectName(QString::fromUtf8("calibTiltTabButton"));
        calibTiltTabButton->setGeometry(QRect(278, 450, 91, 31));
        calibTiltTabButton->setStyleSheet(QString::fromUtf8("border-image:url(:/fold_off2_butt.png);\n"
"border: 0px;\n"
"outline: 0px;\n"
"color: rgb(255, 255, 153);\n"
"font: 80 14pt \"DejaVu Sans\";\n"
""));
        calibKvTabButton = new QPushButton(frame4);
        calibKvTabButton->setObjectName(QString::fromUtf8("calibKvTabButton"));
        calibKvTabButton->setGeometry(QRect(360, 450, 91, 31));
        calibKvTabButton->setStyleSheet(QString::fromUtf8("border-image:url(:/fold_off2_butt.png);\n"
"border: 0px;\n"
"outline: 0px;\n"
"color: rgb(255, 255, 153);\n"
"font: 80 14pt \"DejaVu Sans\";\n"
""));

        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", 0, QApplication::UnicodeUTF8));
        calibPadSequenceLabelDown_8->setText(QApplication::translate("Form", "CALIBRATING ...", 0, QApplication::UnicodeUTF8));
        calibrateTiltNextButton->setText(QString());
        calibPadSequenceLabelDown_9->setText(QApplication::translate("Form", "Calibrate", 0, QApplication::UnicodeUTF8));
        calibTiltPcb249TrxData->setText(QApplication::translate("Form", "200", 0, QApplication::UnicodeUTF8));
        calibTiltPcb204TrxData->setText(QApplication::translate("Form", "200", 0, QApplication::UnicodeUTF8));
        calibPadSequenceLabelDown_10->setText(QApplication::translate("Form", "(pcb249)", 0, QApplication::UnicodeUTF8));
        calibPadSequenceLabelDown_11->setText(QApplication::translate("Form", "(pcb204)", 0, QApplication::UnicodeUTF8));
        calibPadSequenceLabelDown_12->setText(QApplication::translate("Form", "TRX", 0, QApplication::UnicodeUTF8));
        calibTiltArmData->setText(QApplication::translate("Form", "200", 0, QApplication::UnicodeUTF8));
        calibPadSequenceLabelDown_13->setText(QApplication::translate("Form", "ARM", 0, QApplication::UnicodeUTF8));
        label_48->setText(QApplication::translate("Form", "CURRENT POWER SUPPLY", 0, QApplication::UnicodeUTF8));
        calibHVmainPower->setText(QApplication::translate("Form", "--", 0, QApplication::UnicodeUTF8));
        label_50->setText(QApplication::translate("Form", "(V)", 0, QApplication::UnicodeUTF8));
        labelMonitorMas_2->setText(QApplication::translate("Form", "Insert a measured value", 0, QApplication::UnicodeUTF8));
        calibHVeditHV->setText(QApplication::translate("Form", "0", 0, QApplication::UnicodeUTF8));
        labelMonitorMas_3->setText(QApplication::translate("Form", "Press to Calibrate", 0, QApplication::UnicodeUTF8));
        calibHVCalibrateButton->setText(QApplication::translate("Form", "CALIBRATE", 0, QApplication::UnicodeUTF8));
        calibPadNextButton->setText(QString());
        calibPadCalibratedPosUp->setText(QApplication::translate("Form", "200", 0, QApplication::UnicodeUTF8));
        calibPadEditUp->setText(QApplication::translate("Form", "0", 0, QApplication::UnicodeUTF8));
        calibPadSequenceLabelUp->setText(QApplication::translate("Form", "UP", 0, QApplication::UnicodeUTF8));
        calibPadCalibratedPosDwn->setText(QApplication::translate("Form", "200", 0, QApplication::UnicodeUTF8));
        calibPadSequenceLabelDown->setText(QApplication::translate("Form", "DOWN", 0, QApplication::UnicodeUTF8));
        calibPadEditDwn->setText(QApplication::translate("Form", "0", 0, QApplication::UnicodeUTF8));
        calibPadSequenceLabelDown_3->setText(QApplication::translate("Form", "PRESS TO\n"
" CALIBRATE ", 0, QApplication::UnicodeUTF8));
        calibPadSequenceLabelDown_4->setText(QApplication::translate("Form", "PRESS\n"
"TO STORE", 0, QApplication::UnicodeUTF8));
        calibPadCancelButton->setText(QString());
        calibPadCalibratedPosInit->setText(QApplication::translate("Form", "200", 0, QApplication::UnicodeUTF8));
        calibPadSequenceLabelDown_2->setText(QApplication::translate("Form", "PRESS PLAY\n"
" TO START CALIBRATION ", 0, QApplication::UnicodeUTF8));
        calibComprNextButton->setText(QString());
        calibComprEditF1->setText(QApplication::translate("Form", "0", 0, QApplication::UnicodeUTF8));
        calibComprRawData->setText(QApplication::translate("Form", "200", 0, QApplication::UnicodeUTF8));
        calibPadSequenceLabelDown_5->setText(QApplication::translate("Form", "press button\n"
"to get offset", 0, QApplication::UnicodeUTF8));
        calibPadSequenceLabelDown_6->setText(QApplication::translate("Form", "PRESS TO\n"
" CALIBRATE ", 0, QApplication::UnicodeUTF8));
        calibPadSequenceLabelDown_7->setText(QApplication::translate("Form", "PRESS\n"
"TO STORE", 0, QApplication::UnicodeUTF8));
        calibComprCancelButton->setText(QString());
        calibComprCalibratedComprInit->setText(QApplication::translate("Form", "200", 0, QApplication::UnicodeUTF8));
        calibComprSequenceLabelIntro->setText(QApplication::translate("Form", "PRESS PLAY\n"
" TO START CALIBRATION ", 0, QApplication::UnicodeUTF8));
        calibComprEditF2->setText(QApplication::translate("Form", "0", 0, QApplication::UnicodeUTF8));
        calibPadTabButton->setText(QApplication::translate("Form", "Pad", 0, QApplication::UnicodeUTF8));
        calibComprTabButton->setText(QApplication::translate("Form", "Force", 0, QApplication::UnicodeUTF8));
        calibHVTabButton->setText(QApplication::translate("Form", "HV", 0, QApplication::UnicodeUTF8));
        calibTiltTabButton->setText(QApplication::translate("Form", "TILT", 0, QApplication::UnicodeUTF8));
        calibKvTabButton->setText(QApplication::translate("Form", "KV", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CALIBTAB_H
