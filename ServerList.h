#pragma once

#include <QObject>
#include <QStandardItemModel>
#include <QtNetwork>
#include "Server.h"
#include "QTcpSocketTest.h"

class ServerList : public QObject
{
	Q_OBJECT

    QUdpSocket * udpSocket;
    int flg_listen;
    Server * serverObject;
    QTcpSocketTest * tcpSocket;

    QMap <QString, quint16> * serverList;
    QList<QMap<QString,QString> *> gameList;
    

	public slots:
		void processPendingDatagrams();
        void stopListening();
        void askForInfos(QHostAddress senderHost, quint16 senderPort);
        void readDataTcp();
        void displayErrorTcp(QAbstractSocket::SocketError socketError);

        void connected();
        void disconnected();

	public:
		ServerList();
		~ServerList();
        void run();
        QStandardItemModel * get();

    private:
        void processTheDatagram (QByteArray datagram, QHostAddress sender, quint16 senderPort);
        void clearServerList();

    signals:
        void newList();
        void s_askForInfos(QHostAddress senderHost, quint16 senderPort);
};
