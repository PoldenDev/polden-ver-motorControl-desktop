#ifndef LEADSHINEDEBUGPORT_H
#define LEADSHINEDEBUGPORT_H

#include <QObject>
#include <QSerialPort>
#include <QTimer>

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
signals:

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
};

#endif // LEADSHINEDEBUGPORT_H
