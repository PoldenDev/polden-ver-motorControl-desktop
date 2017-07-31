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
    markerXPos(0)
{
    ui->setupUi(this);

    ui->lineEditMaxVal->setText(QString::number(settings.value("maxPosValue", 1000).toInt()));
    ui->lineEditMinVal->setText(QString::number(settings.value("minPosValue", 0).toInt()));

    on_pushButton_refreshCom_clicked();

    initUdpSocket();
    timer.setInterval(50);
    connect(&timer, SIGNAL(timeout()), this, SLOT(sendOnTimer()));
    timer.start();

    dataProcess100msTimer.setInterval(100);
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

    for(int i=0; i<MOTOR_CNT; i++){
//        timerSerialSendTo[i].setSingleShot(true);
//        connect(&(timerSerialSendTo[i]), SIGNAL(timeout()),
//                this, SLOT(sendTimeOut));
    }

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
   serial.setBaudRate(230400);
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



void MainWindow::sendDivPos(int mi, int div, int steps, int dir)
{
    quint32 temp = 0;
    temp = mi&0xf;
    temp |= ((div&0x7fff)<<4);
    temp |= ((steps&0xfff)<<19);
    temp |= ((dir&0x1)<<31);
    QByteArray ba = QByteArray::fromRawData((char*)&temp, sizeof(quint32));
//    ba.resize(5);

//    //ba[0] = 0x09;
//    ba[0] = mi&0xf;
//    ba[1] = 0xc0;
//    ba[2] = 0x38;
//    ba[3] = 0x00;
//    ba[4] = 0x00;

    ba.append((char)0);

    serial.write(ba);
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

    QString debStr;

    for (int i=0; i<str.length(); i++) {
        //debStr.append((char)str[i]);
        int startMi = 0;
        if((str[i]&0x80) != 0){
            startMi = 5;
        }
        bool bIsFreeExist = false;
        bool bIsBusyExist = false;
        QString freeMtrs;
        QString busyMtrs;
        for(int i=0; i<5; i++){
            int curMi = startMi+i;
            if((str[i]&(1<<i)) == 0){
//                if(motorPosCmdData[curMi].isEmpty() == false){
//                    DivPosDataStr ds = motorPosCmdData[curMi].dequeue();
//                    //sendDivPos(startMi+i, 0x4fff, 20);
//                    //sendDivPos(startMi+i, ds.div, ds.steps, ds.dir);
//                    //
//                }
                freeMtrs = QString("%1 %2").arg(curMi).arg(freeMtrs);
                bIsFreeExist = true;
            }
            else{
                bIsBusyExist = true;
                busyMtrs = QString("%1 %2").arg(curMi).arg(busyMtrs);
                //qDebug() << QTime::currentTime().msecsSinceStartOfDay() <<  qPrintable(" b");
            }
        }

       // if(bIsBusyExist)
          //  qDebug() << QTime::currentTime().msecsSinceStartOfDay() << busyMtrs;
    }

    //qDebug() << str;
    //ui->plainTextEdit->moveCursor (QTextCursor::End);
    //ui->plainTextEdit->insertPlainText(str);
    //ui->plainTextEdit->insertPlainText(QString(t.msecsSinceStartOfDay()));
    //ui->plainTextEdit->moveCursor (QTextCursor::End);
    //ui->plainTextEdit->appendPlainText(str);

    ui->plainTextEdit->verticalScrollBar()->setValue(ui->plainTextEdit->verticalScrollBar()->maximum());    
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
    //foreach (QString motorStr, motorStrList) {
    for(int i=0; i<10; i++){
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
            int lastPos = 0;
            if(motorPosCmdData[mn].size() == 0){
                lastPos = 0;
            }
            else{
                lastPos = motorPosCmdData[mn].last().pos;
            }

            int delta = pos - lastPos;

            float prcnt = abs(delta)/100.;
            ds.div = 0x7ff;
            ds.steps = 40000*prcnt;
            ds.dir = delta > 0? 1: 0;
            ds.pos = pos;
            motorPosCmdData[mn].append(ds);
            //qDebug("%s", mem.toLatin1().constData());
            //pos = contrStr.mid(8, 4).toInt();
            //slList[mn]->setValue(pos);

            //            if(mn == 1)
            //                qDebug("mn %d pos %d", mn, pos);
        }
        //        foreach (QString mem, list1) {
        //            int mn = QString(mem[1]).toInt();
        //            if((mn>=0)&&(mn<10)){
        //                //qDebug("%s", mem.toLatin1().constData());
        //                int pos = mem.mid(8, 4).toInt();
        //                slList[mn]->setValue(pos);
        //                if(mn == 1)
        //                    qDebug("mn %d pos %d", mn, pos);
        //            }
        //        }



        //        for(int i=0; i<10; i++){
        //            QString posData = dataStr.mid(8*(i+1),4);
        //            int pos = posData.toInt();
        //            slList[i]->setValue(pos);
        //        }


        //qDebug("%s", contrStr.toLatin1().constData());
        //contrStr+= "\r\n";
        //qDebug() << datagram.data();
        //QByteArray cmdOkba = QString("cmdOK\r\n").toLatin1();
        //QByteArray cmdFailba = QString("cmdFail\r\n").toLatin1();
        //QNetworkDatagram drply = datagram.makeReply(cmdOkba);
        //udpSocket->writeDatagram(drply);
        //if(mn != 2)
        //  return;
        //mtstr[mn].contrStringQueue.enqueue(cmdStr);


        int velIPS;
        if((pos>=0) && (pos<1000)){

            int x=0;
            if(xMap.contains(mn) == true){
                x = xMap[mn];
            }

            polyPoslist[mn].append(QPointF(x, pos/10.));
            xMap[mn] = x+1;

            if(polyPoslist[mn].length() > 1500){
                polyPoslist[mn].removeFirst();
            }
            posCurveList[mn]->setSamples(polyPoslist[mn]);


            int lasPos = 0;
            if(lastPosMap.contains(mn) == true){
                lasPos = lastPosMap[mn];
            }
            int vel = pos - lasPos;

            velIPS=abs( vel*400*10 );
            //qDebug("%d v %d", mn,  vel);
            lastPosMap[mn] = pos;

            polyVellist[mn].append(QPointF(x, vel));
            if(polyVellist[mn].length() > 1500){
                polyVellist[mn].removeFirst();
            }
            velCurveList[mn]->setSamples(polyVellist[mn]);
            plotList[mn]->replot();


            //   UartThread.transaction(mn, contrStr);
            //if(serial.isOpen() /*&& (bCmdOk == true)*/){
            //       serial.write(contrStr.toLatin1());
            //drply = datagram.makeReply(cmdOkba);
            //bCmdOk = false;

            //            if(noDataSending){
            //                serial.write(contrStr.toLatin1());
            //                noDataSending = false;
            //            }
            //            else{
            //                contrStringQueue.enqueue(contrStr);

            //            }

            //if(serial.waitForBytesWritten(10000)){

            //}
            //else{
            //qDebug("serial port write byte error!");
            //}

            // }
        }
        else{
            qDebug("pos diap error %d", pos);
        }

        if(mn==0){
            // QString udpTextField = QString("%1:%2").arg(udpCnt++).arg(contrStr);

            int div = 0xfffff;
            if(velIPS != 0){
                div = 50000000/velIPS;
            }
            QString udpTextField = QString("%1 p%2 v%3 d%4\r\n").arg(xUdpRecv++, 5, 10)
                                                                .arg(pos)
                                                                .arg(velIPS)
                                                                .arg(div, 4, 16);
            //udpSocket->writeDatagram()                ;
            ui->plainTextUDP->moveCursor (QTextCursor::End);
            ui->plainTextUDP->insertPlainText(udpTextField);
            ui->plainTextUDP->moveCursor (QTextCursor::End);
            //ui->plainTextEdit->appendPlainText(str);
        }



    //        if((serial.isOpen() == true)&&(mn<MOTOR_CNT)){

    //            mtstr[mn].contrStringQueue.enqueue(contrStr);
    //            if(uartSendState == idle){
    //                processUartSendExchange();
    //            }
    //            else{
    //                qDebug() << "data added";

    //            };
    //        }

                    //
    //                lastMotorReceptInd = mn;

    //                timerSerialSendTo[mn].start(10);
    //                //drply = datagram.makeReply(cmdOkba);
    //                //bCmdOk = false;

    //                //добавили  посылку в очередь


    //                bool bMotorsIdleState = true;
    //                for(int i=0; i<MOTOR_CNT; i++)
    //                {
    //                    bMotorsIdleState = (bMotorsIdleState &&(mtstr[mn].sendState == idle));

    //                }
    //                //если все молчат, то запустим пересылку
    //                if(bMotorsIdleState == true){
    //                    emit handleSerialDataWritten(-1);

    //                }
    //                if((mtstr[mn].sendState == idle)){
    //                    serial.write(contrStr.toLatin1());
    //                }
    //                contrStringQueue;
    //                if(noDataSending){
    //                    noDataSending = false;

    //                }

                    //if(serial.waitForBytesWritten(10000)){

                    //}
                    //else{
                        //qDebug("serial port write byte error!");
                    //}

                //}
        //        else{
        //            drply = datagram.makeReply(cmdFailba);

        //        }
        //        if(serial.isOpen() /*&& (bCmdOk == true)*/){
        //            //serial.write(contrStr.toLatin1());
//            contrStringQueue.enqueue(contrStr);
//        }
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
            if(dataProcess100msTimer.isActive() == false){
                dataProcess100msTimer.start();
            }
        }
        else if(dataStr.compare("stop\r\n") == 0){
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
            if(motorPosCmdData[i].isEmpty() == false){
                DivPosDataStr ds;
                ds = motorPosCmdData[i].dequeue();
                if(ds.steps > 0)
                    sendDivPos(i, ds.div, ds.steps, ds.dir);
            }
        }
        //QString wrStr = motorPosCmdStrings.dequeue();
        //serial.write(wrStr.toLatin1());
    }
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
//    QStringList strList;
//    strList << "p000p000p000p000p000p000p000p000p000p000\r\n";
//    strList << "p005p000p000p000p000p000p000p000p000p050\r\n";
//    strList << "p010p000p000p000p000p000p000p000p000p050\r\n";
//    strList << "p015p000p000p000p000p000p000p000p000p050\r\n";
//    strList << "p020p000p000p000p000p000p000p000p000p050\r\n";
//    strList << "p025p000p000p000p000p000p000p000p000p050\r\n";
//    strList << "p030p000p000p000p000p000p000p000p000p050\r\n";
//    strList << "p035p000p000p000p000p000p000p000p000p050\r\n";
//    strList << "p040p000p000p000p000p000p000p000p000p050\r\n";
//    strList << "p045p000p000p000p000p000p000p000p000p050\r\n";
//    for(int i=0; i<40; i++)
//        strList << "p050p000p000p000p000p000p000p000p000p050\r\n";

//    parseCmdMultiMotorStrList(strList);

    DivPosDataStr ds;
    //ds.div = 0x4fff;
    ds.div = 0xfff;
    ds.steps = 4000;

    for(int i=0; i<10; i++){
        ds.dir = 0;
        for(int k=0; k<10; k++)
            motorPosCmdData[i] << ds;
        //for(int k=0; k<30; k++)
        //    motorPosCmdData[i] << ds;
    }

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
    if(serial.isOpen()){
        QString str("Sp\r\n");
     serial.write(str.toLatin1());
    }

}

void MainWindow::on_pushButtonPosReset_clicked()
{
//    for(int i=0; i< plotList.size(); i++){
//        polylist[i]<<  QPointF(polylist[i].length(), rand() % 100 );
//        curveList[i]->setSamples(polylist[i]);
//        plotList[i]->replot();
//    }
    if(serial.isOpen()){
        QString str("Sr\r\n");
     serial.write(str.toLatin1());
    }
}

void MainWindow::on_pushTestData_clicked()
{

//    QStringList strList;
//    strList << "p000p000p000p000p000p000p000p000p000p000\r\n";
//    strList << "p005p000p000p000p000p000p000p000p000p050\r\n";
//    strList << "p010p000p000p000p000p000p000p000p000p050\r\n";
//    strList << "p015p000p000p000p000p000p000p000p000p050\r\n";
//    strList << "p020p000p000p000p000p000p000p000p000p050\r\n";
//    strList << "p025p000p000p000p000p000p000p000p000p050\r\n";
//    strList << "p030p000p000p000p000p000p000p000p000p050\r\n";
//    strList << "p035p000p000p000p000p000p000p000p000p050\r\n";
//    strList << "p040p000p000p000p000p000p000p000p000p050\r\n";
//    strList << "p045p000p000p000p000p000p000p000p000p050\r\n";
//    for(int i=0; i<40; i++)
//        strList << "p050p000p000p000p000p000p000p000p000p050\r\n";

////    for(int i=0; i<40; i++)
////        strList << "p080p000p000p000p000p000p000p000p000p080\r\n";


////    for(int i=0; i<40; i++)
////        strList << "p060p000p000p000p000p000p000p000p000p060\r\n";

//    //strList << "p000p000p000p000p000p000p000p000p000p050\r\n";
//    //strList << "p000p000p000p000p000p000p000p000p000p060\r\n";


////    for(int i=0; i<13; i++)
////        strList << "p000p000p000p000p000p000p000p000p000p100\r\n";

////    for(int i=0; i<13; i++)
////        strList << "p000p000p000p000p000p000p000p000p000p150\r\n";

//    parseCmdMultiMotorStrList(strList);

    DivPosDataStr ds;
    //ds.div = 0x4fff;
    ds.div = 0xfff;
    ds.steps = 4000;

    for(int i=0; i<10; i++){
        ds.dir = 1;
        for(int k=0; k<10; k++)
            motorPosCmdData[i] << ds;
        ds.dir = 0;
        //for(int k=0; k<30; k++)
        //    motorPosCmdData[i] << ds;
    }

    //ds.dir = 0;
    //motorPosCmdData[0] << ds;
    //motorPosCmdData[1] << ds;
    //motorPosCmdData[4] << ds;
    //motorPosCmdData[6] << ds;
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
