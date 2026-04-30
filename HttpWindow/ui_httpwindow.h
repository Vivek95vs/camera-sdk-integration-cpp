/********************************************************************************
** Form generated from reading UI file 'httpwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HTTPWINDOW_H
#define UI_HTTPWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_HttpWindow
{
public:
    QLineEdit *urlLineEdit;

    void setupUi(QDialog *HttpWindow)
    {
        if (HttpWindow->objectName().isEmpty())
            HttpWindow->setObjectName(QStringLiteral("HttpWindow"));
        HttpWindow->resize(400, 300);
        urlLineEdit = new QLineEdit(HttpWindow);
        urlLineEdit->setObjectName(QStringLiteral("urlLineEdit"));
        urlLineEdit->setGeometry(QRect(140, 60, 113, 20));

        retranslateUi(HttpWindow);

        QMetaObject::connectSlotsByName(HttpWindow);
    } // setupUi

    void retranslateUi(QDialog *HttpWindow)
    {
        HttpWindow->setWindowTitle(QApplication::translate("HttpWindow", "HttpWindow", 0));
        urlLineEdit->setText(QApplication::translate("HttpWindow", "http://qt-project.org/", 0));
    } // retranslateUi

};

namespace Ui {
    class HttpWindow: public Ui_HttpWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HTTPWINDOW_H
