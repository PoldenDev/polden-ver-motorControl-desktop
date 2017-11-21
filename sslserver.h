#ifndef SSLSERVER_H
#define SSLSERVER_H
#include <QTcpServer>


class SslServer : public QTcpServer
{
public:
    SslServer(QObject *parent = Q_NULLPTR);
//    QTcpServer *tcpServ;

    void incomingConnection(qintptr handle);

private slots:
    void handleReady();
    void handleEncrypted();
};

#endif // SSLSERVER_H
