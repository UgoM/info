#pragma once

#include <QWidget>
#include <QString>
#include <QtNetwork>

class Game : public QWidget
{
	Q_OBJECT

    protected:
		int clientType;
		int idPlayer;

	public:
		Game();
		~Game();
        void setServer(QString hostAddress, quint32 port);
        void setClientType(int clientType); 

	private:
		void send(QByteArray block);
		virtual void processClick();
		virtual void processKey();

    private:
        QTcpSocket * tcpSocket;

    private slots:  
        void connected();
        void disconnected();
        void readDataTcp();
        void displayErrorTcp(QAbstractSocket::SocketError socketError);

		virtual void processReceive(QByteArray block);

    public slots:
        virtual void reSendData(){};

    signals:
        void newGameData(QByteArray block);
};

namespace ClientType {
    enum EClientType {
        NONE,
        OBSERVER,
        PLAYER_1
    };
}