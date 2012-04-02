#include "Server.h"
#include "Checkers.h"

#include <QtCore>
#include <QApplication>
#include <QObject>

#include <iostream>

Server::Server()
{
    std::cout << "Constructeur Server" << std::endl;

	tcpServer = new TcpServer(this);
	udpServer = new UdpServer(this);

	brains = new QList<Brain *>;
	games = new QList<Game *>;

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
    Checkers * newGame = new Checkers();
	//Game * newGame = new Game();
	QThread * newGameThread = new QThread();
	newGame->moveToThread(newGameThread);
	newGameThread->start();

	games->push_back(newGame);

	std::cout << "New game created" << std::endl;
}

void Server::initMessages()
{
    messages = new QMap <QString, QByteArray>;
    
    messages->insert("UDP_ASK_FOR_SERVER", "Is there any server here ?");
    messages->insert("ANSWER_UDP_ASK_FOR_SERVER", "I think Im here. Don't you think that too ?");

    messages->insert("HELLO_FROM_SERVER", "Hello, I'm a server. What do you want ?");

    messages->insert("ASK_LIST_GAMES", "Could you give me your game list ? Please.");
    messages->insert("END_GAME_LIST", "End of the game list.");

}

QByteArray Server::message(QString m)
{
    QString toto(messages->value(m));
    std::cout << "Server : " << toto.toStdString() << std::endl;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint16)0;
    out << messages->value(m);
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    return block;
}

QString Server::decodeDatagram(QAbstractSocket * socket)
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_4_0);

    if (socket->bytesAvailable() < (int)sizeof(quint16))
        return "a";
    quint16 blockSize;
    in >> blockSize;
    

    if (socket->bytesAvailable() < blockSize)
        return "b";

    QString toto;
    in >> toto;

    return toto;

}
