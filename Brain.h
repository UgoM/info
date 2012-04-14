#pragma once

#include <QObject>
#include <QString>

class Brain : public QObject
{
	Q_OBJECT

	public:
		Brain();

        virtual QString name(){return "";};
        virtual QString nPlayers(){return "";};

	private:
		void sendTo(int idClient, QString message);
		void sendToAll(QString message);
		virtual void processReceive(QString message);

};
