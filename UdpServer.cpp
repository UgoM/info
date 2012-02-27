#include "UdpServer.h"

#include <QtNetwork>
#include <QtCore>
#include <QApplication>

#include <iostream>


UdpServer::UdpServer()
{
	std::cout << "Constructeur UdpServer" << std::endl;

	// Démarrage serveur UDP
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
	std::cout << "New connection UDP !" << std::endl;

    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        udpSocket->readDatagram(datagram.data(), datagram.size(),
                             &sender, &senderPort);

        processTheDatagram(datagram, sender, senderPort);
    }
}

void UdpServer::processTheDatagram (QByteArray datagram, QHostAddress sender, quint16 senderPort)
{
	/// the Udp server just respond it is there, so the client 
	/// can connect himself to the tcp server.
	std::cout << "Received Udp data : \"" << datagram.data() << "\"" << std::endl;
    
    /// TODO : check incoming datagram

    datagram = "I'm here " + QByteArray::number(128);
    udpSocket->writeDatagram(datagram.data(), datagram.size(),
                                    sender, senderPort);

	// to create a new game : just use newGame() signal
	//emit newGame();	
	//std::cout << "newGame() UDP emitted" << std::endl;
}

