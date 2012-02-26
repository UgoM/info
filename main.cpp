#include <QApplication>
#include <QtCore>

#include "Server.h"
#include "TcpClient.h"

#include <QPushButton>
#include <QtNetwork>

 
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
 
    QPushButton bouton("Salut les Zéros, la forme ? Test");
    bouton.setText("Pimp mon encore !");
 
    bouton.show();

	Server * server = new Server();

	TcpClient * tcpClient = new TcpClient();


	 QUdpSocket * udpSocket = new QUdpSocket();
     QByteArray datagram = "Broadcast message " + QByteArray::number(128);
     udpSocket->writeDatagram(datagram.data(), datagram.size(),
                              QHostAddress::Broadcast, 12800);

	
    return app.exec();
}
