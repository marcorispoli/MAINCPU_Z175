/********************************************************************************
** Form generated from reading UI file 'setupspecial.ui'
**
** Created: Fri Sep 13 11:22:47 2019
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETUPSPECIAL_H
#define UI_SETUPSPECIAL_H

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

class Ui_SetupSpecial
{
public:
    QFrame *frame;
    QLabel *serviceSetupDiagnosticLabel;
    QFrame *frame_2;
    QPushButton *exitButton;
    QFrame *frame_24;
    QPushButton *pushButtonDgnDemo;
    QLabel *labelDemoMode;
    QFrame *frame_25;
    QPushButton *pushButtonTomo2F;
    QLabel *labelTomo2F;
    QFrame *frame_26;
    QPushButton *pushButtonDeadMan;
    QLabel *labelDeadMan;

    void setupUi(QWidget *SetupSpecial)
    {
        if (SetupSpecial->objectName().isEmpty())
            SetupSpecial->setObjectName(QString::fromUtf8("SetupSpecial"));
        SetupSpecial->resize(800, 480);
        SetupSpecial->setStyleSheet(QString::fromUtf8("background-color:rgb(0,0,0,0);\n"
"background-image:url(://Service_Setup/Service_Setup/setup_menu.png);\n"
"border: 0px;"));
        frame = new QFrame(SetupSpecial);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(0, 0, 800, 480));
        frame->setStyleSheet(QString::fromUtf8(""));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        serviceSetupDiagnosticLabel = new QLabel(frame);
        serviceSetupDiagnosticLabel->setObjectName(QString::fromUtf8("serviceSetupDiagnosticLabel"));
        serviceSetupDiagnosticLabel->setGeometry(QRect(105, 10, 616, 41));
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
        frame_2 = new QFrame(frame);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setGeometry(QRect(640, 330, 151, 136));
        frame_2->setStyleSheet(QString::fromUtf8("background-color:rgb(0,0,0,0);\n"
"background-image:url(://Service_Setup/Service_Setup/special_icon.png);\n"
"border: 0px;"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        exitButton = new QPushButton(frame);
        exitButton->setObjectName(QString::fromUtf8("exitButton"));
        exitButton->setGeometry(QRect(0, 0, 111, 111));
        frame_24 = new QFrame(frame);
        frame_24->setObjectName(QString::fromUtf8("frame_24"));
        frame_24->setGeometry(QRect(200, 95, 400, 50));
        frame_24->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(74, 74, 74,30);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:10px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        frame_24->setFrameShape(QFrame::StyledPanel);
        frame_24->setFrameShadow(QFrame::Raised);
        pushButtonDgnDemo = new QPushButton(frame_24);
        pushButtonDgnDemo->setObjectName(QString::fromUtf8("pushButtonDgnDemo"));
        pushButtonDgnDemo->setGeometry(QRect(295, 5, 100, 40));
        pushButtonDgnDemo->setStyleSheet(QString::fromUtf8("\n"
"background:rgba(0,0,0,0);\n"
"border:0px;\n"
"outline: 0px;"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/toggle_off.png"), QSize(), QIcon::Selected, QIcon::Off);
        icon.addFile(QString::fromUtf8(":/toggle_on.png"), QSize(), QIcon::Selected, QIcon::On);
        pushButtonDgnDemo->setIcon(icon);
        pushButtonDgnDemo->setIconSize(QSize(40, 40));
        pushButtonDgnDemo->setCheckable(true);
        pushButtonDgnDemo->setChecked(false);
        labelDemoMode = new QLabel(frame_24);
        labelDemoMode->setObjectName(QString::fromUtf8("labelDemoMode"));
        labelDemoMode->setGeometry(QRect(10, 5, 236, 40));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Bitstream Charter"));
        font1.setPointSize(16);
        font1.setBold(true);
        font1.setWeight(75);
        labelDemoMode->setFont(font1);
        labelDemoMode->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 153);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        frame_25 = new QFrame(frame);
        frame_25->setObjectName(QString::fromUtf8("frame_25"));
        frame_25->setGeometry(QRect(200, 165, 400, 50));
        frame_25->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(74, 74, 74,30);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:10px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        frame_25->setFrameShape(QFrame::StyledPanel);
        frame_25->setFrameShadow(QFrame::Raised);
        pushButtonTomo2F = new QPushButton(frame_25);
        pushButtonTomo2F->setObjectName(QString::fromUtf8("pushButtonTomo2F"));
        pushButtonTomo2F->setGeometry(QRect(295, 5, 100, 40));
        pushButtonTomo2F->setStyleSheet(QString::fromUtf8("\n"
"background:rgba(0,0,0,0);\n"
"border:0px;\n"
"outline: 0px;"));
        pushButtonTomo2F->setIcon(icon);
        pushButtonTomo2F->setIconSize(QSize(40, 40));
        pushButtonTomo2F->setCheckable(true);
        pushButtonTomo2F->setChecked(false);
        labelTomo2F = new QLabel(frame_25);
        labelTomo2F->setObjectName(QString::fromUtf8("labelTomo2F"));
        labelTomo2F->setGeometry(QRect(10, 5, 236, 40));
        labelTomo2F->setFont(font1);
        labelTomo2F->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 153);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        frame_26 = new QFrame(frame);
        frame_26->setObjectName(QString::fromUtf8("frame_26"));
        frame_26->setGeometry(QRect(200, 235, 400, 50));
        frame_26->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(74, 74, 74,30);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:10px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        frame_26->setFrameShape(QFrame::StyledPanel);
        frame_26->setFrameShadow(QFrame::Raised);
        pushButtonDeadMan = new QPushButton(frame_26);
        pushButtonDeadMan->setObjectName(QString::fromUtf8("pushButtonDeadMan"));
        pushButtonDeadMan->setGeometry(QRect(295, 5, 100, 40));
        pushButtonDeadMan->setStyleSheet(QString::fromUtf8("\n"
"background:rgba(0,0,0,0);\n"
"border:0px;\n"
"outline: 0px;"));
        pushButtonDeadMan->setIcon(icon);
        pushButtonDeadMan->setIconSize(QSize(40, 40));
        pushButtonDeadMan->setCheckable(true);
        pushButtonDeadMan->setChecked(false);
        labelDeadMan = new QLabel(frame_26);
        labelDeadMan->setObjectName(QString::fromUtf8("labelDeadMan"));
        labelDeadMan->setGeometry(QRect(10, 5, 236, 40));
        labelDeadMan->setFont(font1);
        labelDeadMan->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 153);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));

        retranslateUi(SetupSpecial);

        QMetaObject::connectSlotsByName(SetupSpecial);
    } // setupUi

    void retranslateUi(QWidget *SetupSpecial)
    {
        SetupSpecial->setWindowTitle(QApplication::translate("SetupSpecial", "Form", 0, QApplication::UnicodeUTF8));
        serviceSetupDiagnosticLabel->setText(QApplication::translate("SetupSpecial", "Service Panel/Setup/Special", 0, QApplication::UnicodeUTF8));
        exitButton->setText(QString());
        pushButtonDgnDemo->setText(QString());
        labelDemoMode->setText(QApplication::translate("SetupSpecial", "DEMO MODE", 0, QApplication::UnicodeUTF8));
        pushButtonTomo2F->setText(QString());
        labelTomo2F->setText(QApplication::translate("SetupSpecial", "TOMO MODE 2F", 0, QApplication::UnicodeUTF8));
        pushButtonDeadMan->setText(QString());
        labelDeadMan->setText(QApplication::translate("SetupSpecial", "MOTION DEAD MAN", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SetupSpecial: public Ui_SetupSpecial {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETUPSPECIAL_H
