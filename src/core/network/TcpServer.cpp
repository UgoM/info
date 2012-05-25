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
   
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << (quint32)DataType::MESSAGE;
    out << QVariant(QString::number(Message::HELLO_FROM_SERVER)).toByteArray();
    nouveauClient->write(block);

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

    QByteArray block;
    quint32 type;
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_4_6);
    in >> type >> block;

    int messageId;
    switch (type)
    {
        case DataType::MESSAGE:
            messageId = block.toInt();
            switch (messageId)
            {
                case Message::ASK_LIST_GAMES:
                    qDebug() << "TcpServer : ASK_LIST_GAMES";
                    socket->write(mainServer->listOfServers());
                    break;
                default:
                    qDebug() << "Wrong message in TcpServer::readDataTcp";
                    qDebug() << messageId;
                    qDebug() << block;
            }
            break;
        default:
            qDebug() << "Wrong type of data in TcpServer::readDataTcp";
    }
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

