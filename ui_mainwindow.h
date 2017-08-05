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
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
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
    QLabel *label_3;
    QLineEdit *maxSteps;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout;
    QWidget *qwtWdgH1;
    QHBoxLayout *horizontalLayout_2;
    QWidget *widget_7;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QWidget *qwtWdgH2;
    QHBoxLayout *horizontalLayout_5;
    QWidget *widget_8;
    QWidget *widget_6;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *pushMoveDownState;
    QPushButton *pushTestData;
    QPushButton *goToTerm;
    QPushButton *pushButtonGotoPEriodState;
    QPushButton *pushBUttonToIdle;
    QPushButton *pushButtonPosReset;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QPlainTextEdit *plainTextEdit;
    QPlainTextEdit *plainTextUDP;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_4;
    QWidget *widget_5;
    QHBoxLayout *horizontalLayout_6;
    QCheckBox *term1;
    QCheckBox *term2;
    QCheckBox *term3;
    QCheckBox *term4;
    QCheckBox *term5;
    QWidget *widget_9;
    QHBoxLayout *horizontalLayout_7;
    QCheckBox *term6;
    QCheckBox *term7;
    QCheckBox *term8;
    QCheckBox *term9;
    QCheckBox *term10;
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

        label_3 = new QLabel(widget_4);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_3->addWidget(label_3);

        maxSteps = new QLineEdit(widget_4);
        maxSteps->setObjectName(QStringLiteral("maxSteps"));
        maxSteps->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(maxSteps);


        verticalLayout_2->addWidget(widget_4);

        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        verticalLayout = new QVBoxLayout(tab);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        qwtWdgH1 = new QWidget(tab);
        qwtWdgH1->setObjectName(QStringLiteral("qwtWdgH1"));
        horizontalLayout_2 = new QHBoxLayout(qwtWdgH1);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        widget_7 = new QWidget(qwtWdgH1);
        widget_7->setObjectName(QStringLiteral("widget_7"));
        formLayoutWidget = new QWidget(widget_7);
        formLayoutWidget->setObjectName(QStringLiteral("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(80, 150, 160, 80));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);

        horizontalLayout_2->addWidget(widget_7);


        verticalLayout->addWidget(qwtWdgH1);

        qwtWdgH2 = new QWidget(tab);
        qwtWdgH2->setObjectName(QStringLiteral("qwtWdgH2"));
        horizontalLayout_5 = new QHBoxLayout(qwtWdgH2);
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        widget_8 = new QWidget(qwtWdgH2);
        widget_8->setObjectName(QStringLiteral("widget_8"));

        horizontalLayout_5->addWidget(widget_8);


        verticalLayout->addWidget(qwtWdgH2);

        tabWidget->addTab(tab, QString());

        verticalLayout_2->addWidget(tabWidget);

        widget_6 = new QWidget(centralWidget);
        widget_6->setObjectName(QStringLiteral("widget_6"));
        horizontalLayout_4 = new QHBoxLayout(widget_6);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        pushMoveDownState = new QPushButton(widget_6);
        pushMoveDownState->setObjectName(QStringLiteral("pushMoveDownState"));

        horizontalLayout_4->addWidget(pushMoveDownState);

        pushTestData = new QPushButton(widget_6);
        pushTestData->setObjectName(QStringLiteral("pushTestData"));

        horizontalLayout_4->addWidget(pushTestData);

        goToTerm = new QPushButton(widget_6);
        goToTerm->setObjectName(QStringLiteral("goToTerm"));

        horizontalLayout_4->addWidget(goToTerm);

        pushButtonGotoPEriodState = new QPushButton(widget_6);
        pushButtonGotoPEriodState->setObjectName(QStringLiteral("pushButtonGotoPEriodState"));

        horizontalLayout_4->addWidget(pushButtonGotoPEriodState);

        pushBUttonToIdle = new QPushButton(widget_6);
        pushBUttonToIdle->setObjectName(QStringLiteral("pushBUttonToIdle"));

        horizontalLayout_4->addWidget(pushBUttonToIdle);

        pushButtonPosReset = new QPushButton(widget_6);
        pushButtonPosReset->setObjectName(QStringLiteral("pushButtonPosReset"));

        horizontalLayout_4->addWidget(pushButtonPosReset);


        verticalLayout_2->addWidget(widget_6);

        widget = new QWidget(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setMinimumSize(QSize(0, 150));
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

        groupBox = new QGroupBox(widget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setAlignment(Qt::AlignCenter);
        groupBox->setFlat(false);
        verticalLayout_4 = new QVBoxLayout(groupBox);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        widget_5 = new QWidget(groupBox);
        widget_5->setObjectName(QStringLiteral("widget_5"));
        horizontalLayout_6 = new QHBoxLayout(widget_5);
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        term1 = new QCheckBox(widget_5);
        term1->setObjectName(QStringLiteral("term1"));
        term1->setCheckable(true);

        horizontalLayout_6->addWidget(term1);

        term2 = new QCheckBox(widget_5);
        term2->setObjectName(QStringLiteral("term2"));
        term2->setCheckable(true);

        horizontalLayout_6->addWidget(term2);

        term3 = new QCheckBox(widget_5);
        term3->setObjectName(QStringLiteral("term3"));
        term3->setCheckable(true);

        horizontalLayout_6->addWidget(term3);

        term4 = new QCheckBox(widget_5);
        term4->setObjectName(QStringLiteral("term4"));
        term4->setCheckable(true);

        horizontalLayout_6->addWidget(term4);

        term5 = new QCheckBox(widget_5);
        term5->setObjectName(QStringLiteral("term5"));
        term5->setCheckable(true);

        horizontalLayout_6->addWidget(term5);


        verticalLayout_4->addWidget(widget_5);

        widget_9 = new QWidget(groupBox);
        widget_9->setObjectName(QStringLiteral("widget_9"));
        horizontalLayout_7 = new QHBoxLayout(widget_9);
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        term6 = new QCheckBox(widget_9);
        term6->setObjectName(QStringLiteral("term6"));
        term6->setCheckable(true);

        horizontalLayout_7->addWidget(term6);

        term7 = new QCheckBox(widget_9);
        term7->setObjectName(QStringLiteral("term7"));
        term7->setCheckable(true);

        horizontalLayout_7->addWidget(term7);

        term8 = new QCheckBox(widget_9);
        term8->setObjectName(QStringLiteral("term8"));
        term8->setCheckable(true);

        horizontalLayout_7->addWidget(term8);

        term9 = new QCheckBox(widget_9);
        term9->setObjectName(QStringLiteral("term9"));
        term9->setCheckable(true);

        horizontalLayout_7->addWidget(term9);

        term10 = new QCheckBox(widget_9);
        term10->setObjectName(QStringLiteral("term10"));
        term10->setCheckable(true);

        horizontalLayout_7->addWidget(term10);


        verticalLayout_4->addWidget(widget_9);


        horizontalLayout->addWidget(groupBox);


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

        tabWidget->setCurrentIndex(0);


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
        lineEditMaxVal->setInputMask(QApplication::translate("MainWindow", "99999", Q_NULLPTR));
        lineEditMaxVal->setText(QApplication::translate("MainWindow", "20000", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", "steps:", Q_NULLPTR));
        maxSteps->setText(QApplication::translate("MainWindow", "20000", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "\320\262\321\201\320\265", Q_NULLPTR));
        pushMoveDownState->setText(QApplication::translate("MainWindow", "moveDownState", Q_NULLPTR));
        pushTestData->setText(QApplication::translate("MainWindow", "testData", Q_NULLPTR));
        goToTerm->setText(QApplication::translate("MainWindow", "go term", Q_NULLPTR));
        pushButtonGotoPEriodState->setText(QApplication::translate("MainWindow", "to Period", Q_NULLPTR));
        pushBUttonToIdle->setText(QApplication::translate("MainWindow", "to idle", Q_NULLPTR));
        pushButtonPosReset->setText(QApplication::translate("MainWindow", "pos reset", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("MainWindow", "\320\232\320\276\320\275\321\206\320\265\320\262\320\270\320\272\320\270", Q_NULLPTR));
        term1->setText(QApplication::translate("MainWindow", "1", Q_NULLPTR));
        term2->setText(QApplication::translate("MainWindow", "2", Q_NULLPTR));
        term3->setText(QApplication::translate("MainWindow", "3", Q_NULLPTR));
        term4->setText(QApplication::translate("MainWindow", "4", Q_NULLPTR));
        term5->setText(QApplication::translate("MainWindow", "5", Q_NULLPTR));
        term6->setText(QApplication::translate("MainWindow", "6", Q_NULLPTR));
        term7->setText(QApplication::translate("MainWindow", "7", Q_NULLPTR));
        term8->setText(QApplication::translate("MainWindow", "8", Q_NULLPTR));
        term9->setText(QApplication::translate("MainWindow", "9", Q_NULLPTR));
        term10->setText(QApplication::translate("MainWindow", "10", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
