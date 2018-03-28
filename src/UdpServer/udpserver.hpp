#ifndef UDPSERVER_HPP
#define UDPSERVER_HPP

const int SERVER_UDP_PORT = 55555;
const int CLIENT_UDP_PORT = 11111;

#include <QObject>
#include <QUdpSocket>
#include <QString>
#include <QTcpServer>
#include <QTime>

typedef struct {
    quint8 head;
    quint16 length;
    char data[16];
    quint8 tail;
}__attribute__((packed)) MagicFrame;

#define MAGIC_FRAME_LEN     (const int)(sizeof(MagicFrame))
#define FRAME_DATA_LEN     16


class UdpServer : public QObject
{
    Q_OBJECT
public:
    explicit UdpServer(QString serverIP, int serverPort, QObject *parent = nullptr);

    void setMessage(const QString message);

    QString getMessage() const;

    bool isClientAvailable();

    bool startListen();

    // min:随机数的最小值，max:随机数的最大值
    int generateRandomInteger(int min, int max);

signals:

public slots:
    void responder();

    void sendMessageToClient();

private:
    quint16 mClientPort{0};
    QHostAddress mClientAddress{"172.10.11.9"};
    QUdpSocket *mServerSocket = nullptr;
    int mServerPort;
    bool mClientAvailable = false;
    QString mMessage;
    QTcpServer *mServer;
    int mTcpRandPort;
};

#endif // UDPSERVER_HPP
