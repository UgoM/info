#include "Game.h"
#include "Server.h"

#include <iostream>

Game::Game()
{
    connect(this, SIGNAL(newGameData(QByteArray)), this, SLOT(processReceive(QByteArray)));
	std::cout << "Constructeur Game" << std::endl;
    clientType = ClientType::NONE;
    idPlayer = 0;
}

Game::~Game()
{
	std::cout << "Destructeur Game" << std::endl;
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
    std::cout << "Game::setServer" << std::endl;
    std::cout << "hostAddress : " << hostAddress.toStdString()
                << ", id : " << port << std::endl;

    tcpSocket = new QTcpSocket();

    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readDataTcp()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(displayErrorTcp(QAbstractSocket::SocketError)));

    std::cout << "Game : Tcp connection to host " << hostAddress.toStdString() << ":" << port << std::endl;
    tcpSocket->connectToHost(hostAddress, port);

    connect(tcpSocket, SIGNAL(connected()), this, SLOT(connected()));
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

void Game::connected()
{
    std::cout << "Game : Connecté au server TCP" << std::endl;
}
void Game::disconnected()
{
    std::cout << "Game : Déconnécté du server TCP" << std::endl;
}


void Game::readDataTcp()
{
    std::cout << "Game : Tcp data received" << std::endl;

    QByteArray block;
    quint32 type;
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_4_6);
    in >> type >> block;

    if (type == DataType::GAMEDATA) {
        std::cout << "GAMEDATA" << std::endl;
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

void Game::setClientType( int id)
{
    clientType = id;
}
