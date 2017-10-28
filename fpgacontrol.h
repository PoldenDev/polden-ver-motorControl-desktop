#pragma once
#include <QObject>
#include <QTimer>
#include <QSerialPort>
#include <QQueue>

#include "stand.h"

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
    void setMotorStateGoDown();
    void setMotorStateInitiate();
    void setFpgaFreq(quint32);
    void setDirInverse(bool e){bDirInvers=e;}
    qint32 getMotorAbsPosImp(int i) { return motorAbsolutePosCur[i];}

    void posReset();

    TMotorState mtState[MOTOR_CNT];
    quint32 fpgaFreq;
    quint32 comExchanges;
    quint32 bytesOnIter;
//    void addMotorCmd(int, DivPosDataStr&);

    void clearCmdList();
    void addMotorCmd(int mn, int newPosImp, int msecsForMove);
    quint32 getCmdListLength(int id){return motorPosCmdData[id].length();}

private:
    QTimer timer;
    int motorCount;
    QQueue<DivPosDataStr> motorPosCmdData[MOTOR_CNT];
    bool bFreeToWrite[MOTOR_CNT];
    qint32 motorAbsolutePosCur[MOTOR_CNT];
    bool bTermState[MOTOR_CNT];
    //quint32 lastCtrlTimeMsecs[MOTOR_CNT];
    quint32 lastDebugShowTime;
    bool bDirInvers;

    void parseFPGAMsg(QByteArray ba);
    void freeToWrite(int i);
    void terminatorState(int i, bool bEna);
    void allFreeToWrite();
    void sendDivPos(int mi, DivPosDataStr &ds);
    void calcCmd(DivPosDataStr &ds, int delta, quint32 curmSecs, quint32 msecsForMove, int id);

signals:
    void termStateChanged(int, bool);

private slots:
    void handleExchTimer();
    void handleReadyRead();
    void handleSerialDataWritten(qint64 bytes);
    void handleErrorOccurred(QSerialPort::SerialPortError);
};

