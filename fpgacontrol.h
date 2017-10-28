#ifndef FPGACONTROL_H
#define FPGACONTROL_H

#include <QObject>
#include <QTimer>

class FpgaControl : public QObject
{
    Q_OBJECT
public:
    explicit FpgaControl(QObject *parent = nullptr);

private:
    QTimer timer;

signals:



private slots:
    void handleExchTimer();

};

#endif // FPGACONTROL_H
