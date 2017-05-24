#ifndef UARTTHREAD_H
#define UARTTHREAD_H


#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QSerialPort>
#include <QQueue>


typedef enum {idle, waitForAck, waitForFifoFree} TSendState;

#define MOTOR_CNT 10
typedef struct{
    QQueue<QString> contrStringQueue;
    bool bCmdOk;
    TSendState sendState;

} TMotorStr;

class UartThread : public QThread
{
        Q_OBJECT

public:
    explicit UartThread(QObject *parent = nullptr);
    ~UartThread();


    void transaction(int motorInd, const QString &request);
    void run() Q_DECL_OVERRIDE;

    void requestToStart(const QString &portName);

signals:
    void response(const QString s);
    void error(const QString &s);
    void timeout(const QString &s);

private:
    QString portName;
    QMutex mutex;
    QWaitCondition cond;
    bool quit;

    QString uartBuff;

    quint8 curMotorSendIdx;
    TSendState uartSendState;

    void processUartRecvExchange(QString msg);

        TMotorStr mtstr[MOTOR_CNT];


};

#endif // UARTTHREAD_H
