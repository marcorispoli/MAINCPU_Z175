/********************************************************************************
** Form generated from reading UI file 'calibmenu.ui'
**
** Created: Fri Sep 13 11:22:47 2019
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CALIBMENU_H
#define UI_CALIBMENU_H

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

class Ui_CalibMenu
{
public:
    QFrame *frame;
    QFrame *frame_2;
    QPushButton *exitButton;
    QPushButton *kVCalibButton;
    QPushButton *HVCalibButton;
    QPushButton *PositionCalibButton;
    QPushButton *CompressionCalibButton;
    QPushButton *FilterCalibButton;
    QPushButton *StarterCalibButton;
    QPushButton *TiltCalibButton;
    QLabel *serviceSetupDiagnosticLabel;

    void setupUi(QWidget *CalibMenu)
    {
        if (CalibMenu->objectName().isEmpty())
            CalibMenu->setObjectName(QString::fromUtf8("CalibMenu"));
        CalibMenu->resize(800, 480);
        CalibMenu->setStyleSheet(QString::fromUtf8("background-color:rgb(0,0,0,0);\n"
"background-image:url(://Service_Calib/Service_Calib/calib_menu.png);\n"
"border: 0px;"));
        frame = new QFrame(CalibMenu);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(0, 0, 800, 480));
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
        kVCalibButton = new QPushButton(frame);
        kVCalibButton->setObjectName(QString::fromUtf8("kVCalibButton"));
        kVCalibButton->setGeometry(QRect(40, 260, 120, 120));
        kVCalibButton->setStyleSheet(QString::fromUtf8("background-image:url(://Service_Calib/Service_Calib/kV_calib_icon.png);\n"
""));
        HVCalibButton = new QPushButton(frame);
        HVCalibButton->setObjectName(QString::fromUtf8("HVCalibButton"));
        HVCalibButton->setGeometry(QRect(240, 260, 120, 120));
        HVCalibButton->setStyleSheet(QString::fromUtf8("background-image:url(://Service_Calib/Service_Calib/HV_calib_icon.png);\n"
""));
        PositionCalibButton = new QPushButton(frame);
        PositionCalibButton->setObjectName(QString::fromUtf8("PositionCalibButton"));
        PositionCalibButton->setGeometry(QRect(40, 110, 120, 120));
        PositionCalibButton->setStyleSheet(QString::fromUtf8("background-image:url(://Service_Calib/Service_Calib/position_calib_icon.png);\n"
""));
        CompressionCalibButton = new QPushButton(frame);
        CompressionCalibButton->setObjectName(QString::fromUtf8("CompressionCalibButton"));
        CompressionCalibButton->setGeometry(QRect(240, 110, 120, 120));
        CompressionCalibButton->setStyleSheet(QString::fromUtf8("background-image:url(://Service_Calib/Service_Calib/force_calib_icon.png);\n"
""));
        FilterCalibButton = new QPushButton(frame);
        FilterCalibButton->setObjectName(QString::fromUtf8("FilterCalibButton"));
        FilterCalibButton->setGeometry(QRect(440, 110, 120, 120));
        FilterCalibButton->setStyleSheet(QString::fromUtf8("background-image:url(://Service_Calib/Service_Calib/filter_calib_icon.png);\n"
""));
        StarterCalibButton = new QPushButton(frame);
        StarterCalibButton->setObjectName(QString::fromUtf8("StarterCalibButton"));
        StarterCalibButton->setGeometry(QRect(440, 260, 120, 120));
        StarterCalibButton->setStyleSheet(QString::fromUtf8("background-image:url(://Service_Calib/Service_Calib/starter_calib_icon.png);\n"
""));
        TiltCalibButton = new QPushButton(frame);
        TiltCalibButton->setObjectName(QString::fromUtf8("TiltCalibButton"));
        TiltCalibButton->setGeometry(QRect(640, 110, 120, 120));
        TiltCalibButton->setStyleSheet(QString::fromUtf8("background-image:url(://Service_Calib/Service_Calib/tilt_calib_icon.png);\n"
""));
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

        retranslateUi(CalibMenu);

        QMetaObject::connectSlotsByName(CalibMenu);
    } // setupUi

    void retranslateUi(QWidget *CalibMenu)
    {
        CalibMenu->setWindowTitle(QApplication::translate("CalibMenu", "Form", 0, QApplication::UnicodeUTF8));
        exitButton->setText(QString());
        kVCalibButton->setText(QString());
        HVCalibButton->setText(QString());
        PositionCalibButton->setText(QString());
        CompressionCalibButton->setText(QString());
        FilterCalibButton->setText(QString());
        StarterCalibButton->setText(QString());
        TiltCalibButton->setText(QString());
        serviceSetupDiagnosticLabel->setText(QApplication::translate("CalibMenu", "Service Panel/Calibrate", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CalibMenu: public Ui_CalibMenu {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CALIBMENU_H
