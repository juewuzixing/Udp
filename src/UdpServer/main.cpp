#include <QCoreApplication>
#include "udpserver.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    UdpServer *udpServer;
    QString serverIP("172.10.11.9");
    int serverPort = SERVER_UDP_PORT;

    udpServer = new UdpServer("172.10.11.9",serverPort);
    udpServer->getMessage();

//    udpServer->setClientIP("127.0.0.1");
//    udpServer->sendMessageToClient();

    return a.exec();
}
