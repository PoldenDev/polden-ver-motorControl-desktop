#include "uartthread.h"
#include <QtDebug>
#include <QTime>

UartThread::UartThread(QObject *parent)
: QThread(parent), quit(false),
  curMotorSendIdx(0)
{

}

UartThread::~UartThread()
{
    mutex.lock();
    quit = true;
    cond.wakeOne();
    mutex.unlock();
    wait();
}

void UartThread::requestToStart(const QString &portName)
{
    QMutexLocker locker(&mutex);
    this->portName = portName;
    if (!isRunning())
        start();
    else
        cond.wakeOne();

}

void UartThread::transaction(int motorInd, const QString &contrStr)
{
    if(motorInd >= MOTOR_CNT)
        return;
    QMutexLocker locker(&mutex);
    mtstr[motorInd].contrStringQueue.enqueue(contrStr);
}

void UartThread::processUartRecvExchange(QString msg)
{
    if(msg.compare("ff\r\n") == 0){
        qDebug() << "Fifo full!!!";
        uartSendState = idle;
        //processUartSendExchange();
        //waitForFifoFreeTimer.setSingleShot(true);
        //waitForFifoFreeTimer.start(100);
    }
    else if(msg.compare("ok\r\n") == 0){
        //bCmdOk = true;
        qDebug() << "OK!!!";
        uartSendState = idle;
        mtstr[curMotorSendIdx].contrStringQueue.dequeue();
        curMotorSendIdx++;
        if(curMotorSendIdx >= MOTOR_CNT){
            curMotorSendIdx = 0;
        }
        //processUartSendExchange();

//        if(contrStringQueue.isEmpty() == false){
//           contrStringQueue.dequeue();
//            if(contrStringQueue.isEmpty() == false){
//                QString wrStr = contrStringQueue.head();
//                qDebug("%s", qPrintable(wrStr));
//                QThread::msleep(20);
//                serial.write(wrStr.toLatin1());
//            }
//            else{
//                noDataSending = true;
//            }
//        }
//        else{
            //noDataSending = true;
//        }

    }
    else{
        qDebug() << "unexpected " << msg;
    }

    switch(uartSendState){
    case waitForAck:
        //qDebug() << "expected ack msg";
            break;
    }



}

void UartThread::run()
{
    bool currentPortNameChanged = false;

    mutex.lock();
    QString currentPortName;
    if (currentPortName != portName) {
        currentPortName = portName;
        currentPortNameChanged = true;
    }
    //QString currentRequest = request;
    mutex.unlock();
    QSerialPort serial;
    serial.setPortName(currentPortName);

    serial.setBaudRate(115200);
    if (!serial.open(QIODevice::ReadWrite)) {
        //emit error(tr("Can't open %1, error code %2")
                   //.arg(portName).arg(serial.error()));

        return;
    }

    QTime time;
    time.start();


    while (!quit) {
//        if (currentPortNameChanged) {
//            serial.close();
//            serial.setPortName(currentPortName);

//            serial.setBaudRate(115200);
//            if (!serial.open(QIODevice::ReadWrite)) {
//                //emit error(tr("Can't open %1, error code %2")
//                           //.arg(portName).arg(serial.error()));

//                return;
//            }
//        }

        bool bDataOk = false;
        QString wrStr;
        //qDebug()<< time.elapsed() << "start lock";
        mutex.lock();
        //qDebug()<< time.elapsed() << "locked";
        if(mtstr[curMotorSendIdx].contrStringQueue.isEmpty() == false){
           wrStr = mtstr[curMotorSendIdx].contrStringQueue.head();
           bDataOk = true;
        }
        mutex.unlock();

        time.elapsed();

        if(bDataOk){
            //qDebug() << "s" << curMotorSendIdx ;

            //qDebug()<< time.elapsed() << "start write";
            serial.write(wrStr.toLatin1());

            if (serial.waitForBytesWritten(100)) {
                // read response                
                if (serial.waitForReadyRead(40)) {
                    //qDebug()<< time.elapsed() << "data recvd";
                    QByteArray responseData = serial.readAll();
                    while (serial.waitForReadyRead(10))
                         responseData += serial.readAll();

                    uartBuff += QString(responseData);
                    //uartBuff.split("\r\n");
                    int lfInd = uartBuff.indexOf("\r\n");
                    QString msg = uartBuff.left(lfInd+2);
                    uartBuff.remove(0, lfInd+2);

                    if(msg.compare("ff\r\n") == 0){
                        qDebug() << "Fifo full!!!";
                    }
//                    else if(msg.compare("ok\r\n") == 0){
//                            //bCmdOk = true;
//                            //qDebug() << "OK!!!";
//                            mutex.lock();
//                            mtstr[curMotorSendIdx].contrStringQueue.dequeue();
//                            mutex.unlock();
//                    }
                    else{
                        QString response(responseData);
                        emit this->response(response);
                    }
                } else {
                    qDebug("waitForReadyRead to on cmd: %s", qPrintable(wrStr) );
                    mutex.lock();
                    mtstr[curMotorSendIdx].contrStringQueue.dequeue();
                    mutex.unlock();
                }
            } else {
                qDebug("waitForBytesWritten to");

            }
        }
        else{
            if (serial.waitForReadyRead(100)) {
                QByteArray responseData = serial.readAll();
                while (serial.waitForReadyRead(10))
                     responseData += serial.readAll();

                QString response(responseData);
                emit this->response(response);
            } else {
                      //  emit timeout(tr("Wait read response timeout %1")
                      //               .arg(QTime::currentTime().toString()));
            }
        }


        curMotorSendIdx++;
        if(curMotorSendIdx >= MOTOR_CNT){
            curMotorSendIdx = 0;
        }
//        if (serial.waitForReadyRead(currentWaitTimeout)) {

//                QByteArray str = serial.readAll();
//                while (serial.waitForReadyRead(10))
//                    str += serial.readAll();

//                uartBuff += str;
//                //uartBuff.split("\r\n");
//                bool bAcked = false;
//                int lfInd = uartBuff.indexOf("\r\n");
//                QString cmd = uartBuff.left(lfInd+2);
//                uartBuff.remove(0, lfInd+2);






//                QString responseStr(str);
//                emit response(responseStr);
//            } else {
//              //  emit timeout(tr("Wait read response timeout %1")
//              //               .arg(QTime::currentTime().toString()));
//            }

        // write request
//        QByteArray requestData = currentRequest.toLocal8Bit();
//        serial.write(requestData);
//        if (serial.waitForBytesWritten(waitTimeout)) {
//            // read response
//            if (serial.waitForReadyRead(currentWaitTimeout)) {
//                QByteArray responseData = serial.readAll();
//                while (serial.waitForReadyRead(10))
//                    responseData += serial.readAll();

//                QString response(responseData);
//                emit this->response(response);
//            } else {
//              //  emit timeout(tr("Wait read response timeout %1")
//              //               .arg(QTime::currentTime().toString()));
//            }
//        } else {
//            //emit timeout(tr("Wait write request timeout %1")
//            //             .arg(QTime::currentTime().toString()));
//        }
//        mutex.lock();
//        cond.wait(&mutex);
//        if (currentPortName != portName) {
//            currentPortName = portName;
//            currentPortNameChanged = true;
//        } else {
//            currentPortNameChanged = false;
//        }
//        currentWaitTimeout = waitTimeout;
//        currentRequest = request;
//        mutex.unlock();
    }
}
