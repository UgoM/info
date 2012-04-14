#pragma once

#include <QTcpServer>
#include <QObject>
#include "Server.h"
#include "QTcpSocketTest.h"

class Server;

class TcpServer : public QObject
{
	Q_OBJECT

    QList<QTcpSocket *> clients;

	public:
		TcpServer(Server * s);
        ~TcpServer();


	public slots:
		void newConnection();
        void readDataTcp();
        void clientDisconnected();

	private:
		QTcpServer * tcpServer;
        Server * mainServer;

	signals:
		void newGame();
};
