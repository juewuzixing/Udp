#include "udpclient.hpp"
#include <QTextCodec>
#include <QString>

UdpClient::UdpClient(QObject *parent) : QObject(parent) {
    qWarning() << "UdpClient is start!";

    socket = new QUdpSocket(this);
    mTcpSocket = new QTcpSocket;

    // send data to server
    mRand = generateRandomInteger(1024, 65535);

    if (!socket->bind(QHostAddress("172.10.11.9"),mRand)) {
        qWarning() << "bind failed:" << mRand;
    } else {
        qWarning() << "bind success:" << mRand;
        sendMessage(QByteArray::number(mRand));
    }

    connect(socket,&QUdpSocket::readyRead,this,&UdpClient::responder);
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
    socket->readDatagram(receivedDatagram.data(),receivedDatagram.size());

    // startListen();
    if (receivedDatagram.data()) {
        auto tcpPort = receivedDatagram.toInt();
        qWarning() << "tcpPort: " << tcpPort;
        mTcpSocket->connectToHost("172.10.11.9", tcpPort);
    }
}

int UdpClient::generateRandomInteger(int min, int max) {
    Q_ASSERT(min < max);
    static bool seedStatus;
    if (!seedStatus) {
        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
        seedStatus = true;
    }
    int nRandom = qrand() % (max - min);
    nRandom = min + nRandom;

    return nRandom;
}

bool UdpClient::startListen() {
    // 随机port
//    if (!mTcpSocket->isListening()) {
//        mServer->listen(QHostAddress::Any, mRand);
//        return true;
//    } else {
//        return false;
//    }
}
