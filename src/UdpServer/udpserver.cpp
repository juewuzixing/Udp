#include "udpserver.hpp"

UdpServer::UdpServer(QString serverIP, int serverPort, QObject *parent) :
    QObject(parent),mServerPort(serverPort) {
    qWarning() << "UdpServer is start!";
    mServerSocket = new QUdpSocket(this);
    mServer = new QTcpServer(this);

    // bind port
    auto result = mServerSocket->bind(QHostAddress(serverIP),serverPort,QUdpSocket::DefaultForPlatform);
    if (!result) {
        qWarning() << "bind:" << result;
    }
    // parse the data
    connect(mServerSocket,&QUdpSocket::readyRead,this,&UdpServer::responder);
}

void UdpServer::responder() {
    mClientAvailable = true;
    // 读取信息
    QByteArray receivedDatagram;
    receivedDatagram.resize(mServerSocket->pendingDatagramSize());
    mServerSocket->readDatagram(receivedDatagram.data(),receivedDatagram.size(),&mClientAddress);

    qWarning() << "mClientAddress:" << mClientAddress;
    qWarning() << "receivedDatagram.size():" << receivedDatagram.size() << "sizeof(MagicFrame)" <<  sizeof(MagicFrame);
    qWarning() << "receivedDatagram.data():" << receivedDatagram.toHex();

    int firstHeadIndex = receivedDatagram.indexOf(0xAA);
    if (firstHeadIndex > -1) {
        receivedDatagram.remove(0, firstHeadIndex);
    } else {
        receivedDatagram.clear();
        return;
    }

    QByteArray messageToClient;
    QByteArray bf = receivedDatagram.left(MAGIC_FRAME_LEN);
    MagicFrame *magicFrame = (MagicFrame* )bf.data();

    if(bf.length() >= MAGIC_FRAME_LEN) {
        qWarning() << "magicFrame->length == FRAME_DATA_LEN";
        if ((quint8)0x55 == (quint8)bf[MAGIC_FRAME_LEN-1]) {
            mTcpRandPort = generateRandomInteger(1024,65535);
            if (!mServer->isListening() && mServer->listen(QHostAddress::Any,mTcpRandPort)) {
                connect(mServer,&QTcpServer::newConnection,[&]() {
                    qWarning() << "the newConnection QTcpServer newConnection!";
                });
                qWarning() << "listened" << mTcpRandPort;
            } else {
                qWarning() << "listen failed" << mTcpRandPort;
            }
            messageToClient.append(QByteArray::number(mTcpRandPort));
            mServerSocket->writeDatagram(messageToClient,mClientAddress,QString(magicFrame->data).toInt());
        } else {
            qWarning() << "can not find tail";
        }
    }
}

void UdpServer::setMessage(const QString message) {
    mMessage = message;
    qDebug() << "NewMessage was set:" << mMessage;
}

void UdpServer::sendMessageToClient() {
    qDebug() << "Sending message to client...";
    qDebug() << "At IP address: " << mClientAddress.toString();
    qDebug() << "Port: " << mClientPort;
//    QTcpSocket* socket = mServer->nextPendingConnection();
    // 发送数据
    QByteArray messageToSend;
    messageToSend.append(mMessage);
    mServerSocket->writeDatagram(messageToSend,mClientAddress,mClientPort);
}

QString UdpServer::getMessage() const {
    qWarning() << mMessage.data();
    return mMessage;
}

bool UdpServer::isClientAvailable() {
    return mClientAvailable;
}

bool UdpServer::startListen() {
    // 随机port
    if (!mServer->isListening()) {
        mServer->listen(QHostAddress::Any,mTcpRandPort);
        qWarning() << "33333:" << mTcpRandPort;
        return true;
    } else {
        mServer->listen(QHostAddress::Any,mTcpRandPort);
        qWarning() << "33333:" << mTcpRandPort;
        return false;
    }
}

int UdpServer::generateRandomInteger(int min, int max) {
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
