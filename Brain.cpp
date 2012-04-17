#include "Brain.h"
#include "Server.h"
#include <iostream>

Brain::Brain()
{
	std::cout << "Constructeur Brain" << std::endl;

    // Start TcpServer on an unused port
    tcpServer = new QTcpServer();
    port = 12802;
    while (!tcpServer->listen(QHostAddress::Any, port)) {
        port ++;
    }
    std::cout << "Start the server Tcp on port " << port << std::endl;

    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));

    connect(this, SIGNAL(newGameData(QString)), this, SLOT(processReceive(QString)));
}


void Brain::sendTo(int idClient, QByteArray block)
{
}

void Brain::sendToAll(QByteArray dat)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << DataType::GAMEDATA;
    out << dat;

    std::cout << "Brain::sendToAll" << std::endl;
    for (int i = 0; i < clients.size(); i++)
    {
        std::cout << "-> client " << i << std::endl;
        clients[i]->write(block);
    }
}

void Brain::processReceive(QString block)
{
    std::cout << "new data : " << block.toStdString() << std::endl;
}

void Brain::newConnection()
{
    std::cout << "Brain : New connection Tcp !" << std::endl;

    QTcpSocket *nouveauClient = tcpServer->nextPendingConnection();
    clients << nouveauClient;

    connect(nouveauClient, SIGNAL(readyRead()), this, SLOT(readDataTcp()));
    connect(nouveauClient, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
}

void Brain::readDataTcp()
{
    std::cout << "Brain : Tcp data received" << std::endl;

    // On détermine quel client envoie le message (recherche du QTcpSocket du client)
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0)
        return;

    std::cout << "Brain : Sender found" << std::endl;

    QString block;
    quint32 type;
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_4_6);
    in >> type >> block;

    if (type == DataType::GAMEDATA) {
        emit newGameData( block );   
    }
}

void Brain::clientDisconnected()
{
    // On détermine quel client se déconnecte
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0) // Si par hasard on n'a pas trouvé le client à l'origine du signal, on arrête la méthode
        return;

    clients.removeOne(socket);

    socket->deleteLater();
}

