#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtDebug>
#include <QtSerialPort/QSerialPortInfo>
#include <QScrollBar>
#include <QList>
#include <QNetworkDatagram>
#include <QThread>
#include <QDateTime>
#include <QSettings>
#include <QWidget>
#include <QtMath>


//#include <qwt_plot.h>
//#include <qwt_plot_grid.h>

//#include <qwt_legend.h>

//#include <qwt_plot_curve.h>
//#include <qwt_symbol.h>

//#include <qwt_plot_magnifier.h>

//#include <qwt_plot_panner.h>

//#include <qwt_plot_picker.h>
//#include <qwt_picker_machine.h>
//#include <qwt_plot_marker.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    motorCount(0),
    speed(100),
    udpSocket(NULL),    
    cmdNum(0),
    curMotorSendIdx(0),
    settings("murinets", "vertolet"),
    xUdpRecv(0),
    markerXPos(0),
    comExchanges(0),
    bytesOnIter(0),
    dataGramCnt(0),
    paletteGrey(NULL),
    paletteRed(NULL),
    paletteGreen(NULL),
    lastUdpDatagrammRecvd(0)
{
    ui->setupUi(this);

    motorCount = settings.value("motorCount", 0).toInt();
    ui->lineEditMotorCount->setText(QString("%1").arg(motorCount));
    ui->lineEditUDPMaxVal->setText(QString::number(settings.value("maxPosValue", 1000).toInt()));
    ui->lineEditMinVal->setText(QString::number(settings.value("minPosValue", 0).toInt()));
    ui->checkBoxDirInverse->setChecked(settings.value("dirInverse", false).toBool());

    ui->lineEdit_mmPerRot->setText(settings.value("mmPerRot", 10).toString());
    ui->lineEdit_MaxHeightImp->setText(settings.value("maxHeightImp", 200000).toString());
    ui->lineEdit_vmax_mmsec->setText(settings.value("vmax_mmsec", 50).toString());

    on_pushButtonUdpOpenClose_clicked();

    on_lineEdit_MaxHeightImp_editingFinished();

    FPGA_FREQ = settings.value("FPGA_FREQ", FPGA_FREQ_25).toInt();
    if(FPGA_FREQ == FPGA_FREQ_24)
        ui->radioButtonFpgaFreq24->setChecked(true);
    else if(FPGA_FREQ == FPGA_FREQ_25)
        ui->radioButtonFpgaFreq25->setChecked(true);
    else{
        ui->radioButtonFpgaFreq24->setChecked(false);
        ui->radioButtonFpgaFreq25->setChecked(false);

    }

    timer.setInterval(10);
    connect(&timer, SIGNAL(timeout()), this, SLOT(sendOnTimer()));
    timer.start();

    dataProcess100msTimer.setInterval(90);
    connect(&dataProcess100msTimer, SIGNAL(timeout()), this, SLOT(dataProcess100msTimeOut()));

    connect(&waitForFifoFreeTimer, SIGNAL(timeout()), this, SLOT(waitForFifoFreeFifo()));

    //connect(&UartThread, SIGNAL(response(QString)), this, SLOT(response(QString)));

//    QWidget *tabWdg;
//    for(int i=0; i<10; i++){
//        createPlot(QString::number(i));
//        tabWdg = new QWidget();
//        tabWdg->setLayout(new QVBoxLayout());
//        ui->tabWidget->addTab(tabWdg, QString::number(i));
//    }


//    for(int i=0; i<5; i++){
//        ui->qwtWdgH1->layout()->addWidget(plotList[i]);
//    }

//    for(int i=0; i<5; i++){
//        ui->qwtWdgH2->layout()->addWidget(plotList[5+i]);
//    }


    for(int i=0; i<MOTOR_CNT; i++){
        mtState[i] = MT_IDLE;
        bFreeToWrite[i] = false;
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

    createDebugSerialPortInterface();
    createMainInterface();

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

//    absPosSlider.append(ui->SliderPos0);
//    absPosSlider.append(ui->SliderPos1);
//    absPosSlider.append(ui->SliderPos2);
//    absPosSlider.append(ui->SliderPos3);
//    absPosSlider.append(ui->SliderPos4);
//    absPosSlider.append(ui->SliderPos5);
//    absPosSlider.append(ui->SliderPos6);
//    absPosSlider.append(ui->SliderPos7);
//    absPosSlider.append(ui->SliderPos8);
//    absPosSlider.append(ui->SliderPos9);

//    for(int i=0; i<absPosSlider.length(); i++){
//        QSlider *sl = absPosSlider[i];
//        connect(sl, &QSlider::sliderReleased,
//                [this, i](){ handleSliderReleased(i, absPosSlider[i]->pos().rx());});

//    }

    for(int i=0; i<MOTOR_CNT; i++){
        bTermState[i] = false;
    }

    //connectionTime = 0;
    uiUpdateTimer.setInterval(100);
    connect(&uiUpdateTimer, SIGNAL(timeout()), this, SLOT(uiUpdateTimerSlot()));
    uiUpdateTimer.start();

    QString build = QString("%1 ")
    .arg(QLocale(QLocale::C).toDate(QString(__DATE__).simplified(), QLatin1String("MMM d yyyy")).toString("yyyy-MM-dd"))
    /*.arg(QString("%1%20%3%4%5%6").arg(__TIME__[0])
    .arg(__TIME__[1])
    .arg(__TIME__[3])
    .arg(__TIME__[4])
    .arg(__TIME__[6])
    .arg(__TIME__[7]))*/;

    ui->labelCompileTime->setText(build);

    on_pushButton_refreshCom_clicked();

}

void MainWindow::createPlot(QString name)
{
//    QwtPlot *d_plot = new QwtPlot(this);
//    plotList << d_plot;
//    //d_plot->setTitle( "Qwt demonstration" ); // заголовок
//    //d_plot->setCanvasBackground( Qt::white ); // цвет фона

//    d_plot->setMinimumSize(50, 50);
//    // Параметры осей координат
//    //d_plot->setAxisTitle(QwtPlot::yLeft, "Y");
//    d_plot->setAxisScale(QwtPlot::yLeft, 0, 100);
//    //d_plot->setAxisTitle(QwtPlot::xBottom, "X");
//    //d_plot->setAxisScale(QwtPlot::xBottom, 0, 100);

//    d_plot->setAxisScale(QwtPlot::yRight, -100, 100);
//    //d_plot->insertLegend( new QwtLegend() );
//    d_plot->enableAxis(QwtPlot::yRight);



//    // Включить сетку
//    // #include <qwt_plot_grid.h>
//    QwtPlotGrid *gridL = new QwtPlotGrid(); //
//    gridL->setMajorPen(QPen( Qt::black, 1 )); // цвет линий и толщина
//    gridL->attach( d_plot ); // добавить сетку к полю графика

//    QwtPlotGrid *gridR = new QwtPlotGrid(); //
//    gridR->setMajorPen(QPen( Qt::gray, 1 )); // цвет линий и толщина
//    gridR->setYAxis(QwtPlot::yRight);
//    gridR->attach( d_plot ); // добавить сетку к полю графика


//    QwtPlotCurve *curve = new QwtPlotCurve();
//    posCurveList << curve;
//    //curve->setTitle( name );
//    curve->setPen( Qt::blue, 2 ); // цвет и толщина кривой
////        curve->setRenderHint
////                ( QwtPlotItem::RenderAntialiased, true ); // сглаживание
////        QwtSymbol *symbol = new QwtSymbol( QwtSymbol::Ellipse,
////            QBrush( Qt::yellow ), QPen( Qt::red, 2 ), QSize( 8, 8 ) );
////        curve->setSymbol( symbol );

//    //ui->widget_3->layout()->invalidate();

////        points << QPointF( 1.0, 1.0 ) // координаты x, y
////     << QPointF( 1.5, 2.0 ) << QPointF( 3.0, 2.0 )
////     << QPointF( 3.5, 3.0 ) << QPointF( 5.0, 3.0 );

//  //curve->setSamples( points ); // ассоциировать набор точек с кривой

//  curve->attach( d_plot ); // отобразить кривую на графике

//    curve = new QwtPlotCurve();
//    velCurveList << curve;
//    curve->setPen( Qt::red, 2 ); // цвет и толщина кривой
//    curve->setYAxis(QwtPlot::yRight);

//    curve->attach(d_plot ); // отобразить кривую на графике



//  polyPoslist << QPolygonF();
//  polyVellist << QPolygonF();
//  x=0;


//   QwtSymbol *markerSymbol = new QwtSymbol(QwtSymbol::VLine,QBrush(Qt::red),QPen(Qt::red),QSize(50,5000));
//    QwtPlotMarker *mark=new QwtPlotMarker;
//    markrlist << mark;
//    mark->setSymbol(markerSymbol);
//    mark->setXValue(0);//here you have to set the coordinate axis i.e. where the axis are meeting.
//    mark->attach(d_plot);
}

MainWindow::~MainWindow()
{
    settings.setValue("maxPosValue", ui->lineEditUDPMaxVal->text().toInt());
    settings.setValue("minPosValue", ui->lineEditMinVal->text().toInt());
    settings.setValue("dirInverse", ui->checkBoxDirInverse->isChecked());
    settings.setValue("vmax_mmsec", ui->lineEdit_vmax_mmsec->text().toInt());

    delete ui;
}

void MainWindow::sendTimeOut()
{
    qDebug()<< qPrintable("timeout");

}

void MainWindow::dataProcess100msTimeOut()
{
//    for(int i=0; i<markrlist.length(); i++){
//        markrlist[i]->setXValue(markerXPos);
//        plotList[i]->replot();
//    }
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
            QString comName = (ui->comComboBox->currentData().toString());
            if(comName.length() > 0){
                //UartThread.requestToStart(comName);
                serial.setPortName(comName);
                if (!serial.open(QIODevice::ReadWrite)) {
                    //qDebug("%s port open FAIL", qUtf8Printable(comName));
                    ui->plainTextEdit->appendPlainText(QString("%1 port open FAIL %2").arg(qUtf8Printable(comName)).arg(serial.error()));
                    return;                    
                }
                else{
                    ui->comComboBox->setDisabled(true);
                    //qDebug("%s port opened", qUtf8Printable(comName));
                    ui->plainTextEdit->appendPlainText(QString("%1 port opened").arg(qUtf8Printable(comName)));
    //                connect(&serial, SIGNAL(readyRead()),
    //                        this, SLOT(handleReadyRead()));
    //                connect(&serial, SIGNAL(bytesWritten(qint64)),
    //                        this, SLOT(handleSerialDataWritten(qint64)));
                    ui->pushButtonComOpen->setText("close");
                    comExchanges = 0;
                    usbConnectionTime.start();
                }
            }
        }
    }
    else{
        serial.close();
        //udpSocket->close();
        //qDebug("com port closed");
        ui->plainTextEdit->appendPlainText(QString("%1 closed").arg(serial.portName()));
        ui->pushButtonComOpen->setText("open");        
        //contrStringQueue.clear();
        ui->comComboBox->setDisabled(false);
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


int lastMsec = 0;
void MainWindow::sendDivPos(int mi, DivPosDataStr &ds)
{
    quint32 dir = ds.dir;
    quint32 steps = ds.steps;
    quint32 div = FPGA_FREQ/ (steps*10); //calc speed
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

void MainWindow::parseFPGAMsg(QByteArray ba)
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

    bool bAllMotorHasCmd = true;
//    for(int i=0; i<motorCount; i++){
//        bAllMotorHasCmd &= (motorPosCmdData[i].isEmpty() == false);
//    }
    bAllMotorHasCmd = true; // ***
    if((bAllFree == true) && bAllMotorHasCmd){
        allFreeToWrite();
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

//    if(bEna == true){
//        motorAbsolutePosCur[i] = 0;
//    }
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
    bTermState[i] = bEna;
}

//motorPosCmdData not empty
void MainWindow::allFreeToWrite()
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
                //if(i == 0) qDebug() << curMsec << ds.steps;
                sendDivPos(i, ds);
                motorPosCmdData[i].removeFirst();
                bFreeToWrite[i] = false;
            }
            else{
                if(ds.skipStartTime == 0){
                    ds.skipStartTime = curMsec;
                    //if(i == 0) qDebug() << curMsec << "new wait";
                }
            }
        }
            break;
        default:
            break;

        }

    }

}

void MainWindow::freeToWrite(int i)
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
//        if(motorPosCmdData[i].isEmpty() == false){
//            DivPosDataStr ds;
//            ds = motorPosCmdData[i].first();
//            int curMsec = QTime::currentTime().msecsSinceStartOfDay();


//            //qDebug("div=%x, st=%d, d=%d", ds.div, ds.steps, ds.dir);
////            if((ds.pos==0)&&(getMotorAbsPos(i)==0)&&(bTermState[i]==false)){
////                qDebug("not in zero and no term!");
////                ds.pos = -400;
////                sendDivPos(i, ds, ds.pos);
////            }
////            else{
//                if(sendDivPos(i, ds, ds.pos) == true){
//                    motorPosCmdData[i].dequeue();
//                    bFreeToWrite[curMotorSendIdx] = false;
//                }
//                //motorPosCmdData[i].dequeue();
////            }
//        }
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

void MainWindow::parseCmdMultiMotorStr(QString cmdMultiMotorStr, quint32 udpDgRecvInterval)
{                    
    //qDebug() << udpDgRecvInterval;
    int maxUDPVal = ui->lineEditUDPMaxVal->text().toInt();
    int maxHeightImpVal = ui->lineEdit_MaxHeightImp->text().toInt();
//    int msecsForStep = 0;
//    if(motorPosCmdData[0].isEmpty() == true){
//        msecsForStep = QTime::currentTime().msecsSinceStartOfDay();
//    }
//    else{
//        msecsForStep = motorPosCmdData[0].last().absMsec;
//    }
//    msecsForStep += 100;

//last().absMsec + 100;
    QStringList motorStrList =  cmdMultiMotorStr.split("p", QString::SkipEmptyParts);
//    if(motorStrList.length() != MOTOR_CNT){
//        ui->plainTextUDP->appendPlainText("motorStrList.length not equal MOTOR_CNT");
//        return;
//    }
    //foreach (QString motorStr, motorStrList) {
    for(int i=0; i<motorStrList.length(); i++){
        QString vs = motorStrList[i];                
        quint32 convVal = vs.toInt();
        if(convVal > maxUDPVal)
            ui->plainTextUDP->appendPlainText("!!! max UDP val error !!!");
        if( (i>9) || (i<0))
            ui->plainTextUDP->appendPlainText("!!! motInd error !!!");
        quint32 newPos = maxHeightImpVal * (convVal/(float)maxUDPVal);
        //qDebug() << convVal << newPos;
        addMotorCmd(i, newPos, udpDgRecvInterval);
    }
}


void MainWindow::addMotorCmd(int mn, int newPosImp, int msecsForMove)
{
    DivPosDataStr ds;
    ds.finishPos = newPosImp;
    if(motorPosCmdData[mn].length() == 0){
        int delta = newPosImp - getMotorAbsPosImp(mn);
        int vmaxMmsec = ui->lineEdit_vmax_mmsec->text().toInt();
        int maxImpPerDelta = mmToImp(vmaxMmsec)/10;
        //qDebug("%d", maxImpPerDelta);

        int n=1;
        for(; abs(delta)>maxImpPerDelta; n*=2){
            delta/=2;
        }
        if(n>1){
            //qDebug("%d add %d Pts %d", mn, n, delta);
            ui->plainTextUDP->appendPlainText(QString("%1 add %2 Pts,delta=%3")
                                              .arg(mn)
                                              .arg(n)
                                              .arg(delta));
        }
        quint32 pos = getMotorAbsPosImp(mn);

        for(int i=0; i<n; i++){
            //ds.absMsec += 100;
            ds.finishPos = pos;

            pos += delta;
            ds.dir = delta > 0? 1: 0;
            if(ui->checkBoxDirInverse->isChecked())
                ds.dir = delta > 0? 0: 1;

            ds.steps =  abs(delta);
            ds.msecsFor = msecsForMove;
            motorPosCmdData[mn].append(ds);
        }

    }
    else{
        int delta = newPosImp - motorPosCmdData[mn].last().finishPos;
        ds.steps =  abs(delta);
        ds.dir = delta > 0? 1: 0;
        if(ui->checkBoxDirInverse->isChecked())
            ds.dir = delta > 0? 0: 1;

        float secsOnStep = msecsForMove / (ds.steps*1000.);
        ds.div = (quint32)(FPGA_FREQ * secsOnStep) ;
        if(ds.div > 0x1fff){
            qDebug("%d maxSpeed err %x", mn, ds.div);
            ds.div = 0x1fff;
        }

        //if(delta != 0)
        //    qDebug("%d st=%d div=%d, dir=%d", mn, delta, (qint32)FPGA_FREQ/(delta*10), delta/abs(delta));
        ds.msecsFor = msecsForMove;
        ds.skipStartTime = 0;
        motorPosCmdData[mn].append(ds);
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

//    foreach (QPolygonF plg, polyPoslist) {
//        plg.clear();
//    }
//    foreach (QPolygonF plg, polyVellist) {
//        plg.clear();
//    }

//    foreach (QwtPlotCurve* crv, posCurveList) {
//        crv->setSamples(QPolygonF());
//    }
//    foreach (QwtPlotCurve* crv, velCurveList) {
//        crv->setSamples(QPolygonF());
//    }

//    foreach (QwtPlot* plt, plotList) {
//        plt->replot();
//    }

}
void MainWindow::stateChanged(QAbstractSocket::SocketState socketState)
{
    switch(socketState){
    case QAbstractSocket::ListeningState: ui->lineEditUdpState->setText("listen"); break;
    case QAbstractSocket::BoundState: ui->lineEditUdpState->setText("bound"); break;
    case QAbstractSocket::UnconnectedState: ui->lineEditUdpState->setText("uncon"); break;


    }

}

void MainWindow::readPendingDatagrams()
{
    QTime rpdEst;
    rpdEst.start();
    //qDebug("%d readPendingDatagrams", QTime::currentTime().msecsSinceStartOfDay()&0xfff );
    int gridLines = 0;
    int rb = 0;
    while (udpSocket->hasPendingDatagrams()) {
        dataGramCnt++;
        //qDebug() << "1";
        //qDebug() << "pds:"<< udpSocket->pendingDatagramSize();
        QNetworkDatagram datagram = udpSocket->receiveDatagram();
        //qDebug() << "2";
        rb += datagram.data().length();
        QString dataStr = QString(datagram.data());
        //continue;

        //qDebug() << "3";
       // QStringList outStrings;
        if(dataStr.compare("start\r\n") == 0){
//            lastCmdMap.clear();
//            lastPosMap.clear();
//            graphReset();
//            motorPosCmdStrings.clear();
//            if(dataProcess100msTimer.isActive() == false){
//                dataProcess100msTimer.start();
//            }
            //qDebug("start cmd");
            ui->plainTextUDP->appendPlainText("start cmd");
        }
        else if(dataStr.compare("init\r\n") == 0){
            on_pushButtonInitiate_clicked();
            ui->plainTextUDP->appendPlainText("init cmd");
        }
        else if(dataStr.compare("stop\r\n") == 0){            
            qDebug("stop cmd");
            for(int i=0; i<MOTOR_CNT; i++){
                motorPosCmdData[i].clear();                
            }
            ui->plainTextUDP->appendPlainText("stop cmd");
        }
        else{
            QStringList list1 = dataStr.split("\r\n", QString::SkipEmptyParts);
            //qDebug()<<dataStr;
            quint32 curMsecs = QTime::currentTime().msecsSinceStartOfDay();
            qint32 udpDgRecvInterval = curMsecs - lastUdpDatagrammRecvd;
            lastUdpDatagrammRecvd = curMsecs;
            if(udpDgRecvInterval < 1000){
                udpDgrmRecvIntervalList.append(udpDgRecvInterval);
                if(udpDgrmRecvIntervalList.length() > 20){
                    udpDgrmRecvIntervalList.removeFirst();
                    quint32 s=0;
                    foreach (quint32 recvInterval, udpDgrmRecvIntervalList) {
                        s+=recvInterval;
                    }
                    s/=20;
                    ui->statusBar->showMessage(QString("average UDP recv interval %1").arg(s), 5000);
                }
            }

            QString showStr = QString("%1 %2 (%3)").arg(QTime::currentTime().toString("mm:ss:zzz"))
                                                    .arg(dataStr).arg(udpDgRecvInterval);
//            int indOfCrLn = showStr.lastIndexOf("\r\n");
//            if(indOfCrLn != -1){
//                showStr.remove(indOfCrLn, 2);
//            }
            ui->plainTextUDP->appendPlainText(showStr);

            if(udpDgRecvInterval > 400) udpDgRecvInterval=400;

            foreach (QString multiMotorStr, list1) {
                QStringList motorList = multiMotorStr.split("S", QString::SkipEmptyParts);
                QString outString;
                foreach (QString cmdStr, motorList) {
                    outString += cmdStr.mid(1, 4);
                }
                parseCmdMultiMotorStr(outString, udpDgRecvInterval);
                //outString += "\r\n";
                //outStrings << outString;

            }
            gridLines++;
        }
    }
}


void MainWindow::on_pushButton_refreshCom_clicked()
{
    ui->comComboBox->clear();
    foreach (QComboBox *cb, debPortCmbBxList) {
        cb->clear();
    }

    const auto serialPortInfos = QSerialPortInfo::availablePorts();
    const QString blankString = QObject::tr("N/A");
      QString description;
      QString manufacturer;
      QString serialNumber;
    for (int i=0; i< serialPortInfos.length(); i++){
           const QSerialPortInfo &serialPortInfo = serialPortInfos[i];
           description = serialPortInfo.description();
           manufacturer = serialPortInfo.manufacturer();
           serialNumber = serialPortInfo.serialNumber();
//           qDebug() << endl
//               << QObject::tr("Port: ") << serialPortInfo.portName() << endl
//               << QObject::tr("Location: ") << serialPortInfo.systemLocation() << endl
//               << QObject::tr("Description: ") << (!description.isEmpty() ? description : blankString) << endl
//               << QObject::tr("Manufacturer: ") << (!manufacturer.isEmpty() ? manufacturer : blankString) << endl
//               << QObject::tr("Serial number: ") << (!serialNumber.isEmpty() ? serialNumber : blankString) << endl
//               << QObject::tr("Vendor Identifier: ") << (serialPortInfo.hasVendorIdentifier() ? QByteArray::number(serialPortInfo.vendorIdentifier(), 16) : blankString) << endl
//               << QObject::tr("Product Identifier: ") << (serialPortInfo.hasProductIdentifier() ? QByteArray::number(serialPortInfo.productIdentifier(), 16) : blankString) << endl
//               << QObject::tr("Busy: ") << (serialPortInfo.isBusy() ? QObject::tr("Yes") : QObject::tr("No")) << endl;

           QString cbItemName = QString("%1 (%2)").arg(serialPortInfo.portName()).arg(!description.isEmpty() ? description : blankString);
           QString cbToolTipText = QString("Port: %1 <br>"
                                           "Location: %2 <br>"
                                           "Description: %3 <br>"
                                           "Manufacturer: %4 <br>"
                                           "Serial number: %5 <br>"
                                           "Vendor Identifier: %6 <br>"
                                           "Product Identifier: %7 <br>"
                                           "Busy: %8").arg(serialPortInfo.portName())
                                                            .arg(serialPortInfo.systemLocation())
                                                           .arg(!description.isEmpty() ? description : blankString)
                   .arg(!manufacturer.isEmpty() ? manufacturer : blankString)
                   .arg(!serialNumber.isEmpty() ? serialNumber : blankString)
                   .arg(serialPortInfo.hasVendorIdentifier() ? QByteArray::number(serialPortInfo.vendorIdentifier(), 16) : blankString)
                   .arg(serialPortInfo.hasProductIdentifier() ? QByteArray::number(serialPortInfo.productIdentifier(), 16) : blankString)
                   .arg(serialPortInfo.isBusy() ? QObject::tr("Yes") : QObject::tr("No"));

           ui->comComboBox->addItem(cbItemName, serialPortInfo.portName());
           ui->comComboBox->setItemData(i, cbToolTipText, Qt::ToolTipRole);
           foreach (QComboBox *cb, debPortCmbBxList) {
               cb->addItem(cbItemName, serialPortInfo.portName());
               cb->setItemData(i, cbToolTipText, Qt::ToolTipRole);
           }
    }
}

void MainWindow::on_pushButtonClear_clicked()
{    
    ui->plainTextEdit->clear();
    ui->plainTextUDP->clear();

    for(int i=0; i<MOTOR_CNT; i++){
        motorPosCmdData[i].clear();

    }

}

void MainWindow::sendOnTimer()
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
                //if(i == 0) qDebug() << curMsec << "external waiting" << ds.msecsFor;
                if((curMsec - ds.skipStartTime) > ds.msecsFor){
                    motorPosCmdData[i].removeFirst();
                }
            }
        }
    }
    QTime tt;
    tt.start();
    char c = 0xff;
    serial.write(&c, 1);

    int bytesToRecv = 4;
    char dArr[4];
    while(bytesToRecv>0){
        //qDebug("try to recv %d bytes", bytesToRecv);
        if(serial.waitForReadyRead(50) ==false){
            //qDebug("dataRecvTimeout on recvd %d bytes", 4-bytesToRecv);
            ui->plainTextUDP->appendPlainText(QString("dataRecvTimeout on recvd %1 bytes").arg(4-bytesToRecv));
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

//void MainWindow::on_pushMoveDownState_clicked()
//{


//    if(serial.isOpen()){
//        QString str("Sd\r\n");
//     serial.write(str.toLatin1());
//    }
//}

void MainWindow::on_pushBUttonToIdle_clicked()
{
    if(serial.isOpen()){
        QString str("Si\r\n");
     serial.write(str.toLatin1());
    }

    for(int i=0; i<MOTOR_CNT; i++){
        mtState[i] = MT_IDLE;
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
//    DivPosDataStr ds;

//    for(int i=0; i<motorCount; i++){
//        int startPos =0;

//        if(motorPosCmdData[i].length() == 0)
//            startPos = getMotorAbsPosImp(i);
//        else
//            startPos = motorPosCmdData[i].last().pos;

//        ds.pos = startPos+400;
//        for(int k=0; k<5; k++){
//            motorPosCmdData[i] << ds;
//             ds.pos += 400;
//        }
////        ds.dir = 0;
////        //for(int k=0; k<30; k++)
////        //    motorPosCmdData[i] << ds;
//    }

}

void MainWindow::on_pushMoveDown_clicked()
{
//    DivPosDataStr ds;

//    for(int i=0; i<10; i++){
//        int startPos =0;

//        if(motorPosCmdData[i].length() == 0)
//            startPos = getMotorAbsPosImp(i);
//        else
//            startPos = motorPosCmdData[i].last().pos;
//        ds.pos = startPos-400;
//        for(int k=0; k<5; k++){
//            motorPosCmdData[i] << ds;
//            ds.pos -= 400;
//        }
//        //for(int k=0; k<30; k++)
//        //    motorPosCmdData[i] << ds;
//    }
}


//void MainWindow::on_pushTestData_clicked()
//{

//    int startLength = motorPosCmdData[0].length();
//    DivPosDataStr ds;
//    ds.pos = 1;
//    ds.steps = 2400; ds.div = 1000; ds.dir = 1; motorPosCmdData[0] << ds;
//    ds.steps = 32800; ds.div = 73; ds.dir = 1; motorPosCmdData[0] << ds;
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


//    for(int i=0; i<MOTOR_CNT; i++){
//        ds = motorPosCmdData[i].last();
//        for(int j=0; j<(motorPosCmdData[i].length() - startLength); j++){
//            motorPosCmdData[i] << ds;
//        }
//    }

//}

void MainWindow::on_tabWidget_tabBarClicked(int index)
{
//    if( index==0 ){

//        for(int i=0; i<5; i++){
//            ui->qwtWdgH1->layout()->removeWidget(plotList[i]);
//            ui->qwtWdgH2->layout()->removeWidget(plotList[5+i]);
//        }
//        for(int i=0; i<5; i++){
//            ui->qwtWdgH1->layout()->addWidget(plotList[i]);
//        }

//        for(int i=0; i<5; i++){
//            ui->qwtWdgH2->layout()->addWidget(plotList[5+i]);
//        }
//    }
//    else{
//        //int idx = index-1;
//        //if(idx>4)
//        ui->tabWidget->widget(index)->layout()->addWidget(plotList[index-1]);
//    }


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
    int curTabInd = ui->tabWidget->currentIndex();
    QString tabName = ui->tabWidget->tabText(curTabInd);
    if(tabName == "mainStat"){
        bool bOk = false;
        int sliderMaxVal = ui->lineEdit_MaxHeightImp->text().toInt(&bOk);
        for(int i=0; i<motorCount; i++){
            if(bOk){
                absPosSlider[i]->setMaximum(sliderMaxVal);
                absPosSlider[i]->setTickInterval(sliderMaxVal/5);
            }
            switch(mtState[i]){
                case MT_IDLE:   stateLineEdit[i]->setText("idle"); ;break;
                case MT_GoDOWN: stateLineEdit[i]->setText("mvD");break;
                case MT_GoUP:   stateLineEdit[i]->setText("mvU");break;
                case MT_INIT_GoDOWN: stateLineEdit[i]->setText("initMvD");break;
                case MT_INIT_GoUp: stateLineEdit[i]->setText("initMvU");break;
            }

            int mmPerRot = ui->lineEdit_mmPerRot->text().toInt();
            int impPerRot = ui->lineEdit_ImpPerRot->text().toInt();
            int posMm = (motorAbsolutePosCur[i]/(float)impPerRot)*mmPerRot;
            if(ui->checkBoxSliderPosCtrl->isChecked()==false)
                absPosSlider[i]->setValue(motorAbsolutePosCur[i]);
            absPosLineEdit[i]->setText(QString::number(posMm));
            termCheckBox[i]->setChecked(bTermState[i]);
            euqueLineEdit[i]->setText(QString::number(motorPosCmdData[i].length()));

        }
        ui->lineEditComExchanges->setText(QString::number(comExchanges));

        if(serial.isOpen() == true){
            ui->lineEditUSBConnectionTime->setText(QTime::fromMSecsSinceStartOfDay(usbConnectionTime.elapsed()).toString());
        }
        if(udpSocket->state()== QAbstractSocket::BoundState){
            ui->lineEditUDPConnectionTime->setText(QTime::fromMSecsSinceStartOfDay(udpConnectionTime.elapsed()).toString());
        }

        ui->lineEditBytesOnIter->setText(QString::number(bytesOnIter));

        ui->lineEditDataGramCnt->setText(QString::number(dataGramCnt));


        //qDebug() << connectionTime.elapsed() << connectionTime.toString("zzz");
        //connectionTime.elapsed()
    }
    else if(tabName == "timeStat"){
        int curMsec = QTime::currentTime().msecsSinceStartOfDay();
        for(int i=0; i<MOTOR_CNT; i++){
            if(motorPosCmdData[i].size() != 0){
                int shift = 0;
                if(i==0){
                   // qDebug("%d >> %d",curMsec, motorPosCmdData[i].first().absMsec);
                }
                //shift = motorPosCmdData[i].first().absMsec - curMsec;

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

}

void MainWindow::udpServerOpen()
{
    if(udpSocket == NULL){
        udpSocket = new QUdpSocket(this);
        connect(udpSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
                this, SLOT(stateChanged(QAbstractSocket::SocketState)));

        //if(udpSocket->bind(QHostAddress("192.168.0.104"), 8051) == true){
        if(udpSocket->bind(QHostAddress::Any, 8051) == true){
            //qDebug("UDP bind OK");
            ui->plainTextEdit->appendPlainText("UDP bind OK");
        }
        else{
            ui->plainTextEdit->appendPlainText("UDP bind FAIL");

        }

        connect(udpSocket, SIGNAL(readyRead()),
                this, SLOT(readPendingDatagrams()));
        udpConnectionTime.start();
        ui->pushButtonUdpOpenClose->setText("UDP Close");
    }
}

void MainWindow::udpServerClose()
{
    udpSocket->close();
    disconnect(udpSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
               this, SLOT(stateChanged(QAbstractSocket::SocketState)));
    disconnect(udpSocket, SIGNAL(readyRead()),
               this, SLOT(readPendingDatagrams()));
    ui->pushButtonUdpOpenClose->setText("UDP Open");
    ui->plainTextEdit->appendPlainText("UDP closed");

}


void MainWindow::on_pushButtonUdpOpenClose_clicked()
{
    if(ui->pushButtonUdpOpenClose->text() == "UDP Open"){
        udpServerOpen();
    }
    else{
        udpServerClose();
        ui->pushButtonUdpOpenClose->setText("UDP Open");
    }
}

void MainWindow::on_pushButtonInitiate_clicked()
{
    //udpServerClose();
    on_pushButtonClear_clicked();

    for(int i=0; i<MOTOR_CNT; i++){
        mtState[i] = MT_INIT_GoDOWN;
    }
}

void MainWindow::on_pushButtonGoZero_clicked()
{
//    DivPosDataStr ds;
//    int startPos =0;
//    for(int mi=0; mi<MOTOR_CNT; mi++){
//        if(motorPosCmdData[mi].length() == 0)
//            startPos = getMotorAbsPosImp(mi);
//        else
//            startPos = motorPosCmdData[mi].last().pos;

//        int iters = startPos/400;
//        ds.pos = startPos;
//        for(int i=0; i<iters; i++){
//            ds.pos -=400;
//            motorPosCmdData[mi] << ds;
//        }
//        if(ds.pos > 0){
//            ds.pos = 0;
//            motorPosCmdData[mi] << ds;
//        }
//    }


}

void MainWindow::on_pushButtonTest_clicked()
{
    for(int i=0; i<motorCount; i++){
        for(int j=0; j<10; j++){
            if(i == 0){
                for(int k=0; k<0; k++)
                    addMotorCmd(i, 0, 100);
            }
            else if(i == 1){
                for(int k=0; k<10; k++)
                    addMotorCmd(i, 0, 100);
            }
            else if(i == 2){
                for(int k=0; k<20; k++)
                    addMotorCmd(i, 0, 100);
            }


            for(int k=0; k<100; k++){
                //qDebug() << maxHeightImpVal * qSin((k/100.)*M_PI);
                quint32 newPos = 116200 * qSin((k/100.)*M_PI);
                addMotorCmd(i, newPos, 100);
            }

            if(i == 0){
                for(int k=0; k<20; k++)
                    addMotorCmd(i, 0, 100);
            }
            else if(i == 1){
                for(int k=0; k<10; k++)
                    addMotorCmd(i, 0, 100);
            }
            else if(i == 2){
                for(int k=0; k<0; k++)
                    addMotorCmd(i, 0, 100);
            }
        }
    }
}

void MainWindow::on_lineEdit_maxHeightMM_editingFinished()
{
    int mmPerRot = ui->lineEdit_mmPerRot->text().toInt();
    int maxHeightMm = ui->lineEdit_maxHeightMM->text().toInt();
    int impPerRot = ui->lineEdit_ImpPerRot->text().toInt();
    quint32 maxHeightImp = (maxHeightMm/mmPerRot)*impPerRot;
    ui->lineEdit_MaxHeightImp->setText(QString::number(maxHeightImp));
    QSettings settings("Murinets", "MotorControl");
    settings.setValue("maxHeightImp", maxHeightImp);

}

void MainWindow::on_lineEdit_MaxHeightImp_editingFinished()
{
    int mmPerRot = ui->lineEdit_mmPerRot->text().toInt();
    quint32 maxHeightImp = ui->lineEdit_MaxHeightImp->text().toInt();
    int impPerRot = ui->lineEdit_ImpPerRot->text().toInt();
    int maxHeigtMM = (maxHeightImp/impPerRot)*mmPerRot;
    ui->lineEdit_maxHeightMM->setText(QString::number(maxHeigtMM));
    QSettings settings("Murinets", "MotorControl");
    settings.setValue("maxHeightImp", maxHeightImp);

}

void MainWindow::on_radioButtonFpgaFreq25_clicked()
{
    FPGA_FREQ = FPGA_FREQ_25;
    QSettings settings("Murinets", "MotorControl");
    settings.setValue("FPGA_FREQ", FPGA_FREQ_25);
}

void MainWindow::on_radioButtonFpgaFreq24_clicked()
{
    FPGA_FREQ = FPGA_FREQ_24;
    QSettings settings("Murinets", "MotorControl");
    settings.setValue("FPGA_FREQ", FPGA_FREQ_24);

}

void MainWindow::on_lineEdit_mmPerRot_editingFinished()
{
    QSettings settings("Murinets", "MotorControl");
    settings.setValue("mmPerRot", ui->lineEdit_mmPerRot->text().toInt());
    on_lineEdit_maxHeightMM_editingFinished();
}

void MainWindow::on_lineEdit_vmax_mmsec_editingFinished()
{
    QSettings settings("Murinets", "MotorControl");
    settings.setValue("vmax_mmsec", ui->lineEdit_vmax_mmsec->text().toInt());
}


int MainWindow::mmToImp(int mm)
{
    int mmPerRot = ui->lineEdit_mmPerRot->text().toInt();
    int impPerRot = ui->lineEdit_ImpPerRot->text().toInt();
    int impPerMm = impPerRot/mmPerRot;
    return mm*impPerMm;
}

void MainWindow::handleSliderReleased(int id, int newPos)
{
    qDebug() << id << newPos;
}

void MainWindow::createMainInterface()
{
    foreach (QWidget *w, motorStatWidgetList) {
        delete w;
    }
    motorStatWidgetList.clear();
    absPosSlider.clear();
    absPosLineEdit.clear();
    stateLineEdit.clear();
    termCheckBox.clear();
    euqueLineEdit.clear();
    debPortStatusMainLeList.clear();

    //QHBoxLayout *hblo = new QHBoxLayout(ui->groupBoxMain);

    QLayout *lo = ui->groupBoxMain->layout();
    if(lo == NULL){
        lo = new QHBoxLayout();
    }
    else{

        QHBoxLayout *hblo = ((QHBoxLayout*)lo);

        for(int i=0; i<hblo->count(); i++){
            delete hblo->takeAt(i);
        }
    }

    //QWidget *mainWdg = new QWidget(ui->groupBoxMain);
    //lo->addWidget(mainWdg);
    //QHBoxLayout *hblo = new QHBoxLayout(mainWdg);
    //lo->addWidget(mainWdg);

    for(int i=0; i<motorCount; i++){
        QWidget *wdg = new QWidget(ui->groupBoxMain);
        QVBoxLayout *vblo = new QVBoxLayout(wdg);

        QSlider *sl = new QSlider(wdg);
        absPosSlider.append(sl);
        connect(sl, &QSlider::sliderReleased,
                [this, i](){ handleSliderReleased(i, absPosSlider[i]->pos().rx());});
        vblo->addWidget(sl, 0, Qt::AlignHCenter);
        QLineEdit *le = new QLineEdit(wdg);
        le->setReadOnly(true);
        le->setMaximumWidth(40);
        le->setAlignment(Qt::AlignHCenter);
        le->setAttribute(Qt::WA_TransparentForMouseEvents);
        vblo->addWidget(le, 0, Qt::AlignHCenter);
        absPosLineEdit.append(le);

        le = new QLineEdit(wdg);
        le->setReadOnly(true);
        le->setMaximumWidth(40);
        le->setAlignment(Qt::AlignHCenter);
        le->setAttribute(Qt::WA_TransparentForMouseEvents);
        vblo->addWidget(le, 0, Qt::AlignHCenter);
        stateLineEdit.append(le);

        QCheckBox *cb = new QCheckBox(wdg);
        cb->setAttribute(Qt::WA_TransparentForMouseEvents);
        cb->setFocusPolicy(Qt::NoFocus);
        //cb->setChecked(true);
        vblo->addWidget(cb, 0, Qt::AlignHCenter);
        termCheckBox.append(cb);

        le = new QLineEdit(wdg);
        le->setReadOnly(true);
        le->setMaximumWidth(40);
        le->setAlignment(Qt::AlignHCenter);
        le->setAttribute(Qt::WA_TransparentForMouseEvents);
        vblo->addWidget(le, 0, Qt::AlignHCenter);
        euqueLineEdit.append(le);
        //vblo->setAlignment(Qt::AlignHCenter);

        le = new QLineEdit(wdg);
        le->setReadOnly(true);
        le->setAttribute(Qt::WA_TransparentForMouseEvents);
        le->setMaximumWidth(40);
        le->setAlignment(Qt::AlignHCenter);
        le->setText("N/A");
        le->setPalette(*paletteGrey);
        vblo->addWidget(le, 0, Qt::AlignHCenter);
        debPortStatusMainLeList.append(le);

        wdg->setLayout(vblo);
        lo->addWidget(wdg);
        //hblo->addStretch();
        motorStatWidgetList.append(wdg);
    }
    ((QHBoxLayout*)lo)->addStretch(10);
    ui->groupBoxMain->setLayout(lo);
}

void MainWindow::createDebugSerialPortInterface()
{
    if(paletteGrey == NULL){
        paletteGrey = new QPalette();
        paletteGrey->setColor(QPalette::Base,Qt::lightGray);
    }
    if(paletteRed == NULL){
        paletteRed = new QPalette();
        paletteRed->setColor(QPalette::Base,Qt::red);
    }
    if(paletteGreen == NULL){
        paletteGreen = new QPalette();
        paletteGreen->setColor(QPalette::Base,Qt::green);
    }

    checkDebugComTimer.stop();
    foreach (QGroupBox *gb, debPortGbList) {
        delete gb;
    }

    debPortCmbBxList.clear();
    debPortpbList.clear();
    debSerialPortList.clear();
    debPortGbList.clear();
    debPortStatusLeList.clear();

    QLayout *hblo = ui->tabDebugPort->layout();
    if(hblo == NULL){
        hblo = new QHBoxLayout;
    }
    for(int i=0; i<motorCount; i++){
        QGroupBox *gb = new QGroupBox(QString("Port %1").arg(i));
        //gb->setAlignment(Qt::AlignHCenter);
        QVBoxLayout *vblo = new QVBoxLayout(gb);
        QComboBox *cb = new QComboBox(gb);
        vblo->addWidget(cb);
        QPushButton *pb = new QPushButton("open", gb);
        vblo->addWidget(pb);
        QLineEdit *le = new QLineEdit(this);
        le->setReadOnly(true);
        le->setAttribute(Qt::WA_TransparentForMouseEvents);
        le->setAlignment(Qt::AlignHCenter);
        le->setText("N/A");
        le->setPalette(*paletteGrey);
        vblo->addWidget(le);

        vblo->setAlignment(Qt::AlignTop);

        gb->setLayout(vblo);
        hblo->addWidget(gb);

        QSerialPort *sp = new QSerialPort(gb);
        connect(sp, &QSerialPort::errorOccurred, [this, i](QSerialPort::SerialPortError error){ handleComPortErrorOccured(i, error);});
        connect(pb, &QPushButton::clicked, [=](){ pushDebugComPortOpen(i);});
        connect(sp, &QSerialPort::readyRead, [this, i](){ handleReadyRead(i);});

        debPortCmbBxList.append(cb);
        debPortpbList.append(pb);
        debSerialPortList.append(sp);
        debPortGbList.append(gb);
        debPortStatusLeList.append(le);

        motorRespRecvdList.append(new bool(false));
    }

    //ui->tabWidget->ind
    //QWidget *tw = ui->tabWidget->widget(3);
    //ui->widgetComPorts->setLayout(hblo);
    ui->tabDebugPort->setLayout(hblo);

    //cmb->
    //wdg->add
    //ui->widgetComPorts->
    on_pushButton_refreshCom_clicked();


    connect(&checkDebugComTimer, SIGNAL(timeout()), this, SLOT(checkDebugComTimerHandle()));
    checkDebugComTimer.setSingleShot(false);
    checkDebugComTimer.setInterval(500);
    checkDebugComTimer.start();
}

void MainWindow::pushDebugComPortOpen(int id)
{
    QSerialPort &sp = *debSerialPortList[id];
    QPushButton &pb = *debPortpbList[id];
    QComboBox &cb = *debPortCmbBxList[id];

    sp.setBaudRate(38400);
     if(pb.text() == "open"){
         if(sp.isOpen() == false){
             QString comName = cb.currentData().toString();
             if(comName.length() > 0){
                 //UartThread.requestToStart(comName);
                 sp.setPortName(comName);
                 if (!sp.open(QIODevice::ReadWrite)) {
                     //qDebug("%s port open FAIL", qUtf8Printable(comName));
                     ui->plainTextEdit->appendPlainText(QString("%1 port open FAIL").arg(qUtf8Printable(comName)));
                     return;
                 }
                 else{
                     cb.setDisabled(true);
                     ui->plainTextEdit->appendPlainText(QString("%1 port opened").arg(qUtf8Printable(comName)));
     //                connect(&serial, SIGNAL(readyRead()),
     //                        this, SLOT(handleReadyRead()));
     //                connect(&serial, SIGNAL(bytesWritten(qint64)),
     //                        this, SLOT(handleSerialDataWritten(qint64)));
                     pb.setText("close");
                 }
             }
         }
     }
     else{
         sp.close();
         ui->plainTextEdit->appendPlainText(QString("%1 closed").arg(sp.portName()));
         pb.setText("open");
         cb.setDisabled(false);
     }

}

void MainWindow::comPortClose(int id)
{
    QSerialPort &sp = *debSerialPortList[id];
    QPushButton &pb = *debPortpbList[id];
    QComboBox &cb = *debPortCmbBxList[id];
    QLineEdit &le = *debPortStatusLeList[id];

    cb.setEnabled(true);
    pb.setText("open");
    if(sp.isOpen() == true){
        ui->plainTextEdit->appendPlainText(QString("%1 closed").arg(sp.portName()));
        sp.close();
    }
    le.setPalette(*paletteGrey);
}

void MainWindow::on_lineEditMotorCount_editingFinished()
{
    motorCount = ui->lineEditMotorCount->text().toInt();
    settings.setValue("motorCount", motorCount);
    ui->plainTextEdit->appendPlainText(QString("new motor count %1").arg(ui->lineEditMotorCount->text()));
    createDebugSerialPortInterface();
    createMainInterface();
}

void MainWindow::handleComPortErrorOccured(int id, QSerialPort::SerialPortError error)
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

        QString msg = QString("%1 error: %2").arg(qUtf8Printable(debSerialPortList[id]->portName())).arg(errorStr);
        ui->plainTextEdit->appendPlainText(msg);
        //qDebug() <<"!!!!!!!" << id <<error;
        if((error == QSerialPort::ResourceError) ||
           (error == QSerialPort::PermissionError)){
            //pushButtonComOpen_clicked(id);
            comPortClose(id);
        }
    }
}

void MainWindow::handleReadyRead(int id)
{
    QByteArray ba = debSerialPortList[id]->readAll();
    //qDebug() << id << ba;
    parseLeadShineMsg(id, ba);

}

void MainWindow::checkDebugComTimerHandle()
{
    QByteArray reqBase = QByteArrayLiteral("\x01\x03\x00");
    //const quint8 req1Str[] {0x01, 0x03, 0x00, 0xFD, 0x00, 0x01, 0x15, 0xfa};
    QByteArray req1 = reqBase + QByteArrayLiteral("\xFD\x00\x01\x15\xfa");
    QByteArray reqReqErrTrace = reqBase + QByteArrayLiteral("\x10\x00\x0A\xC4\x08");
    //req1 << 0x01 << 0x03 << 0x00 << 0xFD << 0x00 << 0x01 << 0x15 << 0xfa;

    for(int i=0; i<debPortStatusLeList.length(); i++){
        bool &bRespRecv = *(motorRespRecvdList[i]);
        if(bRespRecv == false){
            debPortStatusMainLeList[i]->setPalette(*paletteGrey);
            debPortStatusLeList[i]->setPalette(*paletteGrey);
        }
        QSerialPort *sp = debSerialPortList[i];
        if(sp->isOpen()){
            sp->write(reqReqErrTrace);
        }
        bRespRecv = false;
    }
}

void MainWindow::parseLeadShineMsg(int id, QByteArray &ba)
{
    quint16 crc16 = CRC16_ModBusRTU(ba, ba.length()-2);
    QByteArray crc16ba;
    crc16ba.append(crc16&0xff);
    crc16ba.append((crc16>>8)&0xff);

    //quint8 crc16Recvd = ba[ba.length()-1];
    if(ba.endsWith(crc16ba) == false){
        ui->plainTextEdit->appendPlainText(QString("debResp%1: recv CRC err").arg(id));
        ui->plainTextEdit->appendPlainText(QString("debResp%1: %2 -> %3").arg(id).arg(ba.size()).arg(QString(ba.toHex().toUpper())));
        ui->plainTextEdit->appendPlainText(QString("debResp%1: CRC %2").arg(id).arg(crc16));
        return;
    }

    if((ba.length()==25)&&(ba[0] == 0x01)&&(ba[1] == 0x03)&&(ba[2] == 0x14)){
        bool &bRespRecv = *(motorRespRecvdList[id]);
        bRespRecv = true;

        //ui->plainTextEdit->appendPlainText(QString("debResp%1: %2 -> %3").arg(id).arg(ba.size()).arg(QString(ba.toHex().toUpper())));
        if((ba[4]&0x50) == 0){
            debPortStatusLeList[id]->setPalette(*paletteGreen);
            debPortStatusMainLeList[id]->setPalette(*paletteGreen);
            //ui->plainTextEdit->appendPlainText(QString("debResp%1: OK").arg(id));
            debPortStatusLeList[id]->setText("OK");
            debPortStatusMainLeList[id]->setText("OK");

        }
        else{
            debPortStatusLeList[id]->setPalette(*paletteRed);
            debPortStatusMainLeList[id]->setPalette(*paletteRed);
            if(ba[4]&0x10){ //encErr
                debPortStatusLeList[id]->setText("enc err");
                debPortStatusMainLeList[id]->setText("enc err");
            }
            if(ba[4]&0x40){ //posErr
                debPortStatusLeList[id]->setText("pos err");
                debPortStatusMainLeList[id]->setText("pos err");
            }
        }
    }
    else{
        ui->plainTextEdit->appendPlainText(QString("debResp%1: %2 -> %3").arg(id).arg(ba.size()).arg(QString(ba.toHex().toUpper())));
    }
}

#include <QtEndian>
quint16 MainWindow::CRC16_ModBusRTU(QByteArray buf, quint16 len)
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


