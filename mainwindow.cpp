#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtDebug>
#include <QtSerialPort/QSerialPortInfo>
#include <QScrollBar>
#include <QList>
#include <QNetworkDatagram>
#include <QThread>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    speed(100),
    udpSocket(NULL),
    noDataSending(true),
    cmdNum(0),
    bCmdOk(true)
{
    ui->setupUi(this);



    on_pushButton_refreshCom_clicked();

    initUdpSocket();
    timer.setInterval(50);
    connect(&timer, SIGNAL(timeout()), this, SLOT(sendOnTimer()));
    timer.start();

    slList.append(ui->verticalMotorPos10);
    slList.append(ui->verticalMotorPos9);
    slList.append(ui->verticalMotorPos8);
    slList.append(ui->verticalMotorPos7);
    slList.append(ui->verticalMotorPos6);
    slList.append(ui->verticalMotorPos5);
    slList.append(ui->verticalMotorPos4);
    slList.append(ui->verticalMotorPos3);
    slList.append(ui->verticalMotorPos2);
    slList.append(ui->verticalMotorPos1);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_butPos_clicked()
{
    QString str;
    //for(int i=0; i<64; i++){
        //str = QString("S0p%1t1000\r\n").arg(int((i/64.)*100));
    //}

    str = QString("S0p%1t1000\r\n").arg(ui->lineEditPos->text());
    serial.write(str.toLatin1());


}

void MainWindow::on_pushButtonComOpen_clicked()
{
    serial.setBaudRate(115200);
    if(ui->pushButtonComOpen->text() == "open"){
        if(serial.isOpen() == false){
            QString comName = ui->comComboBox->currentText();
            if(comName.length() > 0){
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
        qDebug("com port closed");
        ui->pushButtonComOpen->setText("open");
        contrStringQueue.clear();
    }

}

//void MainWindow::on_pushButtonClose_clicked()
//{

//}

void MainWindow::on_pushButton_clicked()
{
    QString str;
    str = QString("v00=%1\r\n").arg(ui->lineEditVel->text());
    serial.write(str.toLatin1());
}

void MainWindow::on_pushButtonDir_clicked()
{
    QString str;
    str = QString("d00=%1\r\n").arg(ui->lineEditDir->text());
    serial.write(str.toLatin1());
}

void MainWindow::on_pushButtonSetRange_clicked()
{
    QString str;
    str = QString("r00=%1\r\n").arg(ui->lineEditRange3->text());
    serial.write(str.toLatin1());
}

void MainWindow::on_pushButtonSetCont_clicked()
{
    QString str = "c00=\r\n" ;
    serial.write(str.toLatin1());
}



void MainWindow::on_butPos_2_clicked()
{
    QString str;
    str = QString("p01=%1\r\n").arg(ui->lineEditPos_2->text());
    serial.write(str.toLatin1());

}

void MainWindow::on_pushButton_2_clicked()
{
    QString str;
    str = QString("v01=%1\r\n").arg(ui->lineEditVel_2->text());
    serial.write(str.toLatin1());
}

void MainWindow::on_pushButtonDir_2_clicked()
{
    QString str;
    str = QString("d01=%1\r\n").arg(ui->lineEditDir_2->text());
    serial.write(str.toLatin1());
}

void MainWindow::on_pushButtonSetRange_2_clicked()
{
    QString str;
    str = QString("r01=%1\r\n").arg(ui->lineEditRange3_2->text());
    serial.write(str.toLatin1());
}

void MainWindow::on_pushButtonSetCont_2_clicked()
{
    QString str = "c01=\r\n" ;
    serial.write(str.toLatin1());
}

void MainWindow::on_verticalSlider_sliderReleased()
{
    int slVal = ui->verticalSlider->value();
    setPos(slVal);

}

void MainWindow::on_verticalSlider_sliderMoved(int position)
{
    setPos(position);

}

void MainWindow::setPos(int pos)
{

    pos = pos*10;
    if(pos>1000)
        pos = 1000;
    QString str = QString("S0p%1v%2\r\n").arg(pos).arg(speed) ;
    quint32 ret = serial.write(str.toLatin1());

    //qDebug().nospace() <<  <<" -> " << qPrintable(str) ;



    qDebug(" %s -> %d", qUtf8Printable(str.remove("\r\n")), ret);

}

void MainWindow::on_verticalSlider_2_sliderReleased()
{
    speed = (ui->verticalSlider_2->value()/100.00)*4000;

}

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

void MainWindow::readPendingDatagrams()
{
    //while (udpSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = udpSocket->receiveDatagram();
        //processTheDatagram(datagram);
        QString dataStr = QString(datagram.data());

        QStringList list1 = dataStr.split("\r\n");
        QString  contrStr =  dataStr.left(13);

        //qDebug("%s", dataStr.toLatin1().constData());


        if(dataStr[0] == 'P'){
            int mn = QString(contrStr[1]).toInt();

            if((mn>=0)&&(mn<10)){
                //qDebug("%s", mem.toLatin1().constData());
                int pos = contrStr.mid(8, 4).toInt();
                slList[mn]->setValue(pos);
    //            if(mn == 1)
    //                qDebug("mn %d pos %d", mn, pos);
            }
            QByteArray motorOkba = QString("M%1Ok").arg(mn).toLatin1();
            QNetworkDatagram drply = datagram.makeReply(motorOkba);
            udpSocket->writeDatagram(drply);
            return;

        }
        else if(dataStr[0] == 'S'){
            int mn = QString(contrStr[1]).toInt();
            if((mn>=0)&&(mn<10)){
                //qDebug("%s", mem.toLatin1().constData());
                int pos = contrStr.mid(8, 4).toInt();
                slList[mn]->setValue(pos);
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
            QByteArray cmdOkba = QString("cmdOK\r\n").toLatin1();
            QByteArray cmdFailba = QString("cmdFail\r\n").toLatin1();
            QNetworkDatagram drply = datagram.makeReply(cmdOkba);
            udpSocket->writeDatagram(drply);
            if(mn > 3)
                return;

            if(serial.isOpen() /*&& (bCmdOk == true)*/){
                serial.write(contrStr.toLatin1());
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

            }
    //        else{
    //            drply = datagram.makeReply(cmdFailba);

    //        }
    //        if(serial.isOpen() /*&& (bCmdOk == true)*/){
    //            //serial.write(contrStr.toLatin1());
    //            contrStringQueue.enqueue(contrStr);
    //        }



            //udpSocket->writeDatagram()                ;
            ui->plainTextUDP->moveCursor (QTextCursor::End);
            ui->plainTextUDP->insertPlainText(contrStr);
            ui->plainTextUDP->moveCursor (QTextCursor::End);
            //ui->plainTextEdit->appendPlainText(str);

            //ui->plainTextUDP->verticalScrollBar()->setValue(ui->plainTextUDP->verticalScrollBar()->maximum());

       // }

        }
}

void MainWindow::handleReadyRead()
{
    QByteArray str = serial.readAll();

//    uartBuff += str;
//    uartBuff.split("\r\n");
//    int lfInd = uartBuff.indexOf("\r\n");
//    QString cmd = uartBuff.left(lfInd+2);
//    uartBuff.remove(0, lfInd+2);
//    if(cmd.compare("Fifo full!\r\n") == 0){
//        bCmdOk = false;
//        qDebug() << "Fifo full!!!";
//        if(contrStringQueue.isEmpty() != true){
//            QThread::msleep(1000);
//            QString wrStr = contrStringQueue.head();
//            //qDebug("%s", qPrintable(wrStr));
//            serial.write(wrStr.toLatin1());

//        }
//        else{
//            noDataSending = true;
//        }
//    }
//    if(cmd.compare("cmdOk\r\n") == 0){
//        bCmdOk = true;
//        qDebug() << "OK!!!";

//        if(contrStringQueue.isEmpty() == false){
//            contrStringQueue.dequeue();
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
//            noDataSending = true;
//        }

//    }


    //qDebug() << str;
    ui->plainTextEdit->moveCursor (QTextCursor::End);
    ui->plainTextEdit->insertPlainText(str);
    ui->plainTextEdit->moveCursor (QTextCursor::End);
    //ui->plainTextEdit->appendPlainText(str);

    ui->plainTextEdit->verticalScrollBar()->setValue(ui->plainTextEdit->verticalScrollBar()->maximum());
}

void MainWindow::handleSerialDataWritten(qint64 bytes)
{
//    //qDebug("handleSerialDataWritten %d", bytes);
//    if(serial.isOpen()){
//        if(contrStringQueue.isEmpty()){
//            noDataSending = true;
//        }
//        else{
//            if(bCmdOk){
//                qDebug() << "last send OK, send next";
//                contrStringQueue.dequeue();
//                bCmdOk = false;
//            }
//            else{
//                qDebug() << "last send Fail, repeat ";
//            }

//            if(contrStringQueue.isEmpty()){
//                noDataSending = true;
//            }
//            else{
//                QString wrStr = contrStringQueue.head();
//                //qDebug("%s", qPrintable(wrStr));
//                serial.write(wrStr.toLatin1());
//            }

//        }

//    }


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

}

void MainWindow::sendOnTimer()
{
//    if(serial.isOpen() && !contrStringQueue.isEmpty()){
//        QString wrStr = contrStringQueue.dequeue();
//        serial.write(wrStr.toLatin1());
//    }

}
