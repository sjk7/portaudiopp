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
#include <QtWidgets/QFrame>
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
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QComboBox *cboHostApi;
    QPushButton *btnRefresh;
    QFrame *lineRecLevel_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QComboBox *cboInput;
    QPushButton *btnTestInput;
    QFrame *lineRecLevel;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_4;
    QComboBox *cboOutput;
    QPushButton *btnTestOutput;
    QFrame *lineOutputLervel;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_5;
    QComboBox *cboDuplex;
    QPushButton *btnTestDuplex;
    QFrame *lineRecLevel_3;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_3;
    QListWidget *listWidget;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(856, 375);
        layoutWidget = new QWidget(Dialog);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 22, 837, 327));
        verticalLayout_6 = new QVBoxLayout(layoutWidget);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        cboHostApi = new QComboBox(layoutWidget);
        cboHostApi->setObjectName(QString::fromUtf8("cboHostApi"));

        verticalLayout->addWidget(cboHostApi);

        btnRefresh = new QPushButton(layoutWidget);
        btnRefresh->setObjectName(QString::fromUtf8("btnRefresh"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(btnRefresh->sizePolicy().hasHeightForWidth());
        btnRefresh->setSizePolicy(sizePolicy);
        btnRefresh->setCheckable(false);
        btnRefresh->setAutoDefault(false);

        verticalLayout->addWidget(btnRefresh);

        lineRecLevel_2 = new QFrame(layoutWidget);
        lineRecLevel_2->setObjectName(QString::fromUtf8("lineRecLevel_2"));
        lineRecLevel_2->setAutoFillBackground(false);
        lineRecLevel_2->setFrameShadow(QFrame::Plain);
        lineRecLevel_2->setLineWidth(0);
        lineRecLevel_2->setMidLineWidth(0);
        lineRecLevel_2->setFrameShape(QFrame::HLine);

        verticalLayout->addWidget(lineRecLevel_2);


        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout_2->addWidget(label_2);

        cboInput = new QComboBox(layoutWidget);
        cboInput->setObjectName(QString::fromUtf8("cboInput"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(cboInput->sizePolicy().hasHeightForWidth());
        cboInput->setSizePolicy(sizePolicy1);
        cboInput->setMinimumSize(QSize(200, 0));

        verticalLayout_2->addWidget(cboInput);

        btnTestInput = new QPushButton(layoutWidget);
        btnTestInput->setObjectName(QString::fromUtf8("btnTestInput"));
        sizePolicy.setHeightForWidth(btnTestInput->sizePolicy().hasHeightForWidth());
        btnTestInput->setSizePolicy(sizePolicy);
        btnTestInput->setCheckable(true);
        btnTestInput->setAutoDefault(false);

        verticalLayout_2->addWidget(btnTestInput);

        lineRecLevel = new QFrame(layoutWidget);
        lineRecLevel->setObjectName(QString::fromUtf8("lineRecLevel"));
        sizePolicy1.setHeightForWidth(lineRecLevel->sizePolicy().hasHeightForWidth());
        lineRecLevel->setSizePolicy(sizePolicy1);
        lineRecLevel->setMinimumSize(QSize(200, 0));
        lineRecLevel->setAutoFillBackground(false);
        lineRecLevel->setFrameShadow(QFrame::Plain);
        lineRecLevel->setLineWidth(100);
        lineRecLevel->setMidLineWidth(100);
        lineRecLevel->setFrameShape(QFrame::HLine);

        verticalLayout_2->addWidget(lineRecLevel);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        verticalLayout_4->addWidget(label_4);

        cboOutput = new QComboBox(layoutWidget);
        cboOutput->setObjectName(QString::fromUtf8("cboOutput"));

        verticalLayout_4->addWidget(cboOutput);

        btnTestOutput = new QPushButton(layoutWidget);
        btnTestOutput->setObjectName(QString::fromUtf8("btnTestOutput"));
        sizePolicy.setHeightForWidth(btnTestOutput->sizePolicy().hasHeightForWidth());
        btnTestOutput->setSizePolicy(sizePolicy);
        btnTestOutput->setCheckable(true);
        btnTestOutput->setAutoDefault(false);

        verticalLayout_4->addWidget(btnTestOutput);

        lineOutputLervel = new QFrame(layoutWidget);
        lineOutputLervel->setObjectName(QString::fromUtf8("lineOutputLervel"));
        lineOutputLervel->setAutoFillBackground(false);
        lineOutputLervel->setFrameShadow(QFrame::Plain);
        lineOutputLervel->setLineWidth(50);
        lineOutputLervel->setMidLineWidth(50);
        lineOutputLervel->setFrameShape(QFrame::HLine);

        verticalLayout_4->addWidget(lineOutputLervel);


        horizontalLayout->addLayout(verticalLayout_4);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        label_5 = new QLabel(layoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        verticalLayout_5->addWidget(label_5);

        cboDuplex = new QComboBox(layoutWidget);
        cboDuplex->setObjectName(QString::fromUtf8("cboDuplex"));

        verticalLayout_5->addWidget(cboDuplex);

        btnTestDuplex = new QPushButton(layoutWidget);
        btnTestDuplex->setObjectName(QString::fromUtf8("btnTestDuplex"));
        sizePolicy.setHeightForWidth(btnTestDuplex->sizePolicy().hasHeightForWidth());
        btnTestDuplex->setSizePolicy(sizePolicy);
        btnTestDuplex->setCheckable(true);
        btnTestDuplex->setAutoDefault(false);

        verticalLayout_5->addWidget(btnTestDuplex);

        lineRecLevel_3 = new QFrame(layoutWidget);
        lineRecLevel_3->setObjectName(QString::fromUtf8("lineRecLevel_3"));
        lineRecLevel_3->setAutoFillBackground(false);
        lineRecLevel_3->setFrameShadow(QFrame::Plain);
        lineRecLevel_3->setLineWidth(50);
        lineRecLevel_3->setMidLineWidth(50);
        lineRecLevel_3->setFrameShape(QFrame::HLine);

        verticalLayout_5->addWidget(lineRecLevel_3);


        horizontalLayout->addLayout(verticalLayout_5);


        verticalLayout_6->addLayout(horizontalLayout);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout_3->addWidget(label_3);

        listWidget = new QListWidget(layoutWidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        verticalLayout_3->addWidget(listWidget);


        verticalLayout_6->addLayout(verticalLayout_3);


        retranslateUi(Dialog);

        btnTestOutput->setDefault(true);


        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("Dialog", "Host API Selector", nullptr));
        btnRefresh->setText(QCoreApplication::translate("Dialog", "Refresh Devices", nullptr));
        label_2->setText(QCoreApplication::translate("Dialog", "Input Device Selector", nullptr));
        btnTestInput->setText(QCoreApplication::translate("Dialog", "Record Input to &file", nullptr));
        label_4->setText(QCoreApplication::translate("Dialog", "Output Device Selector", nullptr));
        btnTestOutput->setText(QCoreApplication::translate("Dialog", "Test Output Only", nullptr));
        label_5->setText(QCoreApplication::translate("Dialog", "Duplex Device Selector", nullptr));
        btnTestDuplex->setText(QCoreApplication::translate("Dialog", "Test Duplex", nullptr));
        label_3->setText(QCoreApplication::translate("Dialog", "Log:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
