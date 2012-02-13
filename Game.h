#pragma once

#include <QObject>
#include <QString>

class Game : public QObject
{
	Q_OBJECT

	public:
		Game();
		~Game();

	private:
		void sendTo(int idClient, QString message);
		void sendToAll(QString message);
		virtual void processReceive(QString message);
		virtual void processClick();
		virtual void processKey();
};
