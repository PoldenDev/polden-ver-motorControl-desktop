#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    for(int i=0; i<argc; i++){
        if(QString(argv[i]) == "--init"){
            qDebug("init");
        }
        else{

        }
    }
    MainWindow w;
    w.show();

    return a.exec();
}
