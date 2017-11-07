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
    //motorCount(0),
    udpSocket(NULL),    
    curMotorSendIdx(0),
    settings("murinets", "vertolet"),
    xUdpRecv(0),
    markerXPos(0),
    dataGramCnt(0),
    paletteGrey(NULL),
    paletteRed(NULL),
    paletteGreen(NULL),
    lastUdpDatagrammRecvd(0),
    lastDebugShowTime(0),
    fpgaCtrl(this),
    lsDebugPort(this)
{
    ui->setupUi(this);

    quint32 motorCount = settings.value("motorCount", 0).toInt();
    fpgaCtrl.setMotorCount(motorCount);
    lsDebugPort.setPortCount(motorCount);
    ui->lineEditMotorCount->setText(QString("%1").arg(motorCount));
    ui->lineEditUDPMaxVal->setText(QString::number(settings.value("maxPosValue", 1000).toInt()));
    ui->checkBoxPrintUDPData->setChecked(settings.value("printUdpData", false).toBool());

    ui->lineEditMinVal->setText(QString::number(settings.value("minPosValue", 0).toInt()));
    ui->checkBoxDirInverse->setChecked(settings.value("dirInverse", false).toBool());

    fpgaCtrl.setDirInverse(ui->checkBoxDirInverse->isChecked());
    ui->lineEdit_mmPerRot->setText(settings.value("mmPerRot", 10).toString());
    ui->lineEdit_MaxHeightImp->setText(settings.value("maxHeightImp", 200000).toString());
    ui->lineEdit_vmax_mmsec->setText(settings.value("vmax_mmsec", 50).toString());

    on_pushButtonUdpOpenClose_clicked();

    on_lineEdit_MaxHeightImp_editingFinished();



    quint32 fpgaFreq = settings.value("FPGA_FREQ", FPGA_FREQ_25).toInt();
    fpgaCtrl.setFpgaFreq(fpgaFreq);

    if(fpgaFreq == FPGA_FREQ_24)
        ui->radioButtonFpgaFreq24->setChecked(true);
    else if(fpgaFreq == FPGA_FREQ_25)
        ui->radioButtonFpgaFreq25->setChecked(true);
    else{
        ui->radioButtonFpgaFreq24->setChecked(false);
        ui->radioButtonFpgaFreq25->setChecked(false);

    }

    dataProcess100msTimer.setInterval(90);
    connect(&dataProcess100msTimer, SIGNAL(timeout()), this, SLOT(dataProcess100msTimeOut()));

    connect(&waitForFifoFreeTimer, SIGNAL(timeout()), this, SLOT(waitForFifoFreeFifo()));


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

    connect(&fpgaCtrl, SIGNAL(termStateChanged(int,bool)),
            this, SLOT(fpgaCtrlTermState(int, bool)));
    connect(&fpgaCtrl, SIGNAL(errorOccured(QString)),
            this, SLOT(fpgaCtrlErrorOccured(QString)));

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

    settings.setValue("printUdpData", ui->checkBoxPrintUDPData->isChecked());

    delete ui;
}

void MainWindow::fpgaCtrlTermState(int id, bool bEna)
{
    switch(id){
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
    if(id < termCheckBoxList.length()){
        termCheckBoxList[id]->setChecked(bEna);
    }
}
void MainWindow::fpgaCtrlErrorOccured(QString errStr)
{
    ui->plainTextEdit->appendPlainText(errStr);
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
    if(ui->pushButtonComOpen->text() == "open"){
        if(fpgaCtrl.portIsOpen() == false){
            QString comName = (ui->comComboBox->currentData().toString());
            if(comName.length() > 0){
                if (fpgaCtrl.openPort(comName) == false) {
                    //qDebug("%s port open FAIL", qUtf8Printable(comName));
                    ui->plainTextEdit->appendPlainText(QString("%1 port open FAIL %2")
                                                       .arg(qUtf8Printable(comName))
                                                       .arg(fpgaCtrl.getPortError()));
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
                    usbConnectionTime.start();
                }
            }
        }
    }
    else{
        fpgaCtrl.closePort();
        //udpSocket->close();
        //qDebug("com port closed");
        ui->plainTextEdit->appendPlainText(QString("%1 closed").arg(fpgaCtrl.getPortName()));
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
        fpgaCtrl.addMotorCmd(i, newPos, udpDgRecvInterval);
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

void MainWindow::handleReadPendingDatagrams()
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
            //if(ui->checkBoxPrintUDPData->isChecked())
                ui->plainTextUDP->appendPlainText("start cmd");
        }
        else if(dataStr.compare("init\r\n") == 0){
            on_pushButtonInitiate_clicked();
            //if(ui->checkBoxPrintUDPData->isChecked())
                ui->plainTextUDP->appendPlainText("init cmd");
        }
        else if(dataStr.compare("stop\r\n") == 0){            
            qDebug("stop cmd");
            fpgaCtrl.clearCmdList();
            //if(ui->checkBoxPrintUDPData->isChecked())
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

            if(ui->checkBoxPrintUDPData->isChecked()){
                QString showStr = QString("%1 %2 (%3)").arg(QTime::currentTime().toString("mm:ss:zzz"))
                                                        .arg(dataStr).arg(udpDgRecvInterval);
                int indOfCrLn = showStr.lastIndexOf("\r\n");
                if(indOfCrLn != -1){
                    showStr.remove(indOfCrLn, 2);
                }
                ui->plainTextUDP->appendPlainText(showStr);
            }

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

    fpgaCtrl.clearCmdList();
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
    fpgaCtrl.posReset();
}

void MainWindow::on_pushBUttonToIdle_clicked()
{
    fpgaCtrl.setMotorStateIdle();
}

void MainWindow::on_goToTerm_clicked()
{
    fpgaCtrl.setMotorStateGoDown();
}

void MainWindow::on_pushButtonInitiate_clicked()
{
    fpgaCtrl.setMotorStateInitiate();
    //udpServerClose();
    on_pushButtonClear_clicked();
}


void MainWindow::on_pushMoveUp_clicked()
{
    for(int i=0; i<MOTOR_CNT; i++){
        if(fpgaCtrl.getCmdListLength(i) == 0){
            int pos = fpgaCtrl.getMotorAbsPosImp(i);

            pos +=400;
            for(int k=0; k<5; k++){
                fpgaCtrl.addMotorCmd(i, pos, 100);
                pos +=400;
            }
        }
        else{
            QString msg = QString("m %1 queue not empty").arg(i);
            ui->plainTextEdit->appendPlainText(msg);
        }
    }
}

void MainWindow::on_pushMoveDown_clicked()
{
    for(int i=0; i<MOTOR_CNT; i++){
        if(fpgaCtrl.getCmdListLength(i) == 0){
            int pos = fpgaCtrl.getMotorAbsPosImp(i);

            pos -=400;
            for(int k=0; k<5; k++){
                fpgaCtrl.addMotorCmd(i, pos, 100);
                pos -=400;
//                if(pos <=0){
//                    pos = 0;
//                    break;
//                }
            }
        }
        else{
            QString msg = QString("m %1 queue not empty").arg(i);
            ui->plainTextEdit->appendPlainText(msg);
        }
    }
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

void MainWindow::uiUpdateTimerSlot()
{
    int curTabInd = ui->tabWidget->currentIndex();
    QString tabName = ui->tabWidget->tabText(curTabInd);
    if(tabName == "mainStat"){
        bool bOk = false;
        int sliderMaxVal = ui->lineEdit_MaxHeightImp->text().toInt(&bOk);        
        quint32 motorCount = ui->lineEditMotorCount->text().toInt();
        for(int i=0; i<motorCount; i++){
            if(bOk){
                absPosSlider[i]->setMaximum(sliderMaxVal);
                absPosSlider[i]->setTickInterval(sliderMaxVal/5);
            }
            switch(fpgaCtrl.mtState[i]){
                case MT_IDLE:   stateLineEdit[i]->setText("idle"); ;break;
                case MT_GoDOWN: stateLineEdit[i]->setText("mvD");break;
                case MT_GoUP:   stateLineEdit[i]->setText("mvU");break;
                case MT_INIT_GoDOWN: stateLineEdit[i]->setText("initMvD");break;
                case MT_INIT_GoUp: stateLineEdit[i]->setText("initMvU");break;
            }

            int mmPerRot = ui->lineEdit_mmPerRot->text().toInt();
            int impPerRot = ui->lineEdit_ImpPerRot->text().toInt();
            int posMm = (fpgaCtrl.getMotorAbsPosImp(i)/(float)impPerRot)*mmPerRot;
            if(ui->checkBoxSliderPosCtrl->isChecked()==false)
                absPosSlider[i]->setValue(fpgaCtrl.getMotorAbsPosImp(i));
            absPosLineEdit[i]->setText(QString::number(posMm));
            euqueLineEdit[i]->setText(QString::number(fpgaCtrl.getCmdListLength(i)));

        }
        ui->lineEditComExchanges->setText(QString::number(fpgaCtrl.comExchanges));

        if(fpgaCtrl.portIsOpen() == true){
            ui->lineEditUSBConnectionTime->setText(QTime::fromMSecsSinceStartOfDay(usbConnectionTime.elapsed()).toString());
        }
        if(udpSocket->state()== QAbstractSocket::BoundState){
            ui->lineEditUDPConnectionTime->setText(QTime::fromMSecsSinceStartOfDay(udpConnectionTime.elapsed()).toString());
        }

        ui->lineEditBytesOnIter->setText(QString::number(fpgaCtrl.bytesOnIter));

        ui->lineEditRecvInterval->setText(QString::number(fpgaCtrl.recvInterval));
        ui->lineEditDataGramCnt->setText(QString::number(dataGramCnt));


        //qDebug() << connectionTime.elapsed() << connectionTime.toString("zzz");
        //connectionTime.elapsed()
    }
    else if(tabName == "timeStat"){
        int curMsec = QTime::currentTime().msecsSinceStartOfDay();
        for(int i=0; i<MOTOR_CNT; i++){
            if(fpgaCtrl.getCmdListLength(i) != 0){
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
                this, SLOT(handleReadPendingDatagrams()));
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

void MainWindow::on_pushButtonGoZero_clicked()
{
    DivPosDataStr ds;
    int pos =0;
    for(int mi=0; mi<MOTOR_CNT; mi++){
        if(fpgaCtrl.getCmdListLength(mi) == 0){
            pos = fpgaCtrl.getMotorAbsPosImp(mi);
        //    else
        //        pos = motorPosCmdData[mi].last().pos;

            int iters = pos/400;
            for(int i=0; i<iters; i++){
                pos -=400;
                fpgaCtrl.addMotorCmd(mi, pos, 100);
            }
            if(pos > 0){
                pos = 0;
                fpgaCtrl.addMotorCmd(mi, pos, 100);
            }
        }
        else{
            QString msg = QString("m %1 queue not empty").arg(mi);
            ui->plainTextEdit->appendPlainText(msg);
        }
    }


}

void MainWindow::on_pushButtonTest_clicked()
{

////    for(int k=0; k<100; k++){
////        //qDebug() << maxHeightImpVal * qSin((k/100.)*M_PI);
////        quint32 newPos = k*1000;
////        addMotorCmd(0, newPos, 113);
////    }

//    quint32 motorCount = ui->lineEditMotorCount->text().toInt();
//    quint32 maxHeightImpVal = ui->lineEdit_MaxHeightImp->text().toInt();
//    for(int i=0; i<motorCount; i++){
//        for(int j=0; j<10; j++){
//            if(i == 0){
//                for(int k=0; k<0; k++)
//                    fpgaCtrl.addMotorCmd(i, 0, 100);
//            }
//            else if(i == 1){
//                for(int k=0; k<10; k++)
//                    fpgaCtrl.addMotorCmd(i, 0, 100);
//            }
//            else if(i == 2){
//                for(int k=0; k<20; k++)
//                    fpgaCtrl.addMotorCmd(i, 0, 100);
//            }


//            for(int k=0; k<100; k++){
//                //qDebug() << maxHeightImpVal * qSin((k/100.)*M_PI);
//                if(i==0)qDebug() <<qAbs(qSin((k/100.)*M_PI));
//                quint32 newPos = maxHeightImpVal * qAbs(qSin((k/100.)*M_PI));
//                fpgaCtrl.addMotorCmd(i, newPos, 100);
//            }

//            if(i == 0){
//                for(int k=0; k<20; k++)
//                    fpgaCtrl.addMotorCmd(i, 0, 100);
//            }
//            else if(i == 1){
//                for(int k=0; k<10; k++)
//                    fpgaCtrl.addMotorCmd(i, 0, 100);
//            }
//            else if(i == 2){
//                for(int k=0; k<0; k++)
//                    fpgaCtrl.addMotorCmd(i, 0, 100);
//            }
//        }
//    }
    fpgaCtrl.addRawCmd(0, (quint32)0xffff, (quint32)2, 0);
//    fpgaCtrl.addRawCmd(0, (quint32)0xfff, (quint32)2);
//    fpgaCtrl.addRawCmd(0, (quint32)0xff, (quint32)2);
//    fpgaCtrl.addRawCmd(0, (quint32)0xf, (quint32)2);
}


void MainWindow::on_pushButtonTest1_clicked()
{
    fpgaCtrl.addRawCmd(0, (quint32)0xfff, (quint32)2, 1);
}

void MainWindow::on_pushButtonTest2_clicked()
{
    fpgaCtrl.addRawCmd(0, (quint32)0xf, (quint32)2, 0);
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
    fpgaCtrl.setFpgaFreq(FPGA_FREQ_25);
    QSettings settings("Murinets", "MotorControl");
    settings.setValue("FPGA_FREQ", FPGA_FREQ_25);
}

void MainWindow::on_radioButtonFpgaFreq24_clicked()
{
    fpgaCtrl.setFpgaFreq(FPGA_FREQ_24);
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
    termCheckBoxList.clear();
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
    quint32 motorCount = ui->lineEditMotorCount->text().toInt();
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
        termCheckBoxList.append(cb);

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


    foreach (QGroupBox *gb, debPortGbList) {
        delete gb;
    }

    debPortCmbBxList.clear();
    debPortpbList.clear();
    //debSerialPortList.clear();
    debPortGbList.clear();
    debPortStatusLeList.clear();

    QLayout *hblo = ui->tabDebugPort->layout();
    if(hblo == NULL){
        hblo = new QHBoxLayout;
    }

    quint32 motorCount = ui->lineEditMotorCount->text().toInt();
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

        //QSerialPort *sp = new QSerialPort(gb);
        //connect(sp, &QSerialPort::errorOccurred, [this, i](QSerialPort::SerialPortError error){ handleComPortErrorOccured(i, error);});
        connect(pb, &QPushButton::clicked, [=](){ pushDebugComPortOpen(i);});
        //connect(sp, &QSerialPort::readyRead, [this, i](){ handleReadyRead(i);});

        debPortCmbBxList.append(cb);
        debPortpbList.append(pb);
        //debSerialPortList.append(sp);
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



}

void MainWindow::pushDebugComPortOpen(int id)
{
    //QSerialPort &sp = *debSerialPortList[id];
    QPushButton &pb = *debPortpbList[id];
    QComboBox &cb = *debPortCmbBxList[id];


     if(pb.text() == "open"){
         if(lsDebugPort.isPortOpen(id) == false){
             QString comName = cb.currentData().toString();
             if(comName.length() > 0){
                 //UartThread.requestToStart(comName);
                 //sp.setPortName(comName);
                 lsDebugPort.setPortName(id, comName);
                 if (lsDebugPort.open(id) == false) {
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
         lsDebugPort.close(id);
         ui->plainTextEdit->appendPlainText(QString("%1 closed").arg(lsDebugPort.portName(id)));
         pb.setText("open");
         cb.setDisabled(false);
     }

}

void MainWindow::comPortClose(int id)
{
    //QSerialPort &sp = *debSerialPortList[id];
    QPushButton &pb = *debPortpbList[id];
    QComboBox &cb = *debPortCmbBxList[id];
    QLineEdit &le = *debPortStatusLeList[id];

    cb.setEnabled(true);
    pb.setText("open");
    if(lsDebugPort.isPortOpen(id) == true){
        ui->plainTextEdit->appendPlainText(QString("%1 closed").arg(lsDebugPort.portName(id)));
        //sp.close();
        lsDebugPort.close(id);
    }
    le.setPalette(*paletteGrey);
}

void MainWindow::on_lineEditMotorCount_editingFinished()
{
    quint32 motorCount = ui->lineEditMotorCount->text().toInt();
    settings.setValue("motorCount", motorCount);
    lsDebugPort.setPortCount(motorCount);
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

        QString msg = QString("%1 error: %2").arg(qUtf8Printable(lsDebugPort.portName(id))).arg(errorStr);
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
//    QByteArray ba = debSerialPortList[id]->readAll();
//    //qDebug() << id << ba;
//    parseLeadShineMsg(id, ba);

}



void MainWindow::parseLeadShineMsg(int id, QByteArray &ba)
{
    quint16 crc16 = lsDebugPort.CRC16_ModBusRTU(ba, ba.length()-2);
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

void MainWindow::on_checkBoxDirInverse_clicked()
{
    fpgaCtrl.setDirInverse(ui->checkBoxDirInverse->isChecked());
}
