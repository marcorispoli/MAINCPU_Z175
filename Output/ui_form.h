/********************************************************************************
** Form generated from reading UI file 'form.ui'
**
** Created: Fri Sep 13 11:22:47 2019
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORM_H
#define UI_FORM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QFrame *frame;
    QPushButton *fold1Button;
    QPushButton *exitButton;
    QPushButton *fold2Button;
    QPushButton *fold3Button;
    QPushButton *fold4Button;
    QPushButton *fold6Button;
    QPushButton *fold5Button;
    QFrame *frame6;
    QFrame *frameToolTab_2;
    QFrame *frameToolsInit;
    QPushButton *CArmToolButton;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QString::fromUtf8("Form"));
        Form->resize(480, 640);
        frame = new QFrame(Form);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setEnabled(false);
        frame->setGeometry(QRect(5, -5, 485, 645));
        frame->setStyleSheet(QString::fromUtf8("background-color:rgb(0,0,0,0);\n"
"background-image:url(:/fold_win_bkg.png);\n"
"border: 0px;\n"
""));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        fold1Button = new QPushButton(frame);
        fold1Button->setObjectName(QString::fromUtf8("fold1Button"));
        fold1Button->setGeometry(QRect(5, 584, 140, 55));
        fold1Button->setStyleSheet(QString::fromUtf8("background-image:url(:/fold_off_butt.png);\n"
"border: 0px;\n"
"outline: 0px;\n"
"color: rgb(255, 255, 153);\n"
"font: 80 14pt \"DejaVu Sans\";\n"
""));
        exitButton = new QPushButton(frame);
        exitButton->setObjectName(QString::fromUtf8("exitButton"));
        exitButton->setGeometry(QRect(10, 20, 81, 71));
        exitButton->setStyleSheet(QString::fromUtf8("border: 0px;\n"
"background-image:url(:/transparent.png);\n"
"color:rgba(0,0,0,0);\n"
"outline: 0px;\n"
""));
        fold2Button = new QPushButton(frame);
        fold2Button->setObjectName(QString::fromUtf8("fold2Button"));
        fold2Button->setGeometry(QRect(110, 584, 140, 55));
        fold2Button->setStyleSheet(QString::fromUtf8("background-image:url(:/fold_off_butt.png);\n"
"border: 0px;\n"
"outline: 0px;\n"
"color: rgb(255, 255, 153);\n"
"font: 80 14pt \"DejaVu Sans\";\n"
""));
        fold3Button = new QPushButton(frame);
        fold3Button->setObjectName(QString::fromUtf8("fold3Button"));
        fold3Button->setGeometry(QRect(210, 584, 140, 55));
        fold3Button->setStyleSheet(QString::fromUtf8("background-image:url(:/fold_off_butt.png);\n"
"border: 0px;\n"
"outline: 0px;\n"
"color: rgb(255, 255, 153);\n"
"font: 80 14pt \"DejaVu Sans\";\n"
""));
        fold4Button = new QPushButton(frame);
        fold4Button->setObjectName(QString::fromUtf8("fold4Button"));
        fold4Button->setGeometry(QRect(310, 584, 140, 55));
        fold4Button->setStyleSheet(QString::fromUtf8("background-image:url(:/fold_off_butt.png);\n"
"border: 0px;\n"
"outline: 0px;\n"
"color: rgb(255, 255, 153);\n"
"font: 80 14pt \"DejaVu Sans\";\n"
""));
        fold6Button = new QPushButton(frame);
        fold6Button->setObjectName(QString::fromUtf8("fold6Button"));
        fold6Button->setGeometry(QRect(245, 6, 121, 55));
        fold6Button->setStyleSheet(QString::fromUtf8("background-image:url(:/fold_off_butt_top.png);\n"
"border: 0px;\n"
"outline: 0px;\n"
"color: rgb(255, 255, 153);\n"
"font: 80 14pt \"DejaVu Sans\";\n"
""));
        fold6Button->setText(QString::fromUtf8("Tools"));
        fold5Button = new QPushButton(frame);
        fold5Button->setObjectName(QString::fromUtf8("fold5Button"));
        fold5Button->setGeometry(QRect(345, 6, 140, 55));
        fold5Button->setStyleSheet(QString::fromUtf8("background-image:url(:/fold_off_butt_top.png);\n"
"border: 0px;\n"
"outline: 0px;\n"
"color: rgb(255, 255, 153);\n"
"font: 80 14pt \"DejaVu Sans\";\n"
""));
        frame6 = new QFrame(frame);
        frame6->setObjectName(QString::fromUtf8("frame6"));
        frame6->setGeometry(QRect(5, 105, 466, 456));
        frame6->setStyleSheet(QString::fromUtf8("background-color:rgba(201,201,201,0);\n"
"border: 0px;\n"
"background-image:url(:/transparent.png);\n"
""));
        frame6->setFrameShape(QFrame::StyledPanel);
        frame6->setFrameShadow(QFrame::Raised);
        frameToolTab_2 = new QFrame(frame6);
        frameToolTab_2->setObjectName(QString::fromUtf8("frameToolTab_2"));
        frameToolTab_2->setGeometry(QRect(5, 10, 461, 401));
        frameToolTab_2->setStyleSheet(QString::fromUtf8("background-image:url(/transparent.png);\n"
"\n"
"border: 1px solid  rgb(255, 255, 153);\n"
"border-radius:20px;\n"
""));
        frameToolTab_2->setFrameShape(QFrame::StyledPanel);
        frameToolTab_2->setFrameShadow(QFrame::Raised);
        frameToolsInit = new QFrame(frameToolTab_2);
        frameToolsInit->setObjectName(QString::fromUtf8("frameToolsInit"));
        frameToolsInit->setGeometry(QRect(75, 50, 316, 281));
        frameToolsInit->setStyleSheet(QString::fromUtf8("border-image:url(:/calib.png);"));
        frameToolsInit->setFrameShape(QFrame::StyledPanel);
        frameToolsInit->setFrameShadow(QFrame::Raised);
        CArmToolButton = new QPushButton(frame6);
        CArmToolButton->setObjectName(QString::fromUtf8("CArmToolButton"));
        CArmToolButton->setGeometry(QRect(355, 410, 91, 31));
        CArmToolButton->setStyleSheet(QString::fromUtf8("border-image:url(:/fold_off2_butt.png);\n"
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
        fold1Button->setText(QApplication::translate("Form", "Package", 0, QApplication::UnicodeUTF8));
        exitButton->setText(QApplication::translate("Form", "HOME", 0, QApplication::UnicodeUTF8));
        fold2Button->setText(QApplication::translate("Form", "Power", 0, QApplication::UnicodeUTF8));
        fold3Button->setText(QApplication::translate("Form", "IO", 0, QApplication::UnicodeUTF8));
        fold4Button->setText(QApplication::translate("Form", "Calib", 0, QApplication::UnicodeUTF8));
        fold5Button->setText(QApplication::translate("Form", "Setup", 0, QApplication::UnicodeUTF8));
        CArmToolButton->setText(QApplication::translate("Form", "C-ARM", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORM_H
