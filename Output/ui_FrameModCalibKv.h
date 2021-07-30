/********************************************************************************
** Form generated from reading UI file 'FrameModCalibKv.ui'
**
** Created: Fri Sep 13 11:22:47 2019
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRAMEMODCALIBKV_H
#define UI_FRAMEMODCALIBKV_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLCDNumber>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QFrame *kvOffsetModifyForm;
    QLabel *testo;
    QLCDNumber *display;
    QPushButton *pushDecButton;
    QPushButton *pushIncButton;
    QPushButton *pushOkButt;
    QLabel *quadro;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QString::fromUtf8("Form"));
        Form->resize(480, 640);
        kvOffsetModifyForm = new QFrame(Form);
        kvOffsetModifyForm->setObjectName(QString::fromUtf8("kvOffsetModifyForm"));
        kvOffsetModifyForm->setGeometry(QRect(5, 5, 306, 366));
        kvOffsetModifyForm->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: qlineargradient(spread:pad, x1:0.75, y1:0.824, x2:0.037, y2:0.05, stop:0 rgba(30,30,30, 250), stop:1 rgba(200, 200, 200, 220));\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:10px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);\n"
""));
        kvOffsetModifyForm->setFrameShape(QFrame::StyledPanel);
        kvOffsetModifyForm->setFrameShadow(QFrame::Raised);
        testo = new QLabel(kvOffsetModifyForm);
        testo->setObjectName(QString::fromUtf8("testo"));
        testo->setGeometry(QRect(15, 15, 271, 36));
        QFont font;
        font.setFamily(QString::fromUtf8("Bitstream Charter"));
        font.setPointSize(16);
        font.setBold(true);
        font.setWeight(75);
        testo->setFont(font);
        testo->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(255, 251, 123,40);\n"
"border: 2px solid  rgb(74, 74, 74);\n"
"border-radius:4px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);\n"
""));
        testo->setText(QString::fromUtf8("TextLabel"));
        testo->setAlignment(Qt::AlignCenter);
        display = new QLCDNumber(kvOffsetModifyForm);
        display->setObjectName(QString::fromUtf8("display"));
        display->setGeometry(QRect(20, 60, 91, 53));
        display->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: qrgba(0, 0, 0, 100);\n"
"border: 2px solid  rgb(74, 74, 74);\n"
"border-radius:1px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);\n"
""));
        pushDecButton = new QPushButton(kvOffsetModifyForm);
        pushDecButton->setObjectName(QString::fromUtf8("pushDecButton"));
        pushDecButton->setGeometry(QRect(20, 205, 91, 51));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Bitstream Charter"));
        font1.setPointSize(22);
        font1.setBold(true);
        font1.setItalic(false);
        font1.setWeight(75);
        pushDecButton->setFont(font1);
        pushDecButton->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(139, 139, 139, 50);\n"
"border: 2px solid  rgb(0, 0, 0);\n"
"border-radius:4px;\n"
"border-right-color: rgba(0, 0, 0,40);\n"
"border-left-color: rgba(255, 255, 255,100);\n"
"border-top-color: rgba(255, 255, 153,0);\n"
""));
        pushDecButton->setText(QString::fromUtf8("-"));
        pushDecButton->setAutoRepeat(true);
        pushDecButton->setAutoRepeatDelay(1000);
        pushDecButton->setAutoRepeatInterval(500);
        pushIncButton = new QPushButton(kvOffsetModifyForm);
        pushIncButton->setObjectName(QString::fromUtf8("pushIncButton"));
        pushIncButton->setGeometry(QRect(20, 150, 91, 51));
        pushIncButton->setFont(font1);
        pushIncButton->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(139, 139, 139, 50);\n"
"border: 2px solid  rgb(0, 0, 0);\n"
"border-radius:4px;\n"
"border-right-color: rgba(0, 0, 0,40);\n"
"border-left-color: rgba(255, 255, 255,100);\n"
"border-top-color: rgba(255, 255, 153,0);\n"
""));
        pushIncButton->setText(QString::fromUtf8("+"));
        pushIncButton->setAutoRepeat(true);
        pushIncButton->setAutoRepeatDelay(1000);
        pushIncButton->setAutoRepeatInterval(500);
        pushOkButt = new QPushButton(kvOffsetModifyForm);
        pushOkButt->setObjectName(QString::fromUtf8("pushOkButt"));
        pushOkButt->setGeometry(QRect(20, 290, 91, 51));
        pushOkButt->setFont(font1);
        pushOkButt->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(139, 139, 139, 50);\n"
"border: 2px solid  rgb(0, 0, 0);\n"
"border-radius:4px;\n"
"border-right-color: rgba(0, 0, 0,40);\n"
"border-left-color: rgba(255, 255, 255,100);\n"
"border-top-color: rgba(255, 255, 153,0);\n"
""));
        pushOkButt->setText(QString::fromUtf8("OK"));
        quadro = new QLabel(kvOffsetModifyForm);
        quadro->setObjectName(QString::fromUtf8("quadro"));
        quadro->setGeometry(QRect(135, 63, 150, 276));
        quadro->setStyleSheet(QString::fromUtf8("background:url(:/kv_quadro.png);\n"
"border:0px;\n"
"border-radius:0px;\n"
""));
        quadro->setText(QString::fromUtf8(""));
        testo->raise();
        display->raise();
        pushDecButton->raise();
        pushIncButton->raise();
        pushOkButt->raise();
        quadro->raise();

        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRAMEMODCALIBKV_H
