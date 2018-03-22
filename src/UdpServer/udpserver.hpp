#ifndef UDPSERVER_HPP
#define UDPSERVER_HPP

const int SERVER_UDP_PORT = 55555;
const int CLIENT_UDP_PORT = 11111;

#include <QObject>
#include <QUdpSocket>
#include <QString>

class UdpServer : public QObject
{
    Q_OBJECT
public:
    explicit UdpServer(QString serverIP, int serverPort, QObject *parent = nullptr);

    void setMessage(const QString message);

    QString getMessage() const;

    bool isClientAvailable();

signals:

public slots:
    void responder();

    void sendMessageToClient();

private:
    quint16 mClientPort{CLIENT_UDP_PORT};
    QHostAddress mClientAddress{"172.10.11.9"};
    QUdpSocket *mServerSocket = nullptr;
    int mServerPort;
    bool mClientAvailable = false;
    QString mMessage;
};

#endif // UDPSERVER_HPP
