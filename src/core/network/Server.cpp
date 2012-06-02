#include "src/core/network/Server.h"
#include "src/core/network/TcpServer.h"
#include "src/core/network/UdpServer.h"
#include "src/core/game/Brain.h"
#include "src/core/game/Game.h"
#include "src/core/type.h"
#include <iostream>

#include "src/core/game/GameSelector.h"

/** \brief Constructor
  */
Server::Server()
{
    qDebug() << "Constructeur Server";

	tcpServer = new TcpServer(this);
	udpServer = new UdpServer();

	brains = new QMap <quint32, Brain *>;
}

/** \brief Destructor
  */
Server::~Server()
{
	delete tcpServer;
    delete udpServer;
	brains->clear();
	delete brains;
}

/** \return port of the game server, so we can connect the client to it
  *
  * Create the server of the game (class derived from Brain)
  */
quint32 Server::makeNewBrain(QString gameName)
{
    // Make Server (inherited from Brain)
	Brain * newBrain = GameSelector::startNewBrain(gameName);
	//QThread * newBrainThread = new QThread();
	//newBrain->moveToThread(newBrainThread);
	//newBrainThread->start();
    newBrain->init();

    quint32 port = newBrain->getPort();	
    brains->insert(port, newBrain);

    return port;
}

/** \return The list of game's servers, formated for network sending
  * 
  * Take all the Brain (game servers) currently running, and return the list
  * formated in a way that can be send trought the network (QByteArray), to
  * read the list, use Server::decodeListOfServers()
  */
QByteArray Server::listOfServers() const
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << (quint32) DataType::LISTOFSERVERS ;

    QByteArray byteArray ("");

    QMap<quint32, Brain *>::const_iterator it = brains->constBegin();
    while (it != brains->constEnd()) {
        byteArray.append(QVariant(it.key()).toString()) ;
        byteArray.append(";") ;
        byteArray.append(it.value()->name()) ;
        byteArray.append(";") ;
        byteArray.append(QString::number(it.value()->getNPlayers())) ;
        if (it != brains->constEnd()-1)
            byteArray.append("#") ;
        ++it;
    }
    out << byteArray;
    qDebug() << byteArray;
	return block;
}

/** see Server::listOfServers() for more informations
  */
QList<QMap<QString,QString> *> Server::decodeListOfServers(QByteArray dat)
{
    qDebug() << dat;
    QList<QMap<QString,QString> *> out;
    
    qDebug() << "Server::decodeListOfServers";
    //QStringList tokens = s.split("#");
	QList<QByteArray>  tokens = dat.split('#');
	for (int k = 0; k < tokens.size(); k++) {
        QMap<QString,QString> * m = new QMap<QString,QString>;
        QList<QByteArray>  a = tokens[k].split(';');
        m->insert("port", a[0]);
        m->insert("name", a[1]);
        m->insert("nPlayers", a[2]);
        out << m;
	}
    return out;
}
