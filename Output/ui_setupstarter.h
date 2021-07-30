/********************************************************************************
** Form generated from reading UI file 'setupstarter.ui'
**
** Created: Fri Sep 13 11:22:47 2019
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETUPSTARTER_H
#define UI_SETUPSTARTER_H

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

class Ui_SetupStarter
{
public:
    QFrame *frame;
    QLabel *serviceSetupDiagnosticLabel;
    QFrame *frame_2;
    QPushButton *exitButton;
    QFrame *frame_32;
    QLabel *labelStarterHsTimeout;
    QLineEdit *editStarterHsTimeout;
    QFrame *frame_31;
    QPushButton *pushButtonStarterBrake;
    QLabel *labelStarterBrake;
    QFrame *frame_34;
    QPushButton *pushButtonHsStarter;
    QLabel *labelHsStarter;
    QFrame *frame_35;
    QPushButton *pushButtonStarterOFF;
    QLabel *labelStarterOff;

    void setupUi(QWidget *SetupStarter)
    {
        if (SetupStarter->objectName().isEmpty())
            SetupStarter->setObjectName(QString::fromUtf8("SetupStarter"));
        SetupStarter->resize(800, 480);
        SetupStarter->setStyleSheet(QString::fromUtf8("background-color:rgb(0,0,0,0);\n"
"background-image:url(://Service_Setup/Service_Setup/setup_menu.png);\n"
"border: 0px;"));
        frame = new QFrame(SetupStarter);
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
        frame_2->setGeometry(QRect(635, 295, 151, 151));
        frame_2->setStyleSheet(QString::fromUtf8("background-color:rgb(0,0,0,0);\n"
"background-image:url(://Service_Setup/Service_Setup/starter_icon.png);\n"
"border: 0px;"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        exitButton = new QPushButton(frame);
        exitButton->setObjectName(QString::fromUtf8("exitButton"));
        exitButton->setGeometry(QRect(0, 0, 111, 111));
        frame_32 = new QFrame(frame);
        frame_32->setObjectName(QString::fromUtf8("frame_32"));
        frame_32->setGeometry(QRect(195, 305, 400, 50));
        frame_32->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(74, 74, 74,30);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:10px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        frame_32->setFrameShape(QFrame::StyledPanel);
        frame_32->setFrameShadow(QFrame::Raised);
        labelStarterHsTimeout = new QLabel(frame_32);
        labelStarterHsTimeout->setObjectName(QString::fromUtf8("labelStarterHsTimeout"));
        labelStarterHsTimeout->setGeometry(QRect(10, 5, 236, 36));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Bitstream Charter"));
        font1.setPointSize(16);
        font1.setBold(true);
        font1.setWeight(75);
        labelStarterHsTimeout->setFont(font1);
        labelStarterHsTimeout->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 153);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        editStarterHsTimeout = new QLineEdit(frame_32);
        editStarterHsTimeout->setObjectName(QString::fromUtf8("editStarterHsTimeout"));
        editStarterHsTimeout->setGeometry(QRect(310, 10, 76, 31));
        QFont font2;
        font2.setFamily(QString::fromUtf8("DejaVu Sans"));
        font2.setPointSize(18);
        font2.setBold(false);
        font2.setItalic(false);
        font2.setWeight(9);
        editStarterHsTimeout->setFont(font2);
        editStarterHsTimeout->setFocusPolicy(Qt::NoFocus);
        editStarterHsTimeout->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(79, 79, 79);\n"
"border-left-color: rgb(255, 255, 153);\n"
"border-bottom-color: rgb(255, 255, 153);\n"
"border-top-color: rgb(255, 255, 153);\n"
"font: 75 18pt \"DejaVu Sans\";"));
        editStarterHsTimeout->setAlignment(Qt::AlignCenter);
        frame_31 = new QFrame(frame);
        frame_31->setObjectName(QString::fromUtf8("frame_31"));
        frame_31->setGeometry(QRect(195, 230, 400, 50));
        frame_31->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(74, 74, 74,30);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:10px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        frame_31->setFrameShape(QFrame::StyledPanel);
        frame_31->setFrameShadow(QFrame::Raised);
        pushButtonStarterBrake = new QPushButton(frame_31);
        pushButtonStarterBrake->setObjectName(QString::fromUtf8("pushButtonStarterBrake"));
        pushButtonStarterBrake->setGeometry(QRect(295, 5, 100, 40));
        pushButtonStarterBrake->setStyleSheet(QString::fromUtf8("\n"
"background:rgba(0,0,0,0);\n"
"border:0px;\n"
"outline: 0px;"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/toggle_off.png"), QSize(), QIcon::Selected, QIcon::Off);
        icon.addFile(QString::fromUtf8(":/toggle_on.png"), QSize(), QIcon::Selected, QIcon::On);
        pushButtonStarterBrake->setIcon(icon);
        pushButtonStarterBrake->setIconSize(QSize(40, 40));
        pushButtonStarterBrake->setCheckable(true);
        pushButtonStarterBrake->setChecked(false);
        labelStarterBrake = new QLabel(frame_31);
        labelStarterBrake->setObjectName(QString::fromUtf8("labelStarterBrake"));
        labelStarterBrake->setGeometry(QRect(10, 5, 256, 36));
        labelStarterBrake->setFont(font1);
        labelStarterBrake->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 153);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        frame_34 = new QFrame(frame);
        frame_34->setObjectName(QString::fromUtf8("frame_34"));
        frame_34->setGeometry(QRect(195, 85, 400, 50));
        frame_34->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(74, 74, 74,30);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:10px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        frame_34->setFrameShape(QFrame::StyledPanel);
        frame_34->setFrameShadow(QFrame::Raised);
        pushButtonHsStarter = new QPushButton(frame_34);
        pushButtonHsStarter->setObjectName(QString::fromUtf8("pushButtonHsStarter"));
        pushButtonHsStarter->setGeometry(QRect(295, 5, 100, 40));
        pushButtonHsStarter->setStyleSheet(QString::fromUtf8("\n"
"background:rgba(0,0,0,0);\n"
"border:0px;\n"
"outline: 0px;"));
        pushButtonHsStarter->setIcon(icon);
        pushButtonHsStarter->setIconSize(QSize(40, 40));
        pushButtonHsStarter->setCheckable(true);
        pushButtonHsStarter->setChecked(false);
        labelHsStarter = new QLabel(frame_34);
        labelHsStarter->setObjectName(QString::fromUtf8("labelHsStarter"));
        labelHsStarter->setGeometry(QRect(10, 5, 241, 41));
        labelHsStarter->setFont(font1);
        labelHsStarter->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 153);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        frame_35 = new QFrame(frame);
        frame_35->setObjectName(QString::fromUtf8("frame_35"));
        frame_35->setGeometry(QRect(195, 155, 400, 50));
        frame_35->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(74, 74, 74,30);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:10px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        frame_35->setFrameShape(QFrame::StyledPanel);
        frame_35->setFrameShadow(QFrame::Raised);
        pushButtonStarterOFF = new QPushButton(frame_35);
        pushButtonStarterOFF->setObjectName(QString::fromUtf8("pushButtonStarterOFF"));
        pushButtonStarterOFF->setGeometry(QRect(295, 5, 100, 40));
        pushButtonStarterOFF->setStyleSheet(QString::fromUtf8("\n"
"background:rgba(0,0,0,0);\n"
"border:0px;\n"
"outline: 0px;"));
        pushButtonStarterOFF->setIcon(icon);
        pushButtonStarterOFF->setIconSize(QSize(40, 40));
        pushButtonStarterOFF->setCheckable(true);
        pushButtonStarterOFF->setChecked(false);
        labelStarterOff = new QLabel(frame_35);
        labelStarterOff->setObjectName(QString::fromUtf8("labelStarterOff"));
        labelStarterOff->setGeometry(QRect(10, 5, 241, 36));
        labelStarterOff->setFont(font1);
        labelStarterOff->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 153);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));

        retranslateUi(SetupStarter);

        QMetaObject::connectSlotsByName(SetupStarter);
    } // setupUi

    void retranslateUi(QWidget *SetupStarter)
    {
        SetupStarter->setWindowTitle(QApplication::translate("SetupStarter", "Form", 0, QApplication::UnicodeUTF8));
        serviceSetupDiagnosticLabel->setText(QApplication::translate("SetupStarter", "Service Panel/Setup/Starter", 0, QApplication::UnicodeUTF8));
        exitButton->setText(QString());
        labelStarterHsTimeout->setText(QApplication::translate("SetupStarter", "Starter keeping time (ms)", 0, QApplication::UnicodeUTF8));
        editStarterHsTimeout->setText(QApplication::translate("SetupStarter", "0", 0, QApplication::UnicodeUTF8));
        pushButtonStarterBrake->setText(QString());
        labelStarterBrake->setText(QApplication::translate("SetupStarter", "Use of the brake function", 0, QApplication::UnicodeUTF8));
        pushButtonHsStarter->setText(QString());
        labelHsStarter->setText(QApplication::translate("SetupStarter", "High Speed Starter enable", 0, QApplication::UnicodeUTF8));
        pushButtonStarterOFF->setText(QString());
        labelStarterOff->setText(QApplication::translate("SetupStarter", "Starter OFF after exposure", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SetupStarter: public Ui_SetupStarter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETUPSTARTER_H
