#include "sonoffmanager.h"
#include <QPushButton>

#include <QDebug>
#include <QUrl>
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>

#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslKey>

#include <QLabel>

SonoffManager::SonoffManager(QObject *parent, QTableWidget *tabWdg) :
    QObject(parent)
{
    tableWidget = tabWdg;
    QFile certFile(QStringLiteral("ssl/selfcert.in.crt"));
    QFile keyFile(QStringLiteral("ssl/selfcert.in.key"));

//    QFile certFile(QStringLiteral("ssl/localhost.cert"));
//    QFile keyFile(QStringLiteral("ssl/localhost.key"));
    if(certFile.open(QIODevice::ReadOnly) == false)
        qDebug() << "certFile error";
    if(keyFile.open(QIODevice::ReadOnly) == false)
        qDebug() << "keyFile error";


    QSslCertificate certificate(&certFile, QSsl::Pem);
    QSslKey sslKey(&keyFile, QSsl::Rsa, QSsl::Pem);
    certFile.close();
    keyFile.close();

    //sslConfiguration.setPeerVerifyMode(QSslSocket::VerifyNone);
    sslConfiguration.setLocalCertificate(certificate);
    sslConfiguration.setPrivateKey(sslKey);
    //sslConfiguration.setProtocol(QSsl::AnyProtocol);
    //sslConfiguration.setProtocol(QSsl::TlsV1_2);

    sslServ = new SslServer(this);
    if (sslServ->listen(QHostAddress::Any, PORT1)) {
        qDebug() << "listening on port" << PORT1;
        connect(sslServ, SIGNAL(newConnection()),  this, SLOT(handleNewSslConnection()));
        //connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &EchoServer::closed);
    }

//    connect(&qnam, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
//            this, SLOT(handleSSLError(QNetworkReply*,QList<QSslError>)));
//    connect(&qnam, &QNetworkAccessManager::authenticationRequired,
//            this, &Dialog::slotAuthenticationRequired);
//    connect(&qnam, SIGNAL(finished(QNetworkReply*)),
//            this, SLOT(handleQNmFinished(QNetworkReply*)));


    tabWdg->setRowCount(5);
    tabWdg->setColumnCount(5);
}

void SonoffManager::setAPserverParams(QString s, QString k, QString ip)
{
    ssid = s;
    key = k;
    servIp = ip;
}

void SonoffManager::handleNewSslConnection()
{
    QSslSocket *sslSock = (QSslSocket*)sslServ->nextPendingConnection();

    qDebug() << "onNewSSLConnection" << sslSock->peerAddress();
    sslSockList.append(sslSock);
    connect(sslSock, &QSslSocket::encrypted, [=](){ handleEncrypted(sslSock);});
    //connect(sslSock, &QSslSocket::sslErrors, [this, sslSock](const QList<QSslError> &erl){ handleSSLError(sslSock, erl);} );
    connect(sslSock, &QSslSocket::readyRead, [=](){ handleSslSocketReadyRead(sslSock);} );
    //connect(sslSock, &QSslSocket::error, [=](QAbstractSocket::SocketError serr){ handleSocketError(sslSock, serr);} );
    connect(sslSock, &QSslSocket::disconnected, [=](){ handleSslSocketDisconnected(sslSock);} );

    sslSock->setSslConfiguration(sslConfiguration);
    sslSock->startServerEncryption();
}


void SonoffManager::handleSslSocketReadyRead(QSslSocket* s)
{
    QByteArray ba = s->readAll();
    QString msg(ba);
    if(msg.startsWith("POST /dispatch/device HTTP/1.1\r\n")){
        qDebug() << s->peerAddress()  << "dispatch/device";
        qDebug() << "handleSocketReadyRead" << ba;
        msg.remove("POST /dispatch/device HTTP/1.1\r\n");
        int ind = msg.indexOf("\r\n");
        msg = msg.mid(ind+2);
        ind = msg.indexOf("\r\n");
        msg = msg.mid(ind+2);
        ind = msg.indexOf("\r\n");
        msg = msg.mid(ind+2);
        //qDebug() << qPrintable(msg);

        QJsonDocument itemDoc = QJsonDocument::fromJson(msg.toLatin1());
        QJsonObject itemObject = itemDoc.object();
        //qDebug()<< s->peerAddress()<<itemObject;
        //qDebug()<< s->peerAddress()<< itemObject["deviceid"] << itemObject["deviceid"].toString();
        QString devIdStr = itemObject["deviceid"].toString();
        devIdMap[devIdStr] = s;
        //qDebug() << devIdMap;

        //QString servIp = ui->lineEditServerIp->text();
        QJsonObject json;
        json.insert("error", 0);
        json.insert("reason", "ok");
        json.insert("IP", servIp);
        json.insert("port", PORT1);
        QByteArray data = QJsonDocument(json).toJson().data();
        QByteArray dataAck;
        dataAck = "HTTP/1.1 200 OK\r\n"
               //   "Server: openresty\r\n"
               //   "Date: Mon, 15 May 2017 01:26:00 GMT\r\n"
                  "Content-Type: application/json\r\n"
                  "Content-Length: 84\r\n"
                  "Connection: keep-alive\r\n\r\n";
        dataAck += data;

        s->write(dataAck);
    }
    else if(msg.startsWith("GET /api/ws HTTP/1.1\r\n")){
        //qDebug() << "handleSocketReadyRead" << ba;
        qDebug() << s->peerAddress()  << "Switching Protocols";
        QByteArray dataAck = "HTTP/1.1 101 Switching Protocols\r\n"
                             "Upgrade: websocket\r\n"
                             "Connection: Upgrade\r\n"
                             "Sec-WebSocket-Accept: q1/L5gx6qdQ7y3UWgO/TXXXXXXA=\r\n";
        //qDebug() << dataAck << dataAck.length();
        s->write(dataAck);
    }
    else if(ba[0] == 0x81){
        int ind = ba.indexOf("{");
        ba = ba.mid(ind);
        QJsonDocument itemDoc = QJsonDocument::fromJson(ba);
        QJsonObject io = itemDoc.object();
        //qDebug() << s->peerAddress() <<itemObject;

        QString devIdStr = io["deviceid"].toString();
        devIdMap[devIdStr] = s;
        QJsonObject jsonAck;
        jsonAck["error"] = 0;
        jsonAck["deviceid"] = devIdStr;
        jsonAck["apikey"] = "111111111-1111-1111-1111-111111111111";

        if(io.contains("action")){
            if(io["action"].toString().compare("update") == 0){
                qDebug() << s->peerAddress() << "update" << qPrintable(io["params"].toObject()["switch"].toString());
                qDebug() << io;
                wsSendJson(s, jsonAck);
                if(devTypeMap[devIdStr] == PSFA04GL){
                    qDebug() << "PSFA04GL" << io["params"].toObject()["switches"];

                }
            }
            else if(io["action"].toString().compare("register") == 0){
                //if()
                devTypeMap[devIdStr] = unknown;
                QString mdl = io["model"].toString();
                if(mdl.compare("PSF-A04-GL") == 0){
                    devTypeMap[devIdStr] = PSFA04GL;
                }
                else if(mdl.compare("ITA-GZ1-GL") == 0){
                    devTypeMap[devIdStr] = ITAGZ1GL;
                }
                else if(mdl.compare("PSA-B01-GL") == 0){
                    devTypeMap[devIdStr] = PSAB01GL;
                }


                qDebug() << s->peerAddress() << "register" << mdl; //<< qPrintable(itemObject["params"].toObject()["switch"].toString());
                //qDebug() << mdl;
                //qDebug() << s->peerAddress() << "register" << qPrintable(io["params"].toString());
                wsSendJson(s, jsonAck);

                if(tableWidget != NULL){
                    QLabel *lbl = new QLabel(mdl, tableWidget);
                    lbl->setAlignment(Qt::AlignHCenter);
                    //lbl->setAlignment(Qt::AlignCenter);
                    tableWidget->setCellWidget(devTypeMap.size()-1, 0, lbl);

                    if((devTypeMap[devIdStr] == ITAGZ1GL) || (devTypeMap[devIdStr] == PSAB01GL)){
                        QPushButton *bt = new QPushButton("switch", tableWidget);
                        connect(bt, &QPushButton::clicked, [=](){
                            if(bt->text().compare("on") == 0){
                                turnRele(devIdStr, s, 0, true);
                                bt->setText("off");
                            }
                            else if(bt->text().compare("off") == 0){
                                turnRele(devIdStr, s, 0, false);
                                bt->setText("on");
                            }
                            else{
                                turnRele(devIdStr, s, 0, true);
                                bt->setText("off");
                            }

                        });
                        tableWidget->setCellWidget(devTypeMap.size()-1, 1, bt);
                    }
                    else if(devTypeMap[devIdStr] == PSFA04GL){
                        QPushButton *bt;
                        for(int i=0; i<4; i++){
                            bt = new QPushButton("switch", tableWidget);
                            connect(bt, &QPushButton::clicked, [=](){
                                if(bt->text().compare("on") == 0){
                                    turnRele(devIdStr, s, i, true);
                                    bt->setText("off");
                                }
                                else if(bt->text().compare("off") == 0){
                                    turnRele(devIdStr, s, i, false);
                                    bt->setText("on");
                                }
                                else{
                                    turnRele(devIdStr, s, i, true);
                                    bt->setText("off");
                                }

                            });
                            tableWidget->setCellWidget(devTypeMap.size()-1, 1+i, bt);
                        }
                    }


                }
            }
            else if(io["action"].toString().compare("date") == 0){
                qDebug() << s->peerAddress() << "date";
                jsonAck["date"] = "2017-05-15T01:26:01.498Z";
                wsSendJson(s, jsonAck);
            }
            else if(io["action"].toString().compare("query") == 0){
                qDebug() << s->peerAddress() << "query";
                //qDebug() << io;
                jsonAck["params"] = 0;
                wsSendJson(s, jsonAck);
            }
        }
        else if(io.contains("error")){
            qDebug() << s->peerAddress()  << "respond:" << io["error"].toString().toInt() /*<< "seq:" << io["sequence"].toString()*/;
        }
        else{
            qDebug() << s->peerAddress()  << "unknown" << ba;
        }
    }
    else if(ba[0] == 0x89){
        qDebug() << s->peerAddress() << "PING-PONG" << QTime::currentTime().toString("mm:ss:zzz");
        QByteArray dataAck;
        dataAck.append((char)0x8A);
        dataAck.append((char)0x00);

        dataAck.append((char)0x00);
        dataAck.append((char)0x00);
        dataAck.append((char)0x00);
        dataAck.append((char)0x00);
        s->write(dataAck);
    }
    else{
        qDebug() << s->peerAddress()  << "handleSocketReadyRead" << ba;

    }

}

void SonoffManager::handleSslSocketDisconnected(QSslSocket* s)
{
    qDebug() << "sslSocketDisconnected" << s->peerAddress();
}

void SonoffManager::wsSendJson(QTcpSocket *s, QJsonObject json)
{
    //\x81\xFE\x00\xBA\x00\x00\x00\x00
    QByteArray jsonBa = QJsonDocument(json).toJson().data();

    QByteArray dataAck;
    dataAck.append(0x81);
    if(jsonBa.size() < 126){
        dataAck.append((char)(jsonBa.size()));
    }
    else{
        quint16 jsonBaSize = jsonBa.size();
        dataAck.append((char)0x7e);
        dataAck.append((char)((jsonBaSize>>8)&0xff));
        dataAck.append((char)(jsonBaSize&0xff));
//        qDebug() <<
//                    "!!!!!!!!!!!!!!!!!!";
    }
    dataAck += jsonBa;

    //qDebug() << dataAck;
    s->write(dataAck);

}

void SonoffManager::handleEncrypted(QSslSocket* s)
{
    qDebug()  << "handleEncrypted" << s->peerAddress();
}

void SonoffManager::turnRele(QString devId, QSslSocket* s, bool bEna)
{
    QJsonObject paramJson;
    if(bEna)
        paramJson["switch"] = "on";
    else
        paramJson["switch"] = "off";

    QJsonObject json;
    json["action"] = "update";
    json["deviceid"] = devId;
    json["apikey"] = "111111111-1111-1111-1111-111111111111";
    //json["userAgent"] = "app";
    //json["sequence"] = "1494806715179";
    //json["ts"] = 0;
    //json["from"] = "app";

    json["params"] = paramJson;

    //qDebug() << QJsonDocument(json).toJson().data();
    wsSendJson(s, json);
}

void SonoffManager::turnRele(QString devId, QSslSocket* s, int id, bool bEna)
{
    QJsonArray jsonArr;
    QJsonObject paramJson;
    paramJson["outlet"] = id;
    paramJson["switch"] = bEna?"on":"off";

    jsonArr.append(paramJson);
//    paramJson["outlet"] = 1;
//    jsonArr.append(paramJson);
//    paramJson["outlet"] = 2;
//    jsonArr.append(paramJson);
//    paramJson["outlet"] = 3;
//    jsonArr.append(paramJson);


    QJsonObject jsonSwitches;
    jsonSwitches["switches"] = jsonArr;
    QJsonObject json;
    json["action"] = "update";
    json["deviceid"] = devId;
    json["apikey"] = "111111111-1111-1111-1111-111111111111";
    //json["userAgent"] = "app";
    //json["sequence"] = "1494806715179";
    //json["ts"] = 0;
    //json["from"] = "app";

    json["params"] = jsonSwitches;

    //qDebug() << QJsonDocument(json).toJson().data();
    wsSendJson(s, json);
}
