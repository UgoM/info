#include "src/games/checkers/BrainCheckers.h"

void BrainCheckers::init()
{
    nMaxPlayers = 2;
    gameName = "Jeu de dames";
}

void BrainCheckers::processReceive(QByteArray dat){
	qDebug() << "Board received :";
	qDebug() << dat;
    sendToAll( dat);
    lastData = dat;
}

QByteArray BrainCheckers::getGameState()
{
    return lastData;
}
