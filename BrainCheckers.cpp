#include "BrainCheckers.h"

#include <QDebug>

void BrainCheckers::handleMove(QByteArray boardEnc) {
	qDebug() << "Board received :";
	qDebug() << boardEnc;
}

QString BrainCheckers::nPlayers()
{
    return QString("A implementer");
}
