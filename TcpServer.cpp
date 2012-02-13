#include "TcpServer.h"
#include "Game.h"

#include <QtNetwork>
#include <QtCore>
#include <QApplication>

#include <iostream>


TcpServer::TcpServer()
{
	std::cout << "Constructeur TcpServer" << std::endl;

	// Démarrage serveur TCP
    tcpServer = new QTcpServer();

	// Vérification
	if (!tcpServer->listen(QHostAddress::Any, 12800)) {
        std::cout << "Unable to start the server Tcp : " << tcpServer->errorString().toStdString() << std::endl;
        return;
    }

    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
}


void TcpServer::newConnection()
{
	std::cout << "New connection !" << std::endl;

	/// we need to know in which case we are :
	/// - the client wants to know the list of servers available
	///     -> we send him back the list


	/// - the client wants to join an existing game
	///     -> we connect him to the Brain (pipe with new thread ? QT signals/slots ?)


	// to create a new game : just use newGame() signal
	emit newGame();	
	std::cout << "newGame() emitted" << std::endl;
}

