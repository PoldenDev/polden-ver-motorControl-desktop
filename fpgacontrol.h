#pragma once
#include <QObject>
#include <QTimer>
#include <QSerialPort>
#include <QQueue>
#include <QTime>
#include <QBitArray>

#include "stand.h"

typedef enum{
    standStateInitiating,
    standStateError,
    standStateidle
} TStandState;

class FpgaControl : public QObject
{
    Q_OBJECT
public:
    explicit FpgaControl(QObject *parent = nullptr);
    QSerialPort serial;

    bool openPort(QString portName);
    bool portIsOpen(){ return serial.isOpen();}
    QSerialPort::SerialPortError getPortError(){ return serial.error(); }
    QString getPortName(){return serial.portName();}
    void closePort();

    void setMotorCount(int);
    void setMotorStateIdle();
    //void setMotorStateGoDown();
    void setMotorStateInitiate();
    void setFpgaFreq(quint32);
    void setDirInverse(bool e){bDirInvers=e;}
    qint32 getMotorAbsPosImp(int i) { return motorAbsolutePosCur[i];}

    void posReset();

    TMotorState mtState[MOTOR_CNT];
    quint32 fpgaFreq;
    quint32 comExchanges;

    quint32 bytesOnIter;
    qint32 recvInterval;
//    void addMotorCmd(int, DivPosDataStr&);

    void clearCmdList();
    void addMotorCmd(int mn, int newPosImp, int msecsForMove);
    quint32 getCmdListLength(int id){return motorPosCmdData[id].length();}

    void addRawCmd(int id, quint32 d, quint32 st, int dir);

private:
    int motorCount;
    QQueue<DivPosDataStr> motorPosCmdData[MOTOR_CNT];
    //bool bFreeToWrite[MOTOR_CNT];
    qint32 motorAbsolutePosCur[MOTOR_CNT];
    QBitArray bTermState;
    //quint32 lastCtrlTimeMsecs[MOTOR_CNT];
    quint32 lastDebugShowTime;
    bool bDirInvers;

    QTime exchInterval;
    QTimer timer, exchTimeoutTimer;
    TStandState standState;


    void parseFPGAMsg(QByteArray ba);
    void freeToWrite(int i);
    void terminatorState(int i, bool bEna);
    void allFreeToWrite();
    void sendDivPos(int mi, DivPosDataStr &ds);

//    bool isBufferFree(qint32, int);
//    void setBufferNotFree(qint32&, int);
//    bool issTermEna(qint32, int);
    int maxDiv_debug, maxSteps_debug;

signals:
    void termStateChanged(int, bool);
    void errorOccured(const QString&);
    void initFinished();
    void standStateChanged(TStandState);


private slots:
    void handleReadyRead();
    void handleSerialDataWritten(qint64 bytes);
    void handleComPortErrorOccurred(QSerialPort::SerialPortError);
    void handleExchTimeout();
};

