#include "src/core/game/Game.h"
#include "src/core/network/Server.h"

Game::Game()
{
    connect(this, SIGNAL(newGameData(QByteArray)), this, SLOT(processReceive(QByteArray)));
	qDebug() << "Constructeur Game";
    clientType = ClientType::NONE;
    idPlayer = 0;
}

Game::~Game()
{
	qDebug() << "Destructeur Game";
}

void Game::send(QByteArray block)
{
    (void) block;
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

    if (type == DataType::GAMEDATA) {
        qDebug() << "GAMEDATA";
	    qDebug() << block;
        emit newGameData( block );  
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

void Game::setClientType( int id)
{
    clientType = id;
}
