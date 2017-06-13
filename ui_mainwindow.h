/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QWidget *widget_4;
    QHBoxLayout *horizontalLayout_3;
    QComboBox *comComboBox;
    QPushButton *pushButtonComOpen;
    QPushButton *pushButton_refreshCom;
    QPushButton *pushButtonClear;
    QPushButton *pushClearMap;
    QLabel *label;
    QLineEdit *lineEditMinVal;
    QLabel *label_2;
    QLineEdit *lineEditMaxVal;
    QWidget *qwtWdgH1;
    QHBoxLayout *horizontalLayout_2;
    QWidget *widget_7;
    QWidget *qwtWdgH2;
    QHBoxLayout *horizontalLayout_5;
    QWidget *widget_8;
    QWidget *widget_6;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *pushMoveDownState;
    QPushButton *pushButtonGotoPEriodState;
    QPushButton *pushBUttonToIdle;
    QPushButton *pushButtonPosReset;
    QPushButton *pushTestData;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QPlainTextEdit *plainTextEdit;
    QPlainTextEdit *plainTextUDP;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(840, 820);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        widget_4 = new QWidget(centralWidget);
        widget_4->setObjectName(QStringLiteral("widget_4"));
        widget_4->setMaximumSize(QSize(600, 16777215));
        horizontalLayout_3 = new QHBoxLayout(widget_4);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, -1, 0);
        comComboBox = new QComboBox(widget_4);
        comComboBox->setObjectName(QStringLiteral("comComboBox"));

        horizontalLayout_3->addWidget(comComboBox);

        pushButtonComOpen = new QPushButton(widget_4);
        pushButtonComOpen->setObjectName(QStringLiteral("pushButtonComOpen"));

        horizontalLayout_3->addWidget(pushButtonComOpen);

        pushButton_refreshCom = new QPushButton(widget_4);
        pushButton_refreshCom->setObjectName(QStringLiteral("pushButton_refreshCom"));

        horizontalLayout_3->addWidget(pushButton_refreshCom);

        pushButtonClear = new QPushButton(widget_4);
        pushButtonClear->setObjectName(QStringLiteral("pushButtonClear"));

        horizontalLayout_3->addWidget(pushButtonClear);

        pushClearMap = new QPushButton(widget_4);
        pushClearMap->setObjectName(QStringLiteral("pushClearMap"));

        horizontalLayout_3->addWidget(pushClearMap);

        label = new QLabel(widget_4);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_3->addWidget(label);

        lineEditMinVal = new QLineEdit(widget_4);
        lineEditMinVal->setObjectName(QStringLiteral("lineEditMinVal"));
        lineEditMinVal->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(lineEditMinVal);

        label_2 = new QLabel(widget_4);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_3->addWidget(label_2);

        lineEditMaxVal = new QLineEdit(widget_4);
        lineEditMaxVal->setObjectName(QStringLiteral("lineEditMaxVal"));
        lineEditMaxVal->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(lineEditMaxVal);


        verticalLayout_2->addWidget(widget_4);

        qwtWdgH1 = new QWidget(centralWidget);
        qwtWdgH1->setObjectName(QStringLiteral("qwtWdgH1"));
        horizontalLayout_2 = new QHBoxLayout(qwtWdgH1);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        widget_7 = new QWidget(qwtWdgH1);
        widget_7->setObjectName(QStringLiteral("widget_7"));

        horizontalLayout_2->addWidget(widget_7);


        verticalLayout_2->addWidget(qwtWdgH1);

        qwtWdgH2 = new QWidget(centralWidget);
        qwtWdgH2->setObjectName(QStringLiteral("qwtWdgH2"));
        horizontalLayout_5 = new QHBoxLayout(qwtWdgH2);
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        widget_8 = new QWidget(qwtWdgH2);
        widget_8->setObjectName(QStringLiteral("widget_8"));

        horizontalLayout_5->addWidget(widget_8);


        verticalLayout_2->addWidget(qwtWdgH2);

        widget_6 = new QWidget(centralWidget);
        widget_6->setObjectName(QStringLiteral("widget_6"));
        horizontalLayout_4 = new QHBoxLayout(widget_6);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        pushMoveDownState = new QPushButton(widget_6);
        pushMoveDownState->setObjectName(QStringLiteral("pushMoveDownState"));

        horizontalLayout_4->addWidget(pushMoveDownState);

        pushButtonGotoPEriodState = new QPushButton(widget_6);
        pushButtonGotoPEriodState->setObjectName(QStringLiteral("pushButtonGotoPEriodState"));

        horizontalLayout_4->addWidget(pushButtonGotoPEriodState);

        pushBUttonToIdle = new QPushButton(widget_6);
        pushBUttonToIdle->setObjectName(QStringLiteral("pushBUttonToIdle"));

        horizontalLayout_4->addWidget(pushBUttonToIdle);

        pushButtonPosReset = new QPushButton(widget_6);
        pushButtonPosReset->setObjectName(QStringLiteral("pushButtonPosReset"));

        horizontalLayout_4->addWidget(pushButtonPosReset);

        pushTestData = new QPushButton(widget_6);
        pushTestData->setObjectName(QStringLiteral("pushTestData"));

        horizontalLayout_4->addWidget(pushTestData);


        verticalLayout_2->addWidget(widget_6);

        widget = new QWidget(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setMinimumSize(QSize(0, 300));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, -1, 0, 0);
        plainTextEdit = new QPlainTextEdit(widget);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        plainTextEdit->setMinimumSize(QSize(0, 0));
        plainTextEdit->setReadOnly(true);

        horizontalLayout->addWidget(plainTextEdit);

        plainTextUDP = new QPlainTextEdit(widget);
        plainTextUDP->setObjectName(QStringLiteral("plainTextUDP"));
        plainTextUDP->setMaximumSize(QSize(200, 16777215));
        plainTextUDP->setReadOnly(true);

        horizontalLayout->addWidget(plainTextUDP);


        verticalLayout_2->addWidget(widget);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 840, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        pushButtonComOpen->setText(QApplication::translate("MainWindow", "open", Q_NULLPTR));
        pushButton_refreshCom->setText(QApplication::translate("MainWindow", "refresh", Q_NULLPTR));
        pushButtonClear->setText(QApplication::translate("MainWindow", "clear", Q_NULLPTR));
        pushClearMap->setText(QApplication::translate("MainWindow", "clearMap", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "min:", Q_NULLPTR));
        lineEditMinVal->setInputMask(QApplication::translate("MainWindow", "999", Q_NULLPTR));
        lineEditMinVal->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "max", Q_NULLPTR));
        lineEditMaxVal->setInputMask(QApplication::translate("MainWindow", "999", Q_NULLPTR));
        lineEditMaxVal->setText(QApplication::translate("MainWindow", "999", Q_NULLPTR));
        pushMoveDownState->setText(QApplication::translate("MainWindow", "moveDownState", Q_NULLPTR));
        pushButtonGotoPEriodState->setText(QApplication::translate("MainWindow", "to Period", Q_NULLPTR));
        pushBUttonToIdle->setText(QApplication::translate("MainWindow", "to idle", Q_NULLPTR));
        pushButtonPosReset->setText(QApplication::translate("MainWindow", "pos reset", Q_NULLPTR));
        pushTestData->setText(QApplication::translate("MainWindow", "testData", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
