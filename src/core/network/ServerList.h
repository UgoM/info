#pragma once

#include <QObject>
#include <QStandardItemModel>
#include <QtNetwork>
#include <QTimer>

/// \todo comment class ServerList
class ServerList : public QObject
{
	Q_OBJECT

    QUdpSocket * udpSocket;
    int flg_listen;
    QTcpSocket * tcpSocket;

    QMap <QString, quint16> * serverList;
    QList<QMap<QString,QString> *> gameList;
    

	public slots:
		void processPendingDatagrams();
        void stopListening();
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
        void askForInfos(QHostAddress senderHost, quint16 senderPort);

    signals:
        void newList();
};
