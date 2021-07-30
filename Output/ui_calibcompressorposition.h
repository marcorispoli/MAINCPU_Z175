/********************************************************************************
** Form generated from reading UI file 'calibcompressorposition.ui'
**
** Created: Fri Sep 13 11:22:47 2019
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CALIBCOMPRESSORPOSITION_H
#define UI_CALIBCOMPRESSORPOSITION_H

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

class Ui_calibCompressorPosition
{
public:
    QFrame *frame;
    QFrame *frame_2;
    QPushButton *exitButton;
    QLabel *serviceSetupDiagnosticLabel;
    QFrame *calibPadFrameDwn;
    QLabel *calibPadCalibratedPosDwn;
    QLabel *calibPadSequenceLabelDown;
    QLineEdit *calibPadEditDwn;
    QFrame *calibPadFrameCalibrate;
    QLabel *calibPadSequenceLabelDown_3;
    QFrame *calibPadFrameStore;
    QLabel *calibPadSequenceLabelDown_4;
    QFrame *calibPadFrameUp;
    QLabel *calibPadCalibratedPosUp;
    QLineEdit *calibPadEditUp;
    QLabel *calibPadSequenceLabelUp;
    QFrame *calibPadFrameInit;
    QLabel *calibPadCalibratedPosInit;
    QLabel *calibPadSequenceLabelDown_2;
    QPushButton *calibPadCancelButton;
    QPushButton *calibPadNextButton;
    QLabel *rawPosition;
    QLabel *serviceSetupDiagnosticLabel_2;
    QLabel *serviceSetupDiagnosticLabel_3;
    QLabel *calibratedPosition;

    void setupUi(QWidget *calibCompressorPosition)
    {
        if (calibCompressorPosition->objectName().isEmpty())
            calibCompressorPosition->setObjectName(QString::fromUtf8("calibCompressorPosition"));
        calibCompressorPosition->resize(800, 480);
        calibCompressorPosition->setStyleSheet(QString::fromUtf8("background-color:rgb(0,0,0,0);\n"
"background-image:url(://Service_Calib/Service_Calib/calib_menu.png);\n"
"border: 0px;"));
        frame = new QFrame(calibCompressorPosition);
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
        calibPadFrameDwn = new QFrame(frame);
        calibPadFrameDwn->setObjectName(QString::fromUtf8("calibPadFrameDwn"));
        calibPadFrameDwn->setGeometry(QRect(375, 85, 46, 31));
        calibPadFrameDwn->setStyleSheet(QString::fromUtf8("border-image:url(:/compr_dwn.png);\n"
"background-color: rgba(255, 255, 255,0);\n"
"background-image:url(:/transparent.png);\n"
""));
        calibPadFrameDwn->setFrameShape(QFrame::StyledPanel);
        calibPadFrameDwn->setFrameShadow(QFrame::Raised);
        calibPadCalibratedPosDwn = new QLabel(calibPadFrameDwn);
        calibPadCalibratedPosDwn->setObjectName(QString::fromUtf8("calibPadCalibratedPosDwn"));
        calibPadCalibratedPosDwn->setGeometry(QRect(190, 200, 56, 16));
        QFont font1;
        font1.setFamily(QString::fromUtf8("DejaVu Sans"));
        font1.setPointSize(12);
        font1.setBold(true);
        font1.setItalic(false);
        font1.setWeight(75);
        calibPadCalibratedPosDwn->setFont(font1);
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
        QFont font2;
        font2.setFamily(QString::fromUtf8("DejaVu Sans"));
        font2.setPointSize(14);
        font2.setBold(true);
        font2.setItalic(false);
        font2.setWeight(75);
        calibPadSequenceLabelDown->setFont(font2);
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
        QFont font3;
        font3.setFamily(QString::fromUtf8("DejaVu Sans"));
        font3.setPointSize(22);
        font3.setBold(false);
        font3.setItalic(false);
        font3.setWeight(9);
        calibPadEditDwn->setFont(font3);
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
        calibPadFrameCalibrate = new QFrame(frame);
        calibPadFrameCalibrate->setObjectName(QString::fromUtf8("calibPadFrameCalibrate"));
        calibPadFrameCalibrate->setGeometry(QRect(275, 90, 31, 21));
        calibPadFrameCalibrate->setStyleSheet(QString::fromUtf8("border-image:url(:/compr_dwn.png);\n"
"background-color: rgba(255, 255, 255,0);\n"
"border-image:url(:/calibrate.png);\n"
"background-image:url(:/transparent.png);"));
        calibPadFrameCalibrate->setFrameShape(QFrame::StyledPanel);
        calibPadFrameCalibrate->setFrameShadow(QFrame::Raised);
        calibPadSequenceLabelDown_3 = new QLabel(calibPadFrameCalibrate);
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
        calibPadFrameStore = new QFrame(frame);
        calibPadFrameStore->setObjectName(QString::fromUtf8("calibPadFrameStore"));
        calibPadFrameStore->setGeometry(QRect(430, 85, 56, 31));
        calibPadFrameStore->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255,0);\n"
"border-image:url(:/store.png);\n"
"background-image:url(:/transparent.png);"));
        calibPadFrameStore->setFrameShape(QFrame::StyledPanel);
        calibPadFrameStore->setFrameShadow(QFrame::Raised);
        calibPadSequenceLabelDown_4 = new QLabel(calibPadFrameStore);
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
        calibPadFrameUp = new QFrame(frame);
        calibPadFrameUp->setObjectName(QString::fromUtf8("calibPadFrameUp"));
        calibPadFrameUp->setGeometry(QRect(325, 90, 41, 36));
        calibPadFrameUp->setStyleSheet(QString::fromUtf8("border-image:url(:/compr_up.png);\n"
"background-color: rgba(255, 255, 255,0);\n"
"background-image:url(:/transparent.png);\n"
""));
        calibPadFrameUp->setFrameShape(QFrame::StyledPanel);
        calibPadFrameUp->setFrameShadow(QFrame::Raised);
        calibPadCalibratedPosUp = new QLabel(calibPadFrameUp);
        calibPadCalibratedPosUp->setObjectName(QString::fromUtf8("calibPadCalibratedPosUp"));
        calibPadCalibratedPosUp->setGeometry(QRect(185, 145, 56, 16));
        calibPadCalibratedPosUp->setFont(font1);
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
        calibPadEditUp->setFont(font3);
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
        calibPadSequenceLabelUp->setFont(font2);
        calibPadSequenceLabelUp->setStyleSheet(QString::fromUtf8("background-image:url(:/transparent.png);\n"
"border-image:url(:/transparent.png);\n"
"\n"
"background-color: rgb(255,255,255,0);\n"
"border-color: rgb(255, 255, 153);\n"
"color: rgb(255,0,0);\n"
""));
        calibPadSequenceLabelUp->setAlignment(Qt::AlignCenter);
        calibPadFrameInit = new QFrame(frame);
        calibPadFrameInit->setObjectName(QString::fromUtf8("calibPadFrameInit"));
        calibPadFrameInit->setGeometry(QRect(500, 85, 36, 31));
        calibPadFrameInit->setStyleSheet(QString::fromUtf8("background-image:url(:/transparent.png);\n"
"background-color: rgba(255, 255, 255,0);\n"
"border-image:url(:/intro_calib_pad2.png);\n"
""));
        calibPadFrameInit->setFrameShape(QFrame::StyledPanel);
        calibPadFrameInit->setFrameShadow(QFrame::Raised);
        calibPadCalibratedPosInit = new QLabel(calibPadFrameInit);
        calibPadCalibratedPosInit->setObjectName(QString::fromUtf8("calibPadCalibratedPosInit"));
        calibPadCalibratedPosInit->setGeometry(QRect(235, 125, 56, 16));
        calibPadCalibratedPosInit->setFont(font1);
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
        calibPadSequenceLabelDown_2->setFont(font1);
        calibPadSequenceLabelDown_2->setStyleSheet(QString::fromUtf8("background-image:url(:/transparent.png);\n"
"border-image:url(:/transparent.png);\n"
"\n"
"background-color: rgb(255,255,255,0);\n"
"border-color: rgb(255, 255, 153);\n"
"color: rgb(255,0,0);\n"
""));
        calibPadSequenceLabelDown_2->setAlignment(Qt::AlignCenter);
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
        rawPosition = new QLabel(frame);
        rawPosition->setObjectName(QString::fromUtf8("rawPosition"));
        rawPosition->setGeometry(QRect(635, 150, 131, 46));
        rawPosition->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"background-image:url(:/transparent.png);\n"
"border-image:url(:/transparent.png);\n"
"\n"
"color: rgb(79, 79, 79);\n"
"border-left-color: rgb(255, 255, 153);\n"
"border-bottom-color: rgb(255, 255, 153);\n"
"font: 75 24pt \"DejaVu Sans\";\n"
"border-radius:5px;"));
        rawPosition->setAlignment(Qt::AlignCenter);
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
        calibratedPosition = new QLabel(frame);
        calibratedPosition->setObjectName(QString::fromUtf8("calibratedPosition"));
        calibratedPosition->setGeometry(QRect(635, 245, 131, 46));
        calibratedPosition->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"background-image:url(:/transparent.png);\n"
"border-image:url(:/transparent.png);\n"
"\n"
"color: rgb(79, 79, 79);\n"
"border-left-color: rgb(255, 255, 153);\n"
"border-bottom-color: rgb(255, 255, 153);\n"
"font: 75 24pt \"DejaVu Sans\";\n"
"border-radius:5px;"));
        calibratedPosition->setAlignment(Qt::AlignCenter);

        retranslateUi(calibCompressorPosition);

        QMetaObject::connectSlotsByName(calibCompressorPosition);
    } // setupUi

    void retranslateUi(QWidget *calibCompressorPosition)
    {
        calibCompressorPosition->setWindowTitle(QApplication::translate("calibCompressorPosition", "Form", 0, QApplication::UnicodeUTF8));
        exitButton->setText(QString());
        serviceSetupDiagnosticLabel->setText(QApplication::translate("calibCompressorPosition", "Service Panel/Calibrate/Position calibration", 0, QApplication::UnicodeUTF8));
        calibPadCalibratedPosDwn->setText(QApplication::translate("calibCompressorPosition", "200", 0, QApplication::UnicodeUTF8));
        calibPadSequenceLabelDown->setText(QApplication::translate("calibCompressorPosition", "DOWN", 0, QApplication::UnicodeUTF8));
        calibPadEditDwn->setText(QApplication::translate("calibCompressorPosition", "0", 0, QApplication::UnicodeUTF8));
        calibPadSequenceLabelDown_3->setText(QApplication::translate("calibCompressorPosition", "PRESS TO\n"
" CALIBRATE ", 0, QApplication::UnicodeUTF8));
        calibPadSequenceLabelDown_4->setText(QApplication::translate("calibCompressorPosition", "PRESS\n"
"TO STORE", 0, QApplication::UnicodeUTF8));
        calibPadCalibratedPosUp->setText(QApplication::translate("calibCompressorPosition", "200", 0, QApplication::UnicodeUTF8));
        calibPadEditUp->setText(QApplication::translate("calibCompressorPosition", "0", 0, QApplication::UnicodeUTF8));
        calibPadSequenceLabelUp->setText(QApplication::translate("calibCompressorPosition", "UP", 0, QApplication::UnicodeUTF8));
        calibPadCalibratedPosInit->setText(QApplication::translate("calibCompressorPosition", "200", 0, QApplication::UnicodeUTF8));
        calibPadSequenceLabelDown_2->setText(QApplication::translate("calibCompressorPosition", "PRESS PLAY\n"
" TO START CALIBRATION ", 0, QApplication::UnicodeUTF8));
        calibPadCancelButton->setText(QString());
        calibPadNextButton->setText(QString());
        rawPosition->setText(QApplication::translate("calibCompressorPosition", "TextLabel", 0, QApplication::UnicodeUTF8));
        serviceSetupDiagnosticLabel_2->setText(QApplication::translate("calibCompressorPosition", "POT", 0, QApplication::UnicodeUTF8));
        serviceSetupDiagnosticLabel_3->setText(QApplication::translate("calibCompressorPosition", "POSITION", 0, QApplication::UnicodeUTF8));
        calibratedPosition->setText(QApplication::translate("calibCompressorPosition", "TextLabel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class calibCompressorPosition: public Ui_calibCompressorPosition {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CALIBCOMPRESSORPOSITION_H
