#pragma once

#include <QWidget>
#include <QString>
#include <QtNetwork>

class Game : public QWidget
{
	Q_OBJECT

    protected:
        /// \todo move the 4 next variables to private, and create function to access them
		int clientType;
		int idPlayer;

        // number of players and obs, can only be modified by brain
        quint32 nPlayers;
        quint32 nObs;

		void send(QByteArray data);
	public:
		Game();
		~Game();
        void setServer(QString hostAddress, quint32 port);
        void setClientType(int clientType); 

	private:
        void send(QByteArray dat, int type);
		virtual void processClick();
		virtual void processKey();

    private:
        QTcpSocket * socketServer;

    private slots:  
        void connected();
        void disconnected();
        void readDataTcp();
        void displayErrorTcp(QAbstractSocket::SocketError socketError);

		virtual void processReceive(QByteArray block);

    signals:
        void nConnectedChanged(int nPlayers, int nObs);
        void newStatus(QString status);
};
