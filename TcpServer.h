#pragma once

#include <QTcpServer>
#include <QObject>
#include "Server.h"
#include "TcpClientThread.h"

class Server;
class TcpClientThread;

class TcpServer : public QObject
{
	Q_OBJECT

	public:
		TcpServer(Server * s);
        ~TcpServer();


	public slots:
		void newConnection();

	private:
		QTcpServer * tcpServer;
        Server * mainServer;

	signals:
		void newGame();
};
