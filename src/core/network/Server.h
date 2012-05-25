#pragma once

#include <QtCore>
#include <QApplication>

/** \class Server
  * \brief Get all incoming connection
  *
  * Create a UdpServer and a TcpServer to catch every incoming connection.
  * Redirect them to the right game server (Brain) if necessary
  */

class Brain;
class Game;
class UdpServer;
class TcpServer;

class Server : public QObject
{
	Q_OBJECT

	public:
		Server();
		Server(int n);
		~Server();

	private:
		UdpServer * udpServer;
		TcpServer * tcpServer;
		QMap <quint32, Brain *> * brains;

	public slots:
		quint32 makeNewBrain();

    public:
        QByteArray listOfServers() const;
        static QList <QMap<QString,QString> *> decodeListOfServers(QByteArray dat);
				
};
