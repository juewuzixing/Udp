#ifndef UDPCLIENT_HPP
#define UDPCLIENT_HPP

#include <QObject>
#include <QUdpSocket>
#include <QTimer>

const int SERVER_UDP_PORT = 55555;
const int CLIENT_UDP_PORT = 11111;

typedef struct {
    quint8 head;
    quint16 crc;
    char data[8];
    quint8 tail;
}__attribute__((packed)) MagicFrame;

class UdpClient : public QObject
{
    Q_OBJECT
public:
    explicit UdpClient(QObject *parent = nullptr);

    void sendMessage(const QByteArray &message);

    void packData(QByteArray &out, const int &data, const int &bitStart, const int &bitLength);
    void unpackData(int *out, const QByteArray &data, const int &bitStart, const int &bitLength);
    uint16_t crc16Check(uint8_t *ptr,uint8_t len);
    int test(int n);

signals:

public slots:
    void responder();

private:
    QUdpSocket *socket;
};

#endif // UDPCLIENT_HPP
