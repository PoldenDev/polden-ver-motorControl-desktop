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
#include <QtWidgets/QSlider>
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
    QWidget *tabTimeStat;
    QHBoxLayout *horizontalLayout_8;
    QWidget *widget_10;
    QVBoxLayout *verticalLayout_5;
    QSlider *timeShift_0;
    QLineEdit *lineEditTimeShift_0;
    QWidget *widget_11;
    QVBoxLayout *verticalLayout_6;
    QSlider *timeShift_1;
    QLineEdit *lineEditTimeShift_1;
    QWidget *widget_12;
    QVBoxLayout *verticalLayout_7;
    QSlider *timeShift_2;
    QLineEdit *lineEditTimeShift_2;
    QWidget *widget_13;
    QVBoxLayout *verticalLayout_8;
    QSlider *timeShift_3;
    QLineEdit *lineEditTimeShift_3;
    QWidget *widget_14;
    QVBoxLayout *verticalLayout_9;
    QSlider *timeShift_4;
    QLineEdit *lineEditTimeShift_4;
    QWidget *widget_15;
    QVBoxLayout *verticalLayout_10;
    QSlider *timeShift_5;
    QLineEdit *lineEditTimeShift_5;
    QWidget *widget_16;
    QVBoxLayout *verticalLayout_11;
    QSlider *timeShift_6;
    QLineEdit *lineEditTimeShift_6;
    QWidget *widget_18;
    QVBoxLayout *verticalLayout_13;
    QSlider *timeShift_8;
    QLineEdit *lineEditTimeShift_8;
    QWidget *widget_17;
    QVBoxLayout *verticalLayout_12;
    QSlider *timeShift_7;
    QLineEdit *lineEditTimeShift_7;
    QWidget *widget_19;
    QVBoxLayout *verticalLayout_14;
    QSlider *timeShift_9;
    QLineEdit *lineEditTimeShift_9;
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
    QPushButton *pushMoveDown;
    QPushButton *pushMoveUp;
    QPushButton *goToTerm;
    QCheckBox *checkBoxCycle;
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
        tabTimeStat = new QWidget();
        tabTimeStat->setObjectName(QStringLiteral("tabTimeStat"));
        horizontalLayout_8 = new QHBoxLayout(tabTimeStat);
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        widget_10 = new QWidget(tabTimeStat);
        widget_10->setObjectName(QStringLiteral("widget_10"));
        verticalLayout_5 = new QVBoxLayout(widget_10);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        timeShift_0 = new QSlider(widget_10);
        timeShift_0->setObjectName(QStringLiteral("timeShift_0"));
        timeShift_0->setMinimum(-1999);
        timeShift_0->setMaximum(1999);
        timeShift_0->setTracking(true);
        timeShift_0->setOrientation(Qt::Vertical);
        timeShift_0->setTickPosition(QSlider::TicksBothSides);
        timeShift_0->setTickInterval(500);

        verticalLayout_5->addWidget(timeShift_0);

        lineEditTimeShift_0 = new QLineEdit(widget_10);
        lineEditTimeShift_0->setObjectName(QStringLiteral("lineEditTimeShift_0"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lineEditTimeShift_0->sizePolicy().hasHeightForWidth());
        lineEditTimeShift_0->setSizePolicy(sizePolicy);
        lineEditTimeShift_0->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(lineEditTimeShift_0);


        horizontalLayout_8->addWidget(widget_10, 0, Qt::AlignHCenter);

        widget_11 = new QWidget(tabTimeStat);
        widget_11->setObjectName(QStringLiteral("widget_11"));
        verticalLayout_6 = new QVBoxLayout(widget_11);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        timeShift_1 = new QSlider(widget_11);
        timeShift_1->setObjectName(QStringLiteral("timeShift_1"));
        timeShift_1->setMinimum(-1999);
        timeShift_1->setMaximum(1999);
        timeShift_1->setTracking(true);
        timeShift_1->setOrientation(Qt::Vertical);
        timeShift_1->setTickPosition(QSlider::TicksBothSides);
        timeShift_1->setTickInterval(500);

        verticalLayout_6->addWidget(timeShift_1);

        lineEditTimeShift_1 = new QLineEdit(widget_11);
        lineEditTimeShift_1->setObjectName(QStringLiteral("lineEditTimeShift_1"));
        lineEditTimeShift_1->setAlignment(Qt::AlignCenter);

        verticalLayout_6->addWidget(lineEditTimeShift_1);


        horizontalLayout_8->addWidget(widget_11, 0, Qt::AlignHCenter);

        widget_12 = new QWidget(tabTimeStat);
        widget_12->setObjectName(QStringLiteral("widget_12"));
        verticalLayout_7 = new QVBoxLayout(widget_12);
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        timeShift_2 = new QSlider(widget_12);
        timeShift_2->setObjectName(QStringLiteral("timeShift_2"));
        timeShift_2->setMinimum(-1999);
        timeShift_2->setMaximum(1999);
        timeShift_2->setTracking(true);
        timeShift_2->setOrientation(Qt::Vertical);
        timeShift_2->setTickPosition(QSlider::TicksBothSides);
        timeShift_2->setTickInterval(500);

        verticalLayout_7->addWidget(timeShift_2);

        lineEditTimeShift_2 = new QLineEdit(widget_12);
        lineEditTimeShift_2->setObjectName(QStringLiteral("lineEditTimeShift_2"));
        lineEditTimeShift_2->setAlignment(Qt::AlignCenter);

        verticalLayout_7->addWidget(lineEditTimeShift_2);


        horizontalLayout_8->addWidget(widget_12);

        widget_13 = new QWidget(tabTimeStat);
        widget_13->setObjectName(QStringLiteral("widget_13"));
        verticalLayout_8 = new QVBoxLayout(widget_13);
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setContentsMargins(11, 11, 11, 11);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        timeShift_3 = new QSlider(widget_13);
        timeShift_3->setObjectName(QStringLiteral("timeShift_3"));
        timeShift_3->setMinimum(-1999);
        timeShift_3->setMaximum(1999);
        timeShift_3->setTracking(true);
        timeShift_3->setOrientation(Qt::Vertical);
        timeShift_3->setTickPosition(QSlider::TicksBothSides);
        timeShift_3->setTickInterval(500);

        verticalLayout_8->addWidget(timeShift_3);

        lineEditTimeShift_3 = new QLineEdit(widget_13);
        lineEditTimeShift_3->setObjectName(QStringLiteral("lineEditTimeShift_3"));
        lineEditTimeShift_3->setAlignment(Qt::AlignCenter);

        verticalLayout_8->addWidget(lineEditTimeShift_3);


        horizontalLayout_8->addWidget(widget_13);

        widget_14 = new QWidget(tabTimeStat);
        widget_14->setObjectName(QStringLiteral("widget_14"));
        verticalLayout_9 = new QVBoxLayout(widget_14);
        verticalLayout_9->setSpacing(6);
        verticalLayout_9->setContentsMargins(11, 11, 11, 11);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        timeShift_4 = new QSlider(widget_14);
        timeShift_4->setObjectName(QStringLiteral("timeShift_4"));
        timeShift_4->setMinimum(-1999);
        timeShift_4->setMaximum(1999);
        timeShift_4->setTracking(true);
        timeShift_4->setOrientation(Qt::Vertical);
        timeShift_4->setTickPosition(QSlider::TicksBothSides);
        timeShift_4->setTickInterval(500);

        verticalLayout_9->addWidget(timeShift_4);

        lineEditTimeShift_4 = new QLineEdit(widget_14);
        lineEditTimeShift_4->setObjectName(QStringLiteral("lineEditTimeShift_4"));
        lineEditTimeShift_4->setAlignment(Qt::AlignCenter);

        verticalLayout_9->addWidget(lineEditTimeShift_4);


        horizontalLayout_8->addWidget(widget_14);

        widget_15 = new QWidget(tabTimeStat);
        widget_15->setObjectName(QStringLiteral("widget_15"));
        verticalLayout_10 = new QVBoxLayout(widget_15);
        verticalLayout_10->setSpacing(6);
        verticalLayout_10->setContentsMargins(11, 11, 11, 11);
        verticalLayout_10->setObjectName(QStringLiteral("verticalLayout_10"));
        timeShift_5 = new QSlider(widget_15);
        timeShift_5->setObjectName(QStringLiteral("timeShift_5"));
        timeShift_5->setMinimum(-1999);
        timeShift_5->setMaximum(1999);
        timeShift_5->setTracking(true);
        timeShift_5->setOrientation(Qt::Vertical);
        timeShift_5->setTickPosition(QSlider::TicksBothSides);
        timeShift_5->setTickInterval(500);

        verticalLayout_10->addWidget(timeShift_5);

        lineEditTimeShift_5 = new QLineEdit(widget_15);
        lineEditTimeShift_5->setObjectName(QStringLiteral("lineEditTimeShift_5"));
        lineEditTimeShift_5->setAlignment(Qt::AlignCenter);

        verticalLayout_10->addWidget(lineEditTimeShift_5);


        horizontalLayout_8->addWidget(widget_15);

        widget_16 = new QWidget(tabTimeStat);
        widget_16->setObjectName(QStringLiteral("widget_16"));
        verticalLayout_11 = new QVBoxLayout(widget_16);
        verticalLayout_11->setSpacing(6);
        verticalLayout_11->setContentsMargins(11, 11, 11, 11);
        verticalLayout_11->setObjectName(QStringLiteral("verticalLayout_11"));
        timeShift_6 = new QSlider(widget_16);
        timeShift_6->setObjectName(QStringLiteral("timeShift_6"));
        timeShift_6->setMinimum(-1999);
        timeShift_6->setMaximum(1999);
        timeShift_6->setTracking(true);
        timeShift_6->setOrientation(Qt::Vertical);
        timeShift_6->setTickPosition(QSlider::TicksBothSides);
        timeShift_6->setTickInterval(500);

        verticalLayout_11->addWidget(timeShift_6);

        lineEditTimeShift_6 = new QLineEdit(widget_16);
        lineEditTimeShift_6->setObjectName(QStringLiteral("lineEditTimeShift_6"));
        lineEditTimeShift_6->setAlignment(Qt::AlignCenter);

        verticalLayout_11->addWidget(lineEditTimeShift_6);


        horizontalLayout_8->addWidget(widget_16);

        widget_18 = new QWidget(tabTimeStat);
        widget_18->setObjectName(QStringLiteral("widget_18"));
        verticalLayout_13 = new QVBoxLayout(widget_18);
        verticalLayout_13->setSpacing(6);
        verticalLayout_13->setContentsMargins(11, 11, 11, 11);
        verticalLayout_13->setObjectName(QStringLiteral("verticalLayout_13"));
        timeShift_8 = new QSlider(widget_18);
        timeShift_8->setObjectName(QStringLiteral("timeShift_8"));
        timeShift_8->setMinimum(-1999);
        timeShift_8->setMaximum(1999);
        timeShift_8->setTracking(true);
        timeShift_8->setOrientation(Qt::Vertical);
        timeShift_8->setTickPosition(QSlider::TicksBothSides);
        timeShift_8->setTickInterval(500);

        verticalLayout_13->addWidget(timeShift_8);

        lineEditTimeShift_8 = new QLineEdit(widget_18);
        lineEditTimeShift_8->setObjectName(QStringLiteral("lineEditTimeShift_8"));
        lineEditTimeShift_8->setAlignment(Qt::AlignCenter);

        verticalLayout_13->addWidget(lineEditTimeShift_8);


        horizontalLayout_8->addWidget(widget_18);

        widget_17 = new QWidget(tabTimeStat);
        widget_17->setObjectName(QStringLiteral("widget_17"));
        verticalLayout_12 = new QVBoxLayout(widget_17);
        verticalLayout_12->setSpacing(6);
        verticalLayout_12->setContentsMargins(11, 11, 11, 11);
        verticalLayout_12->setObjectName(QStringLiteral("verticalLayout_12"));
        timeShift_7 = new QSlider(widget_17);
        timeShift_7->setObjectName(QStringLiteral("timeShift_7"));
        timeShift_7->setMinimum(-1999);
        timeShift_7->setMaximum(1999);
        timeShift_7->setTracking(true);
        timeShift_7->setOrientation(Qt::Vertical);
        timeShift_7->setTickPosition(QSlider::TicksBothSides);
        timeShift_7->setTickInterval(500);

        verticalLayout_12->addWidget(timeShift_7);

        lineEditTimeShift_7 = new QLineEdit(widget_17);
        lineEditTimeShift_7->setObjectName(QStringLiteral("lineEditTimeShift_7"));
        lineEditTimeShift_7->setAlignment(Qt::AlignCenter);

        verticalLayout_12->addWidget(lineEditTimeShift_7);


        horizontalLayout_8->addWidget(widget_17);

        widget_19 = new QWidget(tabTimeStat);
        widget_19->setObjectName(QStringLiteral("widget_19"));
        verticalLayout_14 = new QVBoxLayout(widget_19);
        verticalLayout_14->setSpacing(6);
        verticalLayout_14->setContentsMargins(11, 11, 11, 11);
        verticalLayout_14->setObjectName(QStringLiteral("verticalLayout_14"));
        timeShift_9 = new QSlider(widget_19);
        timeShift_9->setObjectName(QStringLiteral("timeShift_9"));
        timeShift_9->setMinimum(-1999);
        timeShift_9->setMaximum(1999);
        timeShift_9->setTracking(true);
        timeShift_9->setOrientation(Qt::Vertical);
        timeShift_9->setTickPosition(QSlider::TicksBothSides);
        timeShift_9->setTickInterval(500);

        verticalLayout_14->addWidget(timeShift_9);

        lineEditTimeShift_9 = new QLineEdit(widget_19);
        lineEditTimeShift_9->setObjectName(QStringLiteral("lineEditTimeShift_9"));
        lineEditTimeShift_9->setAlignment(Qt::AlignCenter);

        verticalLayout_14->addWidget(lineEditTimeShift_9);


        horizontalLayout_8->addWidget(widget_19);

        tabWidget->addTab(tabTimeStat, QString());
        timeShift_2->raise();
        widget_10->raise();
        widget_11->raise();
        widget_12->raise();
        widget_13->raise();
        widget_14->raise();
        widget_15->raise();
        widget_16->raise();
        widget_18->raise();
        widget_17->raise();
        widget_19->raise();
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
        pushMoveDown = new QPushButton(widget_6);
        pushMoveDown->setObjectName(QStringLiteral("pushMoveDown"));

        horizontalLayout_4->addWidget(pushMoveDown);

        pushMoveUp = new QPushButton(widget_6);
        pushMoveUp->setObjectName(QStringLiteral("pushMoveUp"));

        horizontalLayout_4->addWidget(pushMoveUp);

        goToTerm = new QPushButton(widget_6);
        goToTerm->setObjectName(QStringLiteral("goToTerm"));

        horizontalLayout_4->addWidget(goToTerm);

        checkBoxCycle = new QCheckBox(widget_6);
        checkBoxCycle->setObjectName(QStringLiteral("checkBoxCycle"));

        horizontalLayout_4->addWidget(checkBoxCycle);

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
        maxSteps->setText(QApplication::translate("MainWindow", "200000", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tabTimeStat), QApplication::translate("MainWindow", "timeStat", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "\320\262\321\201\320\265", Q_NULLPTR));
        pushMoveDown->setText(QApplication::translate("MainWindow", "moveDown", Q_NULLPTR));
        pushMoveUp->setText(QApplication::translate("MainWindow", "moveUp", Q_NULLPTR));
        goToTerm->setText(QApplication::translate("MainWindow", "go term", Q_NULLPTR));
        checkBoxCycle->setText(QApplication::translate("MainWindow", "cycle", Q_NULLPTR));
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
