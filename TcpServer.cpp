#include "TcpServer.h"
#include "Game.h"

#include <QtNetwork>
#include <QtCore>
#include <QApplication>

#include <iostream>
#include "QTcpSocketTest.h"


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

    //QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    //int socketDescriptor = clientConnection->socketDescriptor();
    //std::cout << "Socket Descriptor : " << socketDescriptor << std::endl;

    //TcpClientThread * t = new TcpClientThread(socketDescriptor, mainServer, this);
    //connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
    //t->start();

   /* QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    connect(clientConnection, SIGNAL(disconnected()), clientConnection, SLOT(deleteLater()));

    tcpSocket = new QTcpSocketTest(this);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readDataTcp()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(displayErrorTcp(QAbstractSocket::SocketError)));

    clientConnection->write(mainServer->message("HELLO_FROM_SERVER"));
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

    QTcpSocket *nouveauClient = tcpServer->nextPendingConnection();
    clients << nouveauClient;

    connect(nouveauClient, SIGNAL(readyRead()), this, SLOT(readDataTcp()));
    connect(nouveauClient, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));


    nouveauClient->write(mainServer->message("HELLO_FROM_SERVER"));
    std::cout << "Server : HELLO_FROM_SERVER" << std::endl;
}

void TcpServer::readDataTcp()
{
    std::cout << "TcpServer : Tcp data received" << std::endl;

    // On détermine quel client envoie le message (recherche du QTcpSocket du client)
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0)
        return;

    std::cout << "TcpServer : Sender found" << std::endl;

   /* int tailleMessage = 0;
    if (tailleMessage == 0) // Si on ne connaît pas encore la taille du message, on essaie de la récupérer
    {
        if (socket->bytesAvailable() < (int)sizeof(quint16)) // On n'a pas reçu la taille du message en entier
             return;

        in >> tailleMessage; // Si on a reçu la taille du message en entier, on la récupère
    }
    // Si on connaît la taille du message, on vérifie si on a reçu le message en entier
    if (socket->bytesAvailable() < tailleMessage) // Si on n'a pas encore tout reçu, on arrête la méthode
        return;

    QString data;
    QDataStream in(tcpSocket);
    std::cout << "TcpServer : toto" << std::endl;
    in >> data;

    std::cout << "TcpServer : toto" << std::endl;
*/
    QString data = mainServer->decodeDatagram(socket);
    if ( data == mainServer->message("ASK_LIST_GAMES")) {   
        std::cout << "TcpServer : ASK_LIST_GAMES" << std::endl;
    }

    std::cout << "TcpServer : toto" << std::endl;
}

void TcpServer::clientDisconnected()
{
    // On détermine quel client se déconnecte
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0) // Si par hasard on n'a pas trouvé le client à l'origine du signal, on arrête la méthode
        return;

    clients.removeOne(socket);

    socket->deleteLater();
}

void TcpServer::displayErrorTcp(QAbstractSocket::SocketError socketError)
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
