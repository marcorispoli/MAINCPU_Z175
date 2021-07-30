/********************************************************************************
** Form generated from reading UI file 'toolSlave.ui'
**
** Created: Fri Sep 13 11:22:47 2019
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TOOLSLAVE_H
#define UI_TOOLSLAVE_H

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

class Ui_toolSlaveUi
{
public:
    QFrame *frame;
    QPushButton *exitButton;
    QFrame *calibKvFrame;
    QLabel *calibKvTab;
    QLabel *title;
    QFrame *entryFrame;
    QLabel *entryMessage;

    void setupUi(QWidget *toolSlaveUi)
    {
        if (toolSlaveUi->objectName().isEmpty())
            toolSlaveUi->setObjectName(QString::fromUtf8("toolSlaveUi"));
        toolSlaveUi->resize(800, 480);
        toolSlaveUi->setStyleSheet(QString::fromUtf8("background-color:rgb(0,0,0,0);\n"
"background-image:url(:/fold_win_bkg.png);\n"
"border: 0px;\n"
""));
        frame = new QFrame(toolSlaveUi);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(0, 0, 800, 480));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        exitButton = new QPushButton(frame);
        exitButton->setObjectName(QString::fromUtf8("exitButton"));
        exitButton->setGeometry(QRect(5, 25, 81, 71));
        exitButton->setStyleSheet(QString::fromUtf8("border: 0px;\n"
"background-image:url(:/transparent.png);\n"
"color:rgba(0,0,0,0);\n"
"outline: 0px;\n"
""));
        exitButton->setText(QString::fromUtf8("HOME"));
        calibKvFrame = new QFrame(frame);
        calibKvFrame->setObjectName(QString::fromUtf8("calibKvFrame"));
        calibKvFrame->setGeometry(QRect(100, 190, 86, 86));
        calibKvFrame->setStyleSheet(QString::fromUtf8("background-image:url(/transparent.png);\n"
"color: rgb(255, 255, 255);\n"
"background-color: rgba(255, 255, 153,60);\n"
"border: 4px solid  rgb(74, 74, 74);\n"
"border-radius:20px;\n"
"border-right-color: rgba(255, 255, 255,0);\n"
"border-left-color: rgba(255, 255, 255,0);\n"
"border-top-color: rgba(255, 255, 255,0);"));
        calibKvFrame->setFrameShape(QFrame::StyledPanel);
        calibKvFrame->setFrameShadow(QFrame::Raised);
        calibKvTab = new QLabel(calibKvFrame);
        calibKvTab->setObjectName(QString::fromUtf8("calibKvTab"));
        calibKvTab->setGeometry(QRect(20, 15, 451, 296));
        calibKvTab->setStyleSheet(QString::fromUtf8("border:0px;\n"
"background:uri(:transparent.png);\n"
"border-radius:20px;"));
        title = new QLabel(frame);
        title->setObjectName(QString::fromUtf8("title"));
        title->setGeometry(QRect(141, 15, 521, 36));
        QFont font;
        font.setFamily(QString::fromUtf8("Bitstream Charter"));
        font.setPointSize(16);
        font.setBold(true);
        font.setWeight(75);
        title->setFont(font);
        title->setStyleSheet(QString::fromUtf8("border:0px;\n"
"background:uri(:transparent.png);\n"
"border-radius:20px;\n"
"color: rgb(255, 255, 127);"));
        title->setAlignment(Qt::AlignCenter);
        entryFrame = new QFrame(frame);
        entryFrame->setObjectName(QString::fromUtf8("entryFrame"));
        entryFrame->setGeometry(QRect(290, 85, 496, 336));
        entryFrame->setStyleSheet(QString::fromUtf8("background-image:url(/transparent.png);\n"
"border-image:url(:/setup.png);"));
        entryFrame->setFrameShape(QFrame::StyledPanel);
        entryFrame->setFrameShadow(QFrame::Raised);
        entryMessage = new QLabel(entryFrame);
        entryMessage->setObjectName(QString::fromUtf8("entryMessage"));
        entryMessage->setGeometry(QRect(25, 215, 451, 111));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Bitstream Charter"));
        font1.setPointSize(14);
        font1.setBold(true);
        font1.setItalic(false);
        font1.setWeight(75);
        entryMessage->setFont(font1);
        entryMessage->setStyleSheet(QString::fromUtf8("background-image:url(/transparent.png);\n"
"border-image:url(:/transparent.png);\n"
"color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,130);\n"
"border: 2px solid  rgb(74, 74, 74);\n"
"border-radius:20px;\n"
"border-right-color: rgba(255, 255, 255,0);\n"
"border-left-color: rgba(255, 255, 255,0);\n"
"border-top-color: rgba(255, 255, 255,0);\n"
""));
        entryMessage->setText(QString::fromUtf8("MESSAGGIO INGRESSO TOOLS"));
        entryMessage->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        entryMessage->raise();
        entryMessage->raise();

        retranslateUi(toolSlaveUi);

        QMetaObject::connectSlotsByName(toolSlaveUi);
    } // setupUi

    void retranslateUi(QWidget *toolSlaveUi)
    {
        toolSlaveUi->setWindowTitle(QApplication::translate("toolSlaveUi", "Form", 0, QApplication::UnicodeUTF8));
        calibKvTab->setText(QString());
        title->setText(QApplication::translate("toolSlaveUi", "TITLE", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class toolSlaveUi: public Ui_toolSlaveUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOOLSLAVE_H
