#include "TcpClient.h"

#include <QtNetwork>
#include <QtCore>
#include <QApplication>

#include <iostream>


TcpClient::TcpClient()
{
	std::cout << "Constructeur TcpClient" << std::endl;

     tcpSocket = new QTcpSocket(this);

     connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readData()));
     connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
             this, SLOT(displayError(QAbstractSocket::SocketError)));


     tcpSocket->connectToHost("193.54.87.190", 12800);


}


void TcpClient::readData()
{

}
