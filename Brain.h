#pragma once

#include <QObject>
#include <QString>

class Brain : QObject
{
	Q_OBJECT

	public:
		Brain();

	private:
		void sendTo(int idClient, QString message);
		void sendToAll(QString message);
		virtual void processReceive(QString message);

};
