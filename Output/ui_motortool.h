/********************************************************************************
** Form generated from reading UI file 'motortool.ui'
**
** Created: Fri Sep 13 11:22:47 2019
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MOTORTOOL_H
#define UI_MOTORTOOL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MotorTool
{
public:

    void setupUi(QWidget *MotorTool)
    {
        if (MotorTool->objectName().isEmpty())
            MotorTool->setObjectName(QString::fromUtf8("MotorTool"));
        MotorTool->resize(640, 480);

        retranslateUi(MotorTool);

        QMetaObject::connectSlotsByName(MotorTool);
    } // setupUi

    void retranslateUi(QWidget *MotorTool)
    {
        MotorTool->setWindowTitle(QApplication::translate("MotorTool", "Form", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MotorTool: public Ui_MotorTool {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MOTORTOOL_H
