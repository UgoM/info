#include "Server.h"
#include "Checkers.h"
#include "BrainCheckers.h"

#include <QtCore>
#include <QApplication>
#include <QObject>

#include <iostream>

Server::Server()
{
    std::cout << "Constructeur Server" << std::endl;

	tcpServer = new TcpServer(this);
	udpServer = new UdpServer(this);

	brains = new QMap <quint32, Brain *>;
	games = new QMap <quint32, Game *>;

    lastIdGame = 1;

 	connect(tcpServer, SIGNAL(newGame()), this, SLOT(makeNewGame()));
 	connect(udpServer, SIGNAL(newGame()), this, SLOT(makeNewGame()));

    initMessages();
}

Server::Server(int n)
{
    if (n) {
        Server();
    } else {
        initMessages();
    }
}

Server::~Server()
{
	delete tcpServer;
    delete udpServer;
	brains->clear();
	delete brains;
	games->clear();
	delete games;
}

void Server::makeNewGame()
{
    // Make Server (inherited from Brain)
	BrainCheckers * newBrain = new BrainCheckers();
	QThread * newBrainThread = new QThread();
	newBrain->moveToThread(newBrainThread);
	newBrainThread->start();

    quint32 port = newBrain->getPort();
    brains->insert(port, newBrain);

	// Make Client (for playing, inherited from Game)
    Checkers * newGame = new Checkers();
	QThread * newGameThread = new QThread();
	newGame->moveToThread(newGameThread);
	newGameThread->start();

    lastIdGame ++;
	games->insert(lastIdGame, newGame);


	QObject::connect(newGame, SIGNAL(moveMade(QByteArray)), newBrain, SLOT(handleMove(QByteArray)));

	std::cout << "New game created" << std::endl;
}

void Server::initMessages()
{
    messages = new QMap <QString, QString>;
    
    messages->insert("UDP_ASK_FOR_SERVER", "Is there any server here ?");
    messages->insert("ANSWER_UDP_ASK_FOR_SERVER", "I think Im here. Don't you think that too ?");

    messages->insert("HELLO_FROM_SERVER", "Hello, I'm a server. What do you want ?");

    messages->insert("ASK_LIST_GAMES", "Could you give me your game list ? Please.");
    messages->insert("END_GAME_LIST", "End of the game list.");

}
QString Server::messageString(QString m)
{
    QString out (messages->value(m));
    return out;
}

QByteArray Server::messageByteArray(QString m)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << (quint32) DataType::MESSAGE ;
    out << messages->value(m);
    
    std::cout << "Server messageByteArray : ";
    for (int i = 0; i < block.size(); ++i) {
        std::cout << block.at(i);
    }
    std::cout << std::endl;

    return block;
}

QByteArray Server::listOfServers() const
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << (quint32) DataType::LISTOFSERVERS ;

    QString s ("");

    QMap<quint32, Brain *>::const_iterator it = brains->constBegin();
    while (it != brains->constEnd()) {
        s.append(QVariant(it.key()).toString()) ;
        s.append(";") ;
        s.append(it.value()->name()) ;
        s.append(";") ;
        s.append(it.value()->nPlayers()) ;
        if (it != brains->constEnd()-1)
            s.append("#") ;
        ++it;
    }
    out << s;

	return block;
}

QList<QMap<QString,QString> *> Server::decodeListOfServers(QString s)
{
    QList<QMap<QString,QString> *> out;
    
    std::cout << s.toStdString() << std::endl;
    std::cout << "Server::decodeListOfServers" << std::endl;
    QStringList tokens = s.split("#");
	for (int k = 0; k < tokens.size(); k++) {
        QMap<QString,QString> * m = new QMap<QString,QString>;
        QStringList a = tokens[k].split(";");
        m->insert("port", a[0]);
        m->insert("name", a[1]);
        m->insert("nPlayers", a[2]);
        out << m;
	}
    return out;
}
