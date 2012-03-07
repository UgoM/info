#ifndef DEF_CHECKERS
#define DEF_CHECKERS

#include "Game.h"
#include "Board.h"

class Checkers : public Game {

	Board * board;

	public:
		Checkers();
		
		void processReceive(QString message);
		void processClick();
		void processKey();

};

#endif