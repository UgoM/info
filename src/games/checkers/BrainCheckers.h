#ifndef DEF_BRAINCHECKERS
#define DEF_BRAINCHECKERS

#include <QString>
#include <QObject>
#include "src/core/game/Brain.h"

class BrainCheckers : public Brain {

	Q_OBJECT

	public slots:
        void init();

        QString name() {return QString("Jeu de dames");};
        QString nPlayers();

        QByteArray getGameState();

    private:
        QByteArray lastData;

        void processReceive(QByteArray dat);
};

#endif
