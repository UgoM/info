#pragma once

#include <QObject>
#include <QStandardItemModel>
#include <QtNetwork>
#include "Server.h"

class ServerList : public QObject
{
	Q_OBJECT

    QUdpSocket * udpSocket;
    QMap <QString, quint16> * serverList;
    int flg_listen;
    Server * serverObject;

	public slots:
		void processPendingDatagrams();
        void stopListening();
        void testSendUdp();

	public:
		ServerList();
		~ServerList();
        void run();
        QStandardItemModel * get();
        void testMode();

    private:
        void processTheDatagram (QByteArray datagram, QHostAddress sender, quint16 senderPort);
        void clearServerList();
};
