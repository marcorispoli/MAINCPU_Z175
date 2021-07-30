/********************************************************************************
** Form generated from reading UI file 'toolsmenu.ui'
**
** Created: Fri Sep 13 11:22:47 2019
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TOOLSMENU_H
#define UI_TOOLSMENU_H

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

class Ui_ToolsMenu
{
public:
    QFrame *frame;
    QFrame *frame_2;
    QPushButton *exitButton;
    QPushButton *MotorToolButton;
    QLabel *label;
    QPushButton *StarterToolButton;

    void setupUi(QWidget *ToolsMenu)
    {
        if (ToolsMenu->objectName().isEmpty())
            ToolsMenu->setObjectName(QString::fromUtf8("ToolsMenu"));
        ToolsMenu->resize(800, 480);
        ToolsMenu->setStyleSheet(QString::fromUtf8("background-color:rgb(0,0,0,0);\n"
"background-image:url(://Service_Tools/Service_Tools/tools_menu.png);\n"
"border: 0px;"));
        frame = new QFrame(ToolsMenu);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(0, 0, 800, 480));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        frame_2 = new QFrame(frame);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setGeometry(QRect(655, 350, 121, 121));
        frame_2->setStyleSheet(QString::fromUtf8("background-color:rgb(0,0,0,0);\n"
"background-image:url(://Service_Tools/Service_Tools/tools_icon.png);\n"
"border: 0px;"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        exitButton = new QPushButton(frame);
        exitButton->setObjectName(QString::fromUtf8("exitButton"));
        exitButton->setGeometry(QRect(0, 0, 111, 111));
        MotorToolButton = new QPushButton(frame);
        MotorToolButton->setObjectName(QString::fromUtf8("MotorToolButton"));
        MotorToolButton->setGeometry(QRect(210, 100, 120, 120));
        MotorToolButton->setStyleSheet(QString::fromUtf8("background-image:url(://Service_Tools/Service_Tools/motor_tool.png);\n"
""));
        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(115, 10, 616, 41));
        QFont font;
        font.setFamily(QString::fromUtf8("Bitstream Charter"));
        font.setPointSize(24);
        font.setBold(true);
        font.setItalic(true);
        font.setWeight(75);
        label->setFont(font);
        label->setStyleSheet(QString::fromUtf8("background-image:url(/transparent.png);\n"
"color: rgb(248, 248, 54);"));
        label->setAlignment(Qt::AlignCenter);
        StarterToolButton = new QPushButton(frame);
        StarterToolButton->setObjectName(QString::fromUtf8("StarterToolButton"));
        StarterToolButton->setGeometry(QRect(475, 100, 120, 120));
        StarterToolButton->setStyleSheet(QString::fromUtf8("background-image:url(://Service_Tools/Service_Tools/starter_tool.png);\n"
""));

        retranslateUi(ToolsMenu);

        QMetaObject::connectSlotsByName(ToolsMenu);
    } // setupUi

    void retranslateUi(QWidget *ToolsMenu)
    {
        ToolsMenu->setWindowTitle(QApplication::translate("ToolsMenu", "Form", 0, QApplication::UnicodeUTF8));
        exitButton->setText(QString());
        MotorToolButton->setText(QString());
        label->setText(QApplication::translate("ToolsMenu", "Service Panel/Tools", 0, QApplication::UnicodeUTF8));
        StarterToolButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ToolsMenu: public Ui_ToolsMenu {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOOLSMENU_H
