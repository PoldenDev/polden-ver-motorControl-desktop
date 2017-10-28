#include "fpgacontrol.h"
#include <QDebug>
#include <QTime>

FpgaControl::FpgaControl(QObject *parent) :
    QObject(parent),
    motorCount(0),
    bytesOnIter(0),
    comExchanges(0),
    fpgaFreq(FPGA_FREQ_25),
    bDirInvers(false)
{
    for(int i=0; i<MOTOR_CNT; i++){
        bTermState[i] = false;
        mtState[i] = MT_IDLE;
        bFreeToWrite[i] = false;
        motorAbsolutePosCur[i] = 0;
        //lastCtrlTimeMsecs[i] = 0;
    }

    connect(&serial, SIGNAL(errorOccurred(QSerialPort::SerialPortError)),
            this, SLOT(handleErrorOccurred(QSerialPort::SerialPortError)));

    timer.setInterval(10);
    connect(&timer, SIGNAL(timeout()), this, SLOT(handleExchTimer()));
    timer.start();

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
    return true;
}

void FpgaControl::closePort()
{
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

void FpgaControl::handleExchTimer()
{
    //    if(serial.isOpen() && (bFreeToWrite[curMotorSendIdx] == true)){
    //        //QString wrStr = motorPosCmdStrings.dequeue();
    //        //serial.write(wrStr.toLatin1());
    //        freeToWrite(curMotorSendIdx);

    //    }
    //    curMotorSendIdx++;
    //    if(curMotorSendIdx >= MOTOR_CNT)
    //        curMotorSendIdx = 0;


        if(serial.isOpen() == false)
            return;
        //qDebug("%d sendOnTimer", QTime::currentTime().msecsSinceStartOfDay()&0xfff );

        //тут подождём
        int curMsec = QTime::currentTime().msecsSinceStartOfDay();
        for(int i=0; i<motorCount; i++){
            if(motorPosCmdData[i].isEmpty())
                continue;
            DivPosDataStr &ds = motorPosCmdData[i].first();
            if(ds.steps == 0){
                if(ds.skipStartTime != 0 ){
                    //if(i == 0) qDebug() << (curMsec&0xffff) << "external waiting" << ds.msecsFor;
                    //if(curMsec >= ds.finishAbsTimeMsec){
                    if((curMsec - ds.skipStartTime) >= ds.msecsFor ){
                        motorPosCmdData[i].removeFirst();
                    }
                }
            }
        }
    //    QTime tt;
    //    tt.start();
        char c = 0xff;
        serial.write(&c, 1);

        int bytesToRecv = 4;
        char dArr[4];
        while(bytesToRecv>0){
            //qDebug("try to recv %d bytes", bytesToRecv);
            if(serial.waitForReadyRead(50) ==false){
                //qDebug("dataRecvTimeout on recvd %d bytes", 4-bytesToRecv);
                //ui->plainTextUDP->appendPlainText(QString("dataRecvTimeout on recvd %1 bytes").arg(4-bytesToRecv));
                return;
            }
            int dRecvd = serial.read(&(dArr[4-bytesToRecv]), bytesToRecv);
            bytesToRecv -= dRecvd;
        }


        comExchanges++;
        parseFPGAMsg(QByteArray(dArr, 4));
    //    qDebug("%d data recvd in %d ms", QTime::currentTime().msecsSinceStartOfDay()&0xfff ,
    //           tt.elapsed());

}


void FpgaControl::parseFPGAMsg(QByteArray ba)
{
    bytesOnIter = ba.length();
    //qDebug("enter bytesRecv %d", str.length());
    QMap<char, char> tempStor;
    foreach (char b, ba) {
        char n = (b>>6)&0x3;
//        if(tempStor.contains(n))
//            qDebug("in this pack already contains!");
        tempStor[n] = b/*|tempStor[n]*/;
    }

    //сначала обработаем концевики
    foreach (char b, tempStor){
        switch((b>>6)&0x3){
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

     //затем обработаем буферы
    foreach (char b, tempStor){
        switch((b>>6)&0x3){
        case 0x0:
            for(int i=0; i<5; i++){
                bFreeToWrite[i] = ((b&(1<<i)) == 0);
                if((b&(1<<i)) == 0){
                    freeToWrite(i);
                }
            }
            break;
        case 0x1:
            for(int i=0; i<5; i++){
                bFreeToWrite[5+i] = ((b&(1<<i)) == 0);
                if((b&(1<<i)) == 0){
                    freeToWrite(5+i);
                }
            }
            break;
        }
    }

    int size = motorPosCmdData[0].length();

//    for(int i=0; i<MOTOR_CNT; i++){
//        if(size != motorPosCmdData[i].length()){
//            ui->plainTextUDP->appendPlainText("size not equal");
//            for(int k=0; k<MOTOR_CNT; k++){
//                ui->plainTextUDP->appendPlainText(QString("%1").arg(motorPosCmdData[k].length()));
//            }

//        }

//    }

    bool bAllFree = true;
    for(int i=0; i<MOTOR_CNT; i++){
        bAllFree = (bAllFree&&bFreeToWrite[i]);

    }

//    bool bAllMotorHasCmd = true;
//    for(int i=0; i<motorCount; i++){
//        bAllMotorHasCmd &= (motorPosCmdData[i].isEmpty() == false);
//    }
//    bAllMotorHasCmd = true; // ***
//    if((bAllFree == true) && bAllMotorHasCmd){
//        allFreeToWrite();
//    }
//    else{
//        int curMsec = QTime::currentTime().msecsSinceStartOfDay();
//        qDebug() << (curMsec&0xffff) << "not all free";
//    }

}



void FpgaControl::terminatorState(int i, bool bEna)
{
    if(bTermState[i] != bEna){
        bTermState[i] = bEna;
        emit(termStateChanged(i, bEna));
    }

    if(bEna == true){
        motorAbsolutePosCur[i] = 0;
    }
    if(mtState[i] == MT_GoDOWN){
        if(bEna == false){
        }
        else{
            motorAbsolutePosCur[i] = 0;
        }

    }
    else if((mtState[i] == MT_INIT_GoDOWN) && (bEna == true)){
//         motorAbsolutePosCur[i] = 0;

//        //mtState[i] = MT_IDLE;
//        mtState[i] = MT_INIT_GoUp;
//        DivPosDataStr ds;
//        int startPos =0;

//        if(motorPosCmdData[i].length() == 0)
//            startPos = getMotorAbsPosImp(i);
//        else
//            startPos = motorPosCmdData[i].last().pos;

//        ds.pos = startPos+400;
//        for(int k=0; k<40; k++){
//            motorPosCmdData[i] << ds;
//             ds.pos += 400;
//        }

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
//        DivPosDataStr ds;
//        if(speedTrig[i]){
//            ds.pos = getMotorAbsPosImp(i)-500;
//            speedTrig[i] = false;
//        }
//        else{
//            ds.pos = getMotorAbsPosImp(i)-10;
//            speedTrig[i] = true;
//        }
//        ds.pos = getMotorAbsPosImp(i)-500;
//        sendDivPos(i, ds);

        break;

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
            bFreeToWrite[i] = false;
        }
        else{
            if(ds.skipStartTime == 0){
                if(i == 0){
                    //qDebug() << (curMsec&0xffff) << ((curMsec-lastDebugShowTime)&0xffff) << ds.msecsFor << "s" << ds.steps << "d" << ds.div << "ws!";
                    lastDebugShowTime = curMsec;
                }
                ds.skipStartTime = curMsec;
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
    if(div > 0x1fff){
        qDebug("mi %d div exceed 0x1fff!", mi);
        div = 0x1fff;
    }
    if(steps > 0x7fff){
        qDebug("mi %d steps exceed 0x7fff!", mi);
        steps = 0x7fff;
    }


    quint64 temp = 0;
    temp = mi&0xf;
    temp |= ((div&0x7fff)<<4);
    temp |= (((quint64)steps&0x7fff)<<19);
    temp |= (((quint64)dir&0x1)<<34);
    QByteArray ba = QByteArray::fromRawData((char*)&temp, sizeof(quint64));

    ba.resize(5);



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

    if(dSend != 5){
        qDebug("dSend %d !!!!!", dSend);
    }
    //if(serial.flush() == false){
    //qDebug("dataFlushed");
    //}
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
                bFreeToWrite[i] = false;
            }
            else{
                if(ds.skipStartTime == 0){
                    if(i == 0){
                        qDebug() << (curMsec&0xffff) << ((curMsec-lastDebugShowTime)&0xffff) << ds.msecsFor << "s" << ds.steps << "d" << ds.div << "ws!";
                        lastDebugShowTime = curMsec;
                    }
                    ds.skipStartTime = curMsec;
                }
            }
        }
            break;
        default:
            break;
        }
    }
}

void FpgaControl::handleErrorOccurred(QSerialPort::SerialPortError err)
{
    qDebug() << "handleErrorOccurred" << err;
}

void FpgaControl::handleReadyRead()
{

    QByteArray str = serial.readAll();

//    uartBuff += str;
//    //uartBuff.split("\r\n");
//    bool bAcked = false;
//    int lfInd = uartBuff.indexOf("\r\n");
//    if(lfInd != -1){
//        QString cmd = uartBuff.left(lfInd+2);
//        uartBuff.remove(0, lfInd+2);

//        bool bOk;
//        //qDebug() << cmd.mid(11, 4);
//        markerXPos = cmd.mid(11, 4).toInt(&bOk, 16);
//        qDebug() << "corr >> "<< markerXPos;
//        for(int i=0; i<markrlist.length(); i++){
//            markrlist[i]->setXValue(markerXPos);
//            plotList[i]->replot();
//        }
//    }

    //processUartRecvExchange(cmd);





//        for(int i=0; i<MOTOR_CNT; i++){
//            if(mtState[i] == MT_GoUP){
//                if(motorAbsolutePos[i] < 360000){
//                    DivPosDataStr ds;
//                    //ds.div = 0x4fff;
//                    ds.div = 500;
//                    ds.dir = 1;
//                    //ds.pos = 1;
//                    ds.steps = 4560;
//                    motorPosCmdData[i] << ds;
//                    motorAbsolutePos[i] += 4560;
//                }
//                else{
//                    mtState[i] = MT_GoDOWN;
//                }
//            }
//        }

    //qDebug() << str;
    //ui->plainTextEdit->moveCursor (QTextCursor::End);
    //ui->plainTextEdit->insertPlainText(str);
    //ui->plainTextEdit->insertPlainText(QString(t.msecsSinceStartOfDay()));
    //ui->plainTextEdit->moveCursor (QTextCursor::End);
    //ui->plainTextEdit->appendPlainText(str);

    //ui->plainTextEdit->verticalScrollBar()->setValue(ui->plainTextEdit->verticalScrollBar()->maximum());
}


void FpgaControl::calcCmd(DivPosDataStr &ds, int delta, quint32 curmSecs, quint32 msecsForMove, int id)
{
    float secsOnMove = msecsForMove/1000.;
    quint32 freqOnMove = fpgaFreq*secsOnMove;
    ds.skipStartTime = 0;
    ds.dir = delta > 0? 1: 0;
    if(bDirInvers)
        ds.dir = delta > 0? 0: 1;

    ds.steps =  abs(delta);
    ds.msecsFor = msecsForMove;
    int dt = 0;
    if(delta == 0){
        ds.finishAbsTimeMsec = curmSecs + 100;
    }
    else{
        //float secsOnStep = msecsForMove / (ds.steps*1000.);
        //if(mn==0) qDebug() << "secsOnStep" << secsOnStep;
        ds.div = (quint32)(freqOnMove/ds.steps);
        if(ds.div > 0x1fff){
            ds.div = 0x1fff;
            int nt = (ds.div*ds.steps*1000)/fpgaFreq;
            dt = msecsForMove - nt;
            if(id==0){
                //qDebug("maxSpeed err %x, msecsForMove %d, newTime %d, delta %d", ds.div, msecsForMove, nt, dt);
            }
        }
    }
    motorPosCmdData[id].append(ds);
    if(dt > 0){
        ds.steps = 0;
        ds.msecsFor = dt;
        motorPosCmdData[id].append(ds);
        //qDebug("added Pt");
    }
}

void FpgaControl::addMotorCmd(int mn, int newPosImp, int msecsForMove)
{
    DivPosDataStr ds;
    ds.finishPos = newPosImp;
    quint32 curmSecs = QTime::currentTime().msecsSinceStartOfDay();

    if(motorPosCmdData[mn].length() == 0){
        quint32 curPos = getMotorAbsPosImp(mn);
        int delta = newPosImp - curPos;
        //int vmaxMmsec = ui->lineEdit_vmax_mmsec->text().toInt();
        //int maxImpPerDelta = mmToImp(vmaxMmsec)/10;
        //qDebug("%d", maxImpPerDelta);

        int maxImpPerDelta = 10000;

        int n=1;
        for(; abs(delta)>maxImpPerDelta; n*=2){
            delta/=2;
        }
        if(n>1){
            //qDebug("%d add %d Pts %d", mn, n, delta);
//            ui->plainTextUDP->appendPlainText(QString("%1 add %2 Pts,delta=%3")
//                                              .arg(mn)
//                                              .arg(n)
//                                              .arg(delta));
        }

        for(int i=0; i<n; i++){
            ds.finishPos = curPos;
            curPos += delta;
            calcCmd(ds, delta, curmSecs, msecsForMove, mn);
            //motorPosCmdData[mn].append(ds);
        }
    }
    else{
        int delta = newPosImp - motorPosCmdData[mn].last().finishPos;
        //if(delta != 0)
        //    qDebug("%d st=%d div=%d, dir=%d", mn, delta, (qint32)FPGA_FREQ/(delta*10), delta/abs(delta));
        calcCmd(ds, delta, curmSecs, msecsForMove, mn);
        //motorPosCmdData[mn].append(ds);
    }
}

void FpgaControl::clearCmdList()
{
    for(int i=0; i<MOTOR_CNT; i++){
        motorPosCmdData[i].clear();
    }
}

