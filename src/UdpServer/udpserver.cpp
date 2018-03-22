#include "udpserver.hpp"

UdpServer::UdpServer(QString serverIP, int serverPort, QObject *parent) :
    QObject(parent),mServerPort(serverPort) {
    mServerSocket = new QUdpSocket(this);

    auto result = mServerSocket->bind(QHostAddress(serverIP),serverPort,QUdpSocket::DefaultForPlatform);
    if (!result) {
        qWarning() << "bind:" << result;
    }
    connect(mServerSocket,&QUdpSocket::readyRead,this,&UdpServer::responder);
}

void UdpServer::responder() {
    mClientAvailable = true;
    // 读取信息
    QByteArray receivedDatagram;
    receivedDatagram.resize(mServerSocket->pendingDatagramSize());

    mServerSocket->readDatagram(receivedDatagram.data(),receivedDatagram.size(),&mClientAddress,&mClientPort);

    qDebug() << "Received message: " << receivedDatagram.toHex();
    qDebug() << "Received message size: " << receivedDatagram.size();
    qDebug() << "Sending client's port: " << mClientPort;
    qDebug() << "Sending client's ip address: " << mClientAddress.toString();

    // 发回确认
    QByteArray messageToSend;
    messageToSend.append("UdpServer received your message.");
    mServerSocket->writeDatagram(messageToSend,mClientAddress,mClientPort);
}

void UdpServer::setMessage(const QString message) {
    mMessage = message;
    qDebug() << "NewMessage was set:" << mMessage;
}

void UdpServer::sendMessageToClient() {
    qDebug() << "Sending message to client...";
    qDebug() << "At IP address: " << mClientAddress.toString();
    qDebug() << "Port: " << mClientPort;

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
