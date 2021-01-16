/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QComboBox *cboHostApi;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QComboBox *cboDevice;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_3;
    QListWidget *listWidget;
    QPushButton *btnTest;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(800, 371);
        layoutWidget = new QWidget(Dialog);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(13, 32, 761, 52));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        cboHostApi = new QComboBox(layoutWidget);
        cboHostApi->setObjectName(QString::fromUtf8("cboHostApi"));

        verticalLayout->addWidget(cboHostApi);


        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout_2->addWidget(label_2);

        cboDevice = new QComboBox(layoutWidget);
        cboDevice->setObjectName(QString::fromUtf8("cboDevice"));

        verticalLayout_2->addWidget(cboDevice);


        horizontalLayout->addLayout(verticalLayout_2);

        layoutWidget1 = new QWidget(Dialog);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(10, 140, 761, 217));
        verticalLayout_3 = new QVBoxLayout(layoutWidget1);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(layoutWidget1);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout_3->addWidget(label_3);

        listWidget = new QListWidget(layoutWidget1);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        verticalLayout_3->addWidget(listWidget);

        btnTest = new QPushButton(Dialog);
        btnTest->setObjectName(QString::fromUtf8("btnTest"));
        btnTest->setGeometry(QRect(310, 110, 80, 25));

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("Dialog", "Host API Selector", nullptr));
        label_2->setText(QCoreApplication::translate("Dialog", "Device Selector", nullptr));
        label_3->setText(QCoreApplication::translate("Dialog", "Log:", nullptr));
        btnTest->setText(QCoreApplication::translate("Dialog", "&Test", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
