#pragma once

#include <QTcpServer>
#include <QObject>

class TcpServer : public QObject
{
	Q_OBJECT

	public:
		TcpServer();


	public slots:
		void newConnection();

	private:
		QTcpServer * tcpServer;

	signals:
		void newGame();
};
