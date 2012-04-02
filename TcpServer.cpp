#include "TcpServer.h"
#include "Game.h"
#include "TcpClientThread.h"

#include <QtNetwork>
#include <QtCore>
#include <QApplication>

#include <iostream>


TcpServer::TcpServer(Server * s)
{
	std::cout << "Constructeur TcpServer" << std::endl;

	// Démarrage serveur TCP
    tcpServer = new QTcpServer();

	// Vérification
	if (!tcpServer->listen(QHostAddress::Any, 12801)) {
        std::cout << "Unable to start the server Tcp : " << tcpServer->errorString().toStdString() << std::endl;
        return;
    }

    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));

    mainServer = s;
}

TcpServer::~TcpServer()
{
	std::cout << "Destructeur TcpServer" << std::endl;
}

void TcpServer::newConnection()
{
    std::cout << "New connection Tcp !" << std::endl;

    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    int socketDescriptor = clientConnection->socketDescriptor();
    std::cout << "Socket Descriptor : " << socketDescriptor << std::endl;

    TcpClientThread * t = new TcpClientThread(socketDescriptor, mainServer, this);
    //connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
    t->start();

    /*QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    connect(clientConnection, SIGNAL(disconnected()), clientConnection, SLOT(deleteLater()));

    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readDataTcp()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(displayErrorTcp(QAbstractSocket::SocketError)));

    clientConnection->write(mainServer->message("HELLO_FROM_SERVER"));
    std::cout << "Server : HELLO_FROM_SERVER" << std::endl;*/
    //clientConnection->disconnectFromHost();

	/// we need to know in which case we are :
	/// - the client wants to know the list of servers available
	///     -> we send him back the list


	/// - the client wants to join an existing game
	///     -> we connect him to the Brain (pipe with new thread ? QT signals/slots ?)


	// to create a new game : just use newGame() signal
	//emit newGame();	
	//std::cout << "newGame() emitted" << std::endl;
}

