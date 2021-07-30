/********************************************************************************
** Form generated from reading UI file 'setupmenu.ui'
**
** Created: Fri Sep 13 11:22:47 2019
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETUPMENU_H
#define UI_SETUPMENU_H

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

class Ui_SetupMenu
{
public:
    QFrame *ServicePanelMenuFrame;
    QLabel *servicePanelMenuLabel;
    QPushButton *diagnosticMenuButton;
    QPushButton *generalMenuButton;
    QPushButton *starterMenuButton;
    QPushButton *specialMenuButton;
    QPushButton *exitButton;

    void setupUi(QWidget *SetupMenu)
    {
        if (SetupMenu->objectName().isEmpty())
            SetupMenu->setObjectName(QString::fromUtf8("SetupMenu"));
        SetupMenu->resize(800, 480);
        SetupMenu->setStyleSheet(QString::fromUtf8("background-color:rgb(0,0,0,0);\n"
"background-image:url(://Service_Setup/Service_Setup/setup_menu.png);\n"
"border: 0px;"));
        ServicePanelMenuFrame = new QFrame(SetupMenu);
        ServicePanelMenuFrame->setObjectName(QString::fromUtf8("ServicePanelMenuFrame"));
        ServicePanelMenuFrame->setGeometry(QRect(0, -1, 806, 481));
        ServicePanelMenuFrame->setFrameShape(QFrame::StyledPanel);
        ServicePanelMenuFrame->setFrameShadow(QFrame::Raised);
        servicePanelMenuLabel = new QLabel(ServicePanelMenuFrame);
        servicePanelMenuLabel->setObjectName(QString::fromUtf8("servicePanelMenuLabel"));
        servicePanelMenuLabel->setGeometry(QRect(121, 10, 566, 41));
        QFont font;
        font.setFamily(QString::fromUtf8("Bitstream Charter"));
        font.setPointSize(24);
        font.setBold(true);
        font.setItalic(true);
        font.setWeight(75);
        servicePanelMenuLabel->setFont(font);
        servicePanelMenuLabel->setStyleSheet(QString::fromUtf8("background-image:url(/transparent.png);\n"
"color: rgb(248, 248, 54);"));
        servicePanelMenuLabel->setAlignment(Qt::AlignCenter);
        diagnosticMenuButton = new QPushButton(ServicePanelMenuFrame);
        diagnosticMenuButton->setObjectName(QString::fromUtf8("diagnosticMenuButton"));
        diagnosticMenuButton->setGeometry(QRect(150, 255, 150, 150));
        diagnosticMenuButton->setStyleSheet(QString::fromUtf8("background-image:url(://Service_Setup/Service_Setup/diagnostic_menu.png);\n"
""));
        generalMenuButton = new QPushButton(ServicePanelMenuFrame);
        generalMenuButton->setObjectName(QString::fromUtf8("generalMenuButton"));
        generalMenuButton->setGeometry(QRect(500, 80, 150, 150));
        generalMenuButton->setStyleSheet(QString::fromUtf8("background-image:url(://Service_Setup/Service_Setup/general_menu.png);\n"
""));
        starterMenuButton = new QPushButton(ServicePanelMenuFrame);
        starterMenuButton->setObjectName(QString::fromUtf8("starterMenuButton"));
        starterMenuButton->setGeometry(QRect(150, 80, 150, 150));
        starterMenuButton->setStyleSheet(QString::fromUtf8("background-image:url(://Service_Setup/Service_Setup/starter_menu.png);\n"
""));
        specialMenuButton = new QPushButton(ServicePanelMenuFrame);
        specialMenuButton->setObjectName(QString::fromUtf8("specialMenuButton"));
        specialMenuButton->setGeometry(QRect(500, 255, 150, 150));
        specialMenuButton->setStyleSheet(QString::fromUtf8("background-image:url(://Service_Setup/Service_Setup/special_menu.png);\n"
""));
        exitButton = new QPushButton(ServicePanelMenuFrame);
        exitButton->setObjectName(QString::fromUtf8("exitButton"));
        exitButton->setGeometry(QRect(0, 0, 111, 111));

        retranslateUi(SetupMenu);

        QMetaObject::connectSlotsByName(SetupMenu);
    } // setupUi

    void retranslateUi(QWidget *SetupMenu)
    {
        SetupMenu->setWindowTitle(QApplication::translate("SetupMenu", "Form", 0, QApplication::UnicodeUTF8));
        servicePanelMenuLabel->setText(QApplication::translate("SetupMenu", "Service Panel/Setup ", 0, QApplication::UnicodeUTF8));
        diagnosticMenuButton->setText(QString());
        generalMenuButton->setText(QString());
        starterMenuButton->setText(QString());
        specialMenuButton->setText(QString());
        exitButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class SetupMenu: public Ui_SetupMenu {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETUPMENU_H
