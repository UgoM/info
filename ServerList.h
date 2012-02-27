#pragma once

#include <QObject>
#include <QtNetwork>

class ServerList : public QObject
{
	Q_OBJECT

    QUdpSocket * udpSocket;
    QMap <QString, quint16> * serverList;
    int flg_listen;


	public slots:
		void processPendingDatagrams();
        void stopListening();

	public:
		ServerList();
		~ServerList();
        void run();
        QMap <QString, quint16> get();

    private:
        void processTheDatagram (QByteArray datagram, QHostAddress sender, quint16 senderPort);
        void clearServerList();
};
