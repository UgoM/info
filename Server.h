#pragma once

#include "TcpServer.h"
#include "UdpServer.h"
#include "Brain.h"
#include "Game.h"

#include <QObject>
#include <QtCore>

class Server : public QObject
{
	Q_OBJECT

	public:
		Server();
		~Server();

	private:
		/// listen to UDP request
		UdpServer * udpServer;
		/// listen to TCP request
		TcpServer * tcpServer;
		/// list of every brains on the local host
		
		/// list of every clients local or distant connected to the host with
		/// their Brain associated

		/// each time a new Brain or Game is created, we keep a track of them here
		QList <Brain *> * brains;
		QList <Game *> * games;

	public slots:
		void makeNewGame();
				
};
