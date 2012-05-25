#pragma once

#include <QWidget>
#include <QString>
#include <QtNetwork>

class Game : public QWidget
{
	Q_OBJECT

    private:
        QTcpSocket * socketServer;
		int clientType;
		int idPlayer;
        quint32 nPlayers;
        quint32 nObs;

        void send(QByteArray dat, int type);

    protected:
		void send(QByteArray data);
		virtual void processReceive(QByteArray dat);
        int getClientType();

	public:
		Game();
		~Game();
        void setServer(QString hostAddress, quint32 port);
        void setClientType(int clientType);

    private slots:  
        void connected();
        void disconnected();
        void readDataTcp();
        void displayErrorTcp(QAbstractSocket::SocketError socketError);


    signals:
        void nConnectedChanged(int nPlayers, int nObs);
        void newStatus(QString status);
};
