/********************************************************************************
** Form generated from reading UI file 'startuppage.ui'
**
** Created: Fri Sep 13 11:22:47 2019
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STARTUPPAGE_H
#define UI_STARTUPPAGE_H

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

class Ui_StartupPage
{
public:
    QFrame *frame;
    QPushButton *exitButton;
    QLabel *StartupTitleLabel;
    QLabel *PackageIdLabel;

    void setupUi(QWidget *StartupPage)
    {
        if (StartupPage->objectName().isEmpty())
            StartupPage->setObjectName(QString::fromUtf8("StartupPage"));
        StartupPage->resize(800, 480);
        StartupPage->setStyleSheet(QString::fromUtf8("background-color:rgb(0,0,0,0);\n"
"background-image:url(://StartupPage/StartupPage/background.png);\n"
"border: 0px;"));
        frame = new QFrame(StartupPage);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(0, 0, 800, 480));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        exitButton = new QPushButton(frame);
        exitButton->setObjectName(QString::fromUtf8("exitButton"));
        exitButton->setGeometry(QRect(0, 0, 111, 111));
        exitButton->setStyleSheet(QString::fromUtf8("background-color:rgb(0,0,0,0);\n"
"background-image:url(://transparent.png);\n"
"border: 0px;"));
        StartupTitleLabel = new QLabel(frame);
        StartupTitleLabel->setObjectName(QString::fromUtf8("StartupTitleLabel"));
        StartupTitleLabel->setGeometry(QRect(140, 10, 566, 41));
        QFont font;
        font.setFamily(QString::fromUtf8("Bitstream Charter"));
        font.setPointSize(24);
        font.setBold(true);
        font.setItalic(true);
        font.setWeight(75);
        StartupTitleLabel->setFont(font);
        StartupTitleLabel->setStyleSheet(QString::fromUtf8("background-image:url(/transparent.png);\n"
"color: rgb(248, 248, 54);"));
        StartupTitleLabel->setAlignment(Qt::AlignCenter);
        PackageIdLabel = new QLabel(frame);
        PackageIdLabel->setObjectName(QString::fromUtf8("PackageIdLabel"));
        PackageIdLabel->setGeometry(QRect(685, 435, 106, 31));
        PackageIdLabel->setFont(font);
        PackageIdLabel->setStyleSheet(QString::fromUtf8("background-image:url(/transparent.png);\n"
"color: rgb(248, 248, 54);"));
        PackageIdLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        retranslateUi(StartupPage);

        QMetaObject::connectSlotsByName(StartupPage);
    } // setupUi

    void retranslateUi(QWidget *StartupPage)
    {
        StartupPage->setWindowTitle(QApplication::translate("StartupPage", "Form", 0, QApplication::UnicodeUTF8));
        exitButton->setText(QString());
        StartupTitleLabel->setText(QApplication::translate("StartupPage", "SYSTEM STARTUP", 0, QApplication::UnicodeUTF8));
        PackageIdLabel->setText(QApplication::translate("StartupPage", "ID 12", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class StartupPage: public Ui_StartupPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STARTUPPAGE_H
