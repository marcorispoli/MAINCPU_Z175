/********************************************************************************
** Form generated from reading UI file 'msgbox.ui'
**
** Created: Fri Sep 13 11:22:47 2019
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MSGBOX_H
#define UI_MSGBOX_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_msgBox
{
public:
    QFrame *frame;
    QLabel *title;
    QFrame *frameText;
    QLabel *content;
    QFrame *buttonFrame;
    QPushButton *okButton;
    QPushButton *funcButton;
    QPushButton *cancButton;
    QProgressBar *progressBar;
    QLineEdit *editable;

    void setupUi(QWidget *msgBox)
    {
        if (msgBox->objectName().isEmpty())
            msgBox->setObjectName(QString::fromUtf8("msgBox"));
        msgBox->resize(420, 440);
        msgBox->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image:url(/transparent.png);\n"
"background-color: qlineargradient(spread:pad, x1:0.75, y1:0.824, x2:0.037, y2:0.05, stop:0 rgba(30,30,30, 200), stop:1 rgba(200, 200, 200, 120));\n"
"border: 4px solid  rgb(74, 74, 74);\n"
"border-radius:10px;\n"
"border-right-color: rgba(255, 255, 153,0);\n"
"border-left-color: rgba(255, 255, 153,0);\n"
"border-top-color: rgba(255, 255, 153,0);\n"
""));
        frame = new QFrame(msgBox);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(0, 0, 420, 440));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        title = new QLabel(frame);
        title->setObjectName(QString::fromUtf8("title"));
        title->setGeometry(QRect(5, 5, 410, 51));
        title->setStyleSheet(QString::fromUtf8("background-color: rgb(79, 79, 79);\n"
"border-color: rgb(255, 255, 153);\n"
"font: 80 16pt \"DejaVu Sans\";\n"
"color: rgb(255, 255, 153);\n"
"\n"
""));
        title->setText(QString::fromUtf8("CALIBRAZIONE COLLIMAZIONI 2D"));
        title->setAlignment(Qt::AlignCenter);
        frameText = new QFrame(frame);
        frameText->setObjectName(QString::fromUtf8("frameText"));
        frameText->setGeometry(QRect(10, 65, 401, 221));
        frameText->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);\n"
""));
        frameText->setFrameShape(QFrame::Box);
        frameText->setFrameShadow(QFrame::Plain);
        frameText->setLineWidth(3);
        content = new QLabel(frameText);
        content->setObjectName(QString::fromUtf8("content"));
        content->setGeometry(QRect(10, 10, 381, 201));
        content->setStyleSheet(QString::fromUtf8("background-color: rgb(201, 201, 201);\n"
"color: rgb(0, 0, 0);\n"
"font: 80 12pt \"DejaVu Sans\";\n"
""));
        content->setText(QString::fromUtf8("CALIBRAZIONE COLLIMAZIONI 2D"));
        content->setAlignment(Qt::AlignCenter);
        buttonFrame = new QFrame(frame);
        buttonFrame->setObjectName(QString::fromUtf8("buttonFrame"));
        buttonFrame->setGeometry(QRect(10, 370, 396, 66));
        buttonFrame->setStyleSheet(QString::fromUtf8(""));
        buttonFrame->setFrameShape(QFrame::StyledPanel);
        buttonFrame->setFrameShadow(QFrame::Raised);
        okButton = new QPushButton(buttonFrame);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setGeometry(QRect(265, 5, 126, 56));
        okButton->setFocusPolicy(Qt::NoFocus);
        okButton->setStyleSheet(QString::fromUtf8("background-color: rgb(79, 79, 79);\n"
"border-color: rgb(255, 255, 153);\n"
"color: rgb(255, 255, 153);\n"
"font: 80 14pt \"DejaVu Sans\";\n"
"\n"
""));
        okButton->setText(QString::fromUtf8("OK"));
        funcButton = new QPushButton(buttonFrame);
        funcButton->setObjectName(QString::fromUtf8("funcButton"));
        funcButton->setGeometry(QRect(135, 5, 126, 56));
        funcButton->setFocusPolicy(Qt::NoFocus);
        funcButton->setStyleSheet(QString::fromUtf8("background-color: rgb(79, 79, 79);\n"
"border-color: rgb(255, 255, 153);\n"
"color: rgb(255, 255, 153);\n"
"font: 80 14pt \"DejaVu Sans\";\n"
""));
        funcButton->setText(QString::fromUtf8("FUNC"));
        cancButton = new QPushButton(buttonFrame);
        cancButton->setObjectName(QString::fromUtf8("cancButton"));
        cancButton->setGeometry(QRect(5, 5, 126, 56));
        cancButton->setFocusPolicy(Qt::NoFocus);
        cancButton->setStyleSheet(QString::fromUtf8("background-color: rgb(79, 79, 79);\n"
"border-color: rgb(255, 255, 153);\n"
"color: rgb(255, 255, 153);\n"
"font: 80 14pt \"DejaVu Sans\";\n"
""));
        cancButton->setText(QString::fromUtf8("CANC"));
        progressBar = new QProgressBar(frame);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(15, 320, 386, 36));
        progressBar->setStyleSheet(QString::fromUtf8(""));
        progressBar->setValue(24);
        progressBar->setAlignment(Qt::AlignCenter);
        progressBar->setTextVisible(true);
        progressBar->setInvertedAppearance(false);
        editable = new QLineEdit(frame);
        editable->setObjectName(QString::fromUtf8("editable"));
        editable->setGeometry(QRect(150, 305, 113, 56));
        QFont font;
        font.setFamily(QString::fromUtf8("Bitstream Charter"));
        font.setPointSize(22);
        font.setBold(true);
        font.setItalic(true);
        font.setWeight(75);
        editable->setFont(font);
        editable->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        editable->setText(QString::fromUtf8("500"));
        editable->setAlignment(Qt::AlignCenter);

        retranslateUi(msgBox);

        QMetaObject::connectSlotsByName(msgBox);
    } // setupUi

    void retranslateUi(QWidget *msgBox)
    {
        msgBox->setWindowTitle(QApplication::translate("msgBox", "Form", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class msgBox: public Ui_msgBox {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MSGBOX_H
