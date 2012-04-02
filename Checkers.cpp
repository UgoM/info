#include "Checkers.h"

Checkers::Checkers() : Game() {
	board = new Board();
	// board->show();
	QByteArray b = board->encodeBoard();
	board->decodeBoard(b);
}

void Checkers::processReceive(QString message) {

}

void Checkers::processClick() {

}

void Checkers::processKey() {

}