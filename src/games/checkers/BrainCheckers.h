#ifndef DEF_BRAINCHECKERS
#define DEF_BRAINCHECKERS

#include <QString>
#include <QObject>
#include "src/core/game/Brain.h"

class BrainCheckers : public Brain {

	Q_OBJECT

	public slots:
        void init();

        QByteArray getGameState();

    private:
        QByteArray lastData;

        void processReceive(QByteArray dat);
};

#endif
