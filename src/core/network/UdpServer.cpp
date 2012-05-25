#include "src/core/network/UdpServer.h"
#include "src/core/network/Server.h"
#include "src/core/type.h"

UdpServer::UdpServer()
{
	// Starting listening for UDP packets
	udpSocket = new QUdpSocket(this);
	udpSocket->bind(12800, QUdpSocket::ShareAddress);

	connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
}

UdpServer::~UdpServer()
{
	delete udpSocket;
}

void UdpServer::processPendingDatagrams()
{
	qDebug() << "New connection UDP on port 12800";

    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress senderHost;
        quint16 senderPort;

        udpSocket->readDatagram(datagram.data(), datagram.size(),
                             &senderHost, &senderPort);

        processTheDatagram(datagram, senderHost, senderPort);
    }
}

void UdpServer::processTheDatagram (QByteArray datagram, QHostAddress senderHost, quint16 senderPort)
{

    qDebug() << "UdpServer::processTheDatagram";

    QByteArray block;
    quint32 type;
    QDataStream in(datagram);
    in.setVersion(QDataStream::Qt_4_6);
    in >> type >> block;

    int messageId;
    QByteArray block_out;
    QDataStream out(&block_out, QIODevice::WriteOnly);
    switch (type)
    {
        case DataType::MESSAGE:
            messageId = block.toInt();
            switch (messageId)
            {
                case Message::UDP_ASK_FOR_SERVER:
                    qDebug() << "UDP_ASK_FOR_SERVER " << senderHost.toString() 
                             << ":" << senderPort;
	                /// the Udp server just respond it is there, so the client 
	                /// can connect himself to the tcp server.
                    out.setVersion(QDataStream::Qt_4_6);
                    out << (quint32)type;
                    out << QVariant(QString::number(Message::ANSWER_UDP_ASK_FOR_SERVER)).toByteArray();
                    udpSocket->writeDatagram(block_out.data(), datagram.size(), senderHost, 12801);
                    break;
                default:
                    qDebug() << "Wrong message in UdpServer::processTheDatagram";
                    qDebug() << messageId;
                    qDebug() << block;
            }
            break;
        default:
            qDebug() << "Wrong type of data in UdpServer::processTheDatagram";
    }
}

