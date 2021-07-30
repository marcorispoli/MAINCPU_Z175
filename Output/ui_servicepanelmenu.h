/********************************************************************************
** Form generated from reading UI file 'servicepanelmenu.ui'
**
** Created: Fri Sep 13 11:22:47 2019
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVICEPANELMENU_H
#define UI_SERVICEPANELMENU_H

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

class Ui_ServicePanelMenu
{
public:
    QFrame *ServicePanelMenuFrame;
    QLabel *servicePanelMenuLabel;
    QPushButton *PowerPanelButton;
    QPushButton *CalibMenuButton;
    QPushButton *SetupMenuButton;
    QPushButton *ToolsMenuButton;
    QPushButton *IoPanelButton;
    QPushButton *PackagePanelButton;
    QPushButton *exitButton;

    void setupUi(QWidget *ServicePanelMenu)
    {
        if (ServicePanelMenu->objectName().isEmpty())
            ServicePanelMenu->setObjectName(QString::fromUtf8("ServicePanelMenu"));
        ServicePanelMenu->resize(800, 480);
        ServicePanelMenu->setStyleSheet(QString::fromUtf8("background-color:rgb(0,0,0,0);\n"
"background-image:url(://ServicePanel/ServicePanel/service_panel_menu.png);\n"
"border: 0px;"));
        ServicePanelMenuFrame = new QFrame(ServicePanelMenu);
        ServicePanelMenuFrame->setObjectName(QString::fromUtf8("ServicePanelMenuFrame"));
        ServicePanelMenuFrame->setGeometry(QRect(0, -1, 800, 480));
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
        PowerPanelButton = new QPushButton(ServicePanelMenuFrame);
        PowerPanelButton->setObjectName(QString::fromUtf8("PowerPanelButton"));
        PowerPanelButton->setGeometry(QRect(120, 280, 120, 120));
        PowerPanelButton->setStyleSheet(QString::fromUtf8("background-image:url(://ServicePanel/ServicePanel/power_menu.png);\n"
""));
        CalibMenuButton = new QPushButton(ServicePanelMenuFrame);
        CalibMenuButton->setObjectName(QString::fromUtf8("CalibMenuButton"));
        CalibMenuButton->setGeometry(QRect(520, 280, 120, 120));
        CalibMenuButton->setStyleSheet(QString::fromUtf8("background-image:url(://ServicePanel/ServicePanel/calib_menu.png);\n"
""));
        SetupMenuButton = new QPushButton(ServicePanelMenuFrame);
        SetupMenuButton->setObjectName(QString::fromUtf8("SetupMenuButton"));
        SetupMenuButton->setGeometry(QRect(320, 110, 120, 120));
        SetupMenuButton->setStyleSheet(QString::fromUtf8("background-image:url(://ServicePanel/ServicePanel/setup_menu.png);\n"
""));
        ToolsMenuButton = new QPushButton(ServicePanelMenuFrame);
        ToolsMenuButton->setObjectName(QString::fromUtf8("ToolsMenuButton"));
        ToolsMenuButton->setGeometry(QRect(120, 110, 120, 120));
        ToolsMenuButton->setStyleSheet(QString::fromUtf8("background-image:url(://ServicePanel/ServicePanel/tools_menu.png);\n"
""));
        IoPanelButton = new QPushButton(ServicePanelMenuFrame);
        IoPanelButton->setObjectName(QString::fromUtf8("IoPanelButton"));
        IoPanelButton->setGeometry(QRect(320, 280, 120, 120));
        IoPanelButton->setStyleSheet(QString::fromUtf8("background-image:url(://ServicePanel/ServicePanel/in_out_menu.png);\n"
""));
        PackagePanelButton = new QPushButton(ServicePanelMenuFrame);
        PackagePanelButton->setObjectName(QString::fromUtf8("PackagePanelButton"));
        PackagePanelButton->setGeometry(QRect(520, 110, 120, 120));
        PackagePanelButton->setStyleSheet(QString::fromUtf8("background-image:url(://ServicePanel/ServicePanel/package_menu.png);\n"
""));
        exitButton = new QPushButton(ServicePanelMenuFrame);
        exitButton->setObjectName(QString::fromUtf8("exitButton"));
        exitButton->setGeometry(QRect(0, 0, 111, 111));

        retranslateUi(ServicePanelMenu);

        QMetaObject::connectSlotsByName(ServicePanelMenu);
    } // setupUi

    void retranslateUi(QWidget *ServicePanelMenu)
    {
        ServicePanelMenu->setWindowTitle(QApplication::translate("ServicePanelMenu", "Form", 0, QApplication::UnicodeUTF8));
        servicePanelMenuLabel->setText(QApplication::translate("ServicePanelMenu", "SERVICE PANEL", 0, QApplication::UnicodeUTF8));
        PowerPanelButton->setText(QString());
        CalibMenuButton->setText(QString());
        SetupMenuButton->setText(QString());
        ToolsMenuButton->setText(QString());
        IoPanelButton->setText(QString());
        PackagePanelButton->setText(QString());
        exitButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ServicePanelMenu: public Ui_ServicePanelMenu {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVICEPANELMENU_H
