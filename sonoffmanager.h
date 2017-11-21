#ifndef SONOFFMANAGER_H
#define SONOFFMANAGER_H

#include <QObject>
#include <QTableWidget>
#include "sslserver.h"
#include <QSslConfiguration>

#include <QNetworkAccessManager>

typedef enum {
    unknown,
    ITAGZ1GL, //1ch basic
    PSFA04GL, //4ch
    PSAB01GL  //smart socket
} TDevTypes;

#define PORT1 9001

class SonoffManager : public QObject
{
    Q_OBJECT
public:
    explicit SonoffManager(QObject *parent, QTableWidget *tabWdg = NULL);

    void setAPserverParams(QString ssid, QString key, QString servIp);
signals:

private slots:
    //void handleQNmFinished(QNetworkReply*);
    void handleNewSslConnection();
    void handleEncrypted(QSslSocket*);
    void handleSslSocketDisconnected(QSslSocket*);
    void handleSslSocketReadyRead(QSslSocket*);

private:
    void wsSendJson(QTcpSocket *s, QJsonObject);
    void turnRele(QString devId, QSslSocket*,bool);
    void turnRele(QString devId, QSslSocket*,int, bool);
    void sendApReq(int port);

    QTableWidget *tableWidget;

    QString ssid, key, servIp;
    QSslConfiguration sslConfiguration;
    SslServer *sslServ;

    //QNetworkAccessManager qnam;
    QList<QSslSocket*> sslSockList;


    QMap<QString, QSslSocket* > devIdMap;
    QMap<QString, TDevTypes> devTypeMap;

};

#endif // SONOFFMANAGER_H
