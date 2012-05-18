#include "src/games/checkers/BrainCheckers.h"

#include <QDebug>

void BrainCheckers::handleMove(QByteArray boardEnc) {
	qDebug() << "Board received :";
	qDebug() << boardEnc;
    sendToAll( boardEnc);
}

QString BrainCheckers::nPlayers()
{
    return QString("2/2");
}
