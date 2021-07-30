/********************************************************************************
** Form generated from reading UI file 'insertcalc.ui'
**
** Created: Fri Sep 13 11:22:47 2019
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INSERTCALC_H
#define UI_INSERTCALC_H

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

class Ui_InsertCalc
{
public:
    QFrame *calcForm;
    QLCDNumber *risultatoText;
    QPushButton *push1;
    QPushButton *push2;
    QPushButton *push3;
    QPushButton *push6;
    QPushButton *push4;
    QPushButton *push5;
    QPushButton *push9;
    QPushButton *push7;
    QPushButton *push8;
    QPushButton *pushBack;
    QPushButton *pushOk;
    QPushButton *pushCanc;
    QLabel *calcTitle;
    QPushButton *push0;
    QPushButton *pushDot;
    QPushButton *pushSgn;

    void setupUi(QWidget *InsertCalc)
    {
        if (InsertCalc->objectName().isEmpty())
            InsertCalc->setObjectName(QString::fromUtf8("InsertCalc"));
        InsertCalc->resize(480, 640);
        InsertCalc->setStyleSheet(QString::fromUtf8("background-color: rgb(161, 161, 161,0);\n"
"background-image:url(:/transparent.png);"));
        calcForm = new QFrame(InsertCalc);
        calcForm->setObjectName(QString::fromUtf8("calcForm"));
        calcForm->setGeometry(QRect(85, 30, 306, 446));
        calcForm->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: qlineargradient(spread:pad, x1:0.75, y1:0.824, x2:0.037, y2:0.05, stop:0 rgba(30,30,30, 200), stop:1 rgba(200, 200, 200, 120));\n"
"border: 1px solid  rgb(74, 74, 74);\n"
"border-radius:10px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);\n"
""));
        calcForm->setFrameShape(QFrame::StyledPanel);
        calcForm->setFrameShadow(QFrame::Raised);
        risultatoText = new QLCDNumber(calcForm);
        risultatoText->setObjectName(QString::fromUtf8("risultatoText"));
        risultatoText->setGeometry(QRect(15, 50, 171, 56));
        risultatoText->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: qrgba(0, 0, 0, 100);\n"
"border: 2px solid  rgb(74, 74, 74);\n"
"border-radius:1px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);\n"
""));
        push1 = new QPushButton(calcForm);
        push1->setObjectName(QString::fromUtf8("push1"));
        push1->setGeometry(QRect(15, 120, 85, 51));
        QFont font;
        font.setFamily(QString::fromUtf8("Bitstream Charter"));
        font.setPointSize(22);
        font.setBold(true);
        font.setItalic(false);
        font.setWeight(75);
        push1->setFont(font);
        push1->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(139, 139, 139, 50);\n"
"border: 2px solid  rgb(0, 0, 0);\n"
"border-radius:4px;\n"
"border-right-color: rgba(0, 0, 0,40);\n"
"border-left-color: rgba(255, 255, 255,100);\n"
"border-top-color: rgba(255, 255, 153,0);\n"
""));
        push1->setText(QString::fromUtf8("1"));
        push2 = new QPushButton(calcForm);
        push2->setObjectName(QString::fromUtf8("push2"));
        push2->setGeometry(QRect(110, 120, 85, 51));
        push2->setFont(font);
        push2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(139, 139, 139, 50);\n"
"border: 2px solid  rgb(0, 0, 0);\n"
"border-radius:4px;\n"
"border-right-color: rgba(0, 0, 0,40);\n"
"border-left-color: rgba(255, 255, 255,100);\n"
"border-top-color: rgba(255, 255, 153,0);\n"
""));
        push2->setText(QString::fromUtf8("2"));
        push3 = new QPushButton(calcForm);
        push3->setObjectName(QString::fromUtf8("push3"));
        push3->setGeometry(QRect(205, 120, 85, 51));
        push3->setFont(font);
        push3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(139, 139, 139, 50);\n"
"border: 2px solid  rgb(0, 0, 0);\n"
"border-radius:4px;\n"
"border-right-color: rgba(0, 0, 0,40);\n"
"border-left-color: rgba(255, 255, 255,100);\n"
"border-top-color: rgba(255, 255, 153,0);\n"
""));
        push3->setText(QString::fromUtf8("3"));
        push6 = new QPushButton(calcForm);
        push6->setObjectName(QString::fromUtf8("push6"));
        push6->setGeometry(QRect(205, 180, 85, 51));
        push6->setFont(font);
        push6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(139, 139, 139, 50);\n"
"border: 2px solid  rgb(0, 0, 0);\n"
"border-radius:4px;\n"
"border-right-color: rgba(0, 0, 0,40);\n"
"border-left-color: rgba(255, 255, 255,100);\n"
"border-top-color: rgba(255, 255, 153,0);\n"
""));
        push6->setText(QString::fromUtf8("6"));
        push4 = new QPushButton(calcForm);
        push4->setObjectName(QString::fromUtf8("push4"));
        push4->setGeometry(QRect(15, 180, 85, 51));
        push4->setFont(font);
        push4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(139, 139, 139, 50);\n"
"border: 2px solid  rgb(0, 0, 0);\n"
"border-radius:4px;\n"
"border-right-color: rgba(0, 0, 0,40);\n"
"border-left-color: rgba(255, 255, 255,100);\n"
"border-top-color: rgba(255, 255, 153,0);\n"
""));
        push4->setText(QString::fromUtf8("4"));
        push5 = new QPushButton(calcForm);
        push5->setObjectName(QString::fromUtf8("push5"));
        push5->setGeometry(QRect(110, 180, 85, 51));
        push5->setFont(font);
        push5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(139, 139, 139, 50);\n"
"border: 2px solid  rgb(0, 0, 0);\n"
"border-radius:4px;\n"
"border-right-color: rgba(0, 0, 0,40);\n"
"border-left-color: rgba(255, 255, 255,100);\n"
"border-top-color: rgba(255, 255, 153,0);\n"
""));
        push5->setText(QString::fromUtf8("5"));
        push9 = new QPushButton(calcForm);
        push9->setObjectName(QString::fromUtf8("push9"));
        push9->setGeometry(QRect(205, 245, 85, 51));
        push9->setFont(font);
        push9->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(139, 139, 139, 50);\n"
"border: 2px solid  rgb(0, 0, 0);\n"
"border-radius:4px;\n"
"border-right-color: rgba(0, 0, 0,40);\n"
"border-left-color: rgba(255, 255, 255,100);\n"
"border-top-color: rgba(255, 255, 153,0);\n"
""));
        push9->setText(QString::fromUtf8("9"));
        push7 = new QPushButton(calcForm);
        push7->setObjectName(QString::fromUtf8("push7"));
        push7->setGeometry(QRect(15, 245, 85, 51));
        push7->setFont(font);
        push7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(139, 139, 139, 50);\n"
"border: 2px solid  rgb(0, 0, 0);\n"
"border-radius:4px;\n"
"border-right-color: rgba(0, 0, 0,40);\n"
"border-left-color: rgba(255, 255, 255,100);\n"
"border-top-color: rgba(255, 255, 153,0);\n"
""));
        push7->setText(QString::fromUtf8("7"));
        push8 = new QPushButton(calcForm);
        push8->setObjectName(QString::fromUtf8("push8"));
        push8->setGeometry(QRect(110, 245, 85, 51));
        push8->setFont(font);
        push8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(139, 139, 139, 50);\n"
"border: 2px solid  rgb(0, 0, 0);\n"
"border-radius:4px;\n"
"border-right-color: rgba(0, 0, 0,40);\n"
"border-left-color: rgba(255, 255, 255,100);\n"
"border-top-color: rgba(255, 255, 153,0);\n"
""));
        push8->setText(QString::fromUtf8("8"));
        pushBack = new QPushButton(calcForm);
        pushBack->setObjectName(QString::fromUtf8("pushBack"));
        pushBack->setGeometry(QRect(205, 55, 85, 51));
        pushBack->setFont(font);
        pushBack->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(139, 139, 139, 50);\n"
"border: 2px solid  rgb(0, 0, 0);\n"
"border-radius:4px;\n"
"border-right-color: rgba(0, 0, 0,40);\n"
"border-left-color: rgba(255, 255, 255,100);\n"
"border-top-color: rgba(255, 255, 153,0);\n"
""));
        pushBack->setText(QString::fromUtf8("<="));
        pushOk = new QPushButton(calcForm);
        pushOk->setObjectName(QString::fromUtf8("pushOk"));
        pushOk->setGeometry(QRect(15, 375, 126, 51));
        pushOk->setFont(font);
        pushOk->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(139, 139, 139, 50);\n"
"border: 2px solid  rgb(0, 0, 0);\n"
"border-radius:4px;\n"
"border-right-color: rgba(0, 0, 0,40);\n"
"border-left-color: rgba(255, 255, 255,100);\n"
"border-top-color: rgba(255, 255, 153,0);\n"
""));
        pushOk->setText(QString::fromUtf8("OK"));
        pushCanc = new QPushButton(calcForm);
        pushCanc->setObjectName(QString::fromUtf8("pushCanc"));
        pushCanc->setGeometry(QRect(165, 375, 126, 51));
        pushCanc->setFont(font);
        pushCanc->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(139, 139, 139, 50);\n"
"border: 2px solid  rgb(0, 0, 0);\n"
"border-radius:4px;\n"
"border-right-color: rgba(0, 0, 0,40);\n"
"border-left-color: rgba(255, 255, 255,100);\n"
"border-top-color: rgba(255, 255, 153,0);\n"
""));
        pushCanc->setText(QString::fromUtf8("CLR"));
        calcTitle = new QLabel(calcForm);
        calcTitle->setObjectName(QString::fromUtf8("calcTitle"));
        calcTitle->setGeometry(QRect(16, 5, 271, 36));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Bitstream Charter"));
        font1.setPointSize(16);
        font1.setBold(true);
        font1.setWeight(75);
        calcTitle->setFont(font1);
        calcTitle->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(255, 251, 123,40);\n"
"border: 2px solid  rgb(74, 74, 74);\n"
"border-radius:4px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);\n"
""));
        calcTitle->setText(QString::fromUtf8("TextLabel"));
        calcTitle->setAlignment(Qt::AlignCenter);
        push0 = new QPushButton(calcForm);
        push0->setObjectName(QString::fromUtf8("push0"));
        push0->setGeometry(QRect(110, 310, 85, 51));
        push0->setFont(font);
        push0->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(139, 139, 139, 50);\n"
"border: 2px solid  rgb(0, 0, 0);\n"
"border-radius:4px;\n"
"border-right-color: rgba(0, 0, 0,40);\n"
"border-left-color: rgba(255, 255, 255,100);\n"
"border-top-color: rgba(255, 255, 153,0);\n"
""));
        push0->setText(QString::fromUtf8("0"));
        pushDot = new QPushButton(calcForm);
        pushDot->setObjectName(QString::fromUtf8("pushDot"));
        pushDot->setGeometry(QRect(15, 310, 85, 51));
        pushDot->setFont(font);
        pushDot->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(139, 139, 139, 50);\n"
"border: 2px solid  rgb(0, 0, 0);\n"
"border-radius:4px;\n"
"border-right-color: rgba(0, 0, 0,40);\n"
"border-left-color: rgba(255, 255, 255,100);\n"
"border-top-color: rgba(255, 255, 153,0);\n"
""));
        pushDot->setText(QString::fromUtf8("."));
        pushSgn = new QPushButton(calcForm);
        pushSgn->setObjectName(QString::fromUtf8("pushSgn"));
        pushSgn->setGeometry(QRect(205, 310, 85, 51));
        pushSgn->setFont(font);
        pushSgn->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: rgba(139, 139, 139, 50);\n"
"border: 2px solid  rgb(0, 0, 0);\n"
"border-radius:4px;\n"
"border-right-color: rgba(0, 0, 0,40);\n"
"border-left-color: rgba(255, 255, 255,100);\n"
"border-top-color: rgba(255, 255, 153,0);\n"
""));
        pushSgn->setText(QString::fromUtf8("+/-"));

        retranslateUi(InsertCalc);

        QMetaObject::connectSlotsByName(InsertCalc);
    } // setupUi

    void retranslateUi(QWidget *InsertCalc)
    {
        InsertCalc->setWindowTitle(QApplication::translate("InsertCalc", "Form", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class InsertCalc: public Ui_InsertCalc {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INSERTCALC_H
