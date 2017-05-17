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
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *butPos;
    QPushButton *pushButtonComOpen;
    QLineEdit *lineEditPos;
    QPushButton *pushButton;
    QLineEdit *lineEditVel;
    QPushButton *pushButtonSetRange;
    QPushButton *pushButtonDir;
    QLineEdit *lineEditDir;
    QLineEdit *lineEditRange3;
    QPushButton *pushButtonSetCont;
    QPushButton *pushButtonSetCont_2;
    QPushButton *pushButtonSetRange_2;
    QLineEdit *lineEditVel_2;
    QLineEdit *lineEditRange3_2;
    QPushButton *pushButton_2;
    QLineEdit *lineEditPos_2;
    QPushButton *pushButtonDir_2;
    QLineEdit *lineEditDir_2;
    QPushButton *butPos_2;
    QSlider *verticalSlider;
    QSlider *verticalSlider_2;
    QComboBox *comComboBox;
    QPlainTextEdit *plainTextEdit;
    QPlainTextEdit *plainTextUDP;
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
    QPushButton *pushButton_refreshCom;
    QPushButton *pushButtonClear;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(784, 742);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        butPos = new QPushButton(centralWidget);
        butPos->setObjectName(QStringLiteral("butPos"));
        butPos->setGeometry(QRect(10, 60, 75, 23));
        pushButtonComOpen = new QPushButton(centralWidget);
        pushButtonComOpen->setObjectName(QStringLiteral("pushButtonComOpen"));
        pushButtonComOpen->setGeometry(QRect(110, 10, 75, 31));
        lineEditPos = new QLineEdit(centralWidget);
        lineEditPos->setObjectName(QStringLiteral("lineEditPos"));
        lineEditPos->setGeometry(QRect(100, 60, 81, 20));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(10, 90, 75, 23));
        lineEditVel = new QLineEdit(centralWidget);
        lineEditVel->setObjectName(QStringLiteral("lineEditVel"));
        lineEditVel->setGeometry(QRect(100, 90, 81, 20));
        pushButtonSetRange = new QPushButton(centralWidget);
        pushButtonSetRange->setObjectName(QStringLiteral("pushButtonSetRange"));
        pushButtonSetRange->setGeometry(QRect(10, 150, 75, 23));
        pushButtonDir = new QPushButton(centralWidget);
        pushButtonDir->setObjectName(QStringLiteral("pushButtonDir"));
        pushButtonDir->setGeometry(QRect(10, 120, 75, 23));
        lineEditDir = new QLineEdit(centralWidget);
        lineEditDir->setObjectName(QStringLiteral("lineEditDir"));
        lineEditDir->setGeometry(QRect(100, 120, 81, 20));
        lineEditRange3 = new QLineEdit(centralWidget);
        lineEditRange3->setObjectName(QStringLiteral("lineEditRange3"));
        lineEditRange3->setGeometry(QRect(100, 150, 81, 20));
        pushButtonSetCont = new QPushButton(centralWidget);
        pushButtonSetCont->setObjectName(QStringLiteral("pushButtonSetCont"));
        pushButtonSetCont->setGeometry(QRect(10, 180, 75, 23));
        pushButtonSetCont_2 = new QPushButton(centralWidget);
        pushButtonSetCont_2->setObjectName(QStringLiteral("pushButtonSetCont_2"));
        pushButtonSetCont_2->setGeometry(QRect(200, 180, 75, 23));
        pushButtonSetRange_2 = new QPushButton(centralWidget);
        pushButtonSetRange_2->setObjectName(QStringLiteral("pushButtonSetRange_2"));
        pushButtonSetRange_2->setGeometry(QRect(200, 150, 75, 23));
        lineEditVel_2 = new QLineEdit(centralWidget);
        lineEditVel_2->setObjectName(QStringLiteral("lineEditVel_2"));
        lineEditVel_2->setGeometry(QRect(290, 90, 81, 20));
        lineEditRange3_2 = new QLineEdit(centralWidget);
        lineEditRange3_2->setObjectName(QStringLiteral("lineEditRange3_2"));
        lineEditRange3_2->setGeometry(QRect(290, 150, 81, 20));
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(200, 90, 75, 23));
        lineEditPos_2 = new QLineEdit(centralWidget);
        lineEditPos_2->setObjectName(QStringLiteral("lineEditPos_2"));
        lineEditPos_2->setGeometry(QRect(290, 60, 81, 20));
        pushButtonDir_2 = new QPushButton(centralWidget);
        pushButtonDir_2->setObjectName(QStringLiteral("pushButtonDir_2"));
        pushButtonDir_2->setGeometry(QRect(200, 120, 75, 23));
        lineEditDir_2 = new QLineEdit(centralWidget);
        lineEditDir_2->setObjectName(QStringLiteral("lineEditDir_2"));
        lineEditDir_2->setGeometry(QRect(290, 120, 81, 20));
        butPos_2 = new QPushButton(centralWidget);
        butPos_2->setObjectName(QStringLiteral("butPos_2"));
        butPos_2->setGeometry(QRect(200, 60, 75, 23));
        verticalSlider = new QSlider(centralWidget);
        verticalSlider->setObjectName(QStringLiteral("verticalSlider"));
        verticalSlider->setGeometry(QRect(390, 20, 22, 181));
        verticalSlider->setMaximum(1000);
        verticalSlider->setValue(0);
        verticalSlider->setOrientation(Qt::Vertical);
        verticalSlider_2 = new QSlider(centralWidget);
        verticalSlider_2->setObjectName(QStringLiteral("verticalSlider_2"));
        verticalSlider_2->setGeometry(QRect(420, 10, 22, 191));
        verticalSlider_2->setMaximum(3999);
        verticalSlider_2->setValue(1000);
        verticalSlider_2->setOrientation(Qt::Vertical);
        comComboBox = new QComboBox(centralWidget);
        comComboBox->setObjectName(QStringLiteral("comComboBox"));
        comComboBox->setGeometry(QRect(10, 10, 91, 22));
        plainTextEdit = new QPlainTextEdit(centralWidget);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(10, 230, 371, 461));
        plainTextEdit->setReadOnly(true);
        plainTextUDP = new QPlainTextEdit(centralWidget);
        plainTextUDP->setObjectName(QStringLiteral("plainTextUDP"));
        plainTextUDP->setGeometry(QRect(400, 230, 371, 461));
        plainTextUDP->setReadOnly(true);
        verticalMotorPos1 = new QSlider(centralWidget);
        verticalMotorPos1->setObjectName(QStringLiteral("verticalMotorPos1"));
        verticalMotorPos1->setGeometry(QRect(460, 40, 22, 160));
        verticalMotorPos1->setMaximum(1000);
        verticalMotorPos1->setOrientation(Qt::Vertical);
        verticalMotorPos2 = new QSlider(centralWidget);
        verticalMotorPos2->setObjectName(QStringLiteral("verticalMotorPos2"));
        verticalMotorPos2->setGeometry(QRect(490, 40, 22, 160));
        verticalMotorPos2->setMaximum(1000);
        verticalMotorPos2->setOrientation(Qt::Vertical);
        verticalMotorPos3 = new QSlider(centralWidget);
        verticalMotorPos3->setObjectName(QStringLiteral("verticalMotorPos3"));
        verticalMotorPos3->setGeometry(QRect(520, 40, 22, 160));
        verticalMotorPos3->setMaximum(1000);
        verticalMotorPos3->setOrientation(Qt::Vertical);
        verticalMotorPos4 = new QSlider(centralWidget);
        verticalMotorPos4->setObjectName(QStringLiteral("verticalMotorPos4"));
        verticalMotorPos4->setGeometry(QRect(550, 40, 22, 160));
        verticalMotorPos4->setMaximum(1000);
        verticalMotorPos4->setOrientation(Qt::Vertical);
        verticalMotorPos5 = new QSlider(centralWidget);
        verticalMotorPos5->setObjectName(QStringLiteral("verticalMotorPos5"));
        verticalMotorPos5->setGeometry(QRect(580, 40, 22, 160));
        verticalMotorPos5->setMaximum(1000);
        verticalMotorPos5->setOrientation(Qt::Vertical);
        verticalMotorPos6 = new QSlider(centralWidget);
        verticalMotorPos6->setObjectName(QStringLiteral("verticalMotorPos6"));
        verticalMotorPos6->setGeometry(QRect(610, 40, 22, 160));
        verticalMotorPos6->setMaximum(1000);
        verticalMotorPos6->setOrientation(Qt::Vertical);
        verticalMotorPos7 = new QSlider(centralWidget);
        verticalMotorPos7->setObjectName(QStringLiteral("verticalMotorPos7"));
        verticalMotorPos7->setGeometry(QRect(640, 40, 22, 160));
        verticalMotorPos7->setMaximum(1000);
        verticalMotorPos7->setOrientation(Qt::Vertical);
        verticalMotorPos8 = new QSlider(centralWidget);
        verticalMotorPos8->setObjectName(QStringLiteral("verticalMotorPos8"));
        verticalMotorPos8->setGeometry(QRect(670, 40, 22, 160));
        verticalMotorPos8->setMaximum(1000);
        verticalMotorPos8->setOrientation(Qt::Vertical);
        verticalMotorPos9 = new QSlider(centralWidget);
        verticalMotorPos9->setObjectName(QStringLiteral("verticalMotorPos9"));
        verticalMotorPos9->setGeometry(QRect(700, 40, 22, 160));
        verticalMotorPos9->setMaximum(1000);
        verticalMotorPos9->setOrientation(Qt::Vertical);
        verticalMotorPos10 = new QSlider(centralWidget);
        verticalMotorPos10->setObjectName(QStringLiteral("verticalMotorPos10"));
        verticalMotorPos10->setGeometry(QRect(730, 40, 22, 160));
        verticalMotorPos10->setMaximum(1000);
        verticalMotorPos10->setOrientation(Qt::Vertical);
        pushButton_refreshCom = new QPushButton(centralWidget);
        pushButton_refreshCom->setObjectName(QStringLiteral("pushButton_refreshCom"));
        pushButton_refreshCom->setGeometry(QRect(190, 10, 75, 31));
        pushButtonClear = new QPushButton(centralWidget);
        pushButtonClear->setObjectName(QStringLiteral("pushButtonClear"));
        pushButtonClear->setGeometry(QRect(270, 10, 75, 31));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 784, 21));
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
        butPos->setText(QApplication::translate("MainWindow", "setPos", Q_NULLPTR));
        pushButtonComOpen->setText(QApplication::translate("MainWindow", "open", Q_NULLPTR));
        pushButton->setText(QApplication::translate("MainWindow", "setVel", Q_NULLPTR));
        pushButtonSetRange->setText(QApplication::translate("MainWindow", "setRange", Q_NULLPTR));
        pushButtonDir->setText(QApplication::translate("MainWindow", "setDir", Q_NULLPTR));
        pushButtonSetCont->setText(QApplication::translate("MainWindow", "setCont", Q_NULLPTR));
        pushButtonSetCont_2->setText(QApplication::translate("MainWindow", "setCont", Q_NULLPTR));
        pushButtonSetRange_2->setText(QApplication::translate("MainWindow", "setRange", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("MainWindow", "setVel", Q_NULLPTR));
        pushButtonDir_2->setText(QApplication::translate("MainWindow", "setDir", Q_NULLPTR));
        butPos_2->setText(QApplication::translate("MainWindow", "setPos", Q_NULLPTR));
        pushButton_refreshCom->setText(QApplication::translate("MainWindow", "refresh", Q_NULLPTR));
        pushButtonClear->setText(QApplication::translate("MainWindow", "clear", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
