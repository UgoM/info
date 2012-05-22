#pragma once

#include <QObject>
#include <QString>
#include <QtNetwork>

class Brain : public QObject
{
	Q_OBJECT

	public:
		Brain();
        quint32 getPort() { return port; };

        virtual QString name(){return "";};
        int getNPlayers(){return nPlayers;};

	protected:
		void sendTo(int idClient, QByteArray dat);
		void sendToAll(QByteArray block);

    private:
        quint32 port;
        QTcpServer * tcpServer;
        QList<QTcpSocket *> clients;
        quint32 nObs;
        quint32 nPlayers;

    private slots:
        void newConnection();
        void readDataTcp();
        void clientDisconnected();
		virtual void processReceive(QString block);
        void sendNConnected();

    signals:
        void newGameData(QString block);
        void newObs();
};
