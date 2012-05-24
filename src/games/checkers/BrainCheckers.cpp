#include "src/games/checkers/BrainCheckers.h"

void BrainCheckers::init()
{
    nMaxPlayers = 2;
}

void BrainCheckers::handleMove(QByteArray boardEnc){
	qDebug() << "Board received :";
	qDebug() << boardEnc;
    sendToAll( boardEnc);
}

QString BrainCheckers::nPlayers()
{
    return QString("2/2");
}
