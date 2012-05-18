#include "src/core/game/Brain.h"
#include "src/core/network/Server.h"

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

    connect(this, SIGNAL(newGameData(QString)), this, SLOT(processReceive(QString)));

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
        clients[i]->write(block);
    }
}

void Brain::processReceive(QString block)
{
    qDebug() << "new data : " << block;
    (void) block;
}

void Brain::newConnection()
{
    qDebug() << "Brain : New connection Tcp !";

    QTcpSocket *nouveauClient = tcpServer->nextPendingConnection();
    clients << nouveauClient;

    connect(nouveauClient, SIGNAL(readyRead()), this, SLOT(readDataTcp()));
    connect(nouveauClient, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));

    emit newObs();

    // update clients (players and obs)
    nObs ++;
    sendNObs();
}

// send to everybody the new number of obs
void Brain::sendNObs()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << DataType::NOBS;
    out << nObs;

    std::cout << "Brain::sendNObs" << std::endl;
    for (int i = 0; i < clients.size(); i++)
    {
        std::cout << "-> client " << i << std::endl;
        clients[i]->write(block);
    }
}
// send to everybody the new number of players
void Brain::sendNPlayers()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);
    out << DataType::NOBS;
    out << nPlayers;

    std::cout << "Brain::sendNObs" << std::endl;
    for (int i = 0; i < clients.size(); i++)
    {
        std::cout << "-> client " << i << std::endl;
        clients[i]->write(block);
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

