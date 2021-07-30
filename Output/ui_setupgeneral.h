/********************************************************************************
** Form generated from reading UI file 'setupgeneral.ui'
**
** Created: Fri Sep 13 11:22:47 2019
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETUPGENERAL_H
#define UI_SETUPGENERAL_H

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

class Ui_SetupGeneral
{
public:
    QFrame *frame;
    QFrame *frame_17;
    QPushButton *pushButtonAccessoryEna;
    QLabel *label;
    QFrame *frame_15;
    QLabel *label_2;
    QLineEdit *editStarterHsTimeout;
    QLabel *serviceSetupDiagnosticLabel;
    QFrame *frame_2;
    QPushButton *exitButton;
    QFrame *frame_16;
    QPushButton *pushButtonUnlockCompressor;
    QLabel *label_3;

    void setupUi(QWidget *SetupGeneral)
    {
        if (SetupGeneral->objectName().isEmpty())
            SetupGeneral->setObjectName(QString::fromUtf8("SetupGeneral"));
        SetupGeneral->resize(800, 480);
        SetupGeneral->setStyleSheet(QString::fromUtf8("background-color:rgb(0,0,0,0);\n"
"background-image:url(://Service_Setup/Service_Setup/setup_menu.png);\n"
"border: 0px;"));
        frame = new QFrame(SetupGeneral);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(0, 0, 800, 480));
        frame->setStyleSheet(QString::fromUtf8(""));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        frame_17 = new QFrame(frame);
        frame_17->setObjectName(QString::fromUtf8("frame_17"));
        frame_17->setGeometry(QRect(195, 100, 400, 50));
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
        pushButtonAccessoryEna = new QPushButton(frame_17);
        pushButtonAccessoryEna->setObjectName(QString::fromUtf8("pushButtonAccessoryEna"));
        pushButtonAccessoryEna->setGeometry(QRect(295, 5, 100, 41));
        pushButtonAccessoryEna->setStyleSheet(QString::fromUtf8("\n"
"background:rgba(0,0,0,0);\n"
"border:0px;\n"
"outline: 0px;"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/toggle_off.png"), QSize(), QIcon::Selected, QIcon::Off);
        icon.addFile(QString::fromUtf8(":/toggle_on.png"), QSize(), QIcon::Selected, QIcon::On);
        pushButtonAccessoryEna->setIcon(icon);
        pushButtonAccessoryEna->setIconSize(QSize(40, 40));
        pushButtonAccessoryEna->setCheckable(true);
        pushButtonAccessoryEna->setChecked(false);
        label = new QLabel(frame_17);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 0, 236, 46));
        QFont font;
        font.setFamily(QString::fromUtf8("Bitstream Charter"));
        font.setPointSize(16);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 153);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        frame_15 = new QFrame(frame);
        frame_15->setObjectName(QString::fromUtf8("frame_15"));
        frame_15->setGeometry(QRect(195, 185, 400, 50));
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
        label_2 = new QLabel(frame_15);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 0, 236, 46));
        label_2->setFont(font);
        label_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 153);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        editStarterHsTimeout = new QLineEdit(frame_15);
        editStarterHsTimeout->setObjectName(QString::fromUtf8("editStarterHsTimeout"));
        editStarterHsTimeout->setGeometry(QRect(310, 10, 81, 31));
        QFont font1;
        font1.setFamily(QString::fromUtf8("DejaVu Sans"));
        font1.setPointSize(18);
        font1.setBold(false);
        font1.setItalic(false);
        font1.setWeight(9);
        editStarterHsTimeout->setFont(font1);
        editStarterHsTimeout->setFocusPolicy(Qt::NoFocus);
        editStarterHsTimeout->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(79, 79, 79);\n"
"border-left-color: rgb(255, 255, 153);\n"
"border-bottom-color: rgb(255, 255, 153);\n"
"border-top-color: rgb(255, 255, 153);\n"
"font: 75 18pt \"DejaVu Sans\";"));
        editStarterHsTimeout->setAlignment(Qt::AlignCenter);
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
"background-image:url(://Service_Setup/Service_Setup/general_icon.png);\n"
"border: 0px;"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        exitButton = new QPushButton(frame);
        exitButton->setObjectName(QString::fromUtf8("exitButton"));
        exitButton->setGeometry(QRect(0, 0, 111, 111));
        frame_16 = new QFrame(frame);
        frame_16->setObjectName(QString::fromUtf8("frame_16"));
        frame_16->setGeometry(QRect(195, 270, 400, 50));
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
        pushButtonUnlockCompressor = new QPushButton(frame_16);
        pushButtonUnlockCompressor->setObjectName(QString::fromUtf8("pushButtonUnlockCompressor"));
        pushButtonUnlockCompressor->setGeometry(QRect(295, 0, 100, 46));
        pushButtonUnlockCompressor->setStyleSheet(QString::fromUtf8("\n"
"background:rgba(0,0,0,0);\n"
"border:0px;\n"
"outline: 0px;"));
        pushButtonUnlockCompressor->setIcon(icon);
        pushButtonUnlockCompressor->setIconSize(QSize(40, 40));
        pushButtonUnlockCompressor->setCheckable(true);
        pushButtonUnlockCompressor->setChecked(false);
        label_3 = new QLabel(frame_16);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 0, 236, 46));
        label_3->setFont(font);
        label_3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 153);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));

        retranslateUi(SetupGeneral);

        QMetaObject::connectSlotsByName(SetupGeneral);
    } // setupUi

    void retranslateUi(QWidget *SetupGeneral)
    {
        SetupGeneral->setWindowTitle(QApplication::translate("SetupGeneral", "Form", 0, QApplication::UnicodeUTF8));
        pushButtonAccessoryEna->setText(QString());
        label->setText(QApplication::translate("SetupGeneral", "Patient protection detection", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("SetupGeneral", "Starter HS timeout (s)", 0, QApplication::UnicodeUTF8));
        editStarterHsTimeout->setText(QApplication::translate("SetupGeneral", "0", 0, QApplication::UnicodeUTF8));
        serviceSetupDiagnosticLabel->setText(QApplication::translate("SetupGeneral", "Service Panel/Setup/General", 0, QApplication::UnicodeUTF8));
        exitButton->setText(QString());
        pushButtonUnlockCompressor->setText(QString());
        label_3->setText(QApplication::translate("SetupGeneral", "Unlock compressor", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SetupGeneral: public Ui_SetupGeneral {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETUPGENERAL_H
