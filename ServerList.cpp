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
	udpSocket->bind(12801, QUdpSocket::ShareAddress);
	connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
    connect(this, SIGNAL(s_askForInfos(QHostAddress, quint16)), this, SLOT(askForInfos(QHostAddress, quint16)));

    /* variables */
    flg_listen = 0;
    serverList =  new QMap <QString, quint16> ;

    /* create an empty Server object, just to get messages definitions */
    serverObject = new Server(0);

}

ServerList::~ServerList()
{
	std::cout << "Destructeur ServerList" << std::endl;
    delete udpSocket;
    delete serverList;
    delete serverObject;
}

void ServerList::run()
{
    std::cout << "ServerList : run" << std::endl;

    // clear server list
    std::cout << "ServerList : clear list" << std::endl;
    clearServerList();

    // start listening for 5s
    flg_listen = 1;

    // get broadcast addresses
    QSettings settings;
    QStringList broadcastAddresses = settings.value("network/broadcastAdresses").toStringList();

    // broadcast message
    QString broadcastAddress;
    QByteArray datagram = serverObject->messageByteArray("UDP_ASK_FOR_SERVER");
    QUdpSocket * udpSocket_send = new QUdpSocket(this);
    qDebug() << "ServerList : Broadcasting message ... "; 
    foreach(broadcastAddress, broadcastAddresses) {
        QHostAddress address (broadcastAddress);
        if (address.isNull()) {
            qDebug() << "invalid Host Address : " << broadcastAddress;
        } else {
            udpSocket_send->writeDatagram(datagram.data(), datagram.size(), address, 12800);
            qDebug() << "sending to  : " << broadcastAddress;
        }
   }
    qDebug() << "Now waiting for responses...";


    // end listening after timeout
     QTimer::singleShot(6000, this, SLOT(stopListening()));
}

void ServerList::clearServerList()
{
    serverList->clear();
    gameList.clear();
    // rafraichissement de l'affichage
    emit newList();
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

    QString data;
    quint32 type;
    QDataStream in(datagram);
    in.setVersion(QDataStream::Qt_4_6);
    in >> type >> data;

    
    if (type == DataType::MESSAGE) {
        std::cout << "MESSAGE" << std::endl;
        if (data == serverObject->messageString("ANSWER_UDP_ASK_FOR_SERVER")) {
            std::cout << "ServerList : processTheDatagram OK" << std::endl;
            std::cout << "new server adress " << senderHost.toString().toStdString() << ":" << senderPort << ", asking for games and infos..." << std::endl;
            serverList->insert(senderHost.toString(), senderPort);
            emit askForInfos(senderHost, senderPort);
        }
    }
}

QStandardItemModel * ServerList::get()
{
    QStandardItemModel * model = new QStandardItemModel();

    //row
    for (int i=0; i<gameList.size(); ++i) {
	    QList<QStandardItem *> items;
        {
            QStandardItem * item = new QStandardItem();
            item->setData( (*gameList.at(i)).value("IP"), Qt::DisplayRole );
            item->setData( (*gameList.at(i)).value("IP"), Qt::UserRole );
            item->setData( (*gameList.at(i)).value("port"), Qt::UserRole + 1);
            items.append(item);
        }
        {
            QStandardItem * item = new QStandardItem();
            item->setData( (*gameList.at(i)).value("port").toInt(), Qt::DisplayRole );
            item->setData( (*gameList.at(i)).value("IP"), Qt::UserRole );
            item->setData( (*gameList.at(i)).value("port"), Qt::UserRole + 1);
            items.append(item);
        }
        {
            QStandardItem * item = new QStandardItem();
            item->setData( (*gameList.at(i)).value("name"), Qt::DisplayRole );
            item->setData( (*gameList.at(i)).value("IP"), Qt::UserRole );
            item->setData( (*gameList.at(i)).value("port"), Qt::UserRole + 1);
            items.append(item);
        }
        {
            QStandardItem * item = new QStandardItem();
            item->setData( (*gameList.at(i)).value("nPlayers"), Qt::DisplayRole );
            item->setData( (*gameList.at(i)).value("IP"), Qt::UserRole );
            item->setData( (*gameList.at(i)).value("port"), Qt::UserRole + 1);
            items.append(item);
        }
	    model->appendRow(items);
    }

	model->setHeaderData(0 , Qt::Horizontal, "IP");
	model->setHeaderData(1 , Qt::Horizontal, "port");
	model->setHeaderData(2 , Qt::Horizontal, "Jeu");
	model->setHeaderData(3 , Qt::Horizontal, "# Joueurs");

    return model;
}

void ServerList::askForInfos(QHostAddress senderHost, quint16 senderPort)
{
    /* tcpClient */    
    tcpSocket = new QTcpSocketTest(this);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readDataTcp()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(displayErrorTcp(QAbstractSocket::SocketError)));

    std::cout << "Tcp connection to host " << senderHost.toString().toStdString() << ":" << "12801" << std::endl;
    tcpSocket->connectToHost(senderHost, 12801);


    connect(tcpSocket, SIGNAL(connected()), this, SLOT(connected()));
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    (void) senderPort;
}

void ServerList::connected()
{
    std::cout << "Connécté au server TCP" << std::endl;
}
void ServerList::disconnected()
{
    std::cout << "Déconnécté du server TCP" << std::endl;
}


void ServerList::readDataTcp()
{
    std::cout << "ServerList : Tcp data received" << std::endl;

    QString data;
    quint32 type;
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_4_6);
    in >> type >> data;
    std::cout << type << std::endl;
    std::cout << data.toStdString() << std::endl;

    //std::cout << serverObject->decodeDatagram(in).toStdString() << std::endl ;

    if (type == DataType::MESSAGE) {
        std::cout << "MESSAGE" << std::endl;
        if (data == serverObject->messageString("HELLO_FROM_SERVER")) {
            std::cout << "ServerList : HELLO_FROM_SERVER" << std::endl;
            tcpSocket->write(serverObject->messageByteArray("ASK_LIST_GAMES"));
        }
    } else if (type == DataType::LISTOFSERVERS) {
        // get infos from server
        QList<QMap<QString,QString> *> l = serverObject->decodeListOfServers(data);
        // add ip to the list
        for (int i=0; i<l.size(); ++i) {
            l.at(i)->insert("IP", tcpSocket->peerAddress().toString());
        }
        gameList << l;
    }

    for (int i=0; i<gameList.size(); ++i) {
        std::cout << "gameList : " << i << std::endl;
        QMapIterator<QString, QString> it(*gameList.at(i));
        while (it.hasNext()) {
            it.next();
            std::cout << it.key().toStdString() << ": " << it.value().toStdString() << std::endl;
        }
        // émission du signal pour rafraichir l'affichage
        emit newList();
        // fermeture de la connexion
        tcpSocket->close();
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
