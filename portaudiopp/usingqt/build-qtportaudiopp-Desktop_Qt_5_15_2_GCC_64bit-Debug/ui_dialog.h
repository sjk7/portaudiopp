/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_3;
    QListWidget *listWidget;
    QWidget *widget;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QComboBox *cboHostApi;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QComboBox *cboInput;
    QPushButton *btnTestInput;
    QLabel *label_4;
    QLabel *label_5;
    QComboBox *cboOutput;
    QComboBox *cboDuplex;
    QPushButton *btnTestOutput;
    QPushButton *btnTestDuplex;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(1116, 371);
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

        widget = new QWidget(Dialog);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(13, 20, 1071, 88));
        gridLayout = new QGridLayout(widget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        cboHostApi = new QComboBox(widget);
        cboHostApi->setObjectName(QString::fromUtf8("cboHostApi"));

        verticalLayout->addWidget(cboHostApi);

        verticalSpacer = new QSpacerItem(20, 30, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer);


        gridLayout->addLayout(verticalLayout, 0, 0, 3, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_2 = new QLabel(widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout_2->addWidget(label_2);

        cboInput = new QComboBox(widget);
        cboInput->setObjectName(QString::fromUtf8("cboInput"));

        verticalLayout_2->addWidget(cboInput);

        btnTestInput = new QPushButton(widget);
        btnTestInput->setObjectName(QString::fromUtf8("btnTestInput"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(btnTestInput->sizePolicy().hasHeightForWidth());
        btnTestInput->setSizePolicy(sizePolicy);

        verticalLayout_2->addWidget(btnTestInput);


        gridLayout->addLayout(verticalLayout_2, 0, 1, 3, 1);

        label_4 = new QLabel(widget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 0, 2, 1, 1);

        label_5 = new QLabel(widget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 0, 3, 1, 1);

        cboOutput = new QComboBox(widget);
        cboOutput->setObjectName(QString::fromUtf8("cboOutput"));

        gridLayout->addWidget(cboOutput, 1, 2, 1, 1);

        cboDuplex = new QComboBox(widget);
        cboDuplex->setObjectName(QString::fromUtf8("cboDuplex"));

        gridLayout->addWidget(cboDuplex, 1, 3, 1, 1);

        btnTestOutput = new QPushButton(widget);
        btnTestOutput->setObjectName(QString::fromUtf8("btnTestOutput"));
        sizePolicy.setHeightForWidth(btnTestOutput->sizePolicy().hasHeightForWidth());
        btnTestOutput->setSizePolicy(sizePolicy);

        gridLayout->addWidget(btnTestOutput, 2, 2, 1, 1);

        btnTestDuplex = new QPushButton(widget);
        btnTestDuplex->setObjectName(QString::fromUtf8("btnTestDuplex"));
        sizePolicy.setHeightForWidth(btnTestDuplex->sizePolicy().hasHeightForWidth());
        btnTestDuplex->setSizePolicy(sizePolicy);

        gridLayout->addWidget(btnTestDuplex, 2, 3, 1, 1);


        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "Dialog", nullptr));
        label_3->setText(QCoreApplication::translate("Dialog", "Log:", nullptr));
        label->setText(QCoreApplication::translate("Dialog", "Host API Selector", nullptr));
        label_2->setText(QCoreApplication::translate("Dialog", "Input Device Selector", nullptr));
        btnTestInput->setText(QCoreApplication::translate("Dialog", "&Record Input to file", nullptr));
        label_4->setText(QCoreApplication::translate("Dialog", "Output Device Selector", nullptr));
        label_5->setText(QCoreApplication::translate("Dialog", "Duplex Device Selector", nullptr));
        btnTestOutput->setText(QCoreApplication::translate("Dialog", "Test Output Only", nullptr));
        btnTestDuplex->setText(QCoreApplication::translate("Dialog", "Test Duplex", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
