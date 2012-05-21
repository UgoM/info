#pragma once

#include <QtCore>
#include <QApplication>

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
		/// listen to UDP request
		UdpServer * udpServer;
		/// listen to TCP request
		TcpServer * tcpServer;
		/// list of every brains on the local host
		
		/// list of every clients local or distant connected to the host with
		/// their Brain associated

		/// each time a new Brain or Game is created, we keep a track of them here
		QMap <quint32, Brain *> * brains;
		QMap <quint32, Game *> * games;

        quint32 lastIdGame;

	public slots:
		Game * makeNewGame();

    private:
        QMap <QString, QString> * messages;
        void initMessages();

    public:
        QByteArray messageByteArray (QString m);
        QString messageString (QString m);
        QByteArray listOfServers() const;
        QList <QMap<QString,QString> *> decodeListOfServers(QString s);
				
};

namespace DataType {
	enum EDataType {
		MESSAGE = 1,
        LISTOFSERVERS,
        GAMEDATA
	};
}
