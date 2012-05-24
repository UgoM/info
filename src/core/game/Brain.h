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
            QTcpSocket * socket;    /**< client's socket */
            int type;               /**< client's type (EClientType) */
    };

	public:
		Brain();
        quint32 getPort() { return port; };

        virtual void init();
        virtual QString name(){return "";};
        int getNPlayers(){return nPlayers;};

	protected:
		void sendTo(int idClient, QByteArray dat);
		void sendToAll(QByteArray block);
        quint32 nMaxPlayers;

    private:
        quint32 port;
        QTcpServer * tcpServer;
        QList<Client *> clients;
        quint32 nObs;
        quint32 nPlayers;

        int clientIdFromSocket(QTcpSocket *socket);
        void addNewPlayer(QTcpSocket *socket);

    private slots:
        void newConnection();
        void readDataTcp();
        void clientDisconnected();
		virtual void processReceive(QByteArray block);
        void sendNConnected();

    signals:
        void newObs();
};
