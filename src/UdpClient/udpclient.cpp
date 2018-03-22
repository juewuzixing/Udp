#include "udpclient.hpp"
#include <QTextCodec>
#include <QString>

UdpClient::UdpClient(QObject *parent) : QObject(parent) {
    socket = new QUdpSocket(this);

    auto result = socket->bind(QHostAddress("172.10.11.9"),CLIENT_UDP_PORT);
    if (!result) {
        qWarning() << "bind:" << result;
    }
    QTimer *timer = new QTimer(this);
    timer->start(1000);
    connect(timer, &QTimer::timeout, [&]() {
        sendMessage("nihao");
    });
//    connect(socket,&QUdpSocket::readyRead,this,&UdpClient::responder);
}

void UdpClient::sendMessage(const QByteArray &message) {
    MagicFrame magicFrame;
    magicFrame.head = 0xAA;
    magicFrame.crc = 0;
    strcpy(magicFrame.data, message.data());
    magicFrame.tail = 0x55;

    auto tmp  = (char*)(&magicFrame);
    auto size = sizeof(magicFrame);

    int checksum = 0;
    for (; size > 0; size--) {
        auto mint8 = (uint8_t)tmp[0];
        checksum += test(mint8);
    }
    qWarning() << checksum;
    socket->writeDatagram((const char*)(&magicFrame), sizeof(magicFrame), QHostAddress("172.10.11.9"), SERVER_UDP_PORT);
    qWarning() << "sendMessage:" << sizeof(magicFrame) << QByteArray((const char*)(&magicFrame), sizeof(magicFrame)).toHex();
}

int UdpClient::test(int n)
{
    int count=0;
    while(n != 0){
        count += n&1;
        n >>= 1;
    }
    return count;
}

void UdpClient::responder() {
    // 读取信息
    QByteArray receivedDatagram;
    receivedDatagram.resize(socket->pendingDatagramSize());

    QHostAddress sender("172.10.11.9");
    quint16 senderPort(SERVER_UDP_PORT);

    socket->readDatagram(receivedDatagram.data(),receivedDatagram.size(),&sender,&senderPort);

    qDebug() << "Received message: " << receivedDatagram.data();
    qDebug() << "Sending server's port: " << senderPort;
    qDebug() << "Sending server's ip address: " << sender.toString();

    // 发回确认
    QByteArray messageToSend;
    messageToSend.append("UdpClient received your message.");
    socket->writeDatagram(messageToSend,sender,senderPort);
}
