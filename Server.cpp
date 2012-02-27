#include "Server.h"

#include <QtCore>
#include <QApplication>
#include <QObject>

#include <iostream>

Server::Server()
{
	TcpServer * tcpServer = new TcpServer();
	UdpServer * udpServer = new UdpServer();

	brains = new QList<Brain *>;
	games = new QList<Game *>;

 	connect(tcpServer, SIGNAL(newGame()), this, SLOT(makeNewGame()));
 	connect(udpServer, SIGNAL(newGame()), this, SLOT(makeNewGame()));
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
	Game * newGame = new Game();
	QThread * newGameThread = new QThread();
	newGame->moveToThread(newGameThread);
	newGameThread->start();

	games->push_back(newGame);

	std::cout << "New game created" << std::endl;
}

