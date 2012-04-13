#pragma once

#include <QTcpServer>
#include <QObject>
#include "Server.h"
#include "QTcpSocketTest.h"

class Server;

class TcpServer : public QObject
{
	Q_OBJECT

    QTcpSocketTest * tcpSocket;
    QList<QTcpSocket *> clients;

	public:
		TcpServer(Server * s);
        ~TcpServer();


	public slots:
		void newConnection();
        void readDataTcp();
        void displayErrorTcp(QAbstractSocket::SocketError socketError);
        void clientDisconnected();

	private:
		QTcpServer * tcpServer;
        Server * mainServer;

	signals:
		void newGame();
};
