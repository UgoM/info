#pragma once

#include <QObject>
#include "src/core/game/Brain.h"

class BrainExample : public Brain {

	Q_OBJECT

	public slots:
        void init();
        QByteArray getLastData();

    private:
        void processReceive(QByteArray dat);
};

