#include "fpgacontrol.h"

#include <QDebug>
FpgaControl::FpgaControl(QObject *parent) : QObject(parent)
{
    timer.setInterval(10);
    connect(&timer, SIGNAL(timeout()), this, SLOT(handleExchTimer()));
    timer.start();

}

void FpgaControl::handleExchTimer()
{
    //qDebug() << "!!";

}
