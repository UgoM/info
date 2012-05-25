#pragma once

#include <QObject>
#include <QtNetwork>
#include <QtCore>

class Server;

/// \todo comment UdpServer class
class UdpServer : public QObject
{
	Q_OBJECT

	public:
		UdpServer();
		~UdpServer();

	public slots:
		void processPendingDatagrams();

	private:
        void processTheDatagram (QByteArray datagram, QHostAddress senderHost, quint16 senderPort);

		QUdpSocket * udpSocket;
};
