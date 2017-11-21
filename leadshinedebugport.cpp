#include "leadshinedebugport.h"
#include <QDebug>

LeadshineDebugPort::LeadshineDebugPort(QObject *parent) :
    QObject(parent),
    portCount(0)
{
    connect(&checkDebugComTimer, SIGNAL(timeout()), this, SLOT(checkDebugComTimerHandle()));
    checkDebugComTimer.setSingleShot(false);
    checkDebugComTimer.setInterval(500);
    checkDebugComTimer.start();
}

void LeadshineDebugPort::setPortCount(int pCnt)
{
    portCount = pCnt;
    checkDebugComTimer.stop();
    debSerialPortList.clear();
    motorRespRecvdList.clear();
    msgDataArrList.clear();
    for(int i=0; i<pCnt; i++){
        QSerialPort *sp = new QSerialPort(this);
        connect(sp, &QSerialPort::errorOccurred, [this, i](QSerialPort::SerialPortError error){ handleComPortErrorOccured(i, error);});
        connect(sp, &QSerialPort::readyRead, [this, i](){ handleReadyRead(i);});
        debSerialPortList.append(sp);
        motorRespRecvdList.append(new bool(false));

        msgDataArrList.append(QByteArray());
    }

    checkDebugComTimer.start();
}

void LeadshineDebugPort::handleComPortErrorOccured(int id, QSerialPort::SerialPortError error)
{
    if(error != QSerialPort::NoError){
        QString errorStr;
        switch(error){
            case QSerialPort::DeviceNotFoundError: errorStr = "DeviceNotFoundError"; break;
            case QSerialPort::PermissionError: errorStr = "PermissionError"; break;
            case QSerialPort::OpenError: errorStr = "OpenError"; break;
            case QSerialPort::ParityError: errorStr = "ParityError"; break;
            case QSerialPort::FramingError: errorStr = "FramingError"; break;
            case QSerialPort::BreakConditionError: errorStr = "BreakConditionError"; break;
            case QSerialPort::WriteError: errorStr = "WriteError"; break;
            case QSerialPort::ReadError: errorStr = "ReadError"; break;
            case QSerialPort::ResourceError: errorStr = "ResourceError"; break;
            case QSerialPort::UnsupportedOperationError: errorStr = "UnsupportedOperationError"; break;
            case QSerialPort::TimeoutError: errorStr = "TimeoutError"; break;
            case QSerialPort::NotOpenError: errorStr = "NotOpenError"; break;
            default:
            case QSerialPort::UnknownError: errorStr = "UnknownError"; break;

        }

        //QString msg = QString("%1 error: %2").arg(qUtf8Printable(debSerialPortList[id]->portName())).arg(errorStr);
        //ui->plainTextEdit->appendPlainText(msg);
        //qDebug() <<"!!!!!!!" << id <<error;
        if((error == QSerialPort::ResourceError) ||
           (error == QSerialPort::PermissionError)){
            //pushButtonComOpen_clicked(id);
            //comPortClose(id);
        }
    }
}

void LeadshineDebugPort::handleReadyRead(int id)
{
    QByteArray ba = debSerialPortList[id]->readAll();
    msgDataArrList[id].append(ba);
    for(int i=0;
        (i<msgDataArrList[id].length()) && (msgDataArrList[id].length()>=2);
        i++)
    {
        if((msgDataArrList[id][0] == 0x01) &&
                (msgDataArrList[id][1] == 0x03))
            break;
        msgDataArrList[id].remove(0, 1);
        //qDebug() <<"d";
    }
    if(msgDataArrList[id].length()>=3){
        int msgSize = msgDataArrList[id][2];
        msgSize += (3 + 2); //3 header, 2 crc
        if( msgDataArrList[id].length() >= msgSize ){
            //qDebug("msg!!");
            QByteArray msgBa = msgDataArrList[id].left(msgSize);
            msgDataArrList[id].remove(0, msgSize);
            //qDebug() << id << msgBa;
            parseLeadShineMsg(id, msgBa);
        }
    }

}

void LeadshineDebugPort::parseLeadShineMsg(int id, QByteArray &ba)
{
    quint16 crc16 = CRC16_ModBusRTU(ba, ba.length()-2);
    QByteArray crc16ba;
    crc16ba.append(crc16&0xff);
    crc16ba.append((crc16>>8)&0xff);

    //quint8 crc16Recvd = ba[ba.length()-1];
    if(ba.endsWith(crc16ba) == false){
//        ui->plainTextEdit->appendPlainText(QString("debResp%1: recv CRC err").arg(id));
//        ui->plainTextEdit->appendPlainText(QString("debResp%1: %2 -> %3").arg(id).arg(ba.size()).arg(QString(ba.toHex().toUpper())));
//        ui->plainTextEdit->appendPlainText(QString("debResp%1: CRC %2").arg(id).arg(crc16));
        return;
    }

    if((ba.length()==25)&&(ba[0] == 0x01)&&(ba[1] == 0x03)&&(ba[2] == 0x14)){
        bool &bRespRecv = *(motorRespRecvdList[id]);
        bRespRecv = true;

        //ui->plainTextEdit->appendPlainText(QString("debResp%1: %2 -> %3").arg(id).arg(ba.size()).arg(QString(ba.toHex().toUpper())));
        if((ba[4]&0x50) == 0){
            emit driverOk(id);
//            debPortStatusLeList[id]->setPalette(*paletteGreen);
//            debPortStatusMainLeList[id]->setPalette(*paletteGreen);
//            //ui->plainTextEdit->appendPlainText(QString("debResp%1: OK").arg(id));
//            debPortStatusLeList[id]->setText("OK");
//            debPortStatusMainLeList[id]->setText("OK");

        }
        else{
            QString msg("");

//            debPortStatusLeList[id]->setPalette(*paletteRed);
//            debPortStatusMainLeList[id]->setPalette(*paletteRed);
            if(ba[4]&0x10){ //encErr
                msg += "encErr";
//                debPortStatusLeList[id]->setText("enc err");
//                debPortStatusMainLeList[id]->setText("enc err");
            }
            if(ba[4]&0x40){ //posErr
                msg += "posErr";
//                debPortStatusLeList[id]->setText("pos err");
//                debPortStatusMainLeList[id]->setText("pos err");
            }
            emit driverErr(id, msg);
        }
    }
    else{
//        ui->plainTextEdit->appendPlainText(QString("debResp%1: %2 -> %3").arg(id).arg(ba.size()).arg(QString(ba.toHex().toUpper())));
    }
}

#include <QtEndian>
quint16 LeadshineDebugPort::CRC16_ModBusRTU(QByteArray buf, quint16 len)
{
    //Вычисляет контрольную сумму CRC16 для ModBus и выдаёт её с нужным порядком байтов
      quint16 crc = 0xFFFF;
      for (int pos = 0; pos < len; pos++)
      {
      crc ^= (quint16)buf[pos];    // XOR byte into least sig. byte of crc

      for (int i = 8; i != 0; i--) {    // Loop over each bit
        if ((crc & 0x0001) != 0) {      // If the LSB is set
          crc >>= 1;                    // Shift right and XOR 0xA001
          crc ^= 0xA001;
        }
        else                            // Else LSB is not set
          crc >>= 1;                    // Just shift right
        }
      }
      return crc;
}


bool LeadshineDebugPort::isPortOpen(int id)
{
    return debSerialPortList[id]->isOpen();
}

void LeadshineDebugPort::setPortName(int id, QString &name)
{
    debSerialPortList[id]->setPortName(name);
}

QString LeadshineDebugPort::portName(int id)
{
    return debSerialPortList[id]->portName();
}

bool LeadshineDebugPort::open(int id)
{
    debSerialPortList[id]->setBaudRate(38400);
    return debSerialPortList[id]->open(QIODevice::ReadWrite);
}

void LeadshineDebugPort::close(int id)
{
    debSerialPortList[id]->close();
}


void LeadshineDebugPort::checkDebugComTimerHandle()
{
    QByteArray reqBase = QByteArrayLiteral("\x01\x03\x00");
    //const quint8 req1Str[] {0x01, 0x03, 0x00, 0xFD, 0x00, 0x01, 0x15, 0xfa};
    QByteArray req1 = reqBase + QByteArrayLiteral("\xFD\x00\x01\x15\xfa");
    QByteArray reqReqErrTrace = reqBase + QByteArrayLiteral("\x10\x00\x0A\xC4\x08");
    //req1 << 0x01 << 0x03 << 0x00 << 0xFD << 0x00 << 0x01 << 0x15 << 0xfa;

    for(int i=0; i<portCount; i++){
        bool &bRespRecv = *(motorRespRecvdList[i]);
        if(bRespRecv == false){
            emit driverTimeOut(i);
        }
        QSerialPort *sp = debSerialPortList[i];
        if(sp->isOpen()){
            sp->write(reqReqErrTrace);
        }
        bRespRecv = false;
    }
}
