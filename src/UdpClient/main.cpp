#include <QCoreApplication>
#include "udpclient.hpp"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    UdpClient socket;
//    QByteArray message("hello niho");
//    socket.sendMessage(message);


    return a.exec();
}
