#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QQueue>
#include <QTimer>
#include <QSlider>
#include <QMap>
#include <QSettings>
//#include <qwt_plot_marker.h>
#include <QLineEdit>
#include <QSlider>
#include <QTime>
#include <QCheckBox>
//#include <uartthread.h>
#include <QComboBox>
#include <QPushButton>
#include <QGroupBox>


//#include <qwt_plot_curve.h>

#include "stand.h"
namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    //void on_butPos_clicked();

    void on_pushButtonComOpen_clicked();

    //void on_pushButtonClose_clicked();
    //void on_pushButton_clicked();
    //void on_pushButtonDir_clicked();
    //void on_pushButtonSetRange_clicked();
    //void on_pushButtonSetCont_clicked();
    //void on_butPos_2_clicked();
    //void on_pushButton_2_clicked();
    //void on_pushButtonDir_2_clicked();
    //void on_pushButtonSetRange_2_clicked();
    //void on_pushButtonSetCont_2_clicked();
    //void on_verticalSlider_sliderReleased();
    //void on_verticalSlider_sliderMoved(int position);
    //void on_verticalSlider_2_sliderReleased();
    void readPendingDatagrams();
    void stateChanged(QAbstractSocket::SocketState socketState);
    void handleReadyRead();
    void handleSerialDataWritten(qint64 bytes);
    void on_pushButton_refreshCom_clicked();
    void on_pushButtonClear_clicked();
    void sendOnTimer();
    void sendTimeOut();
    void dataProcess100msTimeOut();
   // void processUartExchange();
    void waitForFifoFreeFifo();

    void response(QString str);


    //void on_pushMoveDownState_clicked();
    void on_pushBUttonToIdle_clicked();
    void on_pushButtonGotoPEriodState_clicked();
    void on_pushButtonPosReset_clicked();
    void on_pushTestData_clicked();

    void on_tabWidget_tabBarClicked(int index);
    void on_goToTerm_clicked();
    void on_pushMoveUp_clicked();
    void on_pushMoveDown_clicked();

    void uiUpdateTimerSlot();

    void on_pushButtonUdpOpenClose_clicked();

    void on_pushButtonInitiate_clicked();
    void on_pushButtonGoZero_clicked();

    void on_pushButtonTest_clicked();

    void on_lineEdit_maxHeightMM_editingFinished();
    void on_lineEdit_MaxHeightImp_editingFinished();
    void on_radioButtonFpgaFreq25_clicked();
    void on_radioButtonFpgaFreq24_clicked();
    void on_lineEdit_mmPerRot_editingFinished();
    void on_lineEdit_vmax_mmsec_editingFinished();

    void on_lineEditMotorCount_editingFinished();
    void checkDebugComTimerHandle();

private:
    quint32 motorCount;
    void setPos(int pos);
    Ui::MainWindow *ui;
    QSerialPort serial;

    int speed;

    QUdpSocket *udpSocket;


    quint32 cmdNum;
    QString uartBuff;
    //bool bFifoFull;

    QTimer timer;
    QList<QSlider*> slList;

    quint8 lastMotorReceptInd;
    //QTimer timerSerialSendTo[MOTOR_CNT];

    //bool noDataSending;
    //void processUartSendExchange();
    //void processUartRecvExchange(QString str);
    QTimer waitForFifoFreeTimer;
    //UartThread UartThread;
    QTimer dataProcess100msTimer;
    QTimer uiUpdateTimer;


    //TMotorStr mtstr[MOTOR_CNT];
    quint8 curMotorSendIdx;

//    QList<QwtPlot*> plotList;
//    QList<QwtPlotCurve*> posCurveList, velCurveList;
//    QList<QPolygonF> polyPoslist, polyVellist;
//    QList<QwtPlotMarker*> markrlist;
    int markerXPos;
    //QwtPlot *d_plot;
    //QwtPlotCurve *curve;
    //QPolygonF points;
    int x, xUdpRecv;
    void createPlot(QString name);

    void parseCmdMotorStr(int, QString, int msecsForStep);
    void parseCmdMultiMotorStr(QString);
    void parseCmdMultiMotorStrList(QStringList);

    QMap<int, int> xMap;
    QMap<int, QString> lastCmdMap;
    //QQueue<QString> motorPosCmdStrings;

    //int absolutePos[MOTOR_CNT];

    quint32 lastCtrlTimeMsecs[MOTOR_CNT];
    //quint32 motorAbsolutePos[MOTOR_CNT];
    qint32 motorAbsolutePosCur[MOTOR_CNT];
    QQueue<DivPosDataStr> motorPosCmdData[MOTOR_CNT];
    qint32 getMotorAbsPos(int i) { return motorAbsolutePosCur[i];}

    //void convertPosModeToVelMode(QString);
    QMap<int, int> lastPosMap;
    //QQueue<QString> motorVelCmdStrings;

    void graphReset();
    QSettings settings;

    bool sendDivPos(int mi, DivPosDataStr &ds, quint32 pos);

    TMotorState mtState[MOTOR_CNT];

    void freeToWrite(int i);
    void terminatorState(int i, bool bEna);
    void allFreeToWrite();

    QList<QLineEdit*> timeStatLE;
    QList<QSlider*> timeStatSlider;

    QList<QSlider*> absPosSlider;
    QList<QLineEdit*> absPosLineEdit;
    QList<QLineEdit*> stateLineEdit;
    //int timeShiftMaxPos, timeShiftMaxNeg;

    quint32 comExchanges;
    QTime usbConnectionTime, udpConnectionTime;
    int bytesOnIter;
    QList<QCheckBox*> termCheckBox;
    bool bTermState[MOTOR_CNT];
    QList<QLineEdit*> euqueLineEdit;
    quint32 dataGramCnt;

    void udpServerOpen();
    void udpServerClose();

    bool bFreeToWrite[MOTOR_CNT];
    quint32 FPGA_FREQ;

    int mmToImp(int mm);
    void parseFPGAMsg(QByteArray ba);
    bool speedTrig[MOTOR_CNT];

    void handleSliderReleased(int id, int newPos);

    void createDebugSerialPortInterface();
    void createMainInterface();

    QList<QComboBox*> debPortCmbBxList;
    QList<QPushButton*> debPortpbList;
    QList<QSerialPort*> debSerialPortList;
    QList<QGroupBox*> debPortGbList;
    QList<QLineEdit*> debPortStatusLeList;
    QList<QLineEdit*> debPortStatusMainLeList;
    void pushDebugComPortOpen(int id);
    void comPortClose(int id);
    void handleComPortErrorOccured(int id, QSerialPort::SerialPortError error);
    void handleReadyRead(int id);
    QTimer checkDebugComTimer;
    void parseLeadShineMsg(int id, QByteArray &ba);
    quint16 CRC16_ModBusRTU(QByteArray buf, quint16 len);

    QPalette *paletteGrey, *paletteRed, *paletteGreen;

    QList<QWidget*> motorStatWidgetList;

    QList<bool*> motorRespRecvdList;
    quint32 lastUdpDatagrammRecvd;
    QList<quint32> udpDgrmRecvdInterval;


};


#endif // MAINWINDOW_H

