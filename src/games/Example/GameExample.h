#pragma once

#include <QtGui>

#include "src/core/game/Game.h"

class GameExample : public Game {

	Q_OBJECT
	
	public:
		void processReceive(QByteArray block);
};

