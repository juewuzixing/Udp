#ifndef UDPSERVER_HPP
#define UDPSERVER_HPP

const int SERVER_UDP_PORT = 55555;
const int CLIENT_UDP_PORT = 11111;

#include <QObject>
#include <QUdpSocket>
#include <QString>
#include <QTcpServer>

typedef struct {
    quint8 head;
    quint16 length;
    char data[16];
    quint8 tail;
}__attribute__((packed)) MagicFrame;


class UdpServer : public QObject
{
    Q_OBJECT
public:
    explicit UdpServer(QString serverIP, int serverPort, QObject *parent = nullptr);

    void setMessage(const QString message);

    QString getMessage() const;

    bool isClientAvailable();

    bool startListen();

signals:

public slots:
    void responder();

    void sendMessageToClient();

private:
    quint16 mClientPort{0};
    QHostAddress mClientAddress{"0"};
    QUdpSocket *mServerSocket = nullptr;
    int mServerPort;
    bool mClientAvailable = false;
    QString mMessage;
    QTcpServer *mServer;
};

#endif // UDPSERVER_HPP
