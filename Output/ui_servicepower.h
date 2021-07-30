/********************************************************************************
** Form generated from reading UI file 'servicepower.ui'
**
** Created: Fri Sep 13 11:22:47 2019
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVICEPOWER_H
#define UI_SERVICEPOWER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ServicePower
{
public:
    QFrame *frame;
    QPushButton *exitButton;
    QFrame *frame_4;
    QFrame *hVDC_warn_icon;
    QLabel *label_6;
    QLabel *hVDC_value;
    QLabel *acdc_value;
    QFrame *acdc_warn_icon;
    QLabel *rot_value;
    QFrame *rot_warn_icon;
    QLabel *synt_value;
    QFrame *synt_warn_icon;
    QLabel *label_29;
    QLabel *label_32;
    QLabel *label_33;
    QLabel *label_34;
    QFrame *frame_11;
    QFrame *frameGroundTest;
    QLabel *anodicaTest;
    QLabel *masTest;
    QLabel *label_16;
    QLabel *label_18;
    QFrame *masmeter_warn_icon;
    QFrame *mastest_warn_icon;
    QLabel *label_14;
    QFrame *frame_5;
    QFrame *frameMainPower_2;
    QLabel *tubeNameLabel;
    QLabel *label_8;
    QLabel *label_10;
    QLabel *tempAmp;
    QLabel *label_12;
    QLabel *fuoco;
    QLabel *tempTubo;
    QLabel *iFil;
    QFrame *temp_amp_warn_icon;
    QFrame *temp_tube_warn_icon;
    QLabel *statShotsLabel;
    QLabel *statkHuLabel;
    QLabel *max_kV_label;
    QFrame *frame_12;
    QLabel *label_22;
    QLabel *label_28;
    QLabel *v32;
    QLabel *vm32;
    QLabel *label_30;
    QLabel *v12;
    QLabel *label_31;
    QLabel *vm12;
    QLabel *label_37;
    QLabel *v15;
    QLabel *label_39;
    QLabel *v15e;
    QLabel *label_40;
    QFrame *pcb190_v32_warn_icon;
    QFrame *pcb190_m32__warn_icon;
    QFrame *pcb190_v12_warn_icon;
    QFrame *pcb190_m12_warn_icon;
    QFrame *pcb190_v15_warn_icon;
    QFrame *pcb190_ext15_warn_icon;
    QLabel *serviceSetupDiagnosticLabel;
    QFrame *frame_6;
    QLabel *label_7;
    QLabel *synt_value_2;
    QFrame *synt_warn_icon_2;
    QLabel *label_41;
    QFrame *frame_2;

    void setupUi(QWidget *ServicePower)
    {
        if (ServicePower->objectName().isEmpty())
            ServicePower->setObjectName(QString::fromUtf8("ServicePower"));
        ServicePower->resize(800, 480);
        ServicePower->setStyleSheet(QString::fromUtf8("background-color:rgb(0,0,0,0);\n"
"background-image:url(://Service_Power/Service_Power/power_window.png);\n"
"border: 0px;"));
        frame = new QFrame(ServicePower);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(0, 0, 800, 480));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        exitButton = new QPushButton(frame);
        exitButton->setObjectName(QString::fromUtf8("exitButton"));
        exitButton->setGeometry(QRect(0, 0, 111, 111));
        frame_4 = new QFrame(frame);
        frame_4->setObjectName(QString::fromUtf8("frame_4"));
        frame_4->setGeometry(QRect(345, 195, 431, 91));
        frame_4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(74, 74, 74,30);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:10px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        hVDC_warn_icon = new QFrame(frame_4);
        hVDC_warn_icon->setObjectName(QString::fromUtf8("hVDC_warn_icon"));
        hVDC_warn_icon->setGeometry(QRect(10, 65, 26, 21));
        hVDC_warn_icon->setStyleSheet(QString::fromUtf8("background-color:rgba(255,255,255,0);\n"
"border-image:url(:/power_g.png);\n"
"border:0px;\n"
"border-radius:0px;\n"
""));
        hVDC_warn_icon->setFrameShape(QFrame::StyledPanel);
        hVDC_warn_icon->setFrameShadow(QFrame::Raised);
        label_6 = new QLabel(frame_4);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(20, 0, 396, 21));
        QFont font;
        font.setFamily(QString::fromUtf8("Bitstream Charter"));
        font.setPointSize(14);
        font.setBold(true);
        font.setWeight(75);
        label_6->setFont(font);
        label_6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 153);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        label_6->setAlignment(Qt::AlignCenter);
        hVDC_value = new QLabel(frame_4);
        hVDC_value->setObjectName(QString::fromUtf8("hVDC_value"));
        hVDC_value->setGeometry(QRect(15, 40, 76, 41));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Bitstream Charter"));
        font1.setPointSize(18);
        font1.setBold(true);
        font1.setItalic(false);
        font1.setWeight(75);
        font1.setStrikeOut(false);
        hVDC_value->setFont(font1);
        hVDC_value->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,30);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:5px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        hVDC_value->setAlignment(Qt::AlignCenter);
        acdc_value = new QLabel(frame_4);
        acdc_value->setObjectName(QString::fromUtf8("acdc_value"));
        acdc_value->setGeometry(QRect(115, 40, 76, 41));
        acdc_value->setFont(font1);
        acdc_value->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,30);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:5px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        acdc_value->setAlignment(Qt::AlignCenter);
        acdc_warn_icon = new QFrame(frame_4);
        acdc_warn_icon->setObjectName(QString::fromUtf8("acdc_warn_icon"));
        acdc_warn_icon->setGeometry(QRect(110, 65, 26, 21));
        acdc_warn_icon->setStyleSheet(QString::fromUtf8("background-color:rgba(255,255,255,0);\n"
"border-image:url(:/power_g.png);\n"
"border:0px;\n"
"border-radius:0px;\n"
""));
        acdc_warn_icon->setFrameShape(QFrame::StyledPanel);
        acdc_warn_icon->setFrameShadow(QFrame::Raised);
        rot_value = new QLabel(frame_4);
        rot_value->setObjectName(QString::fromUtf8("rot_value"));
        rot_value->setGeometry(QRect(215, 40, 76, 41));
        rot_value->setFont(font1);
        rot_value->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,30);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:5px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        rot_value->setAlignment(Qt::AlignCenter);
        rot_warn_icon = new QFrame(frame_4);
        rot_warn_icon->setObjectName(QString::fromUtf8("rot_warn_icon"));
        rot_warn_icon->setGeometry(QRect(210, 65, 26, 21));
        rot_warn_icon->setStyleSheet(QString::fromUtf8("background-color:rgba(255,255,255,0);\n"
"border-image:url(:/power_g.png);\n"
"border:0px;\n"
"border-radius:0px;\n"
""));
        rot_warn_icon->setFrameShape(QFrame::StyledPanel);
        rot_warn_icon->setFrameShadow(QFrame::Raised);
        synt_value = new QLabel(frame_4);
        synt_value->setObjectName(QString::fromUtf8("synt_value"));
        synt_value->setGeometry(QRect(315, 40, 76, 41));
        synt_value->setFont(font1);
        synt_value->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,30);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:5px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        synt_value->setAlignment(Qt::AlignCenter);
        synt_warn_icon = new QFrame(frame_4);
        synt_warn_icon->setObjectName(QString::fromUtf8("synt_warn_icon"));
        synt_warn_icon->setGeometry(QRect(310, 65, 26, 21));
        synt_warn_icon->setStyleSheet(QString::fromUtf8("background-color:rgba(255,255,255,0);\n"
"border-image:url(:/power_g.png);\n"
"border:0px;\n"
"border-radius:0px;\n"
""));
        synt_warn_icon->setFrameShape(QFrame::StyledPanel);
        synt_warn_icon->setFrameShadow(QFrame::Raised);
        label_29 = new QLabel(frame_4);
        label_29->setObjectName(QString::fromUtf8("label_29"));
        label_29->setGeometry(QRect(20, 17, 46, 26));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Bitstream Charter"));
        font2.setPointSize(12);
        font2.setBold(true);
        font2.setWeight(75);
        label_29->setFont(font2);
        label_29->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        label_29->setAlignment(Qt::AlignCenter);
        label_32 = new QLabel(frame_4);
        label_32->setObjectName(QString::fromUtf8("label_32"));
        label_32->setGeometry(QRect(120, 17, 46, 26));
        label_32->setFont(font2);
        label_32->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        label_32->setAlignment(Qt::AlignCenter);
        label_33 = new QLabel(frame_4);
        label_33->setObjectName(QString::fromUtf8("label_33"));
        label_33->setGeometry(QRect(220, 17, 46, 26));
        label_33->setFont(font2);
        label_33->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        label_33->setAlignment(Qt::AlignCenter);
        label_34 = new QLabel(frame_4);
        label_34->setObjectName(QString::fromUtf8("label_34"));
        label_34->setGeometry(QRect(320, 17, 46, 26));
        label_34->setFont(font2);
        label_34->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        label_34->setAlignment(Qt::AlignCenter);
        frame_11 = new QFrame(frame);
        frame_11->setObjectName(QString::fromUtf8("frame_11"));
        frame_11->setGeometry(QRect(115, 85, 196, 91));
        frame_11->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(74, 74, 74,30);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:10px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        frame_11->setFrameShape(QFrame::StyledPanel);
        frame_11->setFrameShadow(QFrame::Raised);
        frameGroundTest = new QFrame(frame_11);
        frameGroundTest->setObjectName(QString::fromUtf8("frameGroundTest"));
        frameGroundTest->setGeometry(QRect(10, 30, 51, 51));
        frameGroundTest->setStyleSheet(QString::fromUtf8("border-image:url(:/faultR16.png);\n"
"background-color:rgba(255,255,255,0);\n"
"border:0px;\n"
"border-radius:0px;\n"
""));
        frameGroundTest->setFrameShape(QFrame::StyledPanel);
        frameGroundTest->setFrameShadow(QFrame::Raised);
        anodicaTest = new QLabel(frame_11);
        anodicaTest->setObjectName(QString::fromUtf8("anodicaTest"));
        anodicaTest->setGeometry(QRect(120, 55, 61, 26));
        anodicaTest->setFont(font1);
        anodicaTest->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,30);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:5px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        anodicaTest->setAlignment(Qt::AlignCenter);
        masTest = new QLabel(frame_11);
        masTest->setObjectName(QString::fromUtf8("masTest"));
        masTest->setGeometry(QRect(120, 25, 61, 26));
        masTest->setFont(font1);
        masTest->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,30);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:5px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        masTest->setAlignment(Qt::AlignCenter);
        label_16 = new QLabel(frame_11);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(80, 25, 36, 26));
        label_16->setFont(font2);
        label_16->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        label_18 = new QLabel(frame_11);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setGeometry(QRect(75, 55, 41, 26));
        label_18->setFont(font2);
        label_18->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        masmeter_warn_icon = new QFrame(frame_11);
        masmeter_warn_icon->setObjectName(QString::fromUtf8("masmeter_warn_icon"));
        masmeter_warn_icon->setGeometry(QRect(170, 20, 21, 21));
        masmeter_warn_icon->setStyleSheet(QString::fromUtf8("\n"
"background-color:rgba(255,255,255,0);\n"
"border-image:url(:/warning.png);\n"
"border:0px;\n"
"border-radius:0px;\n"
"\n"
""));
        masmeter_warn_icon->setFrameShape(QFrame::StyledPanel);
        masmeter_warn_icon->setFrameShadow(QFrame::Raised);
        mastest_warn_icon = new QFrame(frame_11);
        mastest_warn_icon->setObjectName(QString::fromUtf8("mastest_warn_icon"));
        mastest_warn_icon->setGeometry(QRect(170, 50, 21, 21));
        mastest_warn_icon->setStyleSheet(QString::fromUtf8("\n"
"background-color:rgba(255,255,255,0);\n"
"border-image:url(:/warning.png);\n"
"border:0px;\n"
"border-radius:0px;\n"
"\n"
""));
        mastest_warn_icon->setFrameShape(QFrame::StyledPanel);
        mastest_warn_icon->setFrameShadow(QFrame::Raised);
        label_14 = new QLabel(frame_11);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(5, 0, 136, 26));
        label_14->setFont(font);
        label_14->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 153);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        frame_5 = new QFrame(frame);
        frame_5->setObjectName(QString::fromUtf8("frame_5"));
        frame_5->setGeometry(QRect(345, 85, 431, 91));
        frame_5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(74, 74, 74,30);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:10px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        frame_5->setFrameShape(QFrame::StyledPanel);
        frame_5->setFrameShadow(QFrame::Raised);
        frameMainPower_2 = new QFrame(frame_5);
        frameMainPower_2->setObjectName(QString::fromUtf8("frameMainPower_2"));
        frameMainPower_2->setGeometry(QRect(10, 35, 51, 31));
        frameMainPower_2->setStyleSheet(QString::fromUtf8("background-color:rgba(255,255,255,0);\n"
"border-image:url(:/tubo.png);\n"
"border:0px;\n"
"border-radius:0px;\n"
""));
        frameMainPower_2->setFrameShape(QFrame::StyledPanel);
        frameMainPower_2->setFrameShadow(QFrame::Raised);
        tubeNameLabel = new QLabel(frame_5);
        tubeNameLabel->setObjectName(QString::fromUtf8("tubeNameLabel"));
        tubeNameLabel->setGeometry(QRect(10, 5, 146, 26));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tubeNameLabel->sizePolicy().hasHeightForWidth());
        tubeNameLabel->setSizePolicy(sizePolicy);
        tubeNameLabel->setFont(font2);
        tubeNameLabel->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 153);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        label_8 = new QLabel(frame_5);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(250, 5, 76, 26));
        label_8->setFont(font2);
        label_8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        label_10 = new QLabel(frame_5);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(165, 5, 76, 26));
        label_10->setFont(font2);
        label_10->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        tempAmp = new QLabel(frame_5);
        tempAmp->setObjectName(QString::fromUtf8("tempAmp"));
        tempAmp->setGeometry(QRect(250, 35, 76, 31));
        tempAmp->setFont(font1);
        tempAmp->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,30);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:5px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        tempAmp->setAlignment(Qt::AlignCenter);
        label_12 = new QLabel(frame_5);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(335, 5, 71, 26));
        label_12->setFont(font2);
        label_12->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        fuoco = new QLabel(frame_5);
        fuoco->setObjectName(QString::fromUtf8("fuoco"));
        fuoco->setGeometry(QRect(15, 60, 46, 26));
        fuoco->setFont(font);
        fuoco->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        tempTubo = new QLabel(frame_5);
        tempTubo->setObjectName(QString::fromUtf8("tempTubo"));
        tempTubo->setGeometry(QRect(165, 35, 76, 31));
        tempTubo->setFont(font1);
        tempTubo->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,30);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:5px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        tempTubo->setAlignment(Qt::AlignCenter);
        iFil = new QLabel(frame_5);
        iFil->setObjectName(QString::fromUtf8("iFil"));
        iFil->setGeometry(QRect(335, 35, 76, 31));
        iFil->setFont(font1);
        iFil->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,30);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:5px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        iFil->setAlignment(Qt::AlignCenter);
        temp_amp_warn_icon = new QFrame(frame_5);
        temp_amp_warn_icon->setObjectName(QString::fromUtf8("temp_amp_warn_icon"));
        temp_amp_warn_icon->setGeometry(QRect(315, 30, 21, 21));
        temp_amp_warn_icon->setStyleSheet(QString::fromUtf8("\n"
"background-color:rgba(255,255,255,0);\n"
"border-image:url(:/warning.png);\n"
"border:0px;\n"
"border-radius:0px;\n"
"\n"
""));
        temp_amp_warn_icon->setFrameShape(QFrame::StyledPanel);
        temp_amp_warn_icon->setFrameShadow(QFrame::Raised);
        temp_tube_warn_icon = new QFrame(frame_5);
        temp_tube_warn_icon->setObjectName(QString::fromUtf8("temp_tube_warn_icon"));
        temp_tube_warn_icon->setGeometry(QRect(230, 30, 21, 21));
        temp_tube_warn_icon->setStyleSheet(QString::fromUtf8("\n"
"background-color:rgba(255,255,255,0);\n"
"border-image:url(:/warning.png);\n"
"border:0px;\n"
"border-radius:0px;\n"
"\n"
""));
        temp_tube_warn_icon->setFrameShape(QFrame::StyledPanel);
        temp_tube_warn_icon->setFrameShadow(QFrame::Raised);
        statShotsLabel = new QLabel(frame_5);
        statShotsLabel->setObjectName(QString::fromUtf8("statShotsLabel"));
        statShotsLabel->setGeometry(QRect(70, 33, 91, 16));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Bitstream Charter"));
        font3.setPointSize(11);
        font3.setBold(true);
        font3.setWeight(75);
        statShotsLabel->setFont(font3);
        statShotsLabel->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        statkHuLabel = new QLabel(frame_5);
        statkHuLabel->setObjectName(QString::fromUtf8("statkHuLabel"));
        statkHuLabel->setGeometry(QRect(70, 48, 86, 21));
        statkHuLabel->setFont(font3);
        statkHuLabel->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        max_kV_label = new QLabel(frame_5);
        max_kV_label->setObjectName(QString::fromUtf8("max_kV_label"));
        max_kV_label->setGeometry(QRect(70, 65, 86, 21));
        max_kV_label->setFont(font3);
        max_kV_label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        frame_12 = new QFrame(frame);
        frame_12->setObjectName(QString::fromUtf8("frame_12"));
        frame_12->setGeometry(QRect(345, 305, 431, 91));
        frame_12->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(74, 74, 74,30);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:10px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        frame_12->setFrameShape(QFrame::StyledPanel);
        frame_12->setFrameShadow(QFrame::Raised);
        label_22 = new QLabel(frame_12);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setGeometry(QRect(10, 5, 351, 26));
        label_22->setFont(font);
        label_22->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 153);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        label_28 = new QLabel(frame_12);
        label_28->setObjectName(QString::fromUtf8("label_28"));
        label_28->setGeometry(QRect(40, 35, 46, 26));
        label_28->setFont(font2);
        label_28->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        label_28->setAlignment(Qt::AlignCenter);
        v32 = new QLabel(frame_12);
        v32->setObjectName(QString::fromUtf8("v32"));
        v32->setGeometry(QRect(35, 58, 56, 26));
        v32->setFont(font1);
        v32->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,30);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:5px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        v32->setAlignment(Qt::AlignCenter);
        vm32 = new QLabel(frame_12);
        vm32->setObjectName(QString::fromUtf8("vm32"));
        vm32->setGeometry(QRect(95, 58, 56, 26));
        vm32->setFont(font1);
        vm32->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,30);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:5px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        vm32->setAlignment(Qt::AlignCenter);
        label_30 = new QLabel(frame_12);
        label_30->setObjectName(QString::fromUtf8("label_30"));
        label_30->setGeometry(QRect(100, 35, 46, 26));
        label_30->setFont(font2);
        label_30->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        label_30->setAlignment(Qt::AlignCenter);
        v12 = new QLabel(frame_12);
        v12->setObjectName(QString::fromUtf8("v12"));
        v12->setGeometry(QRect(155, 58, 56, 26));
        v12->setFont(font1);
        v12->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,30);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:5px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        v12->setAlignment(Qt::AlignCenter);
        label_31 = new QLabel(frame_12);
        label_31->setObjectName(QString::fromUtf8("label_31"));
        label_31->setGeometry(QRect(160, 35, 46, 26));
        label_31->setFont(font2);
        label_31->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        label_31->setAlignment(Qt::AlignCenter);
        vm12 = new QLabel(frame_12);
        vm12->setObjectName(QString::fromUtf8("vm12"));
        vm12->setGeometry(QRect(215, 58, 56, 26));
        vm12->setFont(font1);
        vm12->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,30);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:5px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        vm12->setAlignment(Qt::AlignCenter);
        label_37 = new QLabel(frame_12);
        label_37->setObjectName(QString::fromUtf8("label_37"));
        label_37->setGeometry(QRect(220, 35, 46, 26));
        label_37->setFont(font2);
        label_37->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        label_37->setAlignment(Qt::AlignCenter);
        v15 = new QLabel(frame_12);
        v15->setObjectName(QString::fromUtf8("v15"));
        v15->setGeometry(QRect(275, 58, 56, 26));
        v15->setFont(font1);
        v15->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,30);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:5px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        v15->setAlignment(Qt::AlignCenter);
        label_39 = new QLabel(frame_12);
        label_39->setObjectName(QString::fromUtf8("label_39"));
        label_39->setGeometry(QRect(275, 35, 46, 26));
        label_39->setFont(font2);
        label_39->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        label_39->setAlignment(Qt::AlignCenter);
        v15e = new QLabel(frame_12);
        v15e->setObjectName(QString::fromUtf8("v15e"));
        v15e->setGeometry(QRect(335, 58, 56, 26));
        v15e->setFont(font1);
        v15e->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,30);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:5px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        v15e->setAlignment(Qt::AlignCenter);
        label_40 = new QLabel(frame_12);
        label_40->setObjectName(QString::fromUtf8("label_40"));
        label_40->setGeometry(QRect(330, 35, 61, 26));
        label_40->setFont(font2);
        label_40->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        label_40->setAlignment(Qt::AlignCenter);
        pcb190_v32_warn_icon = new QFrame(frame_12);
        pcb190_v32_warn_icon->setObjectName(QString::fromUtf8("pcb190_v32_warn_icon"));
        pcb190_v32_warn_icon->setGeometry(QRect(75, 48, 21, 21));
        pcb190_v32_warn_icon->setStyleSheet(QString::fromUtf8("\n"
"background-color:rgba(255,255,255,0);\n"
"border-image:url(:/warning.png);\n"
"border:0px;\n"
"border-radius:0px;\n"
"\n"
""));
        pcb190_v32_warn_icon->setFrameShape(QFrame::StyledPanel);
        pcb190_v32_warn_icon->setFrameShadow(QFrame::Raised);
        pcb190_m32__warn_icon = new QFrame(frame_12);
        pcb190_m32__warn_icon->setObjectName(QString::fromUtf8("pcb190_m32__warn_icon"));
        pcb190_m32__warn_icon->setGeometry(QRect(135, 48, 21, 21));
        pcb190_m32__warn_icon->setStyleSheet(QString::fromUtf8("\n"
"background-color:rgba(255,255,255,0);\n"
"border-image:url(:/warning.png);\n"
"border:0px;\n"
"border-radius:0px;\n"
"\n"
""));
        pcb190_m32__warn_icon->setFrameShape(QFrame::StyledPanel);
        pcb190_m32__warn_icon->setFrameShadow(QFrame::Raised);
        pcb190_v12_warn_icon = new QFrame(frame_12);
        pcb190_v12_warn_icon->setObjectName(QString::fromUtf8("pcb190_v12_warn_icon"));
        pcb190_v12_warn_icon->setGeometry(QRect(195, 48, 21, 21));
        pcb190_v12_warn_icon->setStyleSheet(QString::fromUtf8("\n"
"background-color:rgba(255,255,255,0);\n"
"border-image:url(:/warning.png);\n"
"border:0px;\n"
"border-radius:0px;\n"
"\n"
""));
        pcb190_v12_warn_icon->setFrameShape(QFrame::StyledPanel);
        pcb190_v12_warn_icon->setFrameShadow(QFrame::Raised);
        pcb190_m12_warn_icon = new QFrame(frame_12);
        pcb190_m12_warn_icon->setObjectName(QString::fromUtf8("pcb190_m12_warn_icon"));
        pcb190_m12_warn_icon->setGeometry(QRect(255, 48, 21, 21));
        pcb190_m12_warn_icon->setStyleSheet(QString::fromUtf8("\n"
"background-color:rgba(255,255,255,0);\n"
"border-image:url(:/warning.png);\n"
"border:0px;\n"
"border-radius:0px;\n"
"\n"
""));
        pcb190_m12_warn_icon->setFrameShape(QFrame::StyledPanel);
        pcb190_m12_warn_icon->setFrameShadow(QFrame::Raised);
        pcb190_v15_warn_icon = new QFrame(frame_12);
        pcb190_v15_warn_icon->setObjectName(QString::fromUtf8("pcb190_v15_warn_icon"));
        pcb190_v15_warn_icon->setGeometry(QRect(315, 48, 21, 21));
        pcb190_v15_warn_icon->setStyleSheet(QString::fromUtf8("\n"
"background-color:rgba(255,255,255,0);\n"
"border-image:url(:/warning.png);\n"
"border:0px;\n"
"border-radius:0px;\n"
"\n"
""));
        pcb190_v15_warn_icon->setFrameShape(QFrame::StyledPanel);
        pcb190_v15_warn_icon->setFrameShadow(QFrame::Raised);
        pcb190_ext15_warn_icon = new QFrame(frame_12);
        pcb190_ext15_warn_icon->setObjectName(QString::fromUtf8("pcb190_ext15_warn_icon"));
        pcb190_ext15_warn_icon->setGeometry(QRect(375, 48, 21, 21));
        pcb190_ext15_warn_icon->setStyleSheet(QString::fromUtf8("\n"
"background-color:rgba(255,255,255,0);\n"
"border-image:url(:/warning.png);\n"
"border:0px;\n"
"border-radius:0px;\n"
"\n"
""));
        pcb190_ext15_warn_icon->setFrameShape(QFrame::StyledPanel);
        pcb190_ext15_warn_icon->setFrameShadow(QFrame::Raised);
        serviceSetupDiagnosticLabel = new QLabel(frame);
        serviceSetupDiagnosticLabel->setObjectName(QString::fromUtf8("serviceSetupDiagnosticLabel"));
        serviceSetupDiagnosticLabel->setGeometry(QRect(115, 10, 616, 41));
        QFont font4;
        font4.setFamily(QString::fromUtf8("Bitstream Charter"));
        font4.setPointSize(24);
        font4.setBold(true);
        font4.setItalic(true);
        font4.setWeight(75);
        serviceSetupDiagnosticLabel->setFont(font4);
        serviceSetupDiagnosticLabel->setStyleSheet(QString::fromUtf8("background-image:url(/transparent.png);\n"
"color: rgb(248, 248, 54);"));
        serviceSetupDiagnosticLabel->setAlignment(Qt::AlignCenter);
        frame_6 = new QFrame(frame);
        frame_6->setObjectName(QString::fromUtf8("frame_6"));
        frame_6->setGeometry(QRect(115, 195, 196, 91));
        frame_6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(74, 74, 74,30);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:10px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        frame_6->setFrameShape(QFrame::StyledPanel);
        frame_6->setFrameShadow(QFrame::Raised);
        label_7 = new QLabel(frame_6);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(25, 5, 131, 21));
        label_7->setFont(font);
        label_7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 153);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        label_7->setAlignment(Qt::AlignCenter);
        synt_value_2 = new QLabel(frame_6);
        synt_value_2->setObjectName(QString::fromUtf8("synt_value_2"));
        synt_value_2->setGeometry(QRect(315, 40, 76, 41));
        synt_value_2->setFont(font1);
        synt_value_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,30);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:5px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        synt_value_2->setAlignment(Qt::AlignCenter);
        synt_warn_icon_2 = new QFrame(frame_6);
        synt_warn_icon_2->setObjectName(QString::fromUtf8("synt_warn_icon_2"));
        synt_warn_icon_2->setGeometry(QRect(310, 65, 26, 21));
        synt_warn_icon_2->setStyleSheet(QString::fromUtf8("background-color:rgba(255,255,255,0);\n"
"border-image:url(:/power_g.png);\n"
"border:0px;\n"
"border-radius:0px;\n"
""));
        synt_warn_icon_2->setFrameShape(QFrame::StyledPanel);
        synt_warn_icon_2->setFrameShadow(QFrame::Raised);
        label_41 = new QLabel(frame_6);
        label_41->setObjectName(QString::fromUtf8("label_41"));
        label_41->setGeometry(QRect(320, 17, 46, 26));
        label_41->setFont(font2);
        label_41->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        label_41->setAlignment(Qt::AlignCenter);
        frame_2 = new QFrame(frame);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setGeometry(QRect(45, 355, 121, 121));
        frame_2->setStyleSheet(QString::fromUtf8("background-color:rgb(0,0,0,0);\n"
"background-image:url(://Service_Power/Service_Power/power_icon.png);\n"
"border: 0px;"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);

        retranslateUi(ServicePower);

        QMetaObject::connectSlotsByName(ServicePower);
    } // setupUi

    void retranslateUi(QWidget *ServicePower)
    {
        ServicePower->setWindowTitle(QApplication::translate("ServicePower", "Form", 0, QApplication::UnicodeUTF8));
        exitButton->setText(QString());
        label_6->setText(QApplication::translate("ServicePower", "SYSTEM POWER SUPPLY", 0, QApplication::UnicodeUTF8));
        hVDC_value->setText(QApplication::translate("ServicePower", "--", 0, QApplication::UnicodeUTF8));
        acdc_value->setText(QApplication::translate("ServicePower", "--", 0, QApplication::UnicodeUTF8));
        rot_value->setText(QApplication::translate("ServicePower", "--", 0, QApplication::UnicodeUTF8));
        synt_value->setText(QApplication::translate("ServicePower", "--", 0, QApplication::UnicodeUTF8));
        label_29->setText(QApplication::translate("ServicePower", "HV-DC", 0, QApplication::UnicodeUTF8));
        label_32->setText(QApplication::translate("ServicePower", "AC-DC", 0, QApplication::UnicodeUTF8));
        label_33->setText(QApplication::translate("ServicePower", "ROT", 0, QApplication::UnicodeUTF8));
        label_34->setText(QApplication::translate("ServicePower", "SYNT", 0, QApplication::UnicodeUTF8));
        anodicaTest->setText(QApplication::translate("ServicePower", "--", 0, QApplication::UnicodeUTF8));
        masTest->setText(QApplication::translate("ServicePower", "--", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("ServicePower", "mAs", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("ServicePower", "I(mA)", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("ServicePower", "mAsMETER", 0, QApplication::UnicodeUTF8));
        tubeNameLabel->setText(QApplication::translate("ServicePower", "XM1016T_SN86754987", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("ServicePower", "T(\302\260) Ampl.", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("ServicePower", "T(\302\260) Tube", 0, QApplication::UnicodeUTF8));
        tempAmp->setText(QApplication::translate("ServicePower", "--", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("ServicePower", "I-Fil (mA)", 0, QApplication::UnicodeUTF8));
        fuoco->setText(QApplication::translate("ServicePower", "Wg", 0, QApplication::UnicodeUTF8));
        tempTubo->setText(QApplication::translate("ServicePower", "--", 0, QApplication::UnicodeUTF8));
        iFil->setText(QApplication::translate("ServicePower", "--", 0, QApplication::UnicodeUTF8));
        statShotsLabel->setText(QApplication::translate("ServicePower", "Shots: 2000", 0, QApplication::UnicodeUTF8));
        statkHuLabel->setText(QApplication::translate("ServicePower", "kHU: 200", 0, QApplication::UnicodeUTF8));
        max_kV_label->setText(QApplication::translate("ServicePower", "Max kV: 49", 0, QApplication::UnicodeUTF8));
        label_22->setText(QApplication::translate("ServicePower", "PCB190: INVERTER CONTROL POWER", 0, QApplication::UnicodeUTF8));
        label_28->setText(QApplication::translate("ServicePower", "+32V", 0, QApplication::UnicodeUTF8));
        v32->setText(QApplication::translate("ServicePower", "32", 0, QApplication::UnicodeUTF8));
        vm32->setText(QApplication::translate("ServicePower", "-32", 0, QApplication::UnicodeUTF8));
        label_30->setText(QApplication::translate("ServicePower", "-32V", 0, QApplication::UnicodeUTF8));
        v12->setText(QApplication::translate("ServicePower", "12", 0, QApplication::UnicodeUTF8));
        label_31->setText(QApplication::translate("ServicePower", "+12V", 0, QApplication::UnicodeUTF8));
        vm12->setText(QApplication::translate("ServicePower", "-12", 0, QApplication::UnicodeUTF8));
        label_37->setText(QApplication::translate("ServicePower", "-12V", 0, QApplication::UnicodeUTF8));
        v15->setText(QApplication::translate("ServicePower", "15", 0, QApplication::UnicodeUTF8));
        label_39->setText(QApplication::translate("ServicePower", "+15V", 0, QApplication::UnicodeUTF8));
        v15e->setText(QApplication::translate("ServicePower", "15", 0, QApplication::UnicodeUTF8));
        label_40->setText(QApplication::translate("ServicePower", "+15VEX", 0, QApplication::UnicodeUTF8));
        serviceSetupDiagnosticLabel->setText(QApplication::translate("ServicePower", "Service Panel/Power", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("ServicePower", "STARTER", 0, QApplication::UnicodeUTF8));
        synt_value_2->setText(QApplication::translate("ServicePower", "--", 0, QApplication::UnicodeUTF8));
        label_41->setText(QApplication::translate("ServicePower", "SYNT", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ServicePower: public Ui_ServicePower {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVICEPOWER_H
