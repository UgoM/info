#include "ServerList.h"

#include <iostream>
#include <QTimer>
#include <QtNetwork>

#include "QTcpSocketTest.h"


ServerList::ServerList()
{
	std::cout << "Constructeur ServerList" << std::endl;

    /* test UDP */
    udpSocket = new QUdpSocket(this);
	udpSocket->bind(12800, QUdpSocket::ShareAddress);
	connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
    connect(this, SIGNAL(s_askForInfos(QHostAddress, quint16)), this, SLOT(askForInfos(QHostAddress, quint16)));

    /* variables */
    flg_listen = 0;
    serverList =  new QMap <QString, quint16> ;
    ipList =  new QStringList ;
    gameList =  new QStringList ;
    nPlayersList =  new QList <int> ;
    nMaxPlayersList =  new QList <int> ;

    /* create an empty Server object, just to get messages definitions */
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

    // start listening for 5s
    flg_listen = 1;

    // broadcast message
    QByteArray datagram = serverObject->message("UDP_ASK_FOR_SERVER");
    QUdpSocket * udpSocket_send = new QUdpSocket(this);
    udpSocket_send->writeDatagram(datagram.data(), datagram.size(), /*QHostAddress("127.0.0.1") *//*QHostAddress("193.54.87.255")*/QHostAddress::Broadcast, 12800);
    std::cout << "ServerList : Broadcast message, now waiting for responses..." << std::endl;


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
	std::cout << "ServerList : Received Udp data : \"" << std::endl; 
    if (!flg_listen) return;
	std::cout << "ok" << std::endl; 

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
    std::cout << "ServerList : processTheDatagram" << std::endl;
	std::cout << "ServerList : Received Udp data : \"" << datagram.data() << "\"" << std::endl; 
    if (datagram == serverObject->message("ANSWER_UDP_ASK_FOR_SERVER")) {
        std::cout << "ServerList : processTheDatagram OK" << std::endl;
        std::cout << "new server adress " << senderHost.toString().toStdString() << ":" << senderPort << ", asking for games and infos..." << std::endl;
        serverList->insert(senderHost.toString(), senderPort);
        emit askForInfos(senderHost, senderPort);
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

void ServerList::askForInfos(QHostAddress senderHost, quint16 senderPort)
{
    /* tcpClient */    
    tcpSocket = new QTcpSocketTest(this);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readDataTcp()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(displayErrorTcp(QAbstractSocket::SocketError)));

    std::cout << "Tcp connection to host " << senderHost.toString().toStdString() << ":" << senderPort << std::endl;
    tcpSocket->connectToHost(senderHost, senderPort);
}

void ServerList::readDataTcp()
{
    std::cout << "ServerList : Tcp data received" << std::endl;

    QString data;
    QDataStream in(tcpSocket);
    in >> data;

    if ( data == serverObject->message("HELLO_FROM_SERVER")) {
        std::cout << "ServerList : HELLO_FROM_SERVER" << std::endl;
        tcpSocket->write(serverObject->message("ASK_LIST_GAMES"));
    }

}

void ServerList::displayErrorTcp(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        std::cout << "The host was not found. Please check the host name and port settings." << std::endl;
        break;
    case QAbstractSocket::ConnectionRefusedError:
        std::cout << "The connection was refused by the peer. Make sure the fortune server is running, \
                        and check that the host name and port settings are correct." << std::endl;
        break;
    default:
        std::cout << "The following error occurred:" << tcpSocket->errorString().toStdString() << std::endl;
    }
}
