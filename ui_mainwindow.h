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
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
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
    QWidget *widget_2;
    QVBoxLayout *verticalLayout;
    QWidget *widget_4;
    QHBoxLayout *horizontalLayout_3;
    QWidget *widget_5;
    QComboBox *comComboBox;
    QPushButton *pushButtonComOpen;
    QPushButton *pushButton_refreshCom;
    QPushButton *pushButtonClear;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_2;
    QSlider *verticalSlider;
    QSlider *verticalSlider_2;
    QSlider *verticalMotorPos1;
    QSlider *verticalMotorPos2;
    QSlider *verticalMotorPos3;
    QSlider *verticalMotorPos4;
    QSlider *verticalMotorPos5;
    QSlider *verticalMotorPos6;
    QSlider *verticalMotorPos7;
    QSlider *verticalMotorPos8;
    QSlider *verticalMotorPos9;
    QSlider *verticalMotorPos10;
    QWidget *widget_6;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *pushMoveDownState;
    QPushButton *pushButtonGotoPEriodState;
    QPushButton *pushBUttonToIdle;
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
        widget_2 = new QWidget(centralWidget);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        widget_2->setMinimumSize(QSize(820, 0));
        widget_2->setMaximumSize(QSize(800, 200));
        verticalLayout = new QVBoxLayout(widget_2);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        widget_4 = new QWidget(widget_2);
        widget_4->setObjectName(QStringLiteral("widget_4"));
        widget_4->setMaximumSize(QSize(300, 16777215));
        horizontalLayout_3 = new QHBoxLayout(widget_4);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, -1, 0);
        widget_5 = new QWidget(widget_4);
        widget_5->setObjectName(QStringLiteral("widget_5"));

        horizontalLayout_3->addWidget(widget_5);

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


        verticalLayout->addWidget(widget_4);

        widget_3 = new QWidget(widget_2);
        widget_3->setObjectName(QStringLiteral("widget_3"));
        widget_3->setMaximumSize(QSize(300, 16777215));
        horizontalLayout_2 = new QHBoxLayout(widget_3);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        verticalSlider = new QSlider(widget_3);
        verticalSlider->setObjectName(QStringLiteral("verticalSlider"));
        verticalSlider->setMaximum(1000);
        verticalSlider->setValue(0);
        verticalSlider->setOrientation(Qt::Vertical);

        horizontalLayout_2->addWidget(verticalSlider);

        verticalSlider_2 = new QSlider(widget_3);
        verticalSlider_2->setObjectName(QStringLiteral("verticalSlider_2"));
        verticalSlider_2->setMaximum(3999);
        verticalSlider_2->setValue(1000);
        verticalSlider_2->setOrientation(Qt::Vertical);

        horizontalLayout_2->addWidget(verticalSlider_2);

        verticalMotorPos1 = new QSlider(widget_3);
        verticalMotorPos1->setObjectName(QStringLiteral("verticalMotorPos1"));
        verticalMotorPos1->setMaximum(1000);
        verticalMotorPos1->setOrientation(Qt::Vertical);

        horizontalLayout_2->addWidget(verticalMotorPos1);

        verticalMotorPos2 = new QSlider(widget_3);
        verticalMotorPos2->setObjectName(QStringLiteral("verticalMotorPos2"));
        verticalMotorPos2->setMaximum(1000);
        verticalMotorPos2->setOrientation(Qt::Vertical);

        horizontalLayout_2->addWidget(verticalMotorPos2);

        verticalMotorPos3 = new QSlider(widget_3);
        verticalMotorPos3->setObjectName(QStringLiteral("verticalMotorPos3"));
        verticalMotorPos3->setMaximum(1000);
        verticalMotorPos3->setOrientation(Qt::Vertical);

        horizontalLayout_2->addWidget(verticalMotorPos3);

        verticalMotorPos4 = new QSlider(widget_3);
        verticalMotorPos4->setObjectName(QStringLiteral("verticalMotorPos4"));
        verticalMotorPos4->setMaximum(1000);
        verticalMotorPos4->setOrientation(Qt::Vertical);

        horizontalLayout_2->addWidget(verticalMotorPos4);

        verticalMotorPos5 = new QSlider(widget_3);
        verticalMotorPos5->setObjectName(QStringLiteral("verticalMotorPos5"));
        verticalMotorPos5->setMaximum(1000);
        verticalMotorPos5->setOrientation(Qt::Vertical);

        horizontalLayout_2->addWidget(verticalMotorPos5);

        verticalMotorPos6 = new QSlider(widget_3);
        verticalMotorPos6->setObjectName(QStringLiteral("verticalMotorPos6"));
        verticalMotorPos6->setMaximum(1000);
        verticalMotorPos6->setOrientation(Qt::Vertical);

        horizontalLayout_2->addWidget(verticalMotorPos6);

        verticalMotorPos7 = new QSlider(widget_3);
        verticalMotorPos7->setObjectName(QStringLiteral("verticalMotorPos7"));
        verticalMotorPos7->setMaximum(1000);
        verticalMotorPos7->setOrientation(Qt::Vertical);

        horizontalLayout_2->addWidget(verticalMotorPos7);

        verticalMotorPos8 = new QSlider(widget_3);
        verticalMotorPos8->setObjectName(QStringLiteral("verticalMotorPos8"));
        verticalMotorPos8->setMaximum(1000);
        verticalMotorPos8->setOrientation(Qt::Vertical);

        horizontalLayout_2->addWidget(verticalMotorPos8);

        verticalMotorPos9 = new QSlider(widget_3);
        verticalMotorPos9->setObjectName(QStringLiteral("verticalMotorPos9"));
        verticalMotorPos9->setMaximum(1000);
        verticalMotorPos9->setOrientation(Qt::Vertical);

        horizontalLayout_2->addWidget(verticalMotorPos9);

        verticalMotorPos10 = new QSlider(widget_3);
        verticalMotorPos10->setObjectName(QStringLiteral("verticalMotorPos10"));
        verticalMotorPos10->setMaximum(1000);
        verticalMotorPos10->setOrientation(Qt::Vertical);

        horizontalLayout_2->addWidget(verticalMotorPos10);


        verticalLayout->addWidget(widget_3);


        verticalLayout_2->addWidget(widget_2);

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


        verticalLayout_2->addWidget(widget_6);

        widget = new QWidget(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, -1, 0, 0);
        plainTextEdit = new QPlainTextEdit(widget);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
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
        pushMoveDownState->setText(QApplication::translate("MainWindow", "moveDownState", Q_NULLPTR));
        pushButtonGotoPEriodState->setText(QApplication::translate("MainWindow", "to Period", Q_NULLPTR));
        pushBUttonToIdle->setText(QApplication::translate("MainWindow", "to idle", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
