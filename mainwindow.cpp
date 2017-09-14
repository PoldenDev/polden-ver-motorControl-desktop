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
    speed(100),
    udpSocket(NULL),    
    cmdNum(0),
    curMotorSendIdx(0),
    settings("murinets", "vertolet"),
    xUdpRecv(0),
    markerXPos(0),
    comExchanges(0),
    bytesOnIter(0),
    dataGramCnt(0)
{
    ui->setupUi(this);

    ui->lineEditMaxVal->setText(QString::number(settings.value("maxPosValue", 1000).toInt()));
    ui->lineEditMinVal->setText(QString::number(settings.value("minPosValue", 0).toInt()));

    on_pushButton_refreshCom_clicked();

    on_pushButtonUdpOpenClose_clicked();

    QSettings settings("Murinets", "MotorControl");
    ui->lineEdit_mmPerRot->setText(settings.value("mmPerRot", 10).toString());
    ui->lineEdit_MaxHeightImp->setText(settings.value("maxHeightImp", 200000).toString());
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

    ui->lineEdit_vmax_mmsec->setText(settings.value("vmax_mmsec",5).toString());


    timer.setInterval(25);
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

    absPosSlider.append(ui->SliderPos0);
    absPosSlider.append(ui->SliderPos1);
    absPosSlider.append(ui->SliderPos2);
    absPosSlider.append(ui->SliderPos3);
    absPosSlider.append(ui->SliderPos4);
    absPosSlider.append(ui->SliderPos5);
    absPosSlider.append(ui->SliderPos6);
    absPosSlider.append(ui->SliderPos7);
    absPosSlider.append(ui->SliderPos8);
    absPosSlider.append(ui->SliderPos9);

    absPosLineEdit.append(ui->lineEditPos0);
    absPosLineEdit.append(ui->lineEditPos1);
    absPosLineEdit.append(ui->lineEditPos2);
    absPosLineEdit.append(ui->lineEditPos3);
    absPosLineEdit.append(ui->lineEditPos4);
    absPosLineEdit.append(ui->lineEditPos5);
    absPosLineEdit.append(ui->lineEditPos6);
    absPosLineEdit.append(ui->lineEditPos7);
    absPosLineEdit.append(ui->lineEditPos8);
    absPosLineEdit.append(ui->lineEditPos9);

    stateLineEdit.append(ui->lineEditState0);
    stateLineEdit.append(ui->lineEditState1);
    stateLineEdit.append(ui->lineEditState2);
    stateLineEdit.append(ui->lineEditState3);
    stateLineEdit.append(ui->lineEditState4);
    stateLineEdit.append(ui->lineEditState5);
    stateLineEdit.append(ui->lineEditState6);
    stateLineEdit.append(ui->lineEditState7);
    stateLineEdit.append(ui->lineEditState8);
    stateLineEdit.append(ui->lineEditState9);

    termCheckBox.append(ui->checkBoxTerm0);
    termCheckBox.append(ui->checkBoxTerm1);
    termCheckBox.append(ui->checkBoxTerm2);
    termCheckBox.append(ui->checkBoxTerm3);
    termCheckBox.append(ui->checkBoxTerm4);
    termCheckBox.append(ui->checkBoxTerm5);
    termCheckBox.append(ui->checkBoxTerm6);
    termCheckBox.append(ui->checkBoxTerm7);
    termCheckBox.append(ui->checkBoxTerm8);
    termCheckBox.append(ui->checkBoxTerm9);


    euqueLineEdit.append(ui->lineEditEuqueCnt0);
    euqueLineEdit.append(ui->lineEditEuqueCnt1);
    euqueLineEdit.append(ui->lineEditEuqueCnt2);
    euqueLineEdit.append(ui->lineEditEuqueCnt3);
    euqueLineEdit.append(ui->lineEditEuqueCnt4);
    euqueLineEdit.append(ui->lineEditEuqueCnt5);
    euqueLineEdit.append(ui->lineEditEuqueCnt6);
    euqueLineEdit.append(ui->lineEditEuqueCnt7);
    euqueLineEdit.append(ui->lineEditEuqueCnt8);
    euqueLineEdit.append(ui->lineEditEuqueCnt9);
    for(int i=0; i<MOTOR_CNT; i++){
        bTermState[i] = false;
    }

    //connectionTime = 0;
    uiUpdateTimer.setInterval(100);
    connect(&uiUpdateTimer, SIGNAL(timeout()), this, SLOT(uiUpdateTimerSlot()));
    uiUpdateTimer.start();

    QString build = QString("%1 ")
    .arg(QLocale(QLocale::C).toDate(QString(__DATE__).simplified(), QLatin1String("MMM d yyyy")).toString("yyyy-MM-dd"))
    .arg(QString("%1%20%3%4%5%6").arg(__TIME__[0])
    .arg(__TIME__[1])
    .arg(__TIME__[3])
    .arg(__TIME__[4])
    .arg(__TIME__[6])
    .arg(__TIME__[7]));

    ui->labelCompileTime->setText(build);

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
            QString comName = ui->comComboBox->currentText();
            if(comName.length() > 0){
                //UartThread.requestToStart(comName);
                serial.setPortName(comName);
                if (!serial.open(QIODevice::ReadWrite)) {
                    qDebug("%s port open FAIL", qUtf8Printable(comName));
                    return;
                }
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
    else{
        serial.close();
        //udpSocket->close();
        //qDebug("com port closed");
        ui->plainTextEdit->appendPlainText(QString("com port closed"));
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


int lastMsec = 0;
bool MainWindow::sendDivPos(int mi, DivPosDataStr &ds, quint32 pos)
{
    int delta = pos - getMotorAbsPos(mi);
    quint32 dir = delta > 0? 1: 0;
    quint32 steps = abs(delta);

    int curMsec = QTime::currentTime().msecsSinceStartOfDay();
    if(steps > 0){
        //calc speed
        quint32 div = FPGA_FREQ/ (steps*10);
        if(div > 0x1fff){
            qDebug() << "maxSpeed err on" << mi;
            div = 0x1fff;
        }

        quint64 temp = 0;
        temp = mi&0xf;
        temp |= ((div&0x7fff)<<4);
        temp |= (((quint64)steps&0x7fff)<<19);
        temp |= (((quint64)dir&0x1)<<34);
        QByteArray ba = QByteArray::fromRawData((char*)&temp, sizeof(quint64));

        ba.resize(5);

        if(div > 0x1fff){
            qDebug("mi %d div exceed 0x1fff!", mi);
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
            //qDebug("pos=%d steps=%d div=%d dir=%d", pos, steps, div, dir);
        }
        quint64 dSend = serial.write(ba);

        if(dSend != 5){
            qDebug("dSend %d !!!!!", dSend);
        }
        //if(serial.flush() == false){
            //qDebug("dataFlushed");
        //}
        motorAbsolutePosCur[mi] += delta;
        if( motorAbsolutePosCur[mi] < 0){
            //qDebug("mi %d motorAbsolutePosCur less 0 =", mi, motorAbsolutePosCur[mi]);
        }
     //   if(mi==0)
    //        serial.write(ba);
        return true;
    }
    else{
//        int shift = ds.absMsec - curMsec;
//        if(mi == 0){
//            //qDebug("%d", shift);
//        }
//        if( (abs(shift) <=100) ||
//             (shift <0) ){        //если проигрывание отстаёт
//            //lastCtrlTimeMsecs[mi] = curMsec;
//            if(mi==0){
//                //qDebug("%d", shift);
//                //static int lastMsecs = 0;
//                //int t = 1000*ds.steps*((float)ds.div/24000000);
//                //qDebug("d=%x s=%d t=%d %c", ds.div, ds.steps, t, ds.div < 0xe4? '!' : ' ');
//                //int delta = curMsec -lastMsecs;
//                //lastMsecs = curMsec;
//                //qDebug("z %d %s", delta, delta <98 ? "!!": " ");
//            }
//            return true;
//        }
//        else{
//            return false;
//        }
        return true;
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

    for(int i=0; i<MOTOR_CNT; i++){
        if(size != motorPosCmdData[i].length()){
            ui->plainTextUDP->appendPlainText("size not equal");
            for(int k=0; k<MOTOR_CNT; k++){
                ui->plainTextUDP->appendPlainText(QString("%1").arg(motorPosCmdData[k].length()));
            }

        }

    }

    bool bAllFree = true;
    for(int i=0; i<MOTOR_CNT; i++){
        bAllFree = (bAllFree&&bFreeToWrite[i]);

    }

    bool bAllMotorHasCmd = true;
    for(int i=0; i<MOTOR_CNT; i++){
        bAllMotorHasCmd &= (motorPosCmdData[i].isEmpty() == false);
    }
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
         motorAbsolutePosCur[i] = 0;

        //mtState[i] = MT_IDLE;
        mtState[i] = MT_INIT_GoUp;
        DivPosDataStr ds;
        int startPos =0;

        if(motorPosCmdData[i].length() == 0)
            startPos = getMotorAbsPos(i);
        else
            startPos = motorPosCmdData[i].last().pos;

        ds.pos = startPos+400;
        for(int k=0; k<40; k++){
            motorPosCmdData[i] << ds;
             ds.pos += 400;
        }

    }
    bTermState[i] = bEna;
}

//motorPosCmdData not empty
void MainWindow::allFreeToWrite()
{
    bool bAllMtStop = true;
    for(int i=0; i<MOTOR_CNT; i++){
        if(mtState[i]==MT_IDLE){
            int delta = motorPosCmdData[i].first().pos - getMotorAbsPos(i);
            bAllMtStop &= (delta==0);
        }
        else{
            bAllMtStop = false;
            break;
        }
    }

    if(bAllMtStop){
        int absPosMsec = motorPosCmdData[0].first().absMsec;
        int curMsec = QTime::currentTime().msecsSinceStartOfDay();
        for(int i=0; i<MOTOR_CNT; i++){
            if(absPosMsec != motorPosCmdData[i].first().absMsec){
                ui->plainTextUDP->appendPlainText(QString("%1 absPosMsec not equal at all on %2").arg(curMsec).arg(motorPosCmdData[i].length()));
                for(int k=0; k<MOTOR_CNT; k++){
                    ui->plainTextUDP->appendPlainText(QString("%1").arg(motorPosCmdData[k].length()));
                }
                break;
            }
        }
        int shift = absPosMsec - curMsec;

        ui->plainTextUDP->appendPlainText(QString("%1 shift %2").arg(curMsec).arg(shift));
        if( (abs(shift) <=100) ||
                (shift <0) ){        //если проигрывание отстаёт
//            for(int i=0; i<MOTOR_CNT; i++){
//                motorPosCmdData[i].removeFirst();  // то прибьём текущий шаг
//            }
            ui->plainTextUDP->appendPlainText(QString("%1 skip").arg(curMsec));
        }
        else{ //если проигрывание опережает то не управляем двигателем
            ui->plainTextUDP->appendPlainText(QString("%1 wait").arg(curMsec));
            return;
        }


    }

    for(int i=0; i<MOTOR_CNT; i++){
        switch(mtState[i]){
        case MT_IDLE:
        case MT_INIT_GoUp:
        {
            DivPosDataStr ds;
            ds = motorPosCmdData[i].first();
            int curMsec = QTime::currentTime().msecsSinceStartOfDay();

            //qDebug("div=%x, st=%d, d=%d", ds.div, ds.steps, ds.dir);
            //            if((ds.pos==0)&&(getMotorAbsPos(i)==0)&&(bTermState[i]==false)){
            //                qDebug("not in zero and no term!");
            //                ds.pos = -400;
            //                sendDivPos(i, ds, ds.pos);
            //            }
            //            else{
            if(sendDivPos(i, ds, ds.pos) == true){
                motorPosCmdData[i].dequeue();
                bFreeToWrite[i] = false;
            }
            //motorPosCmdData[i].dequeue();
            //            }
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
        if(getMotorAbsPos(0) < 360000){
            DivPosDataStr ds;
            ds.pos = getMotorAbsPos(i) + 4560;
            sendDivPos(i, ds, ds.pos);
        }
        else{
            mtState[i] = MT_GoDOWN;
        }
        break;

    case MT_GoDOWN:
    case MT_INIT_GoDOWN:
        DivPosDataStr ds;
        if(speedTrig[i]){
            ds.pos = getMotorAbsPos(i)-500;
            speedTrig[i] = false;
        }
        else{
            ds.pos = getMotorAbsPos(i)-10;
            speedTrig[i] = true;
        }
        ds.pos = getMotorAbsPos(i)-500;
        sendDivPos(i, ds, ds.pos);

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

void MainWindow::parseCmdMultiMotorStrList(QStringList cmdMultiMotorStrList)
{        

    static QTime lastDebTime = QTime::currentTime();
    if((QTime::currentTime().msecsSinceStartOfDay() - lastDebTime.msecsSinceStartOfDay()) > 1000){
        lastDebTime = QTime::currentTime();
        //qDebug() << "  ";
        //qDebug() << "  ";
    }


    foreach (QString cmdStr, cmdMultiMotorStrList) {        
        //convertPosModeToVelMode(cmdStr);        
        parseCmdMultiMotorStr(cmdStr);

        qDebug() << QTime::currentTime().toString("hh:mm:ss") << "--- "<< cmdStr;
    }
}

void MainWindow::parseCmdMultiMotorStr(QString cmdMultiMotorStr)
{
    QString convertedString;
    int maxVal = ui->lineEditMaxVal->text().toInt();  
    int msecsForStep = 0;
    if(motorPosCmdData[0].isEmpty() == true){
        msecsForStep = QTime::currentTime().msecsSinceStartOfDay();
    }
    else{
        msecsForStep = motorPosCmdData[0].last().absMsec;
    }
    msecsForStep += 100;
//last().absMsec + 100;
    QStringList motorStrList =  cmdMultiMotorStr.split("p", QString::SkipEmptyParts);
    if(motorStrList.length() != MOTOR_CNT){
        ui->plainTextUDP->appendPlainText("motorStrList.length not equal MOTOR_CNT");
        return;
    }
    //foreach (QString motorStr, motorStrList) {
    for(int i=0; i<MOTOR_CNT; i++){
        QString vs = motorStrList[i];        
        float ip = vs.toInt()/1000.;
        int convVal = ip*maxVal;
        vs = QString("%1").arg(convVal, 3, 'g', -1, '0');
        //if((i==3)){
            parseCmdMotorStr(i, vs, msecsForStep);
        //}
        convertedString += "p" + vs;

    }
    convertedString += "\r\n";

    //motorPosCmdStrings << convertedString;
}

DivPosDataStr ds;
void MainWindow::parseCmdMotorStr(int mn, QString cmdStr, int msecsForStep)
{
    //if(cmdStr[0] == 'S'){
    if(true){
        //int mn = QString(cmdStr[1]).toInt();
        //QString midstr = cmdStr.mid(3, 3);
        //int pos = cmdStr.mid(3, 3).toInt();
        int pos = cmdStr.toInt();

        //if(mn==3) ui->plainTextUDP->appendPlainText(cmdStr);

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
            int maxVal = ui->lineEdit_MaxHeightImp->text().toInt();
            int newPos = maxVal*(pos/1000.);
            ds.pos = newPos;
            if(motorPosCmdData[mn].length() == 0){
                int delta = newPos - getMotorAbsPos(mn);
                //if(delta != 0)
                //    qDebug("%d st=%d div=%d, dir=%d", mn, delta, FPGA_FREQ/(delta*10), delta/abs(delta));

                //ui->plainTextUDP->appendPlainText("add pts!");
                //qDebug("mn %d d %d", mn, delta);

                int vmaxMmsec = ui->lineEdit_vmax_mmsec->text().toInt();
                int maxImpPerDelta = mmToImp(vmaxMmsec)/10;
                //qDebug("%d", maxImpPerDelta);

                int n=1;
                for(; abs(delta)>maxImpPerDelta; n*=2){
                    delta/=2;
                }
                if(n>1)
                    qDebug("%d add %d Pts %d", mn, n, delta);
                ds.pos = getMotorAbsPos(mn);
                ds.absMsec = msecsForStep; //curMsces + 100;
                for(int i=0; i<n; i++){
                    //ds.absMsec += 100;

                    ds.pos = ds.pos+delta;
                    motorPosCmdData[mn].append(ds);                    
                }

            }
            else{                
                int delta = newPos - motorPosCmdData[mn].last().pos;
                //if(delta != 0)
                //    qDebug("%d st=%d div=%d, dir=%d", mn, delta, (qint32)FPGA_FREQ/(delta*10), delta/abs(delta));

                ds.absMsec = msecsForStep;
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
    //qDebug() << "0";
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
        QStringList outStrings;
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
    //qDebug("readPendingDatagrams rb %d gl %04d time %d", rb, gridLines, rpdEst.elapsed());
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
    //qDebug("data recvd in %d ms", tt.elapsed());

    comExchanges++;
    parseFPGAMsg(QByteArray(dArr, 4));
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

    DivPosDataStr ds;
    ds.pos = 1;
    ds.steps = 2400; ds.div = 1000; ds.dir = 1; motorPosCmdData[0] << ds;
    ds.steps = 32800; ds.div = 73; ds.dir = 1; motorPosCmdData[0] << ds;
    ds.steps = 8800; ds.div = 272; ds.dir = 1; motorPosCmdData[0] << ds;
    ds.steps = 13600; ds.div = 176; ds.dir = 1; motorPosCmdData[0] << ds;
    ds.steps = 18400; ds.div = 130; ds.dir = 1; motorPosCmdData[0] << ds;
    ds.steps = 22400; ds.div = 107; ds.dir = 1; motorPosCmdData[0] << ds;

    ds.steps = 25600; ds.div = 93; ds.dir = 1; motorPosCmdData[0] << ds;
    ds.steps = 28800; ds.div = 83; ds.dir = 1; motorPosCmdData[0] << ds;
    ds.steps = 30400; ds.div = 78; ds.dir = 1; motorPosCmdData[0] << ds;
    ds.steps = 32000; ds.div = 75; ds.dir = 1; motorPosCmdData[0] << ds;
    ds.steps = 32800; ds.div = 73; ds.dir = 1; motorPosCmdData[0] << ds;

    ds.steps = 33599; ds.div = 71; ds.dir = 1; motorPosCmdData[0] << ds;
    ds.steps = 32801; ds.div = 73; ds.dir = 1; motorPosCmdData[0] << ds;
    ds.steps = 32000; ds.div = 75; ds.dir = 1; motorPosCmdData[0] << ds;
    ds.steps = 30400; ds.div = 78; ds.dir = 1; motorPosCmdData[0] << ds;
    ds.steps = 28800; ds.div = 83; ds.dir = 1; motorPosCmdData[0] << ds;

    ds.steps = 25600; ds.div = 93; ds.dir = 1; motorPosCmdData[0] << ds;
    ds.steps = 22400; ds.div = 107; ds.dir = 1; motorPosCmdData[0] << ds;
    ds.steps = 18399; ds.div = 130; ds.dir = 1; motorPosCmdData[0] << ds;
    ds.steps = 13601; ds.div = 176; ds.dir = 1; motorPosCmdData[0] << ds;
    ds.steps = 8800; ds.div = 272; ds.dir = 1; motorPosCmdData[0] << ds;
    ds.steps = 3200; ds.div = 750; ds.dir = 1; motorPosCmdData[0] << ds;

}

void MainWindow::on_pushClearMap_clicked()
{


}

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
        for(int i=0; i<MOTOR_CNT; i++){
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
    DivPosDataStr ds;  
    int startPos =0;
    for(int mi=0; mi<MOTOR_CNT; mi++){
        if(motorPosCmdData[mi].length() == 0)
            startPos = getMotorAbsPos(mi);
        else
            startPos = motorPosCmdData[mi].last().pos;

        int iters = startPos/400;
        ds.pos = startPos;
        for(int i=0; i<iters; i++){
            ds.pos -=400;
            motorPosCmdData[mi] << ds;
        }
        if(ds.pos > 0){
            ds.pos = 0;
            motorPosCmdData[mi] << ds;
        }
    }


}

void MainWindow::on_pushButtonTest_clicked()
{
    DivPosDataStr ds;
//    ds.pos=600; motorPosCmdData[0] << ds;
//    ds.pos=2800; motorPosCmdData[0] << ds;
//    ds.pos=6200; motorPosCmdData[0] << ds;
//    ds.pos=10800; motorPosCmdData[0] << ds;
//    ds.pos=16400; motorPosCmdData[0] << ds;
//    ds.pos=22800; motorPosCmdData[0] << ds;
//    ds.pos=30000; motorPosCmdData[0] << ds;
//    ds.pos=37600; motorPosCmdData[0] << ds;
//    ds.pos=45600; motorPosCmdData[0] << ds;
//    ds.pos=53799; motorPosCmdData[0] << ds;
//    ds.pos=62200; motorPosCmdData[0] << ds;
//    ds.pos=70400; motorPosCmdData[0] << ds;
//    ds.pos=78400; motorPosCmdData[0] << ds;
//    ds.pos=86000; motorPosCmdData[0] << ds;
//    ds.pos=93200; motorPosCmdData[0] << ds;
//    ds.pos=99600; motorPosCmdData[0] << ds;
//    ds.pos=105200; motorPosCmdData[0] << ds;
//    ds.pos=109800; motorPosCmdData[0] << ds;
//    ds.pos=113200; motorPosCmdData[0] << ds;
//    ds.pos=115399; motorPosCmdData[0] << ds;
//    ds.pos=116200; motorPosCmdData[0] << ds;

    ds.pos=1; motorPosCmdData[0] << ds;
    ds.pos=3; motorPosCmdData[0] << ds;
    ds.pos=-1; motorPosCmdData[0] << ds;
    ds.pos=1; motorPosCmdData[0] << ds;
    ds.pos=-1; motorPosCmdData[0] << ds;
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
