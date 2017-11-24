#include "fpgacontrol.h"
#include <QDebug>
#include <QTime>
#include <QBitArray>

FpgaControl::FpgaControl(QObject *parent) :
    QObject(parent),
    motorCount(0),
    bytesOnIter(0),
    comExchanges(0),
    fpgaFreq(FPGA_FREQ_25),
    bDirInvers(false),
    recvInterval(-1),
    bTermState(MOTOR_CNT),
    maxDiv_debug(0), maxSteps_debug(0)
{
    for(int i=0; i<MOTOR_CNT; i++){
        bTermState[i] = false;
        mtState[i] = MT_IDLE;
        //bFreeToWrite[i] = false;
        motorAbsolutePosCur[i] = 0;
        //lastCtrlTimeMsecs[i] = 0;
    }

    connect(&serial, SIGNAL(errorOccurred(QSerialPort::SerialPortError)),
            this, SLOT(handleComPortErrorOccurred(QSerialPort::SerialPortError)));
    connect(&serial, SIGNAL(readyRead()), this, SLOT(handleReadyRead()));

    //timer.setInterval(10);
    //connect(&timer, SIGNAL(timeout()), this, SLOT(handleExchTimer()));
    //timer.start();
    connect(&exchTimeoutTimer, SIGNAL(timeout()),
            this, SLOT(handleExchTimeout()));
    exchTimeoutTimer.setSingleShot(false);
    exchTimeoutTimer.setInterval(100);
}

bool FpgaControl::openPort(QString portName)
{
    serial.setBaudRate(115200);
    serial.setPortName(portName);
    if (!serial.open(QIODevice::ReadWrite)) {
        //qDebug("%s port open FAIL", qUtf8Printable(comName));
        //ui->plainTextEdit->appendPlainText(QString("%1 port open FAIL %2").arg(qUtf8Printable(comName)).arg(serial.error()));
        return false;
    }
    comExchanges = 0;
    char c = 0xff;
    serial.write(&c, 1);
    exchTimeoutTimer.start();
    return true;
}

void FpgaControl::closePort()
{
    exchTimeoutTimer.stop();
    serial.close();
}

void FpgaControl::setMotorCount(int mc)
{
    motorCount = mc;
}

void FpgaControl::setMotorStateIdle()
{
    if(serial.isOpen()){
        QString str("Si\r\n");
     serial.write(str.toLatin1());
    }

    for(int i=0; i<MOTOR_CNT; i++){
        mtState[i] = MT_IDLE;
    }
}

void FpgaControl::setMotorStateGoDown()
{
    for(int i=0; i<MOTOR_CNT; i++)
      mtState[i] = MT_GoDOWN;
}

void FpgaControl::setMotorStateInitiate()
{
    for(int i=0; i<MOTOR_CNT; i++){
        mtState[i] = MT_INIT_GoDOWN;
    }
}

void FpgaControl::setFpgaFreq(quint32 ff)
{
    fpgaFreq = ff;
}

//void FpgaControl::addMotorCmd(int id, DivPosDataStr &ds)
//{
//    motorPosCmdData[id].append(ds);
//}


void FpgaControl::posReset()
{
    //    for(int i=0; i< plotList.size(); i++){
    //        polylist[i]<<  QPointF(polylist[i].length(), rand() % 100 );
    //        curveList[i]->setSamples(polylist[i]);
    //        plotList[i]->replot();
    //    }
    //    if(serial.isOpen()){
    //        QString str("Sr\r\n");
    //     serial.write(str.toLatin1());
    //    }

    for(int i=0; i<MOTOR_CNT; i++){
        motorAbsolutePosCur[i] = 0;
    }
}

//bool FpgaControl::isBufferFree(QByteArray &dt, int id)
//{
//    foreach (char b, dt) {
//        int n = (b>>6)&0x3;
//        if((n==0)||(n==1)){

//        }
//    }
//}

//void FpgaControl::setBufferNotFree(QByteArray &d, int id)
//{

//}

//bool FpgaControl::isTermEna(QByteArray &, int id)
//{

//}


void FpgaControl::parseFPGAMsg(QByteArray ba)
{
//    //qDebug("enter bytesRecv %d", str.length());
//    QMap<char, char> tempStor;
//    foreach (char b, ba) {
//        char n = (b>>6)&0x3;
////        if(tempStor.contains(n))
////            qDebug("in this pack already contains!");
//        tempStor[n] = b/*|tempStor[n]*/;
//    }

//    //сначала обработаем концевики
//    foreach (char b, tempStor){
//        switch((b>>6)&0x3){
//        case 0x2:
//            //qDebug("%02x 1 %02x", b, (b&0x1f));
//            for(int i=0; i<5; i++){
//                terminatorState(i, b&(1<<i));
//            }

//            break;
//        case 0x3:
//            //qDebug("%02x 2 %02x", b, (b&0x1f));
//            for(int i=0; i<5; i++){
//                terminatorState(5+i, b&(1<<i));
//            }
//            break;
//        }
//    }

//     //затем обработаем буферы
//    foreach (char b, tempStor){
//        switch((b>>6)&0x3){
//        case 0x0:
//            for(int i=0; i<5; i++){
//                bFreeToWrite[i] = ((b&(1<<i)) == 0);
//                if((b&(1<<i)) == 0){
//                    freeToWrite(i);
//                }
//            }
//            break;
//        case 0x1:
//            for(int i=0; i<5; i++){
//                bFreeToWrite[5+i] = ((b&(1<<i)) == 0);
//                if((b&(1<<i)) == 0){
//                    freeToWrite(5+i);
//                }
//            }
//            break;
//        }
//    }

//    int size = motorPosCmdData[0].length();

////    for(int i=0; i<MOTOR_CNT; i++){
////        if(size != motorPosCmdData[i].length()){
////            ui->plainTextUDP->appendPlainText("size not equal");
////            for(int k=0; k<MOTOR_CNT; k++){
////                ui->plainTextUDP->appendPlainText(QString("%1").arg(motorPosCmdData[k].length()));
////            }

////        }

////    }

//    bool bAllFree = true;
//    for(int i=0; i<MOTOR_CNT; i++){
//        bAllFree = (bAllFree&&bFreeToWrite[i]);

//    }

////    bool bAllMotorHasCmd = true;
////    for(int i=0; i<motorCount; i++){
////        bAllMotorHasCmd &= (motorPosCmdData[i].isEmpty() == false);
////    }
////    bAllMotorHasCmd = true; // ***
////    if((bAllFree == true) && bAllMotorHasCmd){
////        allFreeToWrite();
////    }
////    else{
////        int curMsec = QTime::currentTime().msecsSinceStartOfDay();
////        qDebug() << (curMsec&0xffff) << "not all free";
////    }

}



void FpgaControl::terminatorState(int i, bool bEna)
{
    if(bTermState[i] != bEna){
        bTermState[i] = bEna;
        emit(termStateChanged(i, bEna));
    }

    if(mtState[i] == MT_INIT_GoDOWN){
        if(bEna == true){
            motorAbsolutePosCur[i] = 0;
        }
        else{
            if(getCmdListLength(i) == 0){
                int pos = getMotorAbsPosImp(i);
                pos -=400;
                addMotorCmd(i, pos, 100);
            }
        }
    }

}


void FpgaControl::freeToWrite(int i)
{
    switch(mtState[i]){
    case MT_GoUP:
//        if(getMotorAbsPosImp(0) < 360000){
//            DivPosDataStr ds;
//            ds.pos = getMotorAbsPosImp(i) + 4560;
//            sendDivPos(i, ds);
//        }
//        else{
//            mtState[i] = MT_GoDOWN;
//        }
        break;

    case MT_GoDOWN:
    case MT_INIT_GoDOWN:
        //DivPosDataStr ds;
        //ds.steps=200;
        //ds.dir
        //sendDivPos(i, ds);

        //break;

    case MT_IDLE:
    case MT_INIT_GoUp:
    {
        if(motorPosCmdData[i].isEmpty())
            break;
        DivPosDataStr &ds = motorPosCmdData[i].first();
            //qDebug("div=%x, st=%d, d=%d", ds.div, ds.steps, ds.dir);
            //            if((ds.pos==0)&&(getMotorAbsPos(i)==0)&&(bTermState[i]==false)){
            //                qDebug("not in zero and no term!");
            //                ds.pos = -400;
            //                sendDivPos(i, ds, ds.pos);
            //            }
            //            else{
        int curMsec = QTime::currentTime().msecsSinceStartOfDay();
        if(ds.steps > 0){
            if(i == 0){
                //qDebug() << (curMsec&0xffff) << ((curMsec-lastDebugShowTime)&0xffff) << ds.msecsFor << "s" << ds.steps << "d" << ds.div ;
                lastDebugShowTime = curMsec;
            }
            sendDivPos(i, ds);
            motorPosCmdData[i].removeFirst();
            //bFreeToWrite[i] = false;
        }
        else{
            if(ds.startTime == 0){
                ds.startTime = curMsec;
                if(i == 0){
                    //qDebug() << (curMsec&0xffff) << ((curMsec-lastDebugShowTime)&0xffff) << ds.msecsFor << "s" << ds.steps << "d" << ds.div << "ws!";
                    lastDebugShowTime = curMsec;
                }                
            }
        }

    }
        break;
    }

    if((mtState[i] == MT_INIT_GoUp) &&
            (motorPosCmdData[i].isEmpty() == true)){
        mtState[i] = MT_IDLE;
        motorAbsolutePosCur[i] = 0;
        bool bState = true;
        for(int k=0; k<MOTOR_CNT; k++){
            bState = ((mtState[k]==MT_IDLE)&&bState);
        }
//        if(bState == true)
//            udpServerOpen();
    }


}

//int lastMsec = 0;
void FpgaControl::sendDivPos(int mi, DivPosDataStr &ds)
{
    quint32 dir = ds.dir;
    quint32 steps = ds.steps;
    //quint32 div = FPGA_FREQ/ (steps*10); //calc speed
    quint32 div = ds.div; //calc speed
    //qDebug() << div <<ds.div;
    if(div > MAX_DIV){
        QString msg;
        msg.sprintf("mi %d div exceed 0x%x!", mi, MAX_DIV);
        qDebug() << qPrintable(msg);
        emit errorOccured(msg);
        div = MAX_DIV;
    }
    if(steps > MAX_STEPS){
        QString msg;
        msg.sprintf("mi %d steps exceed 0x%x!", mi, MAX_STEPS);
        qDebug() << qPrintable(msg);
        emit errorOccured(msg);
        steps = MAX_STEPS;
    }

    quint32 temp = 0;
    temp = mi&0xf;
    temp |= ((div&DIV_MASK)<<4);
    temp |= (((quint64)steps&STEPS_MASK)<<19);
    temp |= (((quint64)dir&0x1)<<31);
    QByteArray ba = QByteArray::fromRawData((char*)&temp, sizeof(quint32));

    //ba.resize(5);



    //    //ba[0] = 0x09;
    //    ba[0] = mi&0xf;
    //    ba[1] = 0xc0;
    //    ba[2] = 0x38;
    //    ba[3] = 0x00;
    //    ba[4] = 0x00;

    //if(mi == 0){
        //int msec = QTime::currentTime().msecsSinceStartOfDay();
        //qDebug("%d mi=%d st=%d", msec-lastMsec, mi, steps);
        //lastMsec = msec;
        //qDebug("pos=%d steps=%d div=%d dir=%d", pos, steps, div, dir);
    //}
    quint64 dSend = serial.write(ba);

//    if(dSend != 5){
//        qDebug("dSend %d !!!!!", dSend);
//    }
    //if(serial.flush() == false){
    //qDebug("dataFlushed");
    //}

    if(bDirInvers)
        motorAbsolutePosCur[mi] += ds.dir? -ds.steps: +ds.steps;
    else
        motorAbsolutePosCur[mi] += ds.dir? +ds.steps: -ds.steps;

    if( motorAbsolutePosCur[mi] < 0){
        //qDebug("mi %d motorAbsolutePosCur less 0 =", mi, motorAbsolutePosCur[mi]);
    }
}

//motorPosCmdData not empty
void FpgaControl::allFreeToWrite()
{
//    bool bAllMtStop = true;
//    for(int i=0; i<motorCount; i++){
//        if((mtState[i]==MT_IDLE) && (motorPosCmdData[i].isEmpty() == false)){
//            int delta = motorPosCmdData[i].first().pos - getMotorAbsPosImp(i);
//            bAllMtStop &= (delta==0);
//        }
//        else{
//            bAllMtStop = false;
//            break;
//        }
//    }

//    if(bAllMtStop){
//        int absPosMsec = motorPosCmdData[0].first().absMsec;
//        int curMsec = QTime::currentTime().msecsSinceStartOfDay();
//        for(int i=0; i<motorCount; i++){
//            if(absPosMsec != motorPosCmdData[i].first().absMsec){
//                ui->plainTextUDP->appendPlainText(QString("%1 absPosMsec not equal at all on %2").arg(curMsec).arg(motorPosCmdData[i].length()));
//                for(int k=0; k<motorCount; k++){
//                    ui->plainTextUDP->appendPlainText(QString("%1").arg(motorPosCmdData[k].length()));
//                }
//                break;
//            }
//        }
//        int shift = absPosMsec - curMsec;

//        ui->plainTextUDP->appendPlainText(QString("%1 shift %2").arg(curMsec).arg(shift));
//        if( (abs(shift) <=100) ||
//                (shift <0) ){        //если проигрывание отстаёт
////            for(int i=0; i<MOTOR_CNT; i++){
////                motorPosCmdData[i].removeFirst();  // то прибьём текущий шаг
////            }
//            ui->plainTextUDP->appendPlainText(QString("%1 skip").arg(curMsec));
//        }
//        else{ //если проигрывание опережает то не управляем двигателем
//            ui->plainTextUDP->appendPlainText(QString("%1 wait").arg(curMsec));
//            return;
//        }


//    }

    for(int i=0; i<motorCount; i++){
        switch(mtState[i]){
        case MT_IDLE:
        case MT_INIT_GoUp:
        {
            if(motorPosCmdData[i].isEmpty())
                continue;
            DivPosDataStr &ds = motorPosCmdData[i].first();
                //qDebug("div=%x, st=%d, d=%d", ds.div, ds.steps, ds.dir);
                //            if((ds.pos==0)&&(getMotorAbsPos(i)==0)&&(bTermState[i]==false)){
                //                qDebug("not in zero and no term!");
                //                ds.pos = -400;
                //                sendDivPos(i, ds, ds.pos);
                //            }
                //            else{
            int curMsec = QTime::currentTime().msecsSinceStartOfDay();
            if(ds.steps > 0){
                if(i == 0){
                    qDebug() << (curMsec&0xffff) << ((curMsec-lastDebugShowTime)&0xffff) << ds.msecsFor << "s" << ds.steps << "d" << ds.div ;
                    lastDebugShowTime = curMsec;
                }
                sendDivPos(i, ds);
                motorPosCmdData[i].removeFirst();
                //bFreeToWrite[i] = false;
            }
            else{
                if(ds.startTime == 0){
                    if(i == 0){
                        qDebug() << (curMsec&0xffff) << ((curMsec-lastDebugShowTime)&0xffff) << ds.msecsFor << "s" << ds.steps << "d" << ds.div << "ws!";
                        lastDebugShowTime = curMsec;
                    }
                    ds.startTime = curMsec;
                }
            }
        }
            break;
        default:
            break;
        }
    }
}

void FpgaControl::handleComPortErrorOccurred(QSerialPort::SerialPortError error)
{
    if(error != QSerialPort::NoError){
        qDebug() << "handleErrorOccurred" << error;

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

        QString msg = QString("fpga %1 error: %2").arg(qUtf8Printable(serial.portName())).arg(errorStr);
        emit errorOccured(msg);

        //qDebug() <<"!!!!!!!" << id <<error;
        if((error == QSerialPort::ResourceError) ||
           (error == QSerialPort::PermissionError)){
            //pushButtonComOpen_clicked(id);
            closePort();
        }
    }
}

void FpgaControl::handleReadyRead()
{
//    QTime t1, t2, t3;
//    t1.start();
//    t2.start();
//    t3.start();

    recvInterval = exchInterval.elapsed();

    quint32 curMsec = QTime::currentTime().msecsSinceStartOfDay();
    //qDebug() << curMsec << "handleReadyRead" << recvInterval;

    QByteArray dArr = serial.readAll();

    bytesOnIter = dArr.length();
    if(bytesOnIter != 4){
        //qDebug() << "handleReadyRead !!! ERROR !!!" << dArr.length();
        QString msg = QString("handleReadyRead !!! ERROR !!! err %1").arg(dArr.length());
        emit errorOccured(msg);
    }

//    int bytesToRecv = 4;
//    char dArr[4];
//    while(bytesToRecv>0){
//        //qDebug("try to recv %d bytes", bytesToRecv);
//        if(serial.waitForReadyRead(50) ==false){
//            //qDebug("dataRecvTimeout on recvd %d bytes", 4-bytesToRecv);
//            //ui->plainTextUDP->appendPlainText(QString("dataRecvTimeout on recvd %1 bytes").arg(4-bytesToRecv));
//            return;
//        }
//        int dRecvd = serial.read(&(dArr[4-bytesToRecv]), bytesToRecv);
//        bytesToRecv -= dRecvd;
//    }

    QBitArray isBufFree(MOTOR_CNT);

    foreach (char b, dArr){
        switch((b>>6)&0x3){
        case 0x0:
            for(int i=0; i<5; i++){
                isBufFree[i] = ((b&(1<<i)) == 0);
            }
            break;
        case 0x1:
            for(int i=0; i<5; i++){
                isBufFree[5+i] = ((b&(1<<i)) == 0);
            }
            break;
        case 0x2:
            //qDebug("%02x 1 %02x", b, (b&0x1f));
            for(int i=0; i<5; i++){
                terminatorState(i, b&(1<<i));
            }
            break;
        case 0x3:
            //qDebug("%02x 2 %02x", b, (b&0x1f));
            for(int i=0; i<5; i++){
                terminatorState(5+i, b&(1<<i));
            }
            break;
        }
    }

    bool bAllDown = true;
    for(int id=0; id<motorCount; id++){
        bAllDown = (bAllDown&&bTermState[id]&&(getCmdListLength(id) == 0));
    }
    if(bAllDown == true){
        for(int id=0; id<motorCount; id++){
            mtState[id] = MT_INIT_GoUp;
            int pos = getMotorAbsPosImp(id);
            pos +=400;
            for(int k=0; k<20; k++){
                addMotorCmd(id, pos, 100);
                pos +=400;
            }
        }
    }

//    for(int id=0; id<motorCount; id++){
//        if(mtState[id] = MT_INIT_GoUp){
//            if(motorPosCmdData[id].isEmpty()){
//                motorPosCmdData[id].clear();
//                mtState[id] = MT_IDLE;
//            }
//        }
//    }

    //тут подождём
    for(int i=0; i<motorCount; i++){
        if(motorPosCmdData[i].isEmpty())
            continue;
        DivPosDataStr &ds = motorPosCmdData[i].first();
        if(ds.steps == 0){
            if(ds.startTime != 0 ){
                //if(i == 0) qDebug() << (curMsec&0xffff) << "external waiting" << ds.msecsFor;
                //if(curMsec >= ds.finishAbsTimeMsec){
                int curDelta = curMsec - ds.startTime;
                if(curDelta >= ds.msecsFor ){
                    motorPosCmdData[i].removeFirst();
                }
                else{
                    isBufFree[i] = false;
                    if(i == 0){
                        if(isBufFree[i]){
                            int nt = (ds.div*ds.steps*1000)/fpgaFreq;
                            qDebug()<<(curMsec&0xffff) << "waiting" << ds.msecsFor << "curDelt" << curDelta
                                   << "calced" << nt;
                        }
                    }
                }
            }
        }
    }

    for(int i=0; i<MOTOR_CNT; i++){
        if(isBufFree[i] == true)
            freeToWrite(i);
    }

    //parseFPGAMsg(dArr);

    char c = 0xff;
    serial.write(&c, 1);
    exchInterval.start();
    exchTimeoutTimer.start();
    comExchanges++;
}

void FpgaControl::handleExchTimeout()
{
    recvInterval = -1;
    QString msg("fpga exch timeout, no data");
    emit errorOccured(msg);
    char c = 0xff;
    serial.write(&c, 1);
}

void FpgaControl::handleSerialDataWritten(qint64 bytes)
{
    //QDateTime current = QDateTime::currentDateTime();
    //uint msecs = setTime.time().msecsTo(current.time());
    //current.currentMSecsSinceEpoch()

   // qDebug() <<  QTime::currentTime().msecsSinceStartOfDay()  << " " << "handleSerialDataWritten";
    return;
//    //qDebug("handleSerialDataWritten %d", bytes);
//    if(serial.isOpen()){

//        for(int i=0; i<MOTOR_CNT; i++){
//            if((mtstr[i].sendState == idle) &&
//                (mtstr[i].contrStringQueue.isEmpty() == false) &&
//                (i == curMotorSendIdx)){
//                QString wrStr = contrStringQueue.head();
//                //qDebug("%s", qPrintable(wrStr));
//                serial.write(wrStr.toLatin1());
//                mtstr[i].sendState = waitForAck;
//            }

//        }
//        while(true){
//            TMotorStr *pMst = &(mtstr[curMotorSendIdx]);
//            if((pMst->sendState == idle) && (pMst->contrStringQueue.isEmpty() == false)){
//                QString wrStr = contrStringQueue.head();
//                //qDebug("%s", qPrintable(wrStr));
//                serial.write(wrStr.toLatin1());
//            }

////            if(mtstr[i].contrStringQueue.isEmpty()){
////                noDataSending = true;
////            }
////            else{
////                if(bCmdOk){
////                    qDebug() << "last send OK, send next";
////                    contrStringQueue.dequeue();
////                    mtstr[i].bCmdOk = false;
////                }
////               else{
////                    qDebug() << "last send Fail, repeat ";
////                }
////               if(contrStringQueue.isEmpty()){
////                    noDataSending = true;
////                }
////                else{
////                    QString wrStr = contrStringQueue.head();
////                    //qDebug("%s", qPrintable(wrStr));
////                    serial.write(wrStr.toLatin1());
////                }
////            }

//            curMotorSendIdx++;
//            if(curMotorSendIdx >= MOTOR_CNT){
//                curMotorSendIdx = 0;

//            }
//        }
//    }

////    if(timerSerialSendTo[lastMotorReceptInd]){

////    }

}

void FpgaControl::addRawCmd(int id, quint32 d, quint32 st, int dir)
{
    DivPosDataStr ds;
    ds.div = d;
    ds.dir = dir;
    ds.steps = st;
    motorPosCmdData[id].append(ds);

}

void FpgaControl::addMotorCmd(int id, int newPosImp, int msecsForMove)
{
    DivPosDataStr ds;
    ds.finishPos = newPosImp;
    quint32 curmSecs = QTime::currentTime().msecsSinceStartOfDay();

    int delta = 0;
    if(motorPosCmdData[id].isEmpty() == true){
//        if(id == 0){
//            qDebug() << "ql epmty"<< "np" << newPosImp << "cp" << getMotorAbsPosImp(id);
//        }
        delta = newPosImp - getMotorAbsPosImp(id);
    }
    else{        
//        if(id == 0){
//            qDebug() << "ql" << motorPosCmdData[id].length() << "np" << newPosImp << "cp" << getMotorAbsPosImp(id)<< "fp" << motorPosCmdData[id].last().finishPos;
//        }
        delta = newPosImp - motorPosCmdData[id].last().finishPos;
    }


    float secsOnMove = msecsForMove/1000.;
    quint32 freqOnMove = fpgaFreq*secsOnMove;
    ds.startTime = 0;
    ds.dir = delta > 0? 1: 0;
    if(bDirInvers)
        ds.dir = delta > 0? 0: 1;

    ds.steps =  abs(delta);
    ds.msecsFor = msecsForMove;


    if(ds.steps > maxSteps_debug){
        maxSteps_debug = ds.steps;
        QString msg;
        msg.sprintf("new max steps == %d", maxSteps_debug);
        //qDebug() << qPrintable(msg);
        emit errorOccured(msg);
    }

    int dt = 0;
    if(delta != 0){
        //float secsOnStep = msecsForMove / (ds.steps*1000.);
        //if(mn==0) qDebug() << "secsOnStep" << secsOnStep;
        ds.div = (quint32)(freqOnMove/ds.steps);
        if(ds.div > maxDiv_debug){
            maxDiv_debug = ds.div;
            QString msg;
            msg.sprintf("new max div == 0x%x", maxDiv_debug);
            emit errorOccured(msg);
            //qDebug() << qPrintable(msg);
        }
        if(ds.div > MAX_DIV){
            ds.div = MAX_DIV;
            int nt = (ds.div*ds.steps*1000)/fpgaFreq;
            dt = msecsForMove - nt;
            if(id==0){
                QString msg;
                msg.sprintf("maxSpeed err %x, msecsForMove %d, newTime %d, delta %d", ds.div, msecsForMove, nt, dt);
                emit errorOccured(msg);
                //qDebug() << qPrintable(msg);
            }
        }
    }
    else{

        if(id == 0){
            QString msg;
            msg.sprintf("delta 0, msecsForMove %d", msecsForMove );
            emit errorOccured(msg);
            //qDebug()<< "ms:" <<msecsForMove << "s:" <<delta << "d:" << qPrintable(QString("0x")+QString::number(ds.div, 16));
        }
        return;
    }



    if(ds.steps > MAX_STEPS){
        int steps = ds.steps;
        int pts = 1;
        if(id==0){
            QString msg;
            msg.sprintf("maxSteps err st: %d, msecsForMove %d", steps, msecsForMove);
            emit errorOccured(msg);
            qDebug() << qPrintable(msg);
        }
        while(steps>MAX_STEPS){
            steps /= 2;
            pts *= 2;
        }
        for(int i=0; i<pts; i++){
            ds.steps = steps;
            motorPosCmdData[id].append(ds);
            if(id==0){
                QString msg;
                msg.sprintf("maxSteps err addPt st %d", ds.steps);
                emit errorOccured(msg);
                qDebug() << qPrintable(msg);
            }
        }
    }
    else{
        motorPosCmdData[id].append(ds);
    }

    if(dt > 0){
        ds.steps = 0;
        ds.msecsFor = dt;
        motorPosCmdData[id].append(ds);
        if(id==0){
            QString msg("added Pt");
            emit errorOccured(msg);
            //qDebug("added Pt");
        }
    }
}

void FpgaControl::clearCmdList()
{
    for(int i=0; i<MOTOR_CNT; i++){
        motorPosCmdData[i].clear();
    }
}


