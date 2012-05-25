#include "src/core/network/TcpServer.h"
#include "src/core/network/Server.h"
#include "src/core/type.h"

TcpServer::TcpServer(Server * s)
{
	qDebug() << "Constructeur TcpServer";

	// Démarrage serveur TCP
    tcpServer = new QTcpServer();

	// Vérification
	if (!tcpServer->listen(QHostAddress::Any, 12801)) {
        qDebug() << "Unable to start the server Tcp : " << tcpServer->errorString();
        return;
    }

    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));

    mainServer = s;
}

TcpServer::~TcpServer()
{
	qDebug() << "Destructeur TcpServer";
}

void TcpServer::newConnection()
{
    qDebug() << "TcpServer : New connection Tcp !";

    //QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    //int socketDescriptor = clientConnection->socketDescriptor();
    //qDebug() << "Socket Descriptor : " << socketDescriptor;

    //TcpClientThread * t = new TcpClientThread(socketDescriptor, mainServer, this);
    //connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
    //t->start();

   /* QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    connect(clientConnection, SIGNAL(disconnected()), clientConnection, SLOT(deleteLater()));

    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readDataTcp()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(displayErrorTcp(QAbstractSocket::SocketError)));

    clientConnection->write(mainServer->message("HELLO_FROM_SERVER"));
    clientConnection->write(mainServer->message("HELLO_FROM_SERVER"));
    qDebug() << "Server : HELLO_FROM_SERVER";*/
    //clientConnection->disconnectFromHost();

    QTcpSocket *nouveauClient = tcpServer->nextPendingConnection();
    clients << nouveauClient;

    connect(nouveauClient, SIGNAL(readyRead()), this, SLOT(readDataTcp()));
    connect(nouveauClient, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));


    nouveauClient->write(mainServer->messageByteArray("HELLO_FROM_SERVER"));
    qDebug() << "Server : HELLO_FROM_SERVER";
}

void TcpServer::readDataTcp()
{
    qDebug() << "TcpServer : Tcp data received";

    // On détermine quel client envoie le message (recherche du QTcpSocket du client)
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0)
        return;

    qDebug() << "TcpServer : Sender found";

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
    qDebug() << "TcpServer : toto";
    in >> data;

    qDebug() << "TcpServer : toto";
*/

    QString data;
    quint32 type;
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_4_6);
    in >> type >> data;

 //   qDebug() << mainServer->decodeDatagram(in);

    if (type == DataType::MESSAGE) {
        if (data == mainServer->messageString("ASK_LIST_GAMES")) {
            qDebug() << "TcpServer : ASK_LIST_GAMES";
            socket->write(mainServer->listOfServers());
        }
    }

    qDebug() << "TcpServer : toto";
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

