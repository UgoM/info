#pragma once

#include <QObject>
#include <QString>
#include <QtNetwork>

class Brain : public QObject
{
	Q_OBJECT

    /** \brief nested class for internal of Brain
      *
      * Contains all data about a client
      */
    class Client
    {
        public:
            QTcpSocket * socket;    /**< socket */
            int type;               /**< type (EClientType) */
            int playerId;           /**< from 1 to nMaxPlayers */ 
    };

	public:
		Brain();
        virtual void init();

        QString name();
        int getNPlayers();
        quint32 getPort();

	protected:
        quint32 nMaxPlayers;
        QString gameName;

		void sendToAll(QByteArray block);

		virtual void processReceive(QByteArray block);
        virtual QByteArray getGameState();

    private:
        quint32 port;
        QTcpServer * tcpServer;
        QList<Client *> clients;
        quint32 nObs;
        quint32 nPlayers;

        int clientIdFromSocket(QTcpSocket *socket);
        int getUnusedPlayerId();
        void addNewPlayer(QTcpSocket *socket);

        void sendTo(QTcpSocket * socket, QByteArray dat, int type);
        void sendTo(Client * client, QByteArray dat, int type);
        void sendToAll(QByteArray dat, int type);

    private slots:
        void newConnection();
        void readDataTcp();
        void clientDisconnected();
        void sendNConnected();

    signals:
        void newObs();
};
