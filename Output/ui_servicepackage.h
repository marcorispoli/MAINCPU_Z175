/********************************************************************************
** Form generated from reading UI file 'servicepackage.ui'
**
** Created: Fri Sep 13 11:22:47 2019
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVICEPACKAGE_H
#define UI_SERVICEPACKAGE_H

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

class Ui_ServicePackage
{
public:
    QFrame *frame;
    QPushButton *exitButton;
    QFrame *frame_2;
    QFrame *frame_32;
    QLabel *labelStarterHsTimeout;
    QFrame *frame_4;
    QLabel *label_2;
    QLabel *requested_master_gui;
    QLabel *found_master_gui;
    QFrame *frame_5;
    QLabel *label_7;
    QLabel *requested_master_driver;
    QLabel *found_master_driver;
    QFrame *frame_8;
    QLabel *requested_pcb269;
    QLabel *requested_pcb269_2;
    QLabel *found_pcb269;
    QFrame *frame_9;
    QLabel *requested_pcb190;
    QLabel *requested_pcb190_2;
    QLabel *found_pcb190;
    QFrame *frame_10;
    QLabel *requested_pcb240;
    QLabel *requested_pcb240_2;
    QLabel *found_pcb240;
    QFrame *frame_33;
    QLabel *labelStarterHsTimeout_4;
    QFrame *frame_6;
    QLabel *label_4;
    QLabel *requested_slave_gui;
    QLabel *found_slave_gui;
    QFrame *frame_7;
    QLabel *label_14;
    QLabel *requested_slave_driver;
    QLabel *found_slave_driver;
    QFrame *frame_34;
    QLabel *labelStarterHsTimeout_5;
    QFrame *frame_11;
    QLabel *label_20;
    QLabel *requested_pcb249_u1;
    QLabel *found_pcb249_u1;
    QFrame *frame_12;
    QLabel *label_29;
    QLabel *requested_pcb249_u2;
    QLabel *found_pcb249_u2;
    QFrame *frame_13;
    QLabel *label_32;
    QLabel *requested_pcb244;
    QLabel *found_pcb244;
    QFrame *frame_14;
    QLabel *requested_package;
    QLabel *package_installation_date;
    QLabel *serviceSetupDiagnosticLabel;

    void setupUi(QWidget *ServicePackage)
    {
        if (ServicePackage->objectName().isEmpty())
            ServicePackage->setObjectName(QString::fromUtf8("ServicePackage"));
        ServicePackage->resize(800, 480);
        ServicePackage->setStyleSheet(QString::fromUtf8("background-color:rgb(0,0,0,0);\n"
"background-image:url(://Service_Package/Service_Package/package_window.png);\n"
"border: 0px;"));
        frame = new QFrame(ServicePackage);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(0, 0, 800, 480));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        exitButton = new QPushButton(frame);
        exitButton->setObjectName(QString::fromUtf8("exitButton"));
        exitButton->setGeometry(QRect(0, 0, 111, 111));
        frame_2 = new QFrame(frame);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setGeometry(QRect(675, 365, 121, 106));
        frame_2->setStyleSheet(QString::fromUtf8("background-color:rgb(0,0,0,0);\n"
"background-image:url(://Service_Package/Service_Package/package_item.png);\n"
"border: 0px;"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        frame_32 = new QFrame(frame);
        frame_32->setObjectName(QString::fromUtf8("frame_32"));
        frame_32->setGeometry(QRect(100, 70, 376, 101));
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
        labelStarterHsTimeout->setGeometry(QRect(10, 0, 236, 36));
        QFont font;
        font.setFamily(QString::fromUtf8("Bitstream Charter"));
        font.setPointSize(16);
        font.setBold(true);
        font.setWeight(75);
        labelStarterHsTimeout->setFont(font);
        labelStarterHsTimeout->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 153);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        frame_4 = new QFrame(frame_32);
        frame_4->setObjectName(QString::fromUtf8("frame_4"));
        frame_4->setGeometry(QRect(10, 35, 166, 61));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        label_2 = new QLabel(frame_4);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 5, 52, 13));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Bitstream Charter"));
        font1.setPointSize(10);
        font1.setBold(true);
        font1.setItalic(true);
        font1.setWeight(75);
        label_2->setFont(font1);
        label_2->setStyleSheet(QString::fromUtf8("color: rgb(205, 255, 211);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        requested_master_gui = new QLabel(frame_4);
        requested_master_gui->setObjectName(QString::fromUtf8("requested_master_gui"));
        requested_master_gui->setGeometry(QRect(10, 25, 126, 16));
        requested_master_gui->setFont(font1);
        requested_master_gui->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        found_master_gui = new QLabel(frame_4);
        found_master_gui->setObjectName(QString::fromUtf8("found_master_gui"));
        found_master_gui->setGeometry(QRect(10, 40, 126, 16));
        found_master_gui->setFont(font1);
        found_master_gui->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        frame_5 = new QFrame(frame_32);
        frame_5->setObjectName(QString::fromUtf8("frame_5"));
        frame_5->setGeometry(QRect(195, 35, 166, 61));
        frame_5->setFrameShape(QFrame::StyledPanel);
        frame_5->setFrameShadow(QFrame::Raised);
        label_7 = new QLabel(frame_5);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(10, 5, 52, 13));
        label_7->setFont(font1);
        label_7->setStyleSheet(QString::fromUtf8("color: rgb(205, 255, 211);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        requested_master_driver = new QLabel(frame_5);
        requested_master_driver->setObjectName(QString::fromUtf8("requested_master_driver"));
        requested_master_driver->setGeometry(QRect(10, 25, 126, 16));
        requested_master_driver->setFont(font1);
        requested_master_driver->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        found_master_driver = new QLabel(frame_5);
        found_master_driver->setObjectName(QString::fromUtf8("found_master_driver"));
        found_master_driver->setGeometry(QRect(10, 40, 126, 16));
        found_master_driver->setFont(font1);
        found_master_driver->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        labelStarterHsTimeout->raise();
        frame_4->raise();
        frame_5->raise();
        frame_8 = new QFrame(frame);
        frame_8->setObjectName(QString::fromUtf8("frame_8"));
        frame_8->setGeometry(QRect(495, 185, 166, 71));
        frame_8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(74, 74, 74,30);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:10px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        frame_8->setFrameShape(QFrame::StyledPanel);
        frame_8->setFrameShadow(QFrame::Raised);
        requested_pcb269 = new QLabel(frame_8);
        requested_pcb269->setObjectName(QString::fromUtf8("requested_pcb269"));
        requested_pcb269->setGeometry(QRect(10, 5, 101, 16));
        requested_pcb269->setFont(font1);
        requested_pcb269->setStyleSheet(QString::fromUtf8("color: rgb(205, 255, 211);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        requested_pcb269_2 = new QLabel(frame_8);
        requested_pcb269_2->setObjectName(QString::fromUtf8("requested_pcb269_2"));
        requested_pcb269_2->setGeometry(QRect(10, 35, 126, 16));
        requested_pcb269_2->setFont(font1);
        requested_pcb269_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        found_pcb269 = new QLabel(frame_8);
        found_pcb269->setObjectName(QString::fromUtf8("found_pcb269"));
        found_pcb269->setGeometry(QRect(10, 50, 126, 16));
        found_pcb269->setFont(font1);
        found_pcb269->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        frame_9 = new QFrame(frame);
        frame_9->setObjectName(QString::fromUtf8("frame_9"));
        frame_9->setGeometry(QRect(495, 260, 166, 71));
        frame_9->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(74, 74, 74,30);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:10px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        frame_9->setFrameShape(QFrame::StyledPanel);
        frame_9->setFrameShadow(QFrame::Raised);
        requested_pcb190 = new QLabel(frame_9);
        requested_pcb190->setObjectName(QString::fromUtf8("requested_pcb190"));
        requested_pcb190->setGeometry(QRect(10, 5, 136, 16));
        requested_pcb190->setFont(font1);
        requested_pcb190->setStyleSheet(QString::fromUtf8("color: rgb(205, 255, 211);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        requested_pcb190_2 = new QLabel(frame_9);
        requested_pcb190_2->setObjectName(QString::fromUtf8("requested_pcb190_2"));
        requested_pcb190_2->setGeometry(QRect(10, 35, 126, 16));
        requested_pcb190_2->setFont(font1);
        requested_pcb190_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        found_pcb190 = new QLabel(frame_9);
        found_pcb190->setObjectName(QString::fromUtf8("found_pcb190"));
        found_pcb190->setGeometry(QRect(10, 50, 126, 16));
        found_pcb190->setFont(font1);
        found_pcb190->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        frame_10 = new QFrame(frame);
        frame_10->setObjectName(QString::fromUtf8("frame_10"));
        frame_10->setGeometry(QRect(495, 335, 166, 71));
        frame_10->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(74, 74, 74,30);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:10px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        frame_10->setFrameShape(QFrame::StyledPanel);
        frame_10->setFrameShadow(QFrame::Raised);
        requested_pcb240 = new QLabel(frame_10);
        requested_pcb240->setObjectName(QString::fromUtf8("requested_pcb240"));
        requested_pcb240->setGeometry(QRect(10, 5, 136, 16));
        requested_pcb240->setFont(font1);
        requested_pcb240->setStyleSheet(QString::fromUtf8("color: rgb(205, 255, 211);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        requested_pcb240_2 = new QLabel(frame_10);
        requested_pcb240_2->setObjectName(QString::fromUtf8("requested_pcb240_2"));
        requested_pcb240_2->setGeometry(QRect(10, 35, 126, 16));
        requested_pcb240_2->setFont(font1);
        requested_pcb240_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        found_pcb240 = new QLabel(frame_10);
        found_pcb240->setObjectName(QString::fromUtf8("found_pcb240"));
        found_pcb240->setGeometry(QRect(10, 50, 126, 16));
        found_pcb240->setFont(font1);
        found_pcb240->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        frame_33 = new QFrame(frame);
        frame_33->setObjectName(QString::fromUtf8("frame_33"));
        frame_33->setGeometry(QRect(100, 185, 376, 101));
        frame_33->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(74, 74, 74,30);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:10px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        frame_33->setFrameShape(QFrame::StyledPanel);
        frame_33->setFrameShadow(QFrame::Raised);
        labelStarterHsTimeout_4 = new QLabel(frame_33);
        labelStarterHsTimeout_4->setObjectName(QString::fromUtf8("labelStarterHsTimeout_4"));
        labelStarterHsTimeout_4->setGeometry(QRect(10, 0, 236, 36));
        labelStarterHsTimeout_4->setFont(font);
        labelStarterHsTimeout_4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 153);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        frame_6 = new QFrame(frame_33);
        frame_6->setObjectName(QString::fromUtf8("frame_6"));
        frame_6->setGeometry(QRect(10, 35, 166, 61));
        frame_6->setFrameShape(QFrame::StyledPanel);
        frame_6->setFrameShadow(QFrame::Raised);
        label_4 = new QLabel(frame_6);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 5, 52, 13));
        label_4->setFont(font1);
        label_4->setStyleSheet(QString::fromUtf8("color: rgb(205, 255, 211);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        requested_slave_gui = new QLabel(frame_6);
        requested_slave_gui->setObjectName(QString::fromUtf8("requested_slave_gui"));
        requested_slave_gui->setGeometry(QRect(10, 25, 126, 16));
        requested_slave_gui->setFont(font1);
        requested_slave_gui->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        found_slave_gui = new QLabel(frame_6);
        found_slave_gui->setObjectName(QString::fromUtf8("found_slave_gui"));
        found_slave_gui->setGeometry(QRect(10, 40, 126, 16));
        found_slave_gui->setFont(font1);
        found_slave_gui->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        frame_7 = new QFrame(frame_33);
        frame_7->setObjectName(QString::fromUtf8("frame_7"));
        frame_7->setGeometry(QRect(195, 35, 166, 61));
        frame_7->setFrameShape(QFrame::StyledPanel);
        frame_7->setFrameShadow(QFrame::Raised);
        label_14 = new QLabel(frame_7);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(10, 5, 52, 13));
        label_14->setFont(font1);
        label_14->setStyleSheet(QString::fromUtf8("color: rgb(205, 255, 211);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        requested_slave_driver = new QLabel(frame_7);
        requested_slave_driver->setObjectName(QString::fromUtf8("requested_slave_driver"));
        requested_slave_driver->setGeometry(QRect(10, 25, 126, 16));
        requested_slave_driver->setFont(font1);
        requested_slave_driver->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        found_slave_driver = new QLabel(frame_7);
        found_slave_driver->setObjectName(QString::fromUtf8("found_slave_driver"));
        found_slave_driver->setGeometry(QRect(10, 40, 126, 16));
        found_slave_driver->setFont(font1);
        found_slave_driver->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        frame_34 = new QFrame(frame);
        frame_34->setObjectName(QString::fromUtf8("frame_34"));
        frame_34->setGeometry(QRect(100, 305, 376, 101));
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
        labelStarterHsTimeout_5 = new QLabel(frame_34);
        labelStarterHsTimeout_5->setObjectName(QString::fromUtf8("labelStarterHsTimeout_5"));
        labelStarterHsTimeout_5->setGeometry(QRect(10, 0, 236, 36));
        labelStarterHsTimeout_5->setFont(font);
        labelStarterHsTimeout_5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 153);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        frame_11 = new QFrame(frame_34);
        frame_11->setObjectName(QString::fromUtf8("frame_11"));
        frame_11->setGeometry(QRect(10, 35, 166, 61));
        frame_11->setFrameShape(QFrame::StyledPanel);
        frame_11->setFrameShadow(QFrame::Raised);
        label_20 = new QLabel(frame_11);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setGeometry(QRect(10, 5, 121, 16));
        label_20->setFont(font1);
        label_20->setStyleSheet(QString::fromUtf8("color: rgb(205, 255, 211);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        requested_pcb249_u1 = new QLabel(frame_11);
        requested_pcb249_u1->setObjectName(QString::fromUtf8("requested_pcb249_u1"));
        requested_pcb249_u1->setGeometry(QRect(10, 25, 126, 16));
        requested_pcb249_u1->setFont(font1);
        requested_pcb249_u1->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        found_pcb249_u1 = new QLabel(frame_11);
        found_pcb249_u1->setObjectName(QString::fromUtf8("found_pcb249_u1"));
        found_pcb249_u1->setGeometry(QRect(10, 40, 126, 16));
        found_pcb249_u1->setFont(font1);
        found_pcb249_u1->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        frame_12 = new QFrame(frame_34);
        frame_12->setObjectName(QString::fromUtf8("frame_12"));
        frame_12->setGeometry(QRect(195, 35, 166, 61));
        frame_12->setFrameShape(QFrame::StyledPanel);
        frame_12->setFrameShadow(QFrame::Raised);
        label_29 = new QLabel(frame_12);
        label_29->setObjectName(QString::fromUtf8("label_29"));
        label_29->setGeometry(QRect(10, 5, 136, 16));
        label_29->setFont(font1);
        label_29->setStyleSheet(QString::fromUtf8("color: rgb(205, 255, 211);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        requested_pcb249_u2 = new QLabel(frame_12);
        requested_pcb249_u2->setObjectName(QString::fromUtf8("requested_pcb249_u2"));
        requested_pcb249_u2->setGeometry(QRect(10, 25, 126, 16));
        requested_pcb249_u2->setFont(font1);
        requested_pcb249_u2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        found_pcb249_u2 = new QLabel(frame_12);
        found_pcb249_u2->setObjectName(QString::fromUtf8("found_pcb249_u2"));
        found_pcb249_u2->setGeometry(QRect(10, 40, 126, 16));
        found_pcb249_u2->setFont(font1);
        found_pcb249_u2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        frame_13 = new QFrame(frame);
        frame_13->setObjectName(QString::fromUtf8("frame_13"));
        frame_13->setGeometry(QRect(495, 110, 166, 71));
        frame_13->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(74, 74, 74,30);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:10px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        frame_13->setFrameShape(QFrame::StyledPanel);
        frame_13->setFrameShadow(QFrame::Raised);
        label_32 = new QLabel(frame_13);
        label_32->setObjectName(QString::fromUtf8("label_32"));
        label_32->setGeometry(QRect(10, 5, 101, 16));
        label_32->setFont(font1);
        label_32->setStyleSheet(QString::fromUtf8("color: rgb(205, 255, 211);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        requested_pcb244 = new QLabel(frame_13);
        requested_pcb244->setObjectName(QString::fromUtf8("requested_pcb244"));
        requested_pcb244->setGeometry(QRect(10, 35, 126, 16));
        requested_pcb244->setFont(font1);
        requested_pcb244->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        found_pcb244 = new QLabel(frame_13);
        found_pcb244->setObjectName(QString::fromUtf8("found_pcb244"));
        found_pcb244->setGeometry(QRect(10, 50, 126, 16));
        found_pcb244->setFont(font1);
        found_pcb244->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        frame_14 = new QFrame(frame);
        frame_14->setObjectName(QString::fromUtf8("frame_14"));
        frame_14->setGeometry(QRect(495, 70, 166, 36));
        frame_14->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(74, 74, 74,30);\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:10px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);"));
        frame_14->setFrameShape(QFrame::StyledPanel);
        frame_14->setFrameShadow(QFrame::Raised);
        requested_package = new QLabel(frame_14);
        requested_package->setObjectName(QString::fromUtf8("requested_package"));
        requested_package->setGeometry(QRect(10, 3, 141, 16));
        requested_package->setFont(font1);
        requested_package->setStyleSheet(QString::fromUtf8("color: rgb(205, 255, 211);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        package_installation_date = new QLabel(frame_14);
        package_installation_date->setObjectName(QString::fromUtf8("package_installation_date"));
        package_installation_date->setGeometry(QRect(10, 18, 146, 16));
        package_installation_date->setFont(font1);
        package_installation_date->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgba(74, 74, 74,0);\n"
"border: 0px solid  rgb(74, 74, 74);\n"
"border-radius:0px;\n"
""));
        serviceSetupDiagnosticLabel = new QLabel(frame);
        serviceSetupDiagnosticLabel->setObjectName(QString::fromUtf8("serviceSetupDiagnosticLabel"));
        serviceSetupDiagnosticLabel->setGeometry(QRect(115, 10, 616, 41));
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

        retranslateUi(ServicePackage);

        QMetaObject::connectSlotsByName(ServicePackage);
    } // setupUi

    void retranslateUi(QWidget *ServicePackage)
    {
        ServicePackage->setWindowTitle(QApplication::translate("ServicePackage", "Form", 0, QApplication::UnicodeUTF8));
        exitButton->setText(QString());
        labelStarterHsTimeout->setText(QApplication::translate("ServicePackage", "MASTER TERMINAL", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ServicePackage", "GUI", 0, QApplication::UnicodeUTF8));
        requested_master_gui->setText(QApplication::translate("ServicePackage", "REQUESTED:", 0, QApplication::UnicodeUTF8));
        found_master_gui->setText(QApplication::translate("ServicePackage", "FOUND:", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("ServicePackage", "DRIVER", 0, QApplication::UnicodeUTF8));
        requested_master_driver->setText(QApplication::translate("ServicePackage", "REQUESTED:", 0, QApplication::UnicodeUTF8));
        found_master_driver->setText(QApplication::translate("ServicePackage", "FOUND:", 0, QApplication::UnicodeUTF8));
        requested_pcb269->setText(QApplication::translate("ServicePackage", "PCB/269", 0, QApplication::UnicodeUTF8));
        requested_pcb269_2->setText(QApplication::translate("ServicePackage", "REQUESTED:", 0, QApplication::UnicodeUTF8));
        found_pcb269->setText(QApplication::translate("ServicePackage", "FOUND:", 0, QApplication::UnicodeUTF8));
        requested_pcb190->setText(QApplication::translate("ServicePackage", "PCB/190", 0, QApplication::UnicodeUTF8));
        requested_pcb190_2->setText(QApplication::translate("ServicePackage", "REQUESTED:", 0, QApplication::UnicodeUTF8));
        found_pcb190->setText(QApplication::translate("ServicePackage", "FOUND:", 0, QApplication::UnicodeUTF8));
        requested_pcb240->setText(QApplication::translate("ServicePackage", "PCB/240", 0, QApplication::UnicodeUTF8));
        requested_pcb240_2->setText(QApplication::translate("ServicePackage", "REQUESTED:", 0, QApplication::UnicodeUTF8));
        found_pcb240->setText(QApplication::translate("ServicePackage", "FOUND:", 0, QApplication::UnicodeUTF8));
        labelStarterHsTimeout_4->setText(QApplication::translate("ServicePackage", "SLAVE TERMINAL", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("ServicePackage", "GUI", 0, QApplication::UnicodeUTF8));
        requested_slave_gui->setText(QApplication::translate("ServicePackage", "REQUESTED:", 0, QApplication::UnicodeUTF8));
        found_slave_gui->setText(QApplication::translate("ServicePackage", "FOUND:", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("ServicePackage", "DRIVER", 0, QApplication::UnicodeUTF8));
        requested_slave_driver->setText(QApplication::translate("ServicePackage", "REQUESTED:", 0, QApplication::UnicodeUTF8));
        found_slave_driver->setText(QApplication::translate("ServicePackage", "FOUND:", 0, QApplication::UnicodeUTF8));
        labelStarterHsTimeout_5->setText(QApplication::translate("ServicePackage", "COLLIMATOR", 0, QApplication::UnicodeUTF8));
        label_20->setText(QApplication::translate("ServicePackage", "PCB/249-U1", 0, QApplication::UnicodeUTF8));
        requested_pcb249_u1->setText(QApplication::translate("ServicePackage", "REQUESTED:", 0, QApplication::UnicodeUTF8));
        found_pcb249_u1->setText(QApplication::translate("ServicePackage", "FOUND:", 0, QApplication::UnicodeUTF8));
        label_29->setText(QApplication::translate("ServicePackage", "PCB/249-U2", 0, QApplication::UnicodeUTF8));
        requested_pcb249_u2->setText(QApplication::translate("ServicePackage", "REQUESTED:", 0, QApplication::UnicodeUTF8));
        found_pcb249_u2->setText(QApplication::translate("ServicePackage", "FOUND:", 0, QApplication::UnicodeUTF8));
        label_32->setText(QApplication::translate("ServicePackage", "PCB/244", 0, QApplication::UnicodeUTF8));
        requested_pcb244->setText(QApplication::translate("ServicePackage", "REQUESTED:", 0, QApplication::UnicodeUTF8));
        found_pcb244->setText(QApplication::translate("ServicePackage", "FOUND:", 0, QApplication::UnicodeUTF8));
        requested_package->setText(QApplication::translate("ServicePackage", "PACKAGE:", 0, QApplication::UnicodeUTF8));
        package_installation_date->setText(QApplication::translate("ServicePackage", "DATE:", 0, QApplication::UnicodeUTF8));
        serviceSetupDiagnosticLabel->setText(QApplication::translate("ServicePackage", "Service Panel/Package panel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ServicePackage: public Ui_ServicePackage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVICEPACKAGE_H
