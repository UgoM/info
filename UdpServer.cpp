#include "UdpServer.h"

#include <QtNetwork>
#include <QtCore>

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
	std::cout << "New connection UDP on port 12800" << std::endl;

    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress senderHost;
        quint16 senderPort;

	    std::cout << "UdpServer : Received Udp data : \"" << mainServer->decodeDatagram(udpSocket).toStdString() << "\"" << std::endl; 
        udpSocket->readDatagram(datagram.data(), datagram.size(),
                             &senderHost, &senderPort);

        processTheDatagram(datagram, senderHost, senderPort);
    }
}

void UdpServer::processTheDatagram (QByteArray datagram, QHostAddress senderHost, quint16 senderPort)
{

    if (datagram == mainServer->message("UDP_ASK_FOR_SERVER")) {
        std::cout << "UDP_ASK_FOR_SERVER " << senderHost.toString().toStdString() << ":" << senderPort << std::endl;
	    /// the Udp server just respond it is there, so the client 
	    /// can connect himself to the tcp server.
        datagram = mainServer->message("ANSWER_UDP_ASK_FOR_SERVER");
        udpSocket->writeDatagram(datagram.data(), datagram.size(), senderHost, 12800);//senderPort);
    }
}

