/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
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
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
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
    QComboBox *comComboBoxUsbMain;
    QPushButton *pushButtonComOpen;
    QPushButton *pushButton_refreshCom;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonUdpOpenClose;
    QTabWidget *tabWidget;
    QWidget *tabMainStat;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_15;
    QWidget *widget_20;
    QHBoxLayout *horizontalLayout_13;
    QLineEdit *lineEditUSBConnectionTime;
    QWidget *widget_22;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_4;
    QLineEdit *lineEditComExchanges;
    QWidget *widget_23;
    QHBoxLayout *horizontalLayout_14;
    QLabel *label_5;
    QLineEdit *lineEditBytesOnIter;
    QWidget *widget_24;
    QHBoxLayout *horizontalLayout_15;
    QLabel *label_14;
    QLineEdit *lineEditRecvInterval;
    QGroupBox *groupBoxMain;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_3;
    QWidget *widget_28;
    QHBoxLayout *horizontalLayout_19;
    QLabel *label_7;
    QLineEdit *lineEditUdpState;
    QLineEdit *lineEditUDPConnectionTime;
    QWidget *widget_26;
    QHBoxLayout *horizontalLayout_17;
    QLabel *label_6;
    QLineEdit *lineEditDataGramCnt;
    QCheckBox *checkBoxSliderPosCtrl;
    QWidget *tabSettings;
    QGroupBox *groupBox_5;
    QLabel *label_8;
    QLineEdit *lineEdit_ImpPerRot;
    QLabel *label_9;
    QLineEdit *lineEdit_mmPerRot;
    QLineEdit *lineEdit_maxHeightMM;
    QLabel *label_10;
    QLineEdit *lineEdit_MaxHeightImp;
    QLabel *label_11;
    QLabel *label_12;
    QLineEdit *lineEdit_vmax_mmsec;
    QGroupBox *groupBox_6;
    QRadioButton *radioButtonFpgaFreq24;
    QRadioButton *radioButtonFpgaFreq25;
    QLabel *label_3;
    QLineEdit *lineEditMotorCount;
    QCheckBox *checkBoxDirInverse;
    QGroupBox *groupBox_7;
    QLineEdit *lineEditMinVal;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *lineEditUDPMaxVal;
    QCheckBox *checkBoxPrintUDPData;
    QGroupBox *groupBox_8;
    QLabel *label_13;
    QLabel *labelCompileTime;
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
    QWidget *tabDebugPort;
    QWidget *widgetComPorts;
    QWidget *tabSonoff;
    QTableWidget *tableWidgetSonOffDevices;
    QLineEdit *lineEditSSID;
    QLabel *label_15;
    QLineEdit *lineEditKey;
    QLineEdit *lineEditServerIp;
    QLabel *label_17;
    QLabel *label_16;
    QPushButton *pushButtonSonoffAPSet;
    QWidget *widget_27;
    QHBoxLayout *horizontalLayout_18;
    QPushButton *pushButtonInitiate;
    QWidget *widget_6;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *pushMoveDown;
    QPushButton *pushMoveUp;
    QPushButton *goToTerm;
    QPushButton *pushButtonGoZero;
    QPushButton *pushButtonTest;
    QPushButton *pushButtonTest1;
    QPushButton *pushButtonTest2;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButtonU1;
    QPushButton *pushButtonD1;
    QPushButton *pushButtonU2;
    QPushButton *pushButtonD2;
    QPushButton *pushButtonU3;
    QPushButton *pushButtonD3;
    QWidget *widget_29;
    QHBoxLayout *horizontalLayout_20;
    QPushButton *pushButtonPosReset;
    QPushButton *pushBUttonToIdle;
    QPushButton *pushButtonClear;
    QPushButton *pushButtonGotoPEriodState;
    QCheckBox *checkBoxDebugComPrint;
    QCheckBox *checkBoxCycle;
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
        MainWindow->resize(816, 739);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        widget_4 = new QWidget(centralWidget);
        widget_4->setObjectName(QStringLiteral("widget_4"));
        widget_4->setMaximumSize(QSize(500, 16777215));
        horizontalLayout_3 = new QHBoxLayout(widget_4);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, -1, 0);
        comComboBoxUsbMain = new QComboBox(widget_4);
        comComboBoxUsbMain->setObjectName(QStringLiteral("comComboBoxUsbMain"));
        comComboBoxUsbMain->setMinimumSize(QSize(100, 0));

        horizontalLayout_3->addWidget(comComboBoxUsbMain);

        pushButtonComOpen = new QPushButton(widget_4);
        pushButtonComOpen->setObjectName(QStringLiteral("pushButtonComOpen"));

        horizontalLayout_3->addWidget(pushButtonComOpen);

        pushButton_refreshCom = new QPushButton(widget_4);
        pushButton_refreshCom->setObjectName(QStringLiteral("pushButton_refreshCom"));

        horizontalLayout_3->addWidget(pushButton_refreshCom);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        pushButtonUdpOpenClose = new QPushButton(widget_4);
        pushButtonUdpOpenClose->setObjectName(QStringLiteral("pushButtonUdpOpenClose"));

        horizontalLayout_3->addWidget(pushButtonUdpOpenClose);


        verticalLayout_2->addWidget(widget_4);

        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setTabPosition(QTabWidget::North);
        tabMainStat = new QWidget();
        tabMainStat->setObjectName(QStringLiteral("tabMainStat"));
        groupBox_2 = new QGroupBox(tabMainStat);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(600, 30, 153, 121));
        verticalLayout_15 = new QVBoxLayout(groupBox_2);
        verticalLayout_15->setSpacing(6);
        verticalLayout_15->setContentsMargins(11, 11, 11, 11);
        verticalLayout_15->setObjectName(QStringLiteral("verticalLayout_15"));
        widget_20 = new QWidget(groupBox_2);
        widget_20->setObjectName(QStringLiteral("widget_20"));
        horizontalLayout_13 = new QHBoxLayout(widget_20);
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        horizontalLayout_13->setContentsMargins(0, 0, 0, 0);
        lineEditUSBConnectionTime = new QLineEdit(widget_20);
        lineEditUSBConnectionTime->setObjectName(QStringLiteral("lineEditUSBConnectionTime"));
        lineEditUSBConnectionTime->setAlignment(Qt::AlignCenter);
        lineEditUSBConnectionTime->setReadOnly(true);

        horizontalLayout_13->addWidget(lineEditUSBConnectionTime);


        verticalLayout_15->addWidget(widget_20);

        widget_22 = new QWidget(groupBox_2);
        widget_22->setObjectName(QStringLiteral("widget_22"));
        horizontalLayout_12 = new QHBoxLayout(widget_22);
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        horizontalLayout_12->setContentsMargins(0, 0, 0, 0);
        label_4 = new QLabel(widget_22);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_12->addWidget(label_4);

        lineEditComExchanges = new QLineEdit(widget_22);
        lineEditComExchanges->setObjectName(QStringLiteral("lineEditComExchanges"));
        lineEditComExchanges->setAlignment(Qt::AlignCenter);
        lineEditComExchanges->setReadOnly(true);

        horizontalLayout_12->addWidget(lineEditComExchanges);


        verticalLayout_15->addWidget(widget_22);

        widget_23 = new QWidget(groupBox_2);
        widget_23->setObjectName(QStringLiteral("widget_23"));
        horizontalLayout_14 = new QHBoxLayout(widget_23);
        horizontalLayout_14->setSpacing(6);
        horizontalLayout_14->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_14->setObjectName(QStringLiteral("horizontalLayout_14"));
        horizontalLayout_14->setContentsMargins(0, 0, 0, 0);
        label_5 = new QLabel(widget_23);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_14->addWidget(label_5);

        lineEditBytesOnIter = new QLineEdit(widget_23);
        lineEditBytesOnIter->setObjectName(QStringLiteral("lineEditBytesOnIter"));
        lineEditBytesOnIter->setAlignment(Qt::AlignCenter);
        lineEditBytesOnIter->setReadOnly(true);

        horizontalLayout_14->addWidget(lineEditBytesOnIter);


        verticalLayout_15->addWidget(widget_23);

        widget_24 = new QWidget(groupBox_2);
        widget_24->setObjectName(QStringLiteral("widget_24"));
        horizontalLayout_15 = new QHBoxLayout(widget_24);
        horizontalLayout_15->setSpacing(6);
        horizontalLayout_15->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_15->setObjectName(QStringLiteral("horizontalLayout_15"));
        horizontalLayout_15->setContentsMargins(0, 0, 0, 0);
        label_14 = new QLabel(widget_24);
        label_14->setObjectName(QStringLiteral("label_14"));

        horizontalLayout_15->addWidget(label_14);

        lineEditRecvInterval = new QLineEdit(widget_24);
        lineEditRecvInterval->setObjectName(QStringLiteral("lineEditRecvInterval"));
        lineEditRecvInterval->setAlignment(Qt::AlignCenter);
        lineEditRecvInterval->setReadOnly(true);

        horizontalLayout_15->addWidget(lineEditRecvInterval);


        verticalLayout_15->addWidget(widget_24);

        groupBoxMain = new QGroupBox(tabMainStat);
        groupBoxMain->setObjectName(QStringLiteral("groupBoxMain"));
        groupBoxMain->setGeometry(QRect(10, 10, 571, 301));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBoxMain->sizePolicy().hasHeightForWidth());
        groupBoxMain->setSizePolicy(sizePolicy);
        groupBox_4 = new QGroupBox(tabMainStat);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(600, 170, 151, 111));
        verticalLayout_3 = new QVBoxLayout(groupBox_4);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        widget_28 = new QWidget(groupBox_4);
        widget_28->setObjectName(QStringLiteral("widget_28"));
        horizontalLayout_19 = new QHBoxLayout(widget_28);
        horizontalLayout_19->setSpacing(6);
        horizontalLayout_19->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_19->setObjectName(QStringLiteral("horizontalLayout_19"));
        horizontalLayout_19->setContentsMargins(-1, 0, -1, 0);
        label_7 = new QLabel(widget_28);
        label_7->setObjectName(QStringLiteral("label_7"));

        horizontalLayout_19->addWidget(label_7);

        lineEditUdpState = new QLineEdit(widget_28);
        lineEditUdpState->setObjectName(QStringLiteral("lineEditUdpState"));
        lineEditUdpState->setAlignment(Qt::AlignCenter);
        lineEditUdpState->setReadOnly(true);

        horizontalLayout_19->addWidget(lineEditUdpState);


        verticalLayout_3->addWidget(widget_28);

        lineEditUDPConnectionTime = new QLineEdit(groupBox_4);
        lineEditUDPConnectionTime->setObjectName(QStringLiteral("lineEditUDPConnectionTime"));
        lineEditUDPConnectionTime->setAlignment(Qt::AlignCenter);
        lineEditUDPConnectionTime->setReadOnly(true);

        verticalLayout_3->addWidget(lineEditUDPConnectionTime);

        widget_26 = new QWidget(groupBox_4);
        widget_26->setObjectName(QStringLiteral("widget_26"));
        horizontalLayout_17 = new QHBoxLayout(widget_26);
        horizontalLayout_17->setSpacing(6);
        horizontalLayout_17->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_17->setObjectName(QStringLiteral("horizontalLayout_17"));
        horizontalLayout_17->setContentsMargins(-1, 0, -1, 0);
        label_6 = new QLabel(widget_26);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_17->addWidget(label_6);

        lineEditDataGramCnt = new QLineEdit(widget_26);
        lineEditDataGramCnt->setObjectName(QStringLiteral("lineEditDataGramCnt"));
        lineEditDataGramCnt->setAlignment(Qt::AlignCenter);
        lineEditDataGramCnt->setReadOnly(true);

        horizontalLayout_17->addWidget(lineEditDataGramCnt);


        verticalLayout_3->addWidget(widget_26);

        checkBoxSliderPosCtrl = new QCheckBox(tabMainStat);
        checkBoxSliderPosCtrl->setObjectName(QStringLiteral("checkBoxSliderPosCtrl"));
        checkBoxSliderPosCtrl->setGeometry(QRect(600, 280, 70, 17));
        tabWidget->addTab(tabMainStat, QString());
        tabSettings = new QWidget();
        tabSettings->setObjectName(QStringLiteral("tabSettings"));
        groupBox_5 = new QGroupBox(tabSettings);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        groupBox_5->setGeometry(QRect(20, 20, 211, 171));
        label_8 = new QLabel(groupBox_5);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(10, 20, 47, 13));
        lineEdit_ImpPerRot = new QLineEdit(groupBox_5);
        lineEdit_ImpPerRot->setObjectName(QStringLiteral("lineEdit_ImpPerRot"));
        lineEdit_ImpPerRot->setGeometry(QRect(110, 20, 81, 20));
        lineEdit_ImpPerRot->setAlignment(Qt::AlignCenter);
        label_9 = new QLabel(groupBox_5);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(10, 50, 47, 13));
        lineEdit_mmPerRot = new QLineEdit(groupBox_5);
        lineEdit_mmPerRot->setObjectName(QStringLiteral("lineEdit_mmPerRot"));
        lineEdit_mmPerRot->setGeometry(QRect(110, 50, 81, 20));
        lineEdit_mmPerRot->setAlignment(Qt::AlignCenter);
        lineEdit_maxHeightMM = new QLineEdit(groupBox_5);
        lineEdit_maxHeightMM->setObjectName(QStringLiteral("lineEdit_maxHeightMM"));
        lineEdit_maxHeightMM->setGeometry(QRect(110, 80, 81, 20));
        lineEdit_maxHeightMM->setAlignment(Qt::AlignCenter);
        label_10 = new QLabel(groupBox_5);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(10, 80, 91, 16));
        lineEdit_MaxHeightImp = new QLineEdit(groupBox_5);
        lineEdit_MaxHeightImp->setObjectName(QStringLiteral("lineEdit_MaxHeightImp"));
        lineEdit_MaxHeightImp->setGeometry(QRect(110, 110, 81, 20));
        lineEdit_MaxHeightImp->setAlignment(Qt::AlignCenter);
        label_11 = new QLabel(groupBox_5);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(10, 110, 91, 16));
        label_12 = new QLabel(groupBox_5);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(10, 140, 91, 16));
        lineEdit_vmax_mmsec = new QLineEdit(groupBox_5);
        lineEdit_vmax_mmsec->setObjectName(QStringLiteral("lineEdit_vmax_mmsec"));
        lineEdit_vmax_mmsec->setGeometry(QRect(110, 140, 81, 20));
        lineEdit_vmax_mmsec->setAlignment(Qt::AlignCenter);
        groupBox_6 = new QGroupBox(tabSettings);
        groupBox_6->setObjectName(QStringLiteral("groupBox_6"));
        groupBox_6->setGeometry(QRect(260, 20, 151, 121));
        radioButtonFpgaFreq24 = new QRadioButton(groupBox_6);
        radioButtonFpgaFreq24->setObjectName(QStringLiteral("radioButtonFpgaFreq24"));
        radioButtonFpgaFreq24->setGeometry(QRect(10, 70, 91, 17));
        radioButtonFpgaFreq25 = new QRadioButton(groupBox_6);
        radioButtonFpgaFreq25->setObjectName(QStringLiteral("radioButtonFpgaFreq25"));
        radioButtonFpgaFreq25->setGeometry(QRect(10, 90, 91, 17));
        radioButtonFpgaFreq25->setChecked(true);
        label_3 = new QLabel(groupBox_6);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 20, 91, 16));
        lineEditMotorCount = new QLineEdit(groupBox_6);
        lineEditMotorCount->setObjectName(QStringLiteral("lineEditMotorCount"));
        lineEditMotorCount->setGeometry(QRect(100, 20, 41, 20));
        lineEditMotorCount->setAlignment(Qt::AlignCenter);
        checkBoxDirInverse = new QCheckBox(groupBox_6);
        checkBoxDirInverse->setObjectName(QStringLiteral("checkBoxDirInverse"));
        checkBoxDirInverse->setGeometry(QRect(10, 50, 121, 17));
        checkBoxDirInverse->setLayoutDirection(Qt::LeftToRight);
        groupBox_7 = new QGroupBox(tabSettings);
        groupBox_7->setObjectName(QStringLiteral("groupBox_7"));
        groupBox_7->setGeometry(QRect(20, 200, 141, 111));
        lineEditMinVal = new QLineEdit(groupBox_7);
        lineEditMinVal->setObjectName(QStringLiteral("lineEditMinVal"));
        lineEditMinVal->setGeometry(QRect(50, 20, 71, 20));
        lineEditMinVal->setAlignment(Qt::AlignCenter);
        label = new QLabel(groupBox_7);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 20, 20, 23));
        label_2 = new QLabel(groupBox_7);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 50, 20, 21));
        lineEditUDPMaxVal = new QLineEdit(groupBox_7);
        lineEditUDPMaxVal->setObjectName(QStringLiteral("lineEditUDPMaxVal"));
        lineEditUDPMaxVal->setGeometry(QRect(50, 51, 71, 20));
        lineEditUDPMaxVal->setAlignment(Qt::AlignCenter);
        checkBoxPrintUDPData = new QCheckBox(groupBox_7);
        checkBoxPrintUDPData->setObjectName(QStringLiteral("checkBoxPrintUDPData"));
        checkBoxPrintUDPData->setGeometry(QRect(20, 80, 61, 20));
        groupBox_8 = new QGroupBox(tabSettings);
        groupBox_8->setObjectName(QStringLiteral("groupBox_8"));
        groupBox_8->setGeometry(QRect(240, 190, 151, 51));
        label_13 = new QLabel(groupBox_8);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(10, 20, 71, 16));
        labelCompileTime = new QLabel(groupBox_8);
        labelCompileTime->setObjectName(QStringLiteral("labelCompileTime"));
        labelCompileTime->setGeometry(QRect(80, 20, 101, 16));
        groupBox = new QGroupBox(tabSettings);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(470, 150, 213, 141));
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

        tabWidget->addTab(tabSettings, QString());
        tabDebugPort = new QWidget();
        tabDebugPort->setObjectName(QStringLiteral("tabDebugPort"));
        widgetComPorts = new QWidget(tabDebugPort);
        widgetComPorts->setObjectName(QStringLiteral("widgetComPorts"));
        widgetComPorts->setGeometry(QRect(80, 70, 671, 201));
        tabWidget->addTab(tabDebugPort, QString());
        tabSonoff = new QWidget();
        tabSonoff->setObjectName(QStringLiteral("tabSonoff"));
        tableWidgetSonOffDevices = new QTableWidget(tabSonoff);
        tableWidgetSonOffDevices->setObjectName(QStringLiteral("tableWidgetSonOffDevices"));
        tableWidgetSonOffDevices->setGeometry(QRect(10, 10, 481, 191));
        tableWidgetSonOffDevices->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        lineEditSSID = new QLineEdit(tabSonoff);
        lineEditSSID->setObjectName(QStringLiteral("lineEditSSID"));
        lineEditSSID->setGeometry(QRect(580, 30, 121, 20));
        label_15 = new QLabel(tabSonoff);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(540, 30, 27, 23));
        lineEditKey = new QLineEdit(tabSonoff);
        lineEditKey->setObjectName(QStringLiteral("lineEditKey"));
        lineEditKey->setGeometry(QRect(580, 60, 121, 20));
        lineEditServerIp = new QLineEdit(tabSonoff);
        lineEditServerIp->setObjectName(QStringLiteral("lineEditServerIp"));
        lineEditServerIp->setGeometry(QRect(580, 90, 121, 20));
        label_17 = new QLabel(tabSonoff);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setGeometry(QRect(540, 90, 41, 23));
        label_16 = new QLabel(tabSonoff);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(540, 60, 23, 23));
        pushButtonSonoffAPSet = new QPushButton(tabSonoff);
        pushButtonSonoffAPSet->setObjectName(QStringLiteral("pushButtonSonoffAPSet"));
        pushButtonSonoffAPSet->setGeometry(QRect(600, 120, 75, 23));
        tabWidget->addTab(tabSonoff, QString());

        verticalLayout_2->addWidget(tabWidget);

        widget_27 = new QWidget(centralWidget);
        widget_27->setObjectName(QStringLiteral("widget_27"));
        widget_27->setMaximumSize(QSize(16777215, 16777215));
        horizontalLayout_18 = new QHBoxLayout(widget_27);
        horizontalLayout_18->setSpacing(6);
        horizontalLayout_18->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_18->setObjectName(QStringLiteral("horizontalLayout_18"));
        pushButtonInitiate = new QPushButton(widget_27);
        pushButtonInitiate->setObjectName(QStringLiteral("pushButtonInitiate"));
        pushButtonInitiate->setMaximumSize(QSize(200, 16777215));

        horizontalLayout_18->addWidget(pushButtonInitiate);


        verticalLayout_2->addWidget(widget_27);

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

        pushButtonGoZero = new QPushButton(widget_6);
        pushButtonGoZero->setObjectName(QStringLiteral("pushButtonGoZero"));

        horizontalLayout_4->addWidget(pushButtonGoZero);

        pushButtonTest = new QPushButton(widget_6);
        pushButtonTest->setObjectName(QStringLiteral("pushButtonTest"));

        horizontalLayout_4->addWidget(pushButtonTest);

        pushButtonTest1 = new QPushButton(widget_6);
        pushButtonTest1->setObjectName(QStringLiteral("pushButtonTest1"));

        horizontalLayout_4->addWidget(pushButtonTest1);

        pushButtonTest2 = new QPushButton(widget_6);
        pushButtonTest2->setObjectName(QStringLiteral("pushButtonTest2"));

        horizontalLayout_4->addWidget(pushButtonTest2);


        verticalLayout_2->addWidget(widget_6);

        widget_2 = new QWidget(centralWidget);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        horizontalLayout_2 = new QHBoxLayout(widget_2);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        pushButtonU1 = new QPushButton(widget_2);
        pushButtonU1->setObjectName(QStringLiteral("pushButtonU1"));

        horizontalLayout_2->addWidget(pushButtonU1);

        pushButtonD1 = new QPushButton(widget_2);
        pushButtonD1->setObjectName(QStringLiteral("pushButtonD1"));

        horizontalLayout_2->addWidget(pushButtonD1);

        pushButtonU2 = new QPushButton(widget_2);
        pushButtonU2->setObjectName(QStringLiteral("pushButtonU2"));

        horizontalLayout_2->addWidget(pushButtonU2);

        pushButtonD2 = new QPushButton(widget_2);
        pushButtonD2->setObjectName(QStringLiteral("pushButtonD2"));

        horizontalLayout_2->addWidget(pushButtonD2);

        pushButtonU3 = new QPushButton(widget_2);
        pushButtonU3->setObjectName(QStringLiteral("pushButtonU3"));

        horizontalLayout_2->addWidget(pushButtonU3);

        pushButtonD3 = new QPushButton(widget_2);
        pushButtonD3->setObjectName(QStringLiteral("pushButtonD3"));

        horizontalLayout_2->addWidget(pushButtonD3);

        pushButtonU1->raise();
        pushButtonGoZero->raise();
        pushButtonD1->raise();
        pushButtonU2->raise();
        pushButtonD2->raise();
        pushButtonU3->raise();
        pushButtonD3->raise();

        verticalLayout_2->addWidget(widget_2);

        widget_29 = new QWidget(centralWidget);
        widget_29->setObjectName(QStringLiteral("widget_29"));
        horizontalLayout_20 = new QHBoxLayout(widget_29);
        horizontalLayout_20->setSpacing(6);
        horizontalLayout_20->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_20->setObjectName(QStringLiteral("horizontalLayout_20"));
        pushButtonPosReset = new QPushButton(widget_29);
        pushButtonPosReset->setObjectName(QStringLiteral("pushButtonPosReset"));

        horizontalLayout_20->addWidget(pushButtonPosReset);

        pushBUttonToIdle = new QPushButton(widget_29);
        pushBUttonToIdle->setObjectName(QStringLiteral("pushBUttonToIdle"));

        horizontalLayout_20->addWidget(pushBUttonToIdle);

        pushButtonClear = new QPushButton(widget_29);
        pushButtonClear->setObjectName(QStringLiteral("pushButtonClear"));

        horizontalLayout_20->addWidget(pushButtonClear);

        pushButtonGotoPEriodState = new QPushButton(widget_29);
        pushButtonGotoPEriodState->setObjectName(QStringLiteral("pushButtonGotoPEriodState"));

        horizontalLayout_20->addWidget(pushButtonGotoPEriodState);

        checkBoxDebugComPrint = new QCheckBox(widget_29);
        checkBoxDebugComPrint->setObjectName(QStringLiteral("checkBoxDebugComPrint"));

        horizontalLayout_20->addWidget(checkBoxDebugComPrint);

        checkBoxCycle = new QCheckBox(widget_29);
        checkBoxCycle->setObjectName(QStringLiteral("checkBoxCycle"));

        horizontalLayout_20->addWidget(checkBoxCycle);


        verticalLayout_2->addWidget(widget_29);

        widget = new QWidget(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setMinimumSize(QSize(0, 0));
        widget->setMaximumSize(QSize(16777215, 150));
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
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(plainTextUDP->sizePolicy().hasHeightForWidth());
        plainTextUDP->setSizePolicy(sizePolicy1);
        plainTextUDP->setMinimumSize(QSize(0, 0));
        plainTextUDP->setMaximumSize(QSize(16777215, 16777215));
        plainTextUDP->setReadOnly(true);

        horizontalLayout->addWidget(plainTextUDP);


        verticalLayout_2->addWidget(widget);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 816, 21));
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
        pushButtonUdpOpenClose->setText(QApplication::translate("MainWindow", "UDP Open", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "FPGA", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", "\320\237\320\260\320\272\320\265\321\202\320\276\320\262", Q_NULLPTR));
        label_5->setText(QApplication::translate("MainWindow", "bytesOnIter", Q_NULLPTR));
        label_14->setText(QApplication::translate("MainWindow", "recvInterval", Q_NULLPTR));
        groupBoxMain->setTitle(QApplication::translate("MainWindow", "\320\237\320\276 \320\277\321\200\320\270\320\262\320\276\320\264\320\260\320\274", Q_NULLPTR));
        groupBox_4->setTitle(QApplication::translate("MainWindow", "UDP", Q_NULLPTR));
        label_7->setText(QApplication::translate("MainWindow", "state", Q_NULLPTR));
        label_6->setText(QApplication::translate("MainWindow", "\320\224\320\263\321\200\320\274", Q_NULLPTR));
        checkBoxSliderPosCtrl->setText(QApplication::translate("MainWindow", "sliderPosCtrl", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tabMainStat), QApplication::translate("MainWindow", "mainStat", Q_NULLPTR));
        groupBox_5->setTitle(QApplication::translate("MainWindow", "\320\234\320\265\321\205\320\260\320\275\320\270\320\272\320\260", Q_NULLPTR));
        label_8->setText(QApplication::translate("MainWindow", "\320\270\320\274\320\277/\320\276\320\261", Q_NULLPTR));
        lineEdit_ImpPerRot->setInputMask(QString());
        lineEdit_ImpPerRot->setText(QApplication::translate("MainWindow", "4000", Q_NULLPTR));
        label_9->setText(QApplication::translate("MainWindow", "\320\274\320\274/\320\276\320\261", Q_NULLPTR));
        lineEdit_mmPerRot->setInputMask(QString());
        lineEdit_mmPerRot->setText(QApplication::translate("MainWindow", "10", Q_NULLPTR));
        lineEdit_maxHeightMM->setInputMask(QString());
        lineEdit_maxHeightMM->setText(QApplication::translate("MainWindow", "100", Q_NULLPTR));
        label_10->setText(QApplication::translate("MainWindow", "\320\274\320\260\320\272\321\201 \320\262\321\213\321\201\320\276\321\202\320\260, \320\274\320\274", Q_NULLPTR));
        lineEdit_MaxHeightImp->setInputMask(QString());
        lineEdit_MaxHeightImp->setText(QApplication::translate("MainWindow", "200000", Q_NULLPTR));
        label_11->setText(QApplication::translate("MainWindow", "\320\274\320\260\320\272\321\201 \320\262\321\213\321\201\320\276\321\202\320\260, \320\270\320\274\320\277", Q_NULLPTR));
        label_12->setText(QApplication::translate("MainWindow", "Vmax, \320\274\320\274/\321\201", Q_NULLPTR));
        groupBox_6->setTitle(QApplication::translate("MainWindow", "\320\255\320\221\320\243", Q_NULLPTR));
        radioButtonFpgaFreq24->setText(QApplication::translate("MainWindow", "24 \320\234\320\223\321\206 (v1.0)", Q_NULLPTR));
        radioButtonFpgaFreq25->setText(QApplication::translate("MainWindow", "25 \320\234\320\223\321\206 (v1.1)", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", "\320\232\320\276\320\273-\320\262\320\276 \320\274\320\276\321\202\320\276\321\200\320\276\320\262:", Q_NULLPTR));
        lineEditMotorCount->setInputMask(QApplication::translate("MainWindow", "90", Q_NULLPTR));
        checkBoxDirInverse->setText(QApplication::translate("MainWindow", "\320\270\320\275\320\262\320\265\321\200\321\201\320\270\321\217 dir", Q_NULLPTR));
        groupBox_7->setTitle(QApplication::translate("MainWindow", "UDP", Q_NULLPTR));
        lineEditMinVal->setInputMask(QApplication::translate("MainWindow", "9", Q_NULLPTR));
        lineEditMinVal->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "min:", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "max", Q_NULLPTR));
        lineEditUDPMaxVal->setInputMask(QString());
        lineEditUDPMaxVal->setText(QApplication::translate("MainWindow", "1000", Q_NULLPTR));
        checkBoxPrintUDPData->setText(QApplication::translate("MainWindow", "print", Q_NULLPTR));
        groupBox_8->setTitle(QApplication::translate("MainWindow", "About", Q_NULLPTR));
        label_13->setText(QApplication::translate("MainWindow", "compile time:", Q_NULLPTR));
        labelCompileTime->setText(QApplication::translate("MainWindow", "TextLabel", Q_NULLPTR));
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
        tabWidget->setTabText(tabWidget->indexOf(tabSettings), QApplication::translate("MainWindow", "settings", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tabDebugPort), QApplication::translate("MainWindow", "debugPort", Q_NULLPTR));
        label_15->setText(QApplication::translate("MainWindow", "SSID:", Q_NULLPTR));
        label_17->setText(QApplication::translate("MainWindow", "servIp:", Q_NULLPTR));
        label_16->setText(QApplication::translate("MainWindow", "key:", Q_NULLPTR));
        pushButtonSonoffAPSet->setText(QApplication::translate("MainWindow", "set", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tabSonoff), QApplication::translate("MainWindow", "power control", Q_NULLPTR));
        pushButtonInitiate->setText(QApplication::translate("MainWindow", "\320\230\320\275\320\270\321\206\320\270\320\260\320\273\320\270\320\267\320\270\321\200\320\276\320\262\320\260\321\202\321\214", Q_NULLPTR));
        pushMoveDown->setText(QApplication::translate("MainWindow", "moveDown", Q_NULLPTR));
        pushMoveUp->setText(QApplication::translate("MainWindow", "moveUp", Q_NULLPTR));
        goToTerm->setText(QApplication::translate("MainWindow", "go term", Q_NULLPTR));
        pushButtonGoZero->setText(QApplication::translate("MainWindow", "go zero", Q_NULLPTR));
        pushButtonTest->setText(QApplication::translate("MainWindow", "test", Q_NULLPTR));
        pushButtonTest1->setText(QApplication::translate("MainWindow", "test1", Q_NULLPTR));
        pushButtonTest2->setText(QApplication::translate("MainWindow", "test2", Q_NULLPTR));
        pushButtonU1->setText(QApplication::translate("MainWindow", "u1", Q_NULLPTR));
        pushButtonD1->setText(QApplication::translate("MainWindow", "d1", Q_NULLPTR));
        pushButtonU2->setText(QApplication::translate("MainWindow", "u2", Q_NULLPTR));
        pushButtonD2->setText(QApplication::translate("MainWindow", "d2", Q_NULLPTR));
        pushButtonU3->setText(QApplication::translate("MainWindow", "u3", Q_NULLPTR));
        pushButtonD3->setText(QApplication::translate("MainWindow", "d3", Q_NULLPTR));
        pushButtonPosReset->setText(QApplication::translate("MainWindow", "pos reset", Q_NULLPTR));
        pushBUttonToIdle->setText(QApplication::translate("MainWindow", "to idle", Q_NULLPTR));
        pushButtonClear->setText(QApplication::translate("MainWindow", "clear queue", Q_NULLPTR));
        pushButtonGotoPEriodState->setText(QApplication::translate("MainWindow", "to Period", Q_NULLPTR));
        checkBoxDebugComPrint->setText(QApplication::translate("MainWindow", "debugComPrint", Q_NULLPTR));
        checkBoxCycle->setText(QApplication::translate("MainWindow", "cycle", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
