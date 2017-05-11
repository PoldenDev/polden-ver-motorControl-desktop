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
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(523, 563);
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
        pushButtonSetCont_2->setGeometry(QRect(270, 180, 75, 23));
        pushButtonSetRange_2 = new QPushButton(centralWidget);
        pushButtonSetRange_2->setObjectName(QStringLiteral("pushButtonSetRange_2"));
        pushButtonSetRange_2->setGeometry(QRect(270, 150, 75, 23));
        lineEditVel_2 = new QLineEdit(centralWidget);
        lineEditVel_2->setObjectName(QStringLiteral("lineEditVel_2"));
        lineEditVel_2->setGeometry(QRect(360, 90, 81, 20));
        lineEditRange3_2 = new QLineEdit(centralWidget);
        lineEditRange3_2->setObjectName(QStringLiteral("lineEditRange3_2"));
        lineEditRange3_2->setGeometry(QRect(360, 150, 81, 20));
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(270, 90, 75, 23));
        lineEditPos_2 = new QLineEdit(centralWidget);
        lineEditPos_2->setObjectName(QStringLiteral("lineEditPos_2"));
        lineEditPos_2->setGeometry(QRect(360, 60, 81, 20));
        pushButtonDir_2 = new QPushButton(centralWidget);
        pushButtonDir_2->setObjectName(QStringLiteral("pushButtonDir_2"));
        pushButtonDir_2->setGeometry(QRect(270, 120, 75, 23));
        lineEditDir_2 = new QLineEdit(centralWidget);
        lineEditDir_2->setObjectName(QStringLiteral("lineEditDir_2"));
        lineEditDir_2->setGeometry(QRect(360, 120, 81, 20));
        butPos_2 = new QPushButton(centralWidget);
        butPos_2->setObjectName(QStringLiteral("butPos_2"));
        butPos_2->setGeometry(QRect(270, 60, 75, 23));
        verticalSlider = new QSlider(centralWidget);
        verticalSlider->setObjectName(QStringLiteral("verticalSlider"));
        verticalSlider->setGeometry(QRect(450, 20, 22, 271));
        verticalSlider->setOrientation(Qt::Vertical);
        verticalSlider_2 = new QSlider(centralWidget);
        verticalSlider_2->setObjectName(QStringLiteral("verticalSlider_2"));
        verticalSlider_2->setGeometry(QRect(490, 20, 22, 271));
        verticalSlider_2->setOrientation(Qt::Vertical);
        comComboBox = new QComboBox(centralWidget);
        comComboBox->setObjectName(QStringLiteral("comComboBox"));
        comComboBox->setGeometry(QRect(10, 10, 91, 22));
        plainTextEdit = new QPlainTextEdit(centralWidget);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(10, 230, 391, 121));
        plainTextEdit->setReadOnly(true);
        plainTextUDP = new QPlainTextEdit(centralWidget);
        plainTextUDP->setObjectName(QStringLiteral("plainTextUDP"));
        plainTextUDP->setGeometry(QRect(10, 380, 391, 111));
        plainTextUDP->setReadOnly(true);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 523, 21));
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
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
