#include "src/games/checkers/BrainCheckers.h"

void BrainCheckers::init()
{
    nMaxPlayers = 2;
}

void BrainCheckers::processReceive(QByteArray dat){
	qDebug() << "Board received :";
	qDebug() << dat;
    sendToAll( dat);
    lastData = dat;
}

QString BrainCheckers::nPlayers()
{
    return QString("2/2");
}

QByteArray BrainCheckers::getGameState()
{
    return lastData;
}
