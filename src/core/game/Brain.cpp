#include "src/core/game/Brain.h"
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


/** \brief send data to a client
  * \param socket : socket of the client
  * \param dat : data to send
  * \param type : from DataType
  *
  * All send function are private, except sendToAll. This function is the basic
  * one, all other sendTo are just more practical way to use it.
  */
void Brain::sendTo(QTcpSocket * socket, QByteArray dat, int type)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << (quint32)type;
    out << dat;
    socket->write(block);
}
/** \brief see Brain::sendTo(QTcpSocket * socket, QByteArray dat, int type)
  */
void Brain::sendTo(Client * client, QByteArray dat, int type)
{
    sendTo(client->socket, dat, type);
}
/** \brief see Brain::sendTo(QTcpSocket * socket, QByteArray dat, int type)
  */
void Brain::sendToAll(QByteArray dat, int type)
{
    for (int i = 0; i < clients.size(); i++)
    {
        qDebug() << "-> client " << i << "#" << dat;
        sendTo(clients[i], dat, type);
    }
}
/** \brief send game data to all clients (players and obs)
  */
void Brain::sendToAll(QByteArray dat)
{
    sendToAll(dat, DataType::GAMEDATA);
}
/** \brief function to be redefined, process gamedata received
  * \param dat : game data to be processed
  */
void Brain::processReceive(QByteArray dat)
{
    qDebug() << "You need to subclass Brain::processReceive(QByteArray block)";
    (void) dat;
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
    nObs++;

    clients << newClient;

    connect(newClient->socket, SIGNAL(readyRead()), this, SLOT(readDataTcp()));
    connect(newClient->socket, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));

    sendTo ( newClient, getGameState(), DataType::GAMEDATA) ;

    // update clients (players and obs) after 2 sec
    // so the new client has time to create his window
    QTimer::singleShot(2000, this, SLOT(sendNConnected()));
}

// send to everybody the new number of obs and players
void Brain::sendNConnected()
{
    QByteArray dat = QVariant(QString::number(nPlayers) + 
                        "," + QString::number(nObs)).toByteArray();
    qDebug() << "sendNConnected" << dat;
    sendToAll(dat, DataType::NCONNECTED);
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
        case DataType::CHATDATA:
            sendToAll(block, DataType::CHATDATA); // simply relay Chat Message
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

    socket->deleteLater();
}

/** \brief To know id of a client when we know its socket
  */
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

/** \brief Used when an obs asked to become a new player
  * \param socket : socket of the client, identify him
  */
void Brain::addNewPlayer(QTcpSocket *socket)
{
    int answer;

    if (nPlayers < nMaxPlayers)
    {
        int id = clientIdFromSocket(socket);
        clients[id]->type = ClientType::PLAYER;
        answer = Message::OK_YOU_CAN_PLAY;
        nObs --;
        nPlayers ++;
    } else {
        answer = Message::NO_YOU_CANT_PLAY;
    }

    sendTo( socket, 
            QVariant(QString::number(answer)).toByteArray(), 
            DataType::MESSAGE);
    QTimer::singleShot(1, this, SLOT(sendNConnected()));
}

/** \brief game initialization, must be subclassed
  */
void Brain::init()
{
    /// \todo : move this function in the constructor
    qDebug() << "ERROR : Brain::init must be subclassed";

    /// Model :
    ///
    /// nMaxPlayers = x;
    /// gameName = "Awesome game"
}

/** \brief get last game data
  *
  * Used, for example, when a new obs is connecting, and wants to have the game
  * state. Must be subclassed.
  */
QByteArray Brain::getGameState()
{
    qDebug() << "ERROR : Brain::getGameState must be subclassed";
    return NULL;
}

/** \brief returns the name of the game
  */
QString Brain::name()
{
    return gameName;
}
/** \brief returns the current number of players
  */
int Brain::getNPlayers()
{
    return nPlayers;
}
/** \brief returns the port on which Brain is listening
  */
quint32 Brain::getPort()
{
    return port;
}

