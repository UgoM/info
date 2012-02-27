#include "ServerList.h"

#include <iostream>
#include <QTimer>
#include <QtNetwork>


ServerList::ServerList()
{
	std::cout << "Constructeur ServerList" << std::endl;

    /* test UDP */
    udpSocket = new QUdpSocket(this);
	udpSocket->bind(12800, QUdpSocket::ShareAddress);
    flg_listen = 0;
    serverList =  new QMap <QString, quint16> ;

	connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
}

ServerList::~ServerList()
{
    delete udpSocket;
}

void ServerList::run()
{
    std::cout << "ServerList : run" << std::endl;

    // clear server list
    std::cout << "ServerList : clear server list" << std::endl;
    clearServerList();

    // broadcast message
    QByteArray datagram = "Broadcast message " + QByteArray::number(128);
    udpSocket->writeDatagram(datagram.data(), datagram.size(), QHostAddress::Broadcast, 12800);
    std::cout << "ServerList : Broadcast message" << std::endl;

    // start listening for 5s
    flg_listen = 1;
    std::cout << "ServerList : start listening" << std::endl;

    // end listening after timeout
     QTimer::singleShot(5000, this, SLOT(stopListening()));
}

void ServerList::clearServerList()
{
    serverList->clear();
}

void ServerList::stopListening()
{
    flg_listen = 0;
    std::cout << "ServerList : end listening" << std::endl;
}

void ServerList::processPendingDatagrams()
{
    if (!flg_listen) return;

    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        udpSocket->readDatagram(datagram.data(), datagram.size(),
                             &sender, &senderPort);

        processTheDatagram(datagram, sender, senderPort);
    }
}

void ServerList::processTheDatagram (QByteArray datagram, QHostAddress sender, quint16 senderPort)
{
    std::cout << "new server " << sender.toString().toStdString() << ":" << senderPort << std::endl;
    serverList->insert(sender.toString(), senderPort);
}

QMap <QString, quint16> ServerList::get()
{
    return *serverList;
}
