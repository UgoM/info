#pragma once

#include <QObject>
#include <QtNetwork>

#include "Server.h"

class Server;

class TcpClientThread : public QThread
{
	Q_OBJECT

    int socDes;
    QTcpSocket * tcpSocket;
    Server * mainServer;

	public:
		TcpClientThread(int socketDescriptor, Server * mainServer, QObject * parent);
		~TcpClientThread();

	public slots:
		void run();
        void readDataTcp();
        void displayErrorTcp(QAbstractSocket::SocketError socketError);

	signals:     
        void error(QTcpSocket::SocketError socketError);
};
