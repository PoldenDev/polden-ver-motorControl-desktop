#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtDebug>
#include <QtSerialPort/QSerialPortInfo>
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    speed(100),
    udpSocket(NULL)
{
    ui->setupUi(this);

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


    initUdpSocket();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_butPos_clicked()
{
    QString str;
    str = QString("p00=%1\r\n").arg(ui->lineEditPos->text());
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
                ui->pushButtonComOpen->setText("close");
            }
        }
    }
    else{
        serial.close();
        qDebug("com port closed");
        ui->pushButtonComOpen->setText("open");
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

    QString str = QString("S0p%1v%2\r\n").arg(pos).arg(speed) ;
    quint32 ret = serial.write(str.toLatin1());

    //qDebug().nospace() <<  <<" -> " << qPrintable(str) ;



    qDebug(" %s -> %d", qUtf8Printable(str.remove("\r\n")), ret);

}

void MainWindow::on_verticalSlider_2_sliderReleased()
{
    speed = (ui->verticalSlider_2->value()/100.00)*1000;

}

void MainWindow::initUdpSocket()
{
    udpSocket = new QUdpSocket(this);
    if(udpSocket->bind(QHostAddress::LocalHost, 8051) == true){
        qDebug("UDP bind OK");
    }

    connect(udpSocket, SIGNAL(readyRead()),
            this, SLOT(readPendingDatagrams()));
}

void MainWindow::readPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = udpSocket->receiveDatagram();
        //processTheDatagram(datagram);
        QString dataStr = QString(datagram.data());
        QString  contrStr =  dataStr.mid(4,12);

        //qDebug("%s", contrStr.toLatin1().constData());
        contrStr+= "\r\n";
        //qDebug() << datagram.data();
        serial.write(contrStr.toLatin1());
        ui->plainTextUDP->moveCursor (QTextCursor::End);
        ui->plainTextUDP->insertPlainText(contrStr);
        ui->plainTextUDP->moveCursor (QTextCursor::End);
        //ui->plainTextEdit->appendPlainText(str);

        ui->plainTextUDP->verticalScrollBar()->setValue(ui->plainTextUDP->verticalScrollBar()->maximum());

    }
}

void MainWindow::handleReadyRead()
{
    QByteArray str = serial.readAll();

    qDebug() << str;
    ui->plainTextEdit->moveCursor (QTextCursor::End);
    ui->plainTextEdit->insertPlainText(str);
    ui->plainTextEdit->moveCursor (QTextCursor::End);
    //ui->plainTextEdit->appendPlainText(str);

    ui->plainTextEdit->verticalScrollBar()->setValue(ui->plainTextEdit->verticalScrollBar()->maximum());
}

