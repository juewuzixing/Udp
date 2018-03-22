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
        sendMessage("nihao122");
    });
//    connect(socket,&QUdpSocket::readyRead,this,&UdpClient::responder);
}

void UdpClient::sendMessage(const QByteArray &message) {
    MagicFrame magicFrame;
    memset(&magicFrame, 0, sizeof(MagicFrame));
    magicFrame.head = 0xAA;
    magicFrame.length = message.length(); // 发送数据的大小
    magicFrame.tail = 0x55;

    memcpy(magicFrame.data, message, message.length());  //数据拷贝

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
