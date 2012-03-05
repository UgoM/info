#pragma once

#include <QObject>
#include <QtNetwork>
#include "Server.h"

class Server;

class UdpServer : public QObject
{
	Q_OBJECT

	public:
		UdpServer(Server * s);
		~UdpServer();

	public slots:
		void processPendingDatagrams();

	signals:
		void newGame();

	private:
        void processTheDatagram (QByteArray datagram, QHostAddress senderHost, quint16 senderPort);

		QUdpSocket * udpSocket;
        Server * mainServer;
};
