#include "src/core/network/ServerList.h"
#include "src/core/network/Server.h"

ServerList::ServerList()
{
	qDebug() << "Constructeur ServerList";

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
	qDebug() << "Destructeur ServerList";
    delete udpSocket;
    delete serverList;
    delete serverObject;
}

void ServerList::run()
{
    qDebug() << "ServerList : run";

    // clear server list
    qDebug() << "ServerList : clear list";
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
    qDebug() << "ServerList : stop listening";
}

void ServerList::processPendingDatagrams()
{
	qDebug() << "ServerList : Received Udp data : \""; 
    if (!flg_listen) return;
	qDebug() << "ok"; 

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
    qDebug() << "ServerList : processTheDatagram";
	qDebug() << "ServerList : Received Udp data : \"" << datagram.data() << "\""; 

    QString data;
    quint32 type;
    QDataStream in(datagram);
    in.setVersion(QDataStream::Qt_4_6);
    in >> type >> data;

    
    if (type == DataType::MESSAGE) {
        qDebug() << "MESSAGE";
        if (data == serverObject->messageString("ANSWER_UDP_ASK_FOR_SERVER")) {
            qDebug() << "ServerList : processTheDatagram OK";
            qDebug() << "new server adress " << senderHost.toString() << ":" << senderPort << ", asking for games and infos...";
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
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readDataTcp()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(displayErrorTcp(QAbstractSocket::SocketError)));

    qDebug() << "Tcp connection to host " << senderHost.toString() << ":" << "12801";
    tcpSocket->connectToHost(senderHost, 12801);


    connect(tcpSocket, SIGNAL(connected()), this, SLOT(connected()));
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    (void) senderPort;
}

void ServerList::connected()
{
    qDebug() << "Connécté au server TCP";
}
void ServerList::disconnected()
{
    qDebug() << "Déconnécté du server TCP";
}


void ServerList::readDataTcp()
{
    qDebug() << "ServerList : Tcp data received";

    QString data;
    quint32 type;
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_4_6);
    in >> type >> data;
    qDebug() << type;
    qDebug() << data;

    //qDebug() << serverObject->decodeDatagram(in);

    if (type == DataType::MESSAGE) {
        qDebug() << "MESSAGE";
        if (data == serverObject->messageString("HELLO_FROM_SERVER")) {
            qDebug() << "ServerList : HELLO_FROM_SERVER";
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
        qDebug() << "gameList : " << i;
        QMapIterator<QString, QString> it(*gameList.at(i));
        while (it.hasNext()) {
            it.next();
            qDebug() << it.key() << ": " << it.value();
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
        qDebug() << "The host was not found. Please check the host name and port settings.";
        break;
    case QAbstractSocket::ConnectionRefusedError:
        qDebug() << "The connection was refused by the peer. Make sure the fortune server is running, \
                        and check that the host name and port settings are correct.";
        break;
    default:
        qDebug() << "The following error occurred:" << tcpSocket->errorString();
    }
}
