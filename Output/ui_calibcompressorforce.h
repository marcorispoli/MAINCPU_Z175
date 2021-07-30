/********************************************************************************
** Form generated from reading UI file 'calibcompressorforce.ui'
**
** Created: Fri Sep 13 11:22:47 2019
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CALIBCOMPRESSORFORCE_H
#define UI_CALIBCOMPRESSORFORCE_H

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

class Ui_CalibCompressorForce
{
public:
    QFrame *frame;
    QFrame *frame_2;
    QPushButton *exitButton;
    QLabel *serviceSetupDiagnosticLabel;
    QPushButton *calibPadCancelButton;
    QPushButton *calibPadNextButton;
    QLabel *rawForce;
    QLabel *serviceSetupDiagnosticLabel_2;
    QLabel *serviceSetupDiagnosticLabel_3;
    QLabel *calibratedForce;
    QFrame *calibComprFrameInit;
    QLabel *calibComprCalibratedComprInit;
    QLabel *calibComprSequenceLabelIntro;
    QFrame *calibComprFrameOffset;
    QLabel *calibPadSequenceLabelDown_5;
    QFrame *calibComprFrameF1;
    QLineEdit *calibComprEditF1;
    QLabel *calibComprRawData;
    QFrame *calibComprFrameF2;
    QLineEdit *calibComprEditF2;
    QFrame *calibForceFrameStore;
    QLabel *calibPadSequenceLabelDown_4;
    QFrame *calibForceFrameCalibrate;
    QLabel *calibPadSequenceLabelDown_3;

    void setupUi(QWidget *CalibCompressorForce)
    {
        if (CalibCompressorForce->objectName().isEmpty())
            CalibCompressorForce->setObjectName(QString::fromUtf8("CalibCompressorForce"));
        CalibCompressorForce->resize(800, 480);
        CalibCompressorForce->setStyleSheet(QString::fromUtf8("background-color:rgb(0,0,0,0);\n"
"background-image:url(://Service_Calib/Service_Calib/calib_menu.png);\n"
"border: 0px;"));
        frame = new QFrame(CalibCompressorForce);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(0, 0, 800, 480));
        frame->setStyleSheet(QString::fromUtf8(""));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        frame_2 = new QFrame(frame);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setGeometry(QRect(660, 365, 121, 121));
        frame_2->setStyleSheet(QString::fromUtf8("background-color:rgb(0,0,0,0);\n"
"background-image:url(://Service_Calib/Service_Calib/calib_icon.png);\n"
"border: 0px;"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        exitButton = new QPushButton(frame);
        exitButton->setObjectName(QString::fromUtf8("exitButton"));
        exitButton->setGeometry(QRect(0, 0, 111, 111));
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
        calibPadCancelButton = new QPushButton(frame);
        calibPadCancelButton->setObjectName(QString::fromUtf8("calibPadCancelButton"));
        calibPadCancelButton->setGeometry(QRect(80, 265, 100, 100));
        calibPadCancelButton->setFocusPolicy(Qt::NoFocus);
        calibPadCancelButton->setStyleSheet(QString::fromUtf8("background-color: rgb(79, 79, 79,0);\n"
"background-image:url(:/transparent.png);\n"
"border-image:url(:/stop.png);\n"
"border-color: rgb(255, 255, 153);\n"
"color: rgb(255, 255, 153);\n"
"font: 80 16pt \"DejaVu Sans\";\n"
"border-radius:10px;\n"
""));
        calibPadNextButton = new QPushButton(frame);
        calibPadNextButton->setObjectName(QString::fromUtf8("calibPadNextButton"));
        calibPadNextButton->setGeometry(QRect(80, 130, 100, 100));
        calibPadNextButton->setFocusPolicy(Qt::NoFocus);
        calibPadNextButton->setStyleSheet(QString::fromUtf8("background-color: rgb(79, 79, 79,0);\n"
"background-image:url(:/transparent.png);\n"
"border-image:url(:/play.png);\n"
"border-color: rgb(255, 255, 153);\n"
"color: rgb(255, 255, 153);\n"
"font: 80 16pt \"DejaVu Sans\";\n"
"border-radius:10px;\n"
""));
        rawForce = new QLabel(frame);
        rawForce->setObjectName(QString::fromUtf8("rawForce"));
        rawForce->setGeometry(QRect(635, 150, 131, 46));
        rawForce->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"background-image:url(:/transparent.png);\n"
"border-image:url(:/transparent.png);\n"
"\n"
"color: rgb(79, 79, 79);\n"
"border-left-color: rgb(255, 255, 153);\n"
"border-bottom-color: rgb(255, 255, 153);\n"
"font: 75 24pt \"DejaVu Sans\";\n"
"border-radius:5px;"));
        rawForce->setAlignment(Qt::AlignCenter);
        serviceSetupDiagnosticLabel_2 = new QLabel(frame);
        serviceSetupDiagnosticLabel_2->setObjectName(QString::fromUtf8("serviceSetupDiagnosticLabel_2"));
        serviceSetupDiagnosticLabel_2->setGeometry(QRect(640, 115, 121, 26));
        serviceSetupDiagnosticLabel_2->setFont(font);
        serviceSetupDiagnosticLabel_2->setStyleSheet(QString::fromUtf8("background-image:url(/transparent.png);\n"
"color: rgb(248, 248, 54);"));
        serviceSetupDiagnosticLabel_2->setAlignment(Qt::AlignCenter);
        serviceSetupDiagnosticLabel_3 = new QLabel(frame);
        serviceSetupDiagnosticLabel_3->setObjectName(QString::fromUtf8("serviceSetupDiagnosticLabel_3"));
        serviceSetupDiagnosticLabel_3->setGeometry(QRect(640, 210, 121, 26));
        serviceSetupDiagnosticLabel_3->setFont(font);
        serviceSetupDiagnosticLabel_3->setStyleSheet(QString::fromUtf8("background-image:url(/transparent.png);\n"
"color: rgb(248, 248, 54);"));
        serviceSetupDiagnosticLabel_3->setAlignment(Qt::AlignCenter);
        calibratedForce = new QLabel(frame);
        calibratedForce->setObjectName(QString::fromUtf8("calibratedForce"));
        calibratedForce->setGeometry(QRect(635, 245, 131, 46));
        calibratedForce->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"background-image:url(:/transparent.png);\n"
"border-image:url(:/transparent.png);\n"
"\n"
"color: rgb(79, 79, 79);\n"
"border-left-color: rgb(255, 255, 153);\n"
"border-bottom-color: rgb(255, 255, 153);\n"
"font: 75 24pt \"DejaVu Sans\";\n"
"border-radius:5px;"));
        calibratedForce->setAlignment(Qt::AlignCenter);
        calibComprFrameInit = new QFrame(frame);
        calibComprFrameInit->setObjectName(QString::fromUtf8("calibComprFrameInit"));
        calibComprFrameInit->setGeometry(QRect(225, 90, 21, 26));
        calibComprFrameInit->setStyleSheet(QString::fromUtf8("\n"
"background-color: rgba(255, 255, 255,0);\n"
"border-image:url(:/calib_compr_intro.png);\n"
""));
        calibComprFrameInit->setFrameShape(QFrame::StyledPanel);
        calibComprFrameInit->setFrameShadow(QFrame::Raised);
        calibComprCalibratedComprInit = new QLabel(calibComprFrameInit);
        calibComprCalibratedComprInit->setObjectName(QString::fromUtf8("calibComprCalibratedComprInit"));
        calibComprCalibratedComprInit->setGeometry(QRect(135, 130, 56, 16));
        QFont font1;
        font1.setFamily(QString::fromUtf8("DejaVu Sans"));
        font1.setPointSize(12);
        font1.setBold(true);
        font1.setItalic(false);
        font1.setWeight(75);
        calibComprCalibratedComprInit->setFont(font1);
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
        calibComprSequenceLabelIntro->setFont(font1);
        calibComprSequenceLabelIntro->setStyleSheet(QString::fromUtf8("background-image:url(:/transparent.png);\n"
"border-image:url(:/transparent.png);\n"
"\n"
"background-color: rgb(255,255,255,0);\n"
"border-color: rgb(255, 255, 153);\n"
"color: rgb(255,0,0);\n"
""));
        calibComprSequenceLabelIntro->setAlignment(Qt::AlignCenter);
        calibComprFrameOffset = new QFrame(frame);
        calibComprFrameOffset->setObjectName(QString::fromUtf8("calibComprFrameOffset"));
        calibComprFrameOffset->setGeometry(QRect(280, 105, 16, 21));
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
        QFont font2;
        font2.setFamily(QString::fromUtf8("DejaVu Sans"));
        font2.setPointSize(14);
        font2.setBold(true);
        font2.setItalic(false);
        font2.setWeight(75);
        calibPadSequenceLabelDown_5->setFont(font2);
        calibPadSequenceLabelDown_5->setStyleSheet(QString::fromUtf8("background-image:url(:/transparent.png);\n"
"border-image:url(:/transparent.png);\n"
"\n"
"background-color: rgb(255,255,255,0);\n"
"border-color: rgb(255, 255, 153);\n"
"color: rgb(255,0,0);\n"
""));
        calibPadSequenceLabelDown_5->setAlignment(Qt::AlignCenter);
        calibComprFrameF1 = new QFrame(frame);
        calibComprFrameF1->setObjectName(QString::fromUtf8("calibComprFrameF1"));
        calibComprFrameF1->setGeometry(QRect(360, 95, 51, 41));
        calibComprFrameF1->setStyleSheet(QString::fromUtf8("\n"
"background-color: rgba(255, 255, 255,0);\n"
"background-image:url(:/transparent.png);\n"
"border-image:url(:/calib_compr_f1.png);"));
        calibComprFrameF1->setFrameShape(QFrame::StyledPanel);
        calibComprFrameF1->setFrameShadow(QFrame::Raised);
        calibComprEditF1 = new QLineEdit(calibComprFrameF1);
        calibComprEditF1->setObjectName(QString::fromUtf8("calibComprEditF1"));
        calibComprEditF1->setGeometry(QRect(200, 105, 96, 46));
        QFont font3;
        font3.setFamily(QString::fromUtf8("DejaVu Sans"));
        font3.setPointSize(22);
        font3.setBold(false);
        font3.setItalic(false);
        font3.setWeight(9);
        calibComprEditF1->setFont(font3);
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
        QFont font4;
        font4.setFamily(QString::fromUtf8("DejaVu Sans"));
        font4.setPointSize(10);
        font4.setBold(true);
        font4.setItalic(false);
        font4.setWeight(75);
        calibComprRawData->setFont(font4);
        calibComprRawData->setStyleSheet(QString::fromUtf8("background-color: rgb(255,255,255,0);\n"
"\n"
"border-color: rgb(255, 255, 153);\n"
"color: rgb(0,0,0);\n"
"border-image:url(:/transparent.png);\n"
"background-image:url(:/transparent.png);"));
        calibComprRawData->setAlignment(Qt::AlignCenter);
        calibComprFrameF2 = new QFrame(frame);
        calibComprFrameF2->setObjectName(QString::fromUtf8("calibComprFrameF2"));
        calibComprFrameF2->setGeometry(QRect(320, 100, 21, 36));
        calibComprFrameF2->setStyleSheet(QString::fromUtf8("\n"
"background-color: rgba(255, 255, 255,0);\n"
"background-image:url(:/transparent.png);\n"
"border-image:url(:/calib_compr_f2.png);"));
        calibComprFrameF2->setFrameShape(QFrame::StyledPanel);
        calibComprFrameF2->setFrameShadow(QFrame::Raised);
        calibComprEditF2 = new QLineEdit(calibComprFrameF2);
        calibComprEditF2->setObjectName(QString::fromUtf8("calibComprEditF2"));
        calibComprEditF2->setGeometry(QRect(220, 105, 96, 46));
        calibComprEditF2->setFont(font3);
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
        calibForceFrameStore = new QFrame(frame);
        calibForceFrameStore->setObjectName(QString::fromUtf8("calibForceFrameStore"));
        calibForceFrameStore->setGeometry(QRect(470, 100, 56, 31));
        calibForceFrameStore->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255,0);\n"
"border-image:url(:/store.png);\n"
"background-image:url(:/transparent.png);"));
        calibForceFrameStore->setFrameShape(QFrame::StyledPanel);
        calibForceFrameStore->setFrameShadow(QFrame::Raised);
        calibPadSequenceLabelDown_4 = new QLabel(calibForceFrameStore);
        calibPadSequenceLabelDown_4->setObjectName(QString::fromUtf8("calibPadSequenceLabelDown_4"));
        calibPadSequenceLabelDown_4->setGeometry(QRect(5, 210, 91, 36));
        calibPadSequenceLabelDown_4->setFont(font1);
        calibPadSequenceLabelDown_4->setStyleSheet(QString::fromUtf8("background-image:url(:/transparent.png);\n"
"border-image:url(:/transparent.png);\n"
"\n"
"background-color: rgb(255,255,255,0);\n"
"border-color: rgb(255, 255, 153);\n"
"color: rgb(255,0,0);\n"
""));
        calibPadSequenceLabelDown_4->setAlignment(Qt::AlignCenter);
        calibForceFrameCalibrate = new QFrame(frame);
        calibForceFrameCalibrate->setObjectName(QString::fromUtf8("calibForceFrameCalibrate"));
        calibForceFrameCalibrate->setGeometry(QRect(420, 105, 31, 21));
        calibForceFrameCalibrate->setStyleSheet(QString::fromUtf8("border-image:url(:/compr_dwn.png);\n"
"background-color: rgba(255, 255, 255,0);\n"
"border-image:url(:/calibrate.png);\n"
"background-image:url(:/transparent.png);"));
        calibForceFrameCalibrate->setFrameShape(QFrame::StyledPanel);
        calibForceFrameCalibrate->setFrameShadow(QFrame::Raised);
        calibPadSequenceLabelDown_3 = new QLabel(calibForceFrameCalibrate);
        calibPadSequenceLabelDown_3->setObjectName(QString::fromUtf8("calibPadSequenceLabelDown_3"));
        calibPadSequenceLabelDown_3->setGeometry(QRect(5, 250, 111, 41));
        calibPadSequenceLabelDown_3->setFont(font1);
        calibPadSequenceLabelDown_3->setStyleSheet(QString::fromUtf8("background-image:url(:/transparent.png);\n"
"border-image:url(:/transparent.png);\n"
"\n"
"background-color: rgb(255,255,255,0);\n"
"border-color: rgb(255, 255, 153);\n"
"color: rgb(255,0,0);\n"
""));
        calibPadSequenceLabelDown_3->setAlignment(Qt::AlignCenter);

        retranslateUi(CalibCompressorForce);

        QMetaObject::connectSlotsByName(CalibCompressorForce);
    } // setupUi

    void retranslateUi(QWidget *CalibCompressorForce)
    {
        CalibCompressorForce->setWindowTitle(QApplication::translate("CalibCompressorForce", "Form", 0, QApplication::UnicodeUTF8));
        exitButton->setText(QString());
        serviceSetupDiagnosticLabel->setText(QApplication::translate("CalibCompressorForce", "Service Panel/Calibrate/Position calibration", 0, QApplication::UnicodeUTF8));
        calibPadCancelButton->setText(QString());
        calibPadNextButton->setText(QString());
        rawForce->setText(QApplication::translate("CalibCompressorForce", "TextLabel", 0, QApplication::UnicodeUTF8));
        serviceSetupDiagnosticLabel_2->setText(QApplication::translate("CalibCompressorForce", "SENSOR", 0, QApplication::UnicodeUTF8));
        serviceSetupDiagnosticLabel_3->setText(QApplication::translate("CalibCompressorForce", "FORCE", 0, QApplication::UnicodeUTF8));
        calibratedForce->setText(QApplication::translate("CalibCompressorForce", "TextLabel", 0, QApplication::UnicodeUTF8));
        calibComprCalibratedComprInit->setText(QApplication::translate("CalibCompressorForce", "200", 0, QApplication::UnicodeUTF8));
        calibComprSequenceLabelIntro->setText(QApplication::translate("CalibCompressorForce", "PRESS PLAY\n"
" TO START CALIBRATION ", 0, QApplication::UnicodeUTF8));
        calibPadSequenceLabelDown_5->setText(QApplication::translate("CalibCompressorForce", "press button\n"
"to get offset", 0, QApplication::UnicodeUTF8));
        calibComprEditF1->setText(QApplication::translate("CalibCompressorForce", "0", 0, QApplication::UnicodeUTF8));
        calibComprRawData->setText(QApplication::translate("CalibCompressorForce", "200", 0, QApplication::UnicodeUTF8));
        calibComprEditF2->setText(QApplication::translate("CalibCompressorForce", "0", 0, QApplication::UnicodeUTF8));
        calibPadSequenceLabelDown_4->setText(QApplication::translate("CalibCompressorForce", "PRESS\n"
"TO STORE", 0, QApplication::UnicodeUTF8));
        calibPadSequenceLabelDown_3->setText(QApplication::translate("CalibCompressorForce", "PRESS TO\n"
" CALIBRATE ", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CalibCompressorForce: public Ui_CalibCompressorForce {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CALIBCOMPRESSORFORCE_H
