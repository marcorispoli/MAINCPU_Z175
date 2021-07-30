/********************************************************************************
** Form generated from reading UI file 'setupdiagnostic.ui'
**
** Created: Fri Sep 13 11:22:47 2019
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETUPDIAGNOSTIC_H
#define UI_SETUPDIAGNOSTIC_H

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

class Ui_SetupDiagnostic
{
public:
    QFrame *frame;
    QFrame *frame_17;
    QPushButton *pushButtonDgnHVEna;
    QLabel *labelMonitorHV;
    QFrame *frame_15;
    QPushButton *pushButtonDgnHeartEna;
    QLabel *labelMonitorHeart;
    QFrame *frame_16;
    QPushButton *pushButtonDgnFilEna;
    QLabel *labelMonitorFil;
    QLabel *labelWarmFil;
    QLineEdit *editWarmingFil;
    QFrame *frame_18;
    QPushButton *pushButtonDgnMasEna;
    QLabel *labelMonitorMas;
    QLabel *labelIAnode;
    QLineEdit *editIAnodeTest;
    QLabel *serviceSetupDiagnosticLabel;
    QFrame *frame_2;
    QPushButton *exitButton;

    void setupUi(QWidget *SetupDiagnostic)
    {
        if (SetupDiagnostic->objectName().isEmpty())
            SetupDiagnostic->setObjectName(QString::fromUtf8("SetupDiagnostic"));
        SetupDiagnostic->resize(800, 480);
        SetupDiagnostic->setStyleSheet(QString::fromUtf8("background-color:rgb(0,0,0,0);\n"
"background-image:url(://Service_Setup/Service_Setup/setup_menu.png);\n"
"border: 0px;"));
        frame = new QFrame(SetupDiagnostic);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(0, 0, 800, 480));
        frame->setStyleSheet(QString::fromUtf8(""));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        frame_17 = new QFrame(frame);
        frame_17->setObjectName(QString::fromUtf8("frame_17"));
        frame_17->setGeometry(QRect(205, 80, 400, 50));
        frame_17->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(74, 74, 74,30);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:10px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        frame_17->setFrameShape(QFrame::StyledPanel);
        frame_17->setFrameShadow(QFrame::Raised);
        pushButtonDgnHVEna = new QPushButton(frame_17);
        pushButtonDgnHVEna->setObjectName(QString::fromUtf8("pushButtonDgnHVEna"));
        pushButtonDgnHVEna->setGeometry(QRect(295, 5, 100, 41));
        pushButtonDgnHVEna->setStyleSheet(QString::fromUtf8("\n"
"background:rgba(0,0,0,0);\n"
"border:0px;\n"
"outline: 0px;"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/toggle_off.png"), QSize(), QIcon::Selected, QIcon::Off);
        icon.addFile(QString::fromUtf8(":/toggle_on.png"), QSize(), QIcon::Selected, QIcon::On);
        pushButtonDgnHVEna->setIcon(icon);
        pushButtonDgnHVEna->setIconSize(QSize(40, 40));
        pushButtonDgnHVEna->setCheckable(true);
        pushButtonDgnHVEna->setChecked(false);
        labelMonitorHV = new QLabel(frame_17);
        labelMonitorHV->setObjectName(QString::fromUtf8("labelMonitorHV"));
        labelMonitorHV->setGeometry(QRect(10, 0, 236, 46));
        QFont font;
        font.setFamily(QString::fromUtf8("Bitstream Charter"));
        font.setPointSize(16);
        font.setBold(true);
        font.setWeight(75);
        labelMonitorHV->setFont(font);
        labelMonitorHV->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 153);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        frame_15 = new QFrame(frame);
        frame_15->setObjectName(QString::fromUtf8("frame_15"));
        frame_15->setGeometry(QRect(205, 145, 400, 50));
        frame_15->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(74, 74, 74,30);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:10px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        frame_15->setFrameShape(QFrame::StyledPanel);
        frame_15->setFrameShadow(QFrame::Raised);
        pushButtonDgnHeartEna = new QPushButton(frame_15);
        pushButtonDgnHeartEna->setObjectName(QString::fromUtf8("pushButtonDgnHeartEna"));
        pushButtonDgnHeartEna->setGeometry(QRect(295, 0, 100, 46));
        pushButtonDgnHeartEna->setStyleSheet(QString::fromUtf8("\n"
"background:rgba(0,0,0,0);\n"
"border:0px;\n"
"outline: 0px;"));
        pushButtonDgnHeartEna->setIcon(icon);
        pushButtonDgnHeartEna->setIconSize(QSize(40, 40));
        pushButtonDgnHeartEna->setCheckable(true);
        pushButtonDgnHeartEna->setChecked(false);
        labelMonitorHeart = new QLabel(frame_15);
        labelMonitorHeart->setObjectName(QString::fromUtf8("labelMonitorHeart"));
        labelMonitorHeart->setGeometry(QRect(10, 0, 236, 46));
        labelMonitorHeart->setFont(font);
        labelMonitorHeart->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 153);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        frame_16 = new QFrame(frame);
        frame_16->setObjectName(QString::fromUtf8("frame_16"));
        frame_16->setGeometry(QRect(205, 210, 400, 90));
        frame_16->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(74, 74, 74,30);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:10px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        frame_16->setFrameShape(QFrame::StyledPanel);
        frame_16->setFrameShadow(QFrame::Raised);
        pushButtonDgnFilEna = new QPushButton(frame_16);
        pushButtonDgnFilEna->setObjectName(QString::fromUtf8("pushButtonDgnFilEna"));
        pushButtonDgnFilEna->setGeometry(QRect(295, 0, 100, 46));
        pushButtonDgnFilEna->setStyleSheet(QString::fromUtf8("\n"
"background:rgba(0,0,0,0);\n"
"border:0px;\n"
"outline: 0px;"));
        pushButtonDgnFilEna->setIcon(icon);
        pushButtonDgnFilEna->setIconSize(QSize(40, 40));
        pushButtonDgnFilEna->setCheckable(true);
        pushButtonDgnFilEna->setChecked(false);
        labelMonitorFil = new QLabel(frame_16);
        labelMonitorFil->setObjectName(QString::fromUtf8("labelMonitorFil"));
        labelMonitorFil->setGeometry(QRect(10, 5, 236, 36));
        labelMonitorFil->setFont(font);
        labelMonitorFil->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 153);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        labelWarmFil = new QLabel(frame_16);
        labelWarmFil->setObjectName(QString::fromUtf8("labelWarmFil"));
        labelWarmFil->setGeometry(QRect(10, 40, 266, 46));
        labelWarmFil->setFont(font);
        labelWarmFil->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 153);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        editWarmingFil = new QLineEdit(frame_16);
        editWarmingFil->setObjectName(QString::fromUtf8("editWarmingFil"));
        editWarmingFil->setGeometry(QRect(310, 50, 81, 31));
        QFont font1;
        font1.setFamily(QString::fromUtf8("DejaVu Sans"));
        font1.setPointSize(18);
        font1.setBold(false);
        font1.setItalic(false);
        font1.setWeight(9);
        editWarmingFil->setFont(font1);
        editWarmingFil->setFocusPolicy(Qt::NoFocus);
        editWarmingFil->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(79, 79, 79);\n"
"border-left-color: rgb(255, 255, 153);\n"
"border-bottom-color: rgb(255, 255, 153);\n"
"border-top-color: rgb(255, 255, 153);\n"
"font: 75 18pt \"DejaVu Sans\";"));
        editWarmingFil->setAlignment(Qt::AlignCenter);
        frame_18 = new QFrame(frame);
        frame_18->setObjectName(QString::fromUtf8("frame_18"));
        frame_18->setGeometry(QRect(205, 315, 400, 90));
        frame_18->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(74, 74, 74,30);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:10px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        frame_18->setFrameShape(QFrame::StyledPanel);
        frame_18->setFrameShadow(QFrame::Raised);
        pushButtonDgnMasEna = new QPushButton(frame_18);
        pushButtonDgnMasEna->setObjectName(QString::fromUtf8("pushButtonDgnMasEna"));
        pushButtonDgnMasEna->setGeometry(QRect(295, 0, 100, 46));
        pushButtonDgnMasEna->setStyleSheet(QString::fromUtf8("\n"
"background:rgba(0,0,0,0);\n"
"border:0px;\n"
"outline: 0px;"));
        pushButtonDgnMasEna->setIcon(icon);
        pushButtonDgnMasEna->setIconSize(QSize(40, 40));
        pushButtonDgnMasEna->setCheckable(true);
        pushButtonDgnMasEna->setChecked(false);
        labelMonitorMas = new QLabel(frame_18);
        labelMonitorMas->setObjectName(QString::fromUtf8("labelMonitorMas"));
        labelMonitorMas->setGeometry(QRect(10, 5, 236, 36));
        labelMonitorMas->setFont(font);
        labelMonitorMas->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 153);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        labelIAnode = new QLabel(frame_18);
        labelIAnode->setObjectName(QString::fromUtf8("labelIAnode"));
        labelIAnode->setGeometry(QRect(10, 40, 266, 46));
        labelIAnode->setFont(font);
        labelIAnode->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 153);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        editIAnodeTest = new QLineEdit(frame_18);
        editIAnodeTest->setObjectName(QString::fromUtf8("editIAnodeTest"));
        editIAnodeTest->setGeometry(QRect(315, 50, 76, 31));
        editIAnodeTest->setFont(font1);
        editIAnodeTest->setFocusPolicy(Qt::NoFocus);
        editIAnodeTest->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(79, 79, 79);\n"
"border-left-color: rgb(255, 255, 153);\n"
"border-bottom-color: rgb(255, 255, 153);\n"
"border-top-color: rgb(255, 255, 153);\n"
"font: 75 18pt \"DejaVu Sans\";"));
        editIAnodeTest->setAlignment(Qt::AlignCenter);
        serviceSetupDiagnosticLabel = new QLabel(frame);
        serviceSetupDiagnosticLabel->setObjectName(QString::fromUtf8("serviceSetupDiagnosticLabel"));
        serviceSetupDiagnosticLabel->setGeometry(QRect(105, 10, 616, 41));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Bitstream Charter"));
        font2.setPointSize(24);
        font2.setBold(true);
        font2.setItalic(true);
        font2.setWeight(75);
        serviceSetupDiagnosticLabel->setFont(font2);
        serviceSetupDiagnosticLabel->setStyleSheet(QString::fromUtf8("background-image:url(/transparent.png);\n"
"color: rgb(248, 248, 54);"));
        serviceSetupDiagnosticLabel->setAlignment(Qt::AlignCenter);
        frame_2 = new QFrame(frame);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setGeometry(QRect(640, 350, 151, 121));
        frame_2->setStyleSheet(QString::fromUtf8("background-color:rgb(0,0,0,0);\n"
"background-image:url(://Service_Setup/Service_Setup/diagnostic_icon.png);\n"
"border: 0px;"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        exitButton = new QPushButton(frame);
        exitButton->setObjectName(QString::fromUtf8("exitButton"));
        exitButton->setGeometry(QRect(0, 0, 111, 111));

        retranslateUi(SetupDiagnostic);

        QMetaObject::connectSlotsByName(SetupDiagnostic);
    } // setupUi

    void retranslateUi(QWidget *SetupDiagnostic)
    {
        SetupDiagnostic->setWindowTitle(QApplication::translate("SetupDiagnostic", "Form", 0, QApplication::UnicodeUTF8));
        pushButtonDgnHVEna->setText(QString());
        labelMonitorHV->setText(QApplication::translate("SetupDiagnostic", "Monitor HV Power Supply", 0, QApplication::UnicodeUTF8));
        pushButtonDgnHeartEna->setText(QString());
        labelMonitorHeart->setText(QApplication::translate("SetupDiagnostic", "Monitor heart connection", 0, QApplication::UnicodeUTF8));
        pushButtonDgnFilEna->setText(QString());
        labelMonitorFil->setText(QApplication::translate("SetupDiagnostic", "Monitor Filament current", 0, QApplication::UnicodeUTF8));
        labelWarmFil->setText(QApplication::translate("SetupDiagnostic", "Warming Filament value (mA)", 0, QApplication::UnicodeUTF8));
        editWarmingFil->setText(QApplication::translate("SetupDiagnostic", "0", 0, QApplication::UnicodeUTF8));
        pushButtonDgnMasEna->setText(QString());
        labelMonitorMas->setText(QApplication::translate("SetupDiagnostic", "Monitor mAsmeter", 0, QApplication::UnicodeUTF8));
        labelIAnode->setText(QApplication::translate("SetupDiagnostic", "I Anodic test (mA)", 0, QApplication::UnicodeUTF8));
        editIAnodeTest->setText(QApplication::translate("SetupDiagnostic", "0", 0, QApplication::UnicodeUTF8));
        serviceSetupDiagnosticLabel->setText(QApplication::translate("SetupDiagnostic", "Service Panel/Setup/Diagnostic ", 0, QApplication::UnicodeUTF8));
        exitButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class SetupDiagnostic: public Ui_SetupDiagnostic {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETUPDIAGNOSTIC_H
