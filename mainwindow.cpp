#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtDebug>
#include <QtSerialPort/QSerialPortInfo>
#include <QScrollBar>
#include <QList>
#include <QNetworkDatagram>
#include <QThread>
#include <QDateTime>
#include <QTime>


#include <qwt_plot.h>
#include <qwt_plot_grid.h>

#include <qwt_legend.h>

#include <qwt_plot_curve.h>
#include <qwt_symbol.h>

#include <qwt_plot_magnifier.h>

#include <qwt_plot_panner.h>

#include <qwt_plot_picker.h>
#include <qwt_picker_machine.h>
#include <qwt_plot_marker.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    speed(100),
    udpSocket(NULL),    
    cmdNum(0),
    udpCnt(0),
    curMotorSendIdx(0),
    settings("murinets", "vertolet"),
    xUdpRecv(0),
    markerXPos(0)/*,
    timeShiftMaxPos(2000),
    timeShiftMaxNeg(-2000)*/
{
    ui->setupUi(this);

    ui->lineEditMaxVal->setText(QString::number(settings.value("maxPosValue", 1000).toInt()));
    ui->lineEditMinVal->setText(QString::number(settings.value("minPosValue", 0).toInt()));

    on_pushButton_refreshCom_clicked();

    initUdpSocket();
    timer.setInterval(50);
    connect(&timer, SIGNAL(timeout()), this, SLOT(sendOnTimer()));
    timer.start();

    dataProcess100msTimer.setInterval(90);
    connect(&dataProcess100msTimer, SIGNAL(timeout()), this, SLOT(dataProcess100msTimeOut()));

//    slList.append(ui->verticalMotorPos10);
//    slList.append(ui->verticalMotorPos9);
//    slList.append(ui->verticalMotorPos8);
//    slList.append(ui->verticalMotorPos7);
//    slList.append(ui->verticalMotorPos6);
//    slList.append(ui->verticalMotorPos5);
//    slList.append(ui->verticalMotorPos4);
//    slList.append(ui->verticalMotorPos3);
//    slList.append(ui->verticalMotorPos2);
//    slList.append(ui->verticalMotorPos1);

    connect(&waitForFifoFreeTimer, SIGNAL(timeout()), this, SLOT(waitForFifoFreeFifo()));

    //connect(&UartThread, SIGNAL(response(QString)), this, SLOT(response(QString)));

    QWidget *tabWdg;
    for(int i=0; i<10; i++){
        createPlot(QString::number(i));
        tabWdg = new QWidget();
        tabWdg->setLayout(new QVBoxLayout());
        ui->tabWidget->addTab(tabWdg, QString::number(i));
    }


    for(int i=0; i<5; i++){
        ui->qwtWdgH1->layout()->addWidget(plotList[i]);
    }

    for(int i=0; i<5; i++){
        ui->qwtWdgH2->layout()->addWidget(plotList[5+i]);
    }


    for(int i=0; i<MOTOR_CNT; i++){
        mtState[i] = MT_IDLE;
    }

    for(int i=0; i<MOTOR_CNT; i++){
//        timerSerialSendTo[i].setSingleShot(true);
//        connect(&(timerSerialSendTo[i]), SIGNAL(timeout()),
//                this, SLOT(sendTimeOut));
        motorAbsolutePosCur[i] = 0;
        lastCtrlTimeMsecs[i] = 0;
    }

    ui->widget_10->layout()->setAlignment(Qt::AlignHCenter);
    ui->widget_11->layout()->setAlignment(Qt::AlignHCenter);

    timeStatSlider.append(ui->timeShift_0);
    timeStatSlider.append(ui->timeShift_1);
    timeStatSlider.append(ui->timeShift_2);
    timeStatSlider.append(ui->timeShift_3);
    timeStatSlider.append(ui->timeShift_4);
    timeStatSlider.append(ui->timeShift_5);
    timeStatSlider.append(ui->timeShift_6);
    timeStatSlider.append(ui->timeShift_7);
    timeStatSlider.append(ui->timeShift_8);
    timeStatSlider.append(ui->timeShift_9);

    timeStatLE.append(ui->lineEditTimeShift_0);
    timeStatLE.append(ui->lineEditTimeShift_1);
    timeStatLE.append(ui->lineEditTimeShift_2);
    timeStatLE.append(ui->lineEditTimeShift_3);
    timeStatLE.append(ui->lineEditTimeShift_4);
    timeStatLE.append(ui->lineEditTimeShift_5);
    timeStatLE.append(ui->lineEditTimeShift_6);
    timeStatLE.append(ui->lineEditTimeShift_7);
    timeStatLE.append(ui->lineEditTimeShift_8);
    timeStatLE.append(ui->lineEditTimeShift_9);

    uiUpdateTimer.setInterval(500);
    connect(&uiUpdateTimer, SIGNAL(timeout()), this, SLOT(uiUpdateTimerSlot()));
    uiUpdateTimer.start();


}
void MainWindow::createPlot(QString name)
{
    QwtPlot *d_plot = new QwtPlot(this);    
    plotList << d_plot;
    //d_plot->setTitle( "Qwt demonstration" ); // заголовок
    //d_plot->setCanvasBackground( Qt::white ); // цвет фона

    d_plot->setMinimumSize(50, 50);
    // Параметры осей координат
    //d_plot->setAxisTitle(QwtPlot::yLeft, "Y");
    d_plot->setAxisScale(QwtPlot::yLeft, 0, 100);
    //d_plot->setAxisTitle(QwtPlot::xBottom, "X");
    //d_plot->setAxisScale(QwtPlot::xBottom, 0, 100);

    d_plot->setAxisScale(QwtPlot::yRight, -100, 100);
    //d_plot->insertLegend( new QwtLegend() );
    d_plot->enableAxis(QwtPlot::yRight);



    // Включить сетку
    // #include <qwt_plot_grid.h>
    QwtPlotGrid *gridL = new QwtPlotGrid(); //
    gridL->setMajorPen(QPen( Qt::black, 1 )); // цвет линий и толщина
    gridL->attach( d_plot ); // добавить сетку к полю графика

    QwtPlotGrid *gridR = new QwtPlotGrid(); //
    gridR->setMajorPen(QPen( Qt::gray, 1 )); // цвет линий и толщина
    gridR->setYAxis(QwtPlot::yRight);
    gridR->attach( d_plot ); // добавить сетку к полю графика


    QwtPlotCurve *curve = new QwtPlotCurve();
    posCurveList << curve;
    //curve->setTitle( name );
    curve->setPen( Qt::blue, 2 ); // цвет и толщина кривой
//        curve->setRenderHint
//                ( QwtPlotItem::RenderAntialiased, true ); // сглаживание
//        QwtSymbol *symbol = new QwtSymbol( QwtSymbol::Ellipse,
//            QBrush( Qt::yellow ), QPen( Qt::red, 2 ), QSize( 8, 8 ) );
//        curve->setSymbol( symbol );

    //ui->widget_3->layout()->invalidate();

//        points << QPointF( 1.0, 1.0 ) // координаты x, y
//     << QPointF( 1.5, 2.0 ) << QPointF( 3.0, 2.0 )
//     << QPointF( 3.5, 3.0 ) << QPointF( 5.0, 3.0 );

  //curve->setSamples( points ); // ассоциировать набор точек с кривой

  curve->attach( d_plot ); // отобразить кривую на графике

    curve = new QwtPlotCurve();
    velCurveList << curve;
    curve->setPen( Qt::red, 2 ); // цвет и толщина кривой
    curve->setYAxis(QwtPlot::yRight);

    curve->attach(d_plot ); // отобразить кривую на графике



  polyPoslist << QPolygonF();
  polyVellist << QPolygonF();
  x=0;


   QwtSymbol *markerSymbol = new QwtSymbol(QwtSymbol::VLine,QBrush(Qt::red),QPen(Qt::red),QSize(50,5000));
    QwtPlotMarker *mark=new QwtPlotMarker;
    markrlist << mark;
    mark->setSymbol(markerSymbol);
    mark->setXValue(0);//here you have to set the coordinate axis i.e. where the axis are meeting.
    mark->attach(d_plot);
}

MainWindow::~MainWindow()
{
    settings.setValue("maxPosValue", ui->lineEditMaxVal->text().toInt());
    settings.setValue("minPosValue", ui->lineEditMinVal->text().toInt());
    delete ui;
}

void MainWindow::sendTimeOut()
{
    qDebug()<< qPrintable("timeout");

}

void MainWindow::dataProcess100msTimeOut()
{
    for(int i=0; i<markrlist.length(); i++){
        markrlist[i]->setXValue(markerXPos);
        plotList[i]->replot();
    }
    markerXPos++;

    //qDebug() << "tt " << markerXPos;

    if(markerXPos == xUdpRecv)
        dataProcess100msTimer.stop();
}

//void MainWindow::on_butPos_clicked()
//{
//    QString str;
//    //for(int i=0; i<64; i++){
//        //str = QString("S0p%1t1000\r\n").arg(int((i/64.)*100));
//    //}

//    //str = QString("S0p%1t1000\r\n").arg(ui->lineEditPos->text());
//    //serial.write(str.toLatin1());


//}

void MainWindow::on_pushButtonComOpen_clicked()
{
   serial.setBaudRate(115200);
    if(ui->pushButtonComOpen->text() == "open"){
        if(serial.isOpen() == false){
            QString comName = ui->comComboBox->currentText();
            if(comName.length() > 0){
                //UartThread.requestToStart(comName);
                serial.setPortName(comName);
                if (!serial.open(QIODevice::ReadWrite)) {
                    qDebug("%s port open FAIL", qUtf8Printable(comName));
                    return;
                }
                qDebug("%s port opened", qUtf8Printable(comName));
                connect(&serial, SIGNAL(readyRead()),
                        this, SLOT(handleReadyRead()));
                connect(&serial, SIGNAL(bytesWritten(qint64)),
                        this, SLOT(handleSerialDataWritten(qint64)));
                ui->pushButtonComOpen->setText("close");
            }
        }
    }
    else{
        serial.close();
        //udpSocket->close();
        qDebug("com port closed");
        ui->pushButtonComOpen->setText("open");
        //contrStringQueue.clear();
    }

}

//void MainWindow::on_pushButtonClose_clicked()
//{

//}

//void MainWindow::on_pushButton_clicked()
//{
//    QString str;
    //str = QString("v00=%1\r\n").arg(ui->lineEditVel->text());
    //serial.write(str.toLatin1());
//}

//void MainWindow::on_pushButtonDir_clicked()
//{
//    QString str;
    //str = QString("d00=%1\r\n").arg(ui->lineEditDir->text());
    //serial.write(str.toLatin1());
//}

//void MainWindow::on_pushButtonSetRange_clicked()
//{
//    QString str;
//    //str = QString("r00=%1\r\n").arg(ui->lineEditRange3->text());
//    //serial.write(str.toLatin1());
//}

//void MainWindow::on_pushButtonSetCont_clicked()
//{
//    QString str = "c00=\r\n" ;
//    //serial.write(str.toLatin1());
//}



//void MainWindow::on_butPos_2_clicked()
//{
//    QString str;
//    //str = QString("p01=%1\r\n").arg(ui->lineEditPos_2->text());
//    //serial.write(str.toLatin1());

//}

//void MainWindow::on_pushButton_2_clicked()
//{
//    QString str;
//    //str = QString("v01=%1\r\n").arg(ui->lineEditVel_2->text());
//    //serial.write(str.toLatin1());
//}

//void MainWindow::on_pushButtonDir_2_clicked()
//{
//    QString str;
//    //str = QString("d01=%1\r\n").arg(ui->lineEditDir_2->text());
//    //serial.write(str.toLatin1());
//}

//void MainWindow::on_pushButtonSetRange_2_clicked()
//{
//    QString str;
//    //str = QString("r01=%1\r\n").arg(ui->lineEditRange3_2->text());
//    //serial.write(str.toLatin1());
//}

//void MainWindow::on_pushButtonSetCont_2_clicked()
//{
//    QString str = "c01=\r\n" ;
//    //serial.write(str.toLatin1());
//}

//void MainWindow::on_verticalSlider_sliderReleased()
//{
////    int slVal = ui->verticalSlider->value();
////    setPos(slVal);

//}

//void MainWindow::on_verticalSlider_sliderMoved(int position)
//{
//    setPos(position);

//}

void MainWindow::setPos(int pos)
{

    pos = pos*10;
    if(pos>1000)
        pos = 1000;
    QString str = QString("S0p%1v%2\r\n").arg(pos).arg(speed) ;
    //quint32 ret = serial.write(str.toLatin1());

    //qDebug().nospace() <<  <<" -> " << qPrintable(str) ;



    //qDebug(" %s -> %d", qUtf8Printable(str.remove("\r\n")), ret);

}

//void MainWindow::on_verticalSlider_2_sliderReleased()
//{
////    speed = (ui->verticalSlider_2->value()/100.00)*4000;

//}

void MainWindow::initUdpSocket()
{
    udpSocket = new QUdpSocket(this);
    //if(udpSocket->bind(QHostAddress("192.168.0.104"), 8051) == true){
    if(udpSocket->bind(QHostAddress::Any, 8051) == true){
        qDebug("UDP bind OK");
    }

    connect(udpSocket, SIGNAL(readyRead()),
            this, SLOT(readPendingDatagrams()));
}


int lastMsec = 0;
bool MainWindow::sendDivPos(int mi, DivPosDataStr &ds, quint32 div, quint32 steps, quint32 dir, quint32 pos)
{
    int delta = pos - getMotorAbsPos(mi);
    dir = delta > 0? 1: 0;
    steps = abs(delta);

    int curMsec = QTime::currentTime().msecsSinceStartOfDay();
    if(steps > 0){
        //calc speed
        div = FPGA_FREQ/ (steps*10);
        if(div > 0x7fff){
            qDebug() << "maxSpeed err on" << mi;
            div = 0x7fff;
        }

        quint64 temp = 0;
        temp = mi&0xf;
        temp |= ((div&0x7fff)<<4);
        temp |= (((quint64)steps&0x7fff)<<19);
        temp |= (((quint64)dir&0x1)<<34);
        QByteArray ba = QByteArray::fromRawData((char*)&temp, sizeof(quint64));

        ba.resize(5);

        if(div > 0x7fff){
            qDebug("mi %d div exceed 0x7fff!", mi);
        }
        if(steps > 0x7fff){
            qDebug("mi %d steps exceed 0x7fff!", mi);
        }

    //    //ba[0] = 0x09;
    //    ba[0] = mi&0xf;
    //    ba[1] = 0xc0;
    //    ba[2] = 0x38;
    //    ba[3] = 0x00;
    //    ba[4] = 0x00;

        if(mi == 0){
            int msec = QTime::currentTime().msecsSinceStartOfDay();
            //qDebug("%d mi=%d st=%d", msec-lastMsec, mi, steps);
            lastMsec = msec;
        }
        serial.write(ba);
        motorAbsolutePosCur[mi] += delta;
        if( motorAbsolutePosCur[mi] < 0){
            qDebug("mi %d motorAbsolutePosCur less 0 =", mi, motorAbsolutePosCur[mi]);
        }
     //   if(mi==0)
    //        serial.write(ba);
        return true;
    }
    else{
        int shift = ds.absMsec - curMsec;
        if(mi == 0){
            //qDebug("%d", shift);
        }
        if( (abs(shift) <=100) ||
             (shift <0) ){        //если проигрывание отстаёт
            //lastCtrlTimeMsecs[mi] = curMsec;
            if(mi==0){
                qDebug("%d", shift);
                //static int lastMsecs = 0;
                //int t = 1000*ds.steps*((float)ds.div/24000000);
                //qDebug("d=%x s=%d t=%d %c", ds.div, ds.steps, t, ds.div < 0xe4? '!' : ' ');
                //int delta = curMsec -lastMsecs;
                //lastMsecs = curMsec;
                //qDebug("z %d %s", delta, delta <98 ? "!!": " ");
            }
            return true;
        }
        else{
            return false;
        }
    }

}

void MainWindow::handleReadyRead()
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


    //qDebug("enter bytesRecv %d", str.length());
    QMap<char, char> tempStor;
    for (int i=0; i<str.length(); i++) {
        char b = str[i];
        char n = (b>>6)&0x3;
//        if(tempStor.contains(n))
//            qDebug("in this pack already contains!");
        tempStor[n] = b|tempStor[n];
    }



    foreach (char b, tempStor){
        switch((b>>6)&0x3){
        case 0x0:
            for(int i=0; i<5; i++){
                if((b&(1<<i)) == 0){
                    freeToWrite(i);
                }
            }
            break;
        case 0x1:
            for(int i=0; i<5; i++){
                if((b&(1<<i)) == 0){
                    freeToWrite(5+i);
                }
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

    ui->plainTextEdit->verticalScrollBar()->setValue(ui->plainTextEdit->verticalScrollBar()->maximum());    


}

void MainWindow::terminatorState(int i, bool bEna)
{
    switch(i){
        case 0: ui->term1->setChecked(bEna); break;
        case 1: ui->term2->setChecked(bEna); break;
        case 2: ui->term3->setChecked(bEna); break;
        case 3: ui->term4->setChecked(bEna); break;
        case 4: ui->term5->setChecked(bEna); break;
        case 5: ui->term6->setChecked(bEna); break;
        case 6: ui->term7->setChecked(bEna); break;
        case 7: ui->term8->setChecked(bEna); break;
        case 8: ui->term9->setChecked(bEna); break;
        case 9: ui->term10->setChecked(bEna); break;
    }

    if(mtState[i] == MT_GoDOWN){
        if(bEna == false){
        }
        else{
            motorAbsolutePosCur[i] = 0;
            if(ui->checkBoxCycle->isChecked())
                mtState[i] = MT_GoUP;
            else
                mtState[i] = MT_IDLE;
        }

    }
}


void MainWindow::freeToWrite(int i)
{
    switch(mtState[i]){
    case MT_GoUP:
        if(getMotorAbsPos(0) < 360000){
            DivPosDataStr ds;
            ds.pos = getMotorAbsPos(i) + 4560;
            sendDivPos(i, ds, ds.div, ds.steps, ds.dir, ds.pos);
        }
        else{
            mtState[i] = MT_GoDOWN;
        }
        break;

    case MT_GoDOWN:
        DivPosDataStr ds;
        ds.pos = getMotorAbsPos(i)-500;
        sendDivPos(i, ds, ds.div, ds.steps, ds.dir, ds.pos);

        break;

    case MT_IDLE:
        if(motorPosCmdData[i].isEmpty() == false){
            DivPosDataStr ds;
            ds = motorPosCmdData[i].first();
            int curMsec = QTime::currentTime().msecsSinceStartOfDay();

            //if(ds.div < 0x100)
            //    ds.div = 0x100;

            //qDebug("div=%x, st=%d, d=%d", ds.div, ds.steps, ds.dir);
            if(sendDivPos(i, ds, ds.div, ds.steps, ds.dir, ds.pos) == true){
                motorPosCmdData[i].dequeue();
            }
        }
        break;
    }

}

void MainWindow::handleSerialDataWritten(qint64 bytes)
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
//                serial.write(wrStr.toLatin1());\
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

void MainWindow::parseCmdMultiMotorStrList(QStringList cmdMultiMotorStrList)
{        
    foreach (QString cmdStr, cmdMultiMotorStrList) {        
        //convertPosModeToVelMode(cmdStr);        
        parseCmdMultiMotorStr(cmdStr);
        //qDebug() << "--- "<< cmdStr;
    }
}

void MainWindow::parseCmdMultiMotorStr(QString cmdMultiMotorStr)
{
    QString convertedString;
    int maxVal = ui->lineEditMaxVal->text().toInt();

    QStringList motorStrList =  cmdMultiMotorStr.split("p", QString::SkipEmptyParts);
    //if(motorStr)
    //foreach (QString motorStr, motorStrList) {
    for(int i=0; i<motorStrList.length(); i++){
        QString vs = motorStrList[i];        
        float ip = vs.toInt()/1000.;
        int convVal = ip*maxVal;
        vs = QString("%1").arg(convVal, 3, 'g', -1, '0');
        parseCmdMotorStr(i, vs);
        convertedString += "p" + vs;

    }
    convertedString += "\r\n";

    motorPosCmdStrings << convertedString;
}

DivPosDataStr ds;
void MainWindow::parseCmdMotorStr(int mn, QString cmdStr)
{
    //if(cmdStr[0] == 'S'){
    if(true){
        //int mn = QString(cmdStr[1]).toInt();
        //QString midstr = cmdStr.mid(3, 3);
        //int pos = cmdStr.mid(3, 3).toInt();
        int pos = cmdStr.toInt();
        //int pos = cmdStr.right(3).toInt();
        if((mn>=0)&&(mn<10)){
//            int lastPos = 0;
//            if(motorPosCmdData[mn].size() == 0){
//                lastPos = 0;
//            }
//            else{
//                lastPos = motorPosCmdData[mn].last().pos;
//            }

            if(pos > 999){
                qDebug("pos exceed 100! pos=%d", pos);
            }
            if(pos <0){
                qDebug("pos less 0! pos=%d", pos);
            }
            int maxVal = ui->maxSteps->text().toInt();
            int newPos = maxVal*(pos/1000.);
            ds.pos = newPos;
            int delta = newPos - getMotorAbsPos(mn);
            if(motorPosCmdData[mn].length() == 0){                
                qDebug("mn %d d %d", mn, delta);                
                int n=1;
                for(; abs(delta)>2000; n*=2){
                    delta/=2;
                }
                ds.pos = getMotorAbsPos(mn);
                ds.absMsec = QTime::currentTime().msecsSinceStartOfDay();
                for(int i=0; i<n; i++){
                    ds.absMsec += 100;
                    ds.pos = ds.pos+delta;
                    motorPosCmdData[mn].append(ds);                    
                }

            }
            else{
                ds.absMsec = motorPosCmdData[mn].last().absMsec + 100;
                motorPosCmdData[mn].append(ds);
            }
        }
    }
}

//void MainWindow::convertPosModeToVelMode(QString cmdStr)
//{
//    QStringList motorStrList =  cmdStr.split("p", QString::SkipEmptyParts);


//    for(int i=0; i<10; i++){
//        int pos = motorStrList[i].left(3).toInt();
//        if(lastPosMap.contains(i)==false){
//            lastPosMap[i]=pos;
//        }
//        else{
//            //int vel =
//            //motorVelCmdStrings

//        }

//    }

//}
void MainWindow::graphReset()
{
    xMap.clear();

    foreach (QPolygonF plg, polyPoslist) {
        plg.clear();
    }
    foreach (QPolygonF plg, polyVellist) {
        plg.clear();
    }

    foreach (QwtPlotCurve* crv, posCurveList) {
        crv->setSamples(QPolygonF());
    }
    foreach (QwtPlotCurve* crv, velCurveList) {
        crv->setSamples(QPolygonF());
    }

    foreach (QwtPlot* plt, plotList) {
        plt->replot();
    }

}

void MainWindow::readPendingDatagrams()
{
    QTime rpdEst;
    rpdEst.start();
    //qDebug() << "0";
    int gridLines = 0;
    int rb = 0;
    while (udpSocket->hasPendingDatagrams()) {
        //qDebug() << "1";
        //qDebug() << "pds:"<< udpSocket->pendingDatagramSize();
        QNetworkDatagram datagram = udpSocket->receiveDatagram();
        //qDebug() << "2";
        rb += datagram.data().length();
        QString dataStr = QString(datagram.data());
        //continue;

        //qDebug() << "3";
        QStringList outStrings;
        if(dataStr.compare("start\r\n") == 0){
//            lastCmdMap.clear();
//            lastPosMap.clear();
//            graphReset();
//            motorPosCmdStrings.clear();
//            if(dataProcess100msTimer.isActive() == false){
//                dataProcess100msTimer.start();
//            }
            qDebug("start cmd");
        }
        else if(dataStr.compare("stop\r\n") == 0){            
            qDebug("stop cmd");
            for(int i=0; i<MOTOR_CNT; i++){
                motorPosCmdData[i].clear();
            }
        }
        else{
            QStringList list1 = dataStr.split("\r\n", QString::SkipEmptyParts);
            //qDebug()<<dataStr;
            //QString  contrStr =  dataStr.left(13);
//            QString debStr;
//            foreach (QString posStr, list1) {
//                debStr.append(posStr + " ");
//            }
            //qDebug("%s", debStr.toLatin1().constData());


//            if(list1.size() == 20){
//                foreach (QString cmdStr, list1) {
//                    int mn = QString(cmdStr[1]).toInt();
//                    lastCmdMap[mn] = cmdStr.mid(2, 4);
//                    QString outString;
//                    for(int i=0; i<10; i++){
//                        if(lastCmdMap[i] == NULL){
//                            lastCmdMap[i] = QString("p000");
//                        }
//                        outString += lastCmdMap[i];
//                    }
//                    outString += "\r\n";
//                    outStrings << outString;
//                }
//            }
//            else{

            foreach (QString multiMotorStr, list1) {
                QStringList motorList = multiMotorStr.split("S", QString::SkipEmptyParts);
                QString outString;
                foreach (QString cmdStr, motorList) {
                    outString += cmdStr.mid(1, 4);
                }
                outString += "\r\n";
                outStrings << outString;

            }
            gridLines++;
//            }
            parseCmdMultiMotorStrList(outStrings);
        }





//        foreach (QString cmdStr, list1) {
//            parseCmdStr(cmdStr);
//        }


//        if(dataStr[0] == 'P'){
//            int mn = QString(contrStr[1]).toInt();

//            if((mn>=0)&&(mn<10)){
//                //qDebug("%s", mem.toLatin1().constData());
//                int pos = contrStr.mid(3, 4).toInt();
//                slList[mn]->setValue(pos);
//    //            if(mn == 1)
//    //                qDebug("mn %d pos %d", mn, pos);
//            }
//            QByteArray motorOkba = QString("M%1Ok\r\n").arg(mn).toLatin1();
//            QNetworkDatagram drply = datagram.makeReply(motorOkba);
//            udpSocket->writeDatagram(drply);
//            return;

//        }
    }
    //qDebug() << "readPendingDatagrams time" << rpdEst.elapsed() << " gl " << gridLines;
    qDebug("readPendingDatagrams rb %d gl %04d time %d", rb, gridLines, rpdEst.elapsed());
}


void MainWindow::on_pushButton_refreshCom_clicked()
{
    ui->comComboBox->clear();
    const auto serialPortInfos = QSerialPortInfo::availablePorts();
    const QString blankString = QObject::tr("N/A");
      QString description;
      QString manufacturer;
      QString serialNumber;
    for (const QSerialPortInfo &serialPortInfo : serialPortInfos) {
           description = serialPortInfo.description();
           manufacturer = serialPortInfo.manufacturer();
           serialNumber = serialPortInfo.serialNumber();
           qDebug() << endl
               << QObject::tr("Port: ") << serialPortInfo.portName() << endl
               << QObject::tr("Location: ") << serialPortInfo.systemLocation() << endl
               << QObject::tr("Description: ") << (!description.isEmpty() ? description : blankString) << endl
               << QObject::tr("Manufacturer: ") << (!manufacturer.isEmpty() ? manufacturer : blankString) << endl
               << QObject::tr("Serial number: ") << (!serialNumber.isEmpty() ? serialNumber : blankString) << endl
               << QObject::tr("Vendor Identifier: ") << (serialPortInfo.hasVendorIdentifier() ? QByteArray::number(serialPortInfo.vendorIdentifier(), 16) : blankString) << endl
               << QObject::tr("Product Identifier: ") << (serialPortInfo.hasProductIdentifier() ? QByteArray::number(serialPortInfo.productIdentifier(), 16) : blankString) << endl
               << QObject::tr("Busy: ") << (serialPortInfo.isBusy() ? QObject::tr("Yes") : QObject::tr("No")) << endl;
           ui->comComboBox->addItem(serialPortInfo.portName());
    }

}

void MainWindow::on_pushButtonClear_clicked()
{
    ui->plainTextEdit->clear();
    ui->plainTextUDP->clear();
    udpCnt = 0;

}

void MainWindow::sendOnTimer()
{
    if(serial.isOpen() && !motorPosCmdStrings.isEmpty()){
        QString wrStr = motorPosCmdStrings.dequeue();
        //serial.write(wrStr.toLatin1());
    }

//    if(mtstr[curMotorSendIdx].contrStringQueue.isEmpty() == false){
//       // qDebug() << QTime::currentTime().msecsSinceStartOfDay() << "send "
//       //          << curMotorSendIdx << " " << mtstr[curMotorSendIdx].contrStringQueue.size();
//        if(serial.isOpen()){
//            QString wrStr = mtstr[curMotorSendIdx].contrStringQueue.dequeue();
//            serial.write(wrStr.toLatin1());
//        }

//    }
//    curMotorSendIdx++;
//    if(curMotorSendIdx >= MOTOR_CNT){
//        curMotorSendIdx = 0;
//    }

    if(serial.isOpen()){
        for(int i=0; i<10; i++){
//            if(motorPosCmdData[i].isEmpty() == false){
//                DivPosDataStr ds;
//                ds = motorPosCmdData[i].dequeue();
//                if(i==9){
//                    int t = 1000*ds.steps*((float)ds.div/24000000);
//                    qDebug("d=%x s=%d t=%d %c", ds.div, ds.steps, t, ds.div < 0xe4? '!' : ' ');

//                }
//                if(ds.div < 0x100)
//                    ds.div = 0x100;

                //if(ds.steps > 0)
                //    sendDivPos(i, ds.div, ds.steps, ds.dir);
//            }
        }
        //QString wrStr = motorPosCmdStrings.dequeue();
        //serial.write(wrStr.toLatin1());
    }
//   qDebug("deq %d %d %d %d %d", motorPosCmdData[0].length(),
//                                motorPosCmdData[1].length(),
//                                motorPosCmdData[2].length(),
//                                motorPosCmdData[3].length(),
//                                motorPosCmdData[4].length());
}


void MainWindow::waitForFifoFreeFifo()
{
    //qDebug() << "waitForFifoFreeFifo timeout. resend";
    //processUartSendExchange();


}

//void MainWindow::processUartRecvExchange(QString msg)
//{
//    if(msg.compare("ff\r\n") == 0){
//        qDebug() << "Fifo full!!!";
//        uartSendState = idle;
//        processUartSendExchange();
//        //waitForFifoFreeTimer.setSingleShot(true);
//        //waitForFifoFreeTimer.start(100);
//    }
//    else if(msg.compare("ok\r\n") == 0){
//        //bCmdOk = true;
//        qDebug() << "OK!!!";
//        uartSendState = idle;
//        mtstr[curMotorSendIdx].contrStringQueue.dequeue();
//        curMotorSendIdx++;
//        if(curMotorSendIdx >= MOTOR_CNT){
//            curMotorSendIdx = 0;
//        }
//        processUartSendExchange();

////        if(contrStringQueue.isEmpty() == false){
////           contrStringQueue.dequeue();
////            if(contrStringQueue.isEmpty() == false){
////                QString wrStr = contrStringQueue.head();
////                qDebug("%s", qPrintable(wrStr));
////                QThread::msleep(20);
////                serial.write(wrStr.toLatin1());
////            }
////            else{
////                noDataSending = true;
////            }
////        }
////        else{
//            //noDataSending = true;
////        }

//    }
//    else{
//        qDebug() << "unexpected " << msg;
//    }

//    switch(uartSendState){
//    case waitForAck:
//        //qDebug() << "expected ack msg";
//            break;
//    }



//}

//void MainWindow::processUartSendExchange()
//{
//    if(serial.isOpen() /*&& (bCmdOk == true)*/){
//        if((uartSendState ==idle)){
//            for(int motorsCheked=MOTOR_CNT; motorsCheked >0; motorsCheked--){
//                if(mtstr[curMotorSendIdx].contrStringQueue.isEmpty() == false){
//                    qDebug() << "s" << curMotorSendIdx ;
//                        QString wrStr = mtstr[curMotorSendIdx].contrStringQueue.head();
//                        qDebug("%s", qPrintable(wrStr));
//                        serial.write(wrStr.toLatin1());
//                        serial.flush();
//                        uartSendState = waitForAck;
//                        break;
//                }
//                else{
//                    curMotorSendIdx++;
//                    if(curMotorSendIdx >= MOTOR_CNT){
//                        curMotorSendIdx = 0;
//                    }
//                }
//            }
//        }
//    }
//}

void MainWindow::response(QString str)
{
    //qDebug() << str;
    ui->plainTextEdit->moveCursor (QTextCursor::End);
    ui->plainTextEdit->insertPlainText(str);
    ui->plainTextEdit->moveCursor (QTextCursor::End);
    //ui->plainTextEdit->appendPlainText(str);

    ui->plainTextEdit->verticalScrollBar()->setValue(ui->plainTextEdit->verticalScrollBar()->maximum());
}

void MainWindow::on_pushMoveDownState_clicked()
{


//    if(serial.isOpen()){
//        QString str("Sd\r\n");
//     serial.write(str.toLatin1());
//    }
}

void MainWindow::on_pushBUttonToIdle_clicked()
{
    if(serial.isOpen()){
        QString str("Si\r\n");
     serial.write(str.toLatin1());
    }

}

void MainWindow::on_pushButtonGotoPEriodState_clicked()
{
//    if(serial.isOpen()){
//        QString str("Sp\r\n");
//     serial.write(str.toLatin1());
//    }
    //bCycle =

}

void MainWindow::on_pushButtonPosReset_clicked()
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

void MainWindow::on_pushMoveUp_clicked()
{
    DivPosDataStr ds;

    for(int i=0; i<10; i++){
        int startPos =0;

        if(motorPosCmdData[i].length() == 0)
            startPos = getMotorAbsPos(i);
        else
            startPos = motorPosCmdData[i].last().pos;

        ds.pos = startPos+400;
        for(int k=0; k<5; k++){
            motorPosCmdData[i] << ds;
             ds.pos += 400;
        }
//        ds.dir = 0;
//        //for(int k=0; k<30; k++)
//        //    motorPosCmdData[i] << ds;
    }

}

void MainWindow::on_pushMoveDown_clicked()
{
    DivPosDataStr ds;

    for(int i=0; i<10; i++){
        int startPos =0;

        if(motorPosCmdData[i].length() == 0)
            startPos = getMotorAbsPos(i);
        else
            startPos = motorPosCmdData[i].last().pos;
        ds.pos = startPos-400;
        for(int k=0; k<5; k++){
            motorPosCmdData[i] << ds;
            ds.pos -= 400;
        }
        //for(int k=0; k<30; k++)
        //    motorPosCmdData[i] << ds;
    }
}


void MainWindow::on_pushTestData_clicked()
{

//    DivPosDataStr ds;
//    ds.pos = 1;
//    ds.steps = 2400; ds.div = 1000; ds.dir = 1; motorPosCmdData[0] << ds;
    //ds.steps = 32800; ds.div = 73; ds.dir = 1; motorPosCmdData[0] << ds;
//    ds.steps = 8800; ds.div = 272; ds.dir = 1; motorPosCmdData[0] << ds;
//    ds.steps = 13600; ds.div = 176; ds.dir = 1; motorPosCmdData[0] << ds;
//    ds.steps = 18400; ds.div = 130; ds.dir = 1; motorPosCmdData[0] << ds;
//    ds.steps = 22400; ds.div = 107; ds.dir = 1; motorPosCmdData[0] << ds;

//    ds.steps = 25600; ds.div = 93; ds.dir = 1; motorPosCmdData[0] << ds;
//    ds.steps = 28800; ds.div = 83; ds.dir = 1; motorPosCmdData[0] << ds;
//    ds.steps = 30400; ds.div = 78; ds.dir = 1; motorPosCmdData[0] << ds;
//    ds.steps = 32000; ds.div = 75; ds.dir = 1; motorPosCmdData[0] << ds;
//    ds.steps = 32800; ds.div = 73; ds.dir = 1; motorPosCmdData[0] << ds;

//    ds.steps = 33599; ds.div = 71; ds.dir = 1; motorPosCmdData[0] << ds;
//    ds.steps = 32801; ds.div = 73; ds.dir = 1; motorPosCmdData[0] << ds;
//    ds.steps = 32000; ds.div = 75; ds.dir = 1; motorPosCmdData[0] << ds;
//    ds.steps = 30400; ds.div = 78; ds.dir = 1; motorPosCmdData[0] << ds;
//    ds.steps = 28800; ds.div = 83; ds.dir = 1; motorPosCmdData[0] << ds;

//    ds.steps = 25600; ds.div = 93; ds.dir = 1; motorPosCmdData[0] << ds;
//    ds.steps = 22400; ds.div = 107; ds.dir = 1; motorPosCmdData[0] << ds;
//    ds.steps = 18399; ds.div = 130; ds.dir = 1; motorPosCmdData[0] << ds;
//    ds.steps = 13601; ds.div = 176; ds.dir = 1; motorPosCmdData[0] << ds;
//    ds.steps = 8800; ds.div = 272; ds.dir = 1; motorPosCmdData[0] << ds;
//    ds.steps = 3200; ds.div = 750; ds.dir = 1; motorPosCmdData[0] << ds;

}

void MainWindow::on_pushClearMap_clicked()
{


}

void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    if( index==0 ){

        for(int i=0; i<5; i++){
            ui->qwtWdgH1->layout()->removeWidget(plotList[i]);
            ui->qwtWdgH2->layout()->removeWidget(plotList[5+i]);
        }
        for(int i=0; i<5; i++){
            ui->qwtWdgH1->layout()->addWidget(plotList[i]);
        }

        for(int i=0; i<5; i++){
            ui->qwtWdgH2->layout()->addWidget(plotList[5+i]);
        }
    }
    else{
        //int idx = index-1;
        //if(idx>4)
        ui->tabWidget->widget(index)->layout()->addWidget(plotList[index-1]);
    }


//    else if((index>5) && (index<=10)){
//        ui->tabWidget->widget(index)->layout()->addWidget(plotList[0]);
//    }

}

void MainWindow::on_goToTerm_clicked()
{
    for(int i=0; i<MOTOR_CNT; i++)
      mtState[i] = MT_GoDOWN;

}

void MainWindow::uiUpdateTimerSlot()
{
    int curMsec = QTime::currentTime().msecsSinceStartOfDay();
    for(int i=0; i<MOTOR_CNT; i++){
        if(motorPosCmdData[i].size() != 0){
            int shift = 0;
            if(i==0){
               // qDebug("%d >> %d",curMsec, motorPosCmdData[i].first().absMsec);
            }
            shift = motorPosCmdData[i].first().absMsec - curMsec;

//            if(shift > timeShiftMaxPos){
//                timeShiftMaxPos = (shift-shift%100)+500;
//                timeStatSlider[i]->setMinimum(timeShiftMaxPos);
//            }
//            else if(shift < timeShiftMaxNeg){
//                timeShiftMaxNeg = (shift-shift%100)-500;
//                timeStatSlider[i]->setMinimum(timeShiftMaxNeg);
//            }
            timeStatLE[i]->setText(QString::number(shift));
            timeStatSlider[i]->setValue(shift);
        }
        else{
            timeStatLE[i]->setText("n/a");
             timeStatSlider[i]->setValue(0);
        }

        //timeStatSlider[i]->setStyleSheet("QSlider::handle:vertical {background: red;}");
        //timeStatSlider[i]->setStyleSheet("QSlider::sub-page:vertical {background: red;}");
    }

}
