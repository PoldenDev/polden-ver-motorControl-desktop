#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_butPos_clicked();

    void on_pushButtonComOpen_clicked();

    void on_pushButtonClose_clicked();

    void on_pushButton_clicked();

    void on_pushButtonDir_clicked();

    void on_pushButtonSetRange_clicked();

    void on_pushButtonSetCont_clicked();

    void on_butPos_2_clicked();

    void on_pushButton_2_clicked();

    void on_pushButtonDir_2_clicked();

    void on_pushButtonSetRange_2_clicked();

    void on_pushButtonSetCont_2_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort serial;
};

#endif // MAINWINDOW_H
