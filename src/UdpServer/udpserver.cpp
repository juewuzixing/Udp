#include "udpserver.hpp"

UdpServer::UdpServer(QString serverIP, int serverPort, QObject *parent) :
    QObject(parent),mServerPort(serverPort) {
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

    if (receivedDatagram.size() >= sizeof(MagicFrame)) {
        MagicFrame *magicFrame = (MagicFrame *)((uint8_t *)receivedDatagram.data());
        // 解析去掉A
        QByteArray receivedData = magicFrame->data;
        qWarning() << "receivedData" << receivedData;
        if (receivedData.contains("A")) {
            mClientPort = receivedData.remove(0, 1).toInt();
        }
        qDebug() << "Received the data is: "<< mClientPort;

        auto bindResult = mServer->listen(QHostAddress::Any,22222);
        if (!bindResult) {
            qWarning() << "listen is not success!";
        } else {
            qWarning() << "listen success!";
        }
        connect(mServer,&QTcpServer::newConnection,[&]() {
            qWarning() << "the newConnection!";
        });
        // 发回确认,暂时是固定端口
        QByteArray messageToSend;
        messageToSend.append("B22222");
        mServerSocket->writeDatagram(messageToSend,mClientAddress,mClientPort);
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
    //     QTcpSocket* socket = mServer->nextPendingConnection();
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
    if (!mServer->listen(QHostAddress::Any,11111) && !mServer->isListening()) {
        qWarning() << "Nothing listen.";
        //        return false;
    } else {
        qWarning() << "listened" << mServer->isListening();
        return true;
    }
}
