#pragma once

#include <QObject>
#include <QtNetwork>

class UdpServer : public QObject
{
	Q_OBJECT

	public:
		UdpServer();
		~UdpServer();

	public slots:
		void processPendingDatagrams();

	signals:
		void newGame();

	private:
        void processTheDatagram (QByteArray datagram, QHostAddress sender, quint16 senderPort);

		QUdpSocket * udpSocket;
};
