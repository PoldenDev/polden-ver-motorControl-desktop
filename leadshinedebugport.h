#ifndef LEADSHINEDEBUGPORT_H
#define LEADSHINEDEBUGPORT_H

#include <QObject>
#include <QSerialPort>
#include <QTimer>
#include <QMap>

typedef enum{
    LS_STAT_OK,
    LS_STAT_Error,
    LS_STAT_Unknown
} TDriversState;

class LeadshineDebugPort : public QObject
{
    Q_OBJECT
public:
    explicit LeadshineDebugPort(QObject *parent = nullptr);

    void setPortCount(int);
    void parseLeadShineMsg(int id, QByteArray &ba);
    quint16 CRC16_ModBusRTU(QByteArray buf, quint16 len);

    bool isPortOpen(int);
    void setPortName(int, QString&);
    QString portName(int);
    bool open(int);
    void close(int);

    bool isDriversOk();
    TDriversState driverState(int);
    QList<QString> comNames;
signals:
    void driverOk(int);
    void driverErr(int, QString&);
    void driverTimeOut(int);

public slots:
private slots:
    void checkDebugComTimerHandle();
private:
    int portCount;
    QList<QSerialPort*> debSerialPortList;
    QList<bool*> motorRespRecvdList;

    QTimer checkDebugComTimer;

    void handleReadyRead(int id);
    void handleComPortErrorOccured(int id, QSerialPort::SerialPortError error);

    QList<QByteArray> msgDataArrList;

    QMap<int, TDriversState> driverStateMap;
};

#endif // LEADSHINEDEBUGPORT_H
