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
    ipList =  new QStringList ;
    gameList =  new QStringList ;
    nPlayersList =  new QList <int> ;
    nMaxPlayersList =  new QList <int> ;

	connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));

    /* create an empty Server, just to get messages definitions */
    serverObject = new Server(0);

}

ServerList::~ServerList()
{
    delete udpSocket;
}

void ServerList::run()
{
    std::cout << "ServerList : run" << std::endl;

    // clear server list
    std::cout << "ServerList : clear list" << std::endl;
    clearServerList();

    // broadcast message
    QByteArray datagram = serverObject->message("UDP_ASK_FOR_SERVER");
    udpSocket->writeDatagram(datagram.data(), datagram.size(), QHostAddress("193.54.87.255")/*QHostAddress::Broadcast*/, 12800);
    std::cout << "ServerList : Broadcast message, now waiting for responses..." << std::endl;

    // start listening for 5s
    flg_listen = 1;

    // end listening after timeout
     QTimer::singleShot(6000, this, SLOT(stopListening()));
}

void ServerList::clearServerList()
{
    serverList->clear();
    ipList->clear();
    gameList->clear();
    nPlayersList->clear();
    nMaxPlayersList->clear();
}

void ServerList::stopListening()
{
    flg_listen = 0;
    std::cout << "ServerList : stop listening" << std::endl;
}

void ServerList::processPendingDatagrams()
{
    if (!flg_listen) return;

    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress senderHost;
        quint16 senderPort;

        udpSocket->readDatagram(datagram.data(), datagram.size(),
                             &senderHost, &senderPort);

        processTheDatagram(datagram, senderHost, senderPort);
    }
}

void ServerList::processTheDatagram (QByteArray datagram, QHostAddress senderHost, quint16 senderPort)
{
    if (datagram == serverObject->message("ANSWER_UDP_ASK_FOR_SERVER")) {
        std::cout << "new server " << senderHost.toString().toStdString() << ":" << senderPort << std::endl;
        serverList->insert(senderHost.toString(), senderPort);
    }
}

QStandardItemModel * ServerList::get()
{
    QStandardItemModel * model = new QStandardItemModel();

    //row
    for (int i=0; i<ipList->size(); ++i)
    {
	    QList<QStandardItem *> items;

        {
            QStandardItem * item = new QStandardItem();
            item->setData(ipList->at(i));
            items.append(item);
        }

        {
            QStandardItem * item = new QStandardItem();
            item->setData(gameList->at(i));
            items.append(item);
        }

        {
            QStandardItem * item = new QStandardItem();
            item->setData(nPlayersList->at(i));
            items.append(item);
        }

        {
            QStandardItem * item = new QStandardItem();
            item->setData(nMaxPlayersList->at(i));
            items.append(item);
        }

	    model->appendRow(items);
    }


	model->setHeaderData(0 , Qt::Horizontal, "IP");
	model->setHeaderData(1 , Qt::Horizontal, "Jeu");
	model->setHeaderData(2 , Qt::Horizontal, "# Joueurs");
	model->setHeaderData(3 , Qt::Horizontal, "# Joueurs Max");

    return model;
}

void ServerList::testMode()
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(testSendUdp()));
    timer->start(1000);
}

void ServerList::testSendUdp()
{
    QByteArray datagram = serverObject->message("UDP_ASK_FOR_SERVER");
    udpSocket->writeDatagram(datagram.data(), datagram.size(), QHostAddress("193.54.87.255")/*QHostAddress::Broadcast*/, 12800);
    std::cout << "ServerList : message sent..." << std::endl;
}
