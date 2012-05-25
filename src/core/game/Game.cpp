#include "src/core/game/Game.h"
#include "src/core/network/Server.h"
#include "src/core/type.h"

Game::Game()
{
    qDebug() << "Constructeur Game";
    clientType = ClientType::OBSERVER;
    idPlayer = 0;

    nPlayers = 0;
    nObs = 0;
}

Game::~Game()
{
	qDebug() << "Destructeur Game";
}

/** \brief send data to the server of the game (Brain)
  * \param dat : data to be send
  *
  * Send only data of type DataType::GAMEDATA to the Brain, other types are not
  * to be used by subclasses.
  */
void Game::send(QByteArray dat)
{
    send(dat, DataType::GAMEDATA);
}
/** \brief same as Game::send(QByteArray dat), but is private can send other DataType
  * \param dat : data to send
  * \param type : from DataType
  */
void Game::send(QByteArray dat, int type)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << (quint32)type;
    out << dat;
    tcpSocket->write(block);
}


void Game::processReceive(QByteArray block)
{
    (void) block;
}

void Game::processClick()
{
}

void Game::processKey()
{
}

void Game::setServer(QString hostAddress, quint32 port)
{
    qDebug() << "Game::setServer";
    qDebug() << "hostAddress : " << hostAddress << ", id : " << port;

    /// \todo change name of tcpSocket
    tcpSocket = new QTcpSocket();

    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readDataTcp()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(displayErrorTcp(QAbstractSocket::SocketError)));

    qDebug() << "Game : Tcp connection to host " << hostAddress << ":" << port;
    tcpSocket->connectToHost(hostAddress, port);

    connect(tcpSocket, SIGNAL(connected()), this, SLOT(connected()));
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

void Game::connected()
{
    qDebug() << "Game : Connecté au server TCP";
}
void Game::disconnected()
{
    qDebug() << "Game : Déconnécté du server TCP";
}


void Game::readDataTcp()
{
    qDebug() << "Game : Tcp data received";

    QByteArray block;
    quint32 type;
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_4_6);
    in >> type >> block;

    /// \todo : change these else if by a switch
    if (type == DataType::GAMEDATA) {
        qDebug() << "GAMEDATA";
	    qDebug() << block;
        processReceive( block );  
    } else if (type == DataType::NCONNECTED) {
        qDebug() << "NCONNECTED";
        qDebug() << block;
        QList<QByteArray> l = block.split(',');
        nPlayers = l[0].toInt(); nObs = l[1].toInt();
        emit nConnectedChanged(nPlayers, nObs);
    } else if (type == DataType::MESSAGE) {
        int messageId = block.toInt();
        switch (messageId)
        {
            case Message::OK_YOU_CAN_PLAY:
                clientType = ClientType::PLAYER;
                emit newStatus("Playing now");
                qDebug() << "Playing now";
                break;
            case Message::NO_YOU_CANT_PLAY:
                emit newStatus("Server refused you as a player, observing now");
                qDebug() << "Server refused you as a player, observing now";
                break;
            default:
                qDebug() << "Wrong message in Game::readDataTcp";
        }
    }
}

void Game::displayErrorTcp(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        qDebug() << "The host was not found. Please check the host name and port settings.";
        break;
    case QAbstractSocket::ConnectionRefusedError:
        qDebug() << "The connection was refused by the peer. Make sure the fortune server is running, \
                        and check that the host name and port settings are correct.";
        break;
    default:
        qDebug() << "The following error occurred:" << tcpSocket->errorString();
    }
}

/** \brief try to change client type
  *
  * By default, everybody is an obs, so in order to become a player, client
  * should ask Brain if he can.
  * If client is a player and wants to become an obs, he simply inform Brain.
  */
void Game::setClientType( int id )
{
    if (!tcpSocket) {
        qDebug() << "Need to setServer before calling setClientType";
        return;
    }

    if (clientType == id) {
        qDebug() << "Client is already " << id;
        return;
    }

    quint32 message = 0;
    switch (clientType) {
        case ClientType::NONE:
            qDebug() << "Error : ClientType is None";
        case ClientType::OBSERVER:
            if (id == ClientType::PLAYER) {
                message = Message::I_WANT_TO_PLAY;
            } else {
                qDebug() << "Invalid Client Type given";
            }
            break;
        case ClientType::PLAYER:
            if (id == ClientType::OBSERVER) {
                message = Message::I_AM_AN_OBS_NOW;
            } else {
                qDebug() << "Invalid Client Type given";
            }
            break;
        default:
            qDebug() << "Invalid Client Type given";
     }
            
    if (message != 0) {
        send(QVariant(QString::number(message)).toByteArray(), DataType::MESSAGE);
        qDebug() << QVariant(QString::number(message)).toByteArray();
    }
}
