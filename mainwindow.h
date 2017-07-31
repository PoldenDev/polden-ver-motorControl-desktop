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
#include <qwt_plot_marker.h>

//#include <uartthread.h>


#include <qwt_plot_curve.h>

#include "stand.h"
namespace Ui {
class MainWindow;
}


typedef struct{
    int div;
    int steps;
} DivPosDataStr;

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


    void on_pushMoveDownState_clicked();

    void on_pushBUttonToIdle_clicked();

    void on_pushButtonGotoPEriodState_clicked();

    void on_pushButtonPosReset_clicked();

    void on_pushTestData_clicked();

    void on_pushClearMap_clicked();


    void on_tabWidget_tabBarClicked(int index);

private:
    void setPos(int pos);
    Ui::MainWindow *ui;
    QSerialPort serial;

    int speed;

    void initUdpSocket();
    QUdpSocket *udpSocket;


    quint32 cmdNum;
    QString uartBuff;
    //bool bFifoFull;

    QTimer timer;
    QList<QSlider*> slList;

    quint32 udpCnt;

    quint8 lastMotorReceptInd;
    //QTimer timerSerialSendTo[MOTOR_CNT];

    //bool noDataSending;
    //void processUartSendExchange();
    //void processUartRecvExchange(QString str);
    QTimer waitForFifoFreeTimer;
    //UartThread UartThread;
    QTimer dataProcess100msTimer;


    //TMotorStr mtstr[MOTOR_CNT];
    quint8 curMotorSendIdx;

    QList<QwtPlot*> plotList;
    QList<QwtPlotCurve*> posCurveList, velCurveList;
    QList<QPolygonF> polyPoslist, polyVellist;
    QList<QwtPlotMarker*> markrlist;
    int markerXPos;
    //QwtPlot *d_plot;
    //QwtPlotCurve *curve;
    //QPolygonF points;
    int x, xUdpRecv;
    void createPlot(QString name);

    void parseCmdMotorStr(int, QString);
    void parseCmdMultiMotorStr(QString);
    void parseCmdMultiMotorStrList(QStringList);




    QMap<int, int> xMap;
    QMap<int, QString> lastCmdMap;
    QQueue<QString> motorPosCmdStrings;


    QQueue<DivPosDataStr> motorPosCmdData[MOTOR_CNT];

    //void convertPosModeToVelMode(QString);
    QMap<int, int> lastPosMap;
    //QQueue<QString> motorVelCmdStrings;

    void graphReset();
    QSettings settings;

    void sendDivPos(int mi, int div, int steps);
};


#endif // MAINWINDOW_H
