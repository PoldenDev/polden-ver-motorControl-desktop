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

//#include <uartthread.h>


#include <qwt_plot_curve.h>

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
    void handleReadyRead();
    void handleSerialDataWritten(qint64 bytes);
    void on_pushButton_refreshCom_clicked();
    void on_pushButtonClear_clicked();
    void sendOnTimer();

    void sendTimeOut();
   // void processUartExchange();
    void waitForFifoFreeFifo();

    void response(QString str);


    void on_pushMoveDownState_clicked();

    void on_pushBUttonToIdle_clicked();

    void on_pushButtonGotoPEriodState_clicked();

    void on_pushButtonPosReset_clicked();

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


    TMotorStr mtstr[MOTOR_CNT];
    quint8 curMotorSendIdx;

    QList<QwtPlot*> plotList;
    QList<QwtPlotCurve*> curveList;
    QList<QPolygonF> polylist;
    //QwtPlot *d_plot;
    //QwtPlotCurve *curve;
    //QPolygonF points;
    int x;
    void createPlot(QString name);

    void parseCmdMotorStr(QString);
    void parseCmdMultiMotorStr(QString);

    QMap<int, int> xMap;
    QMap<int, QString> lastCmdMap;
    QQueue<QString> motorPosCmdStrings;

};


#endif // MAINWINDOW_H
