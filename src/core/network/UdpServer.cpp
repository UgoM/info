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
	std::cout << "New connection UDP on port 12800" << std::endl;

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

    std::cout << "UdpServer::processTheDatagram" << std::endl;

    QString data;
    quint32 type;
    QDataStream in(datagram);
    in >> type >> data;

    if (type == DataType::MESSAGE) {
        std::cout << "MESSAGE" << std::endl;
        std::cout << type << std::endl;
        std::cout << data.toStdString() << std::endl;
        std::cout << "UdpServer messageByteArray : ";
        for (int i = 0; i < data.size(); ++i) {
            std::cout << data.at(i).toAscii();
        }
    std::cout << std::endl;
        std::cout << mainServer->messageString("UDP_ASK_FOR_SERVER").toStdString() << std::endl;
        if (data == mainServer->messageString("UDP_ASK_FOR_SERVER")) {
            std::cout << "UDP_ASK_FOR_SERVER " << senderHost.toString().toStdString() << ":" << senderPort << std::endl;
	        /// the Udp server just respond it is there, so the client 
	        /// can connect himself to the tcp server.
            datagram = mainServer->messageByteArray("ANSWER_UDP_ASK_FOR_SERVER");
            udpSocket->writeDatagram(datagram.data(), datagram.size(), senderHost, 12801);//senderPort);
        }
    }
}

