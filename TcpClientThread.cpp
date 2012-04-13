#include "TcpClientThread.h"

#include <QtNetwork>
#include <QtCore>

#include <iostream>

 #include <QMetaType>
Q_DECLARE_METATYPE(QAbstractSocket::SocketError) 


TcpClientThread::TcpClientThread(int socketDescriptor, Server * mainServer, QObject * parent)
    : QThread(parent), socDes(socketDescriptor), mainServer(mainServer)
{
    std::cout << "Constructeur TcpClientThread()" << std::endl;
}

TcpClientThread::~TcpClientThread()
{
    std::cout << "Destructeur TcpClientThread()" << std::endl;
}

void TcpClientThread::run()
{
    qRegisterMetatype<QAbstractSocket::SocketError>();

    tcpSocket = new QTcpSocket();
    if (!tcpSocket->setSocketDescriptor(socDes)) {
        emit error(tcpSocket->error());
        return;
    }


    connect(tcpSocket, SIGNAL(disconnected()), tcpSocket, SLOT(deleteLater()));

    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readDataTcp()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(displayErrorTcp(QAbstractSocket::SocketError)));


    std::cout << "TcpServerThread : run()" << std::endl;

    //tcpSocket->write(mainServer->message("HELLO_FROM_SERVER"));
    //std::cout << "Server : HELLO_FROM_SERVER" << std::endl;

   /* tcpSocket.write(block);
    tcpSocket.disconnectFromHost();
    tcpSocket.waitForDisconnected();*/
    tcpSocket->waitForDisconnected();
    std::cout << "TcpServerThread : run() -- Disconnected" << std::endl;
}


void TcpClientThread::readDataTcp()
{
    std::cout << "TcpServer : Tcp data received" << std::endl;

    QString data;
    QDataStream in(tcpSocket);
    in >> data;

    if ( data == mainServer->message("ASK_LIST_GAMES")) {   
        std::cout << "TcpServer : ASK_LIST_GAMES" << std::endl;
    }

}

void TcpClientThread::displayErrorTcp(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        std::cout << "The host was not found. Please check the host name and port settings." << std::endl;
        break;
    case QAbstractSocket::ConnectionRefusedError:
        std::cout << "The connection was refused by the peer. Make sure the fortune server is running, \
                        and check that the host name and port settings are correct." << std::endl;
        break;
    default:
        std::cout << "The following error occurred:" << tcpSocket->errorString().toStdString() << std::endl;
    }
}
