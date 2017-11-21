#include "sslserver.h"
#include <QSslSocket>
#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslKey>
#include <QSslConfiguration>
#include <QFile>
//#include <QWebSocket>

SslServer::SslServer(QObject *parent)
    :QTcpServer(parent)
{
//    tcpServ = new QTcpServer(parent)
//    if (tcpServ->listen(QHostAddress::Any, PORT)) {
//        qDebug() << "Echoserver listening on port" << PORT;
//        connect(&tcpServ, SIGNAL(newConnection()),
//                this, SLOT(onNewConnection()));
//            //connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &EchoServer::closed);
//    }
}


void SslServer::incomingConnection(qintptr socketDescriptor)
{
    QSslSocket *sslSock = new QSslSocket;
    //QWebSocket *sslSock = new QWebSocket();
    if (sslSock->setSocketDescriptor(socketDescriptor)) {
        //qDebug() << "setSocketDescriptor ok";


//        QSslConfiguration sslConfiguration;
//        QFile certFile(QStringLiteral("ssl/server.crt"));
//        QFile keyFile(QStringLiteral("ssl/priv.key"));

//    //    QFile certFile(QStringLiteral("ssl/localhost.cert"));
//    //    QFile keyFile(QStringLiteral("ssl/localhost.key"));
//        qDebug() << certFile.open(QIODevice::ReadOnly);
//        qDebug() << keyFile.open(QIODevice::ReadOnly);

//        QSslCertificate certificate(&certFile, QSsl::Pem);
//        QSslKey sslKey(&keyFile, QSsl::Rsa, QSsl::Pem);
//        certFile.close();
//        keyFile.close();

//        sslConfiguration.setPeerVerifyMode(QSslSocket::VerifyNone);
//        sslConfiguration.setLocalCertificate(certificate);
//        sslConfiguration.setPrivateKey(sslKey);
//        sslConfiguration.setProtocol(QSsl::SslV2);

//        sslSock->setSslConfiguration(sslConfiguration);


        addPendingConnection(sslSock);

//        connect(sslSock, &QSslSocket::encrypted,
//                this, &SslServer::handleEncrypted);

//        sslSock->startServerEncryption();

    } else {
        qDebug() << "setSocketDescriptor fail";
        delete sslSock;
    }

}

void SslServer::handleReady()
{
    qDebug() << "SslServer::ready";

}

void SslServer::handleEncrypted()
{
    qDebug() << "SslServer::encrypted";
}
