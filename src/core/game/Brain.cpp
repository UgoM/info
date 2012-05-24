#include "src/core/game/Brain.h"
#include "src/core/network/Server.h"
#include "src/core/type.h"
#include <QTimer>

Brain::Brain()
{
	qDebug() << "Constructeur Brain";

    // Start TcpServer on an unused port
    tcpServer = new QTcpServer();
    port = 12802;
    while (!tcpServer->listen(QHostAddress::Any, port)) {
        port ++;
    }
    qDebug() << "Start the server Tcp on port " << port;

    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));

    nPlayers = 0;
    nObs = 0;
}


void Brain::sendTo(int idClient, QByteArray block)
{
    (void) idClient;
    (void) block;
}

void Brain::sendToAll(QByteArray dat)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << DataType::GAMEDATA;
    out << dat;

    qDebug() << "Brain::sendToAll";
    for (int i = 0; i < clients.size(); i++)
    {
        qDebug() << "-> client " << i;
        clients[i]->socket->write(block);
    }
}

void Brain::processReceive(QByteArray block)
{
    qDebug() << "new data : " << block;
    (void) block;
}

/** \brief create a new client from a new connection
  */
void Brain::newConnection()
{
    qDebug() << "Brain : New connection Tcp !";

    Client * newClient = new Client();
    newClient->socket = tcpServer->nextPendingConnection();
    newClient->type = ClientType::OBSERVER; // default : a new client is an obs
                                            // it has to ask to become a player
    clients << newClient;

    connect(newClient->socket, SIGNAL(readyRead()), this, SLOT(readDataTcp()));
    connect(newClient->socket, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));

    /// \todo differenciate players and observators
    //emit newObs();

    // update clients (players and obs) after 2 sec
    // so the new client has time to create his window
    //nObs ++;
    //QTimer::singleShot(2000, this, SLOT(sendNConnected()));
}

// send to everybody the new number of obs and players
void Brain::sendNConnected()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << (quint32)DataType::NCONNECTED;
    out << QVariant(QString::number(nPlayers) + "," + QString::number(nObs)).toByteArray();

    qDebug() << "Brain::sendNConnected";
    for (int i = 0; i < clients.size(); i++)
    {
        qDebug() << "-> client " << i;
        clients[i]->socket->write(block);
    }
}

void Brain::readDataTcp()
{
    qDebug() << "Brain : Tcp data received";

    // On détermine quel client envoie le message (recherche du QTcpSocket du client)
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0)
        return;

    qDebug() << "Brain : Sender found";

    QByteArray block;
    quint32 type;
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_4_6);
    in >> type >> block;

    int messageId;
    switch (type)
    {
        case DataType::GAMEDATA:
            processReceive( block );
            break;
        case DataType::MESSAGE:
            messageId = block.toInt();
            switch (messageId)
            {
                case Message::I_WANT_TO_PLAY:
                    addNewPlayer(socket);
                    break;
                default:
                    qDebug() << "Wrong message in Brain::readDataTcp";
                    qDebug() << messageId;
                    qDebug() << block;
            }
            break;
        default:
            qDebug() << "Wrong type of data in Brain::readDataTcp";
    }
}

void Brain::clientDisconnected()
{
    // On détermine quel client se déconnecte
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0) // Si par hasard on n'a pas trouvé le client à l'origine du signal, on arrête la méthode
        return;

    int id = clientIdFromSocket(socket);
    if (id != -1) 
        clients.removeAt(id);
/*    for (int i = 0; i < clients.size(); i++)
    {
        if (clients[i]->socket == socket)
        {
            clients.removeAt(i);
            break;
        }
    }*/

    socket->deleteLater();
}

int Brain::clientIdFromSocket(QTcpSocket *socket)
{
    for (int i = 0; i < clients.size(); i++)
    {
        if (clients[i]->socket == socket)
        {
            return i;
        }
    }
    return -1;
}

void Brain::addNewPlayer(QTcpSocket *socket)
{
    int answer;
    int id = clientIdFromSocket(socket);

    if (nPlayers < nMaxPlayers)
    {
        clients[id]->type = ClientType::PLAYER;
        answer = Message::OK_YOU_CAN_PLAY;
    } else {
        answer = Message::NO_YOU_CANT_PLAY;
    }

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << DataType::MESSAGE;
    out << QVariant(QString::number(answer)).toByteArray();;

    clients[id]->socket->write(block);
    /// \todo create a function send(clientid, datatype, (int, qstring, qbytearray...) data)
}

/** \brief game initialization, must be subclassed
  */
void Brain::init()
{
    qDebug() << "ERROR : Brain::init must be subclassed";

    /// Model :
    ///
    /// nMaxPlayers = x;
}
