#include "src/core/network/UdpServer.h"
#include <iostream>


UdpServer::UdpServer(Server * s)
{
	// Starting listening for UDP packets
	udpSocket = new QUdpSocket(this);
	udpSocket->bind(12800, QUdpSocket::ShareAddress);

	connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));

    mainServer = s;
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

    QString data;
    quint32 type;
    QDataStream in(datagram);
    in >> type >> data;

    if (type == DataType::MESSAGE) {
        qDebug() << "MESSAGE";
        qDebug() << type;
        qDebug() << data;
        qDebug() << "UdpServer messageByteArray : ";
        for (int i = 0; i < data.size(); ++i) {
            qDebug() << data.at(i).toAscii();
        }
    qDebug();
        qDebug() << mainServer->messageString("UDP_ASK_FOR_SERVER");
        if (data == mainServer->messageString("UDP_ASK_FOR_SERVER")) {
            qDebug() << "UDP_ASK_FOR_SERVER " << senderHost.toString() << ":" << senderPort;
	        /// the Udp server just respond it is there, so the client 
	        /// can connect himself to the tcp server.
            datagram = mainServer->messageByteArray("ANSWER_UDP_ASK_FOR_SERVER");
            udpSocket->writeDatagram(datagram.data(), datagram.size(), senderHost, 12801);//senderPort);
        }
    }
}

