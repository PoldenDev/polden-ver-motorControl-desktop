#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    serial.setBaudRate(18200);
    if(serial.isOpen() == false){
        serial.setPortName(ui->lineEdit->text());
        if (!serial.open(QIODevice::ReadWrite)) {
            return;
        }
    }

}

void MainWindow::on_pushButtonClose_clicked()
{
    serial.close();
}

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
