#include <stdexcept>
#include <sstream>
#include <QLabel>
#include "Board.h"

Board::Board() : QWidget() {
	image = new Image();
	table = new Cell**[MAX_COL];
	for (int i = 0; i < MAX_COL ; i++) {
		table[i] = new Cell*[MAX_ROW];
	}
	for (int i = 0; i < MAX_ROW; i++) {
		for (int j = 0; j < MAX_COL; j++) {
			if ( (i + j) % 2 == 0) {
				table[i][j] = new Cell(QPoint(i, j), image->getEvenCell());
			} else {
				if (i <= 3) {
					table[i][j] = new Cell(QPoint(i, j), BLACK_PAWN, image->getOddCell());
				} else if (i >= 6) {
					table[i][j] = new Cell(QPoint(i, j), WHITE_PAWN, image->getOddCell());
				} else {
					table[i][j] = new Cell(QPoint(i, j), image->getOddCell());
				}
			}
		}
	}
}

Cell * Board::getCellAt(const QPoint & pos) const {
	if (pos.x() < 0 || pos.x() >= MAX_COL || pos.y() < 0 || pos.y() >= MAX_ROW) {
		std::stringstream sstm;
		sstm << "Index out of bounds : (" << pos.x() << "," << pos.y() + ")";
		throw std::logic_error(sstm.str());
	}
	return table[pos.x()][pos.y()];
}

void Board::update() {
	for (int i = 0; i < MAX_ROW; i++) {
		for (int j = 0; j < MAX_COL; j++) {
			QLabel label(this);
			Cell *cell = table[i][j];
			label.setPixmap(cell->getPicture());
			if (!cell->isEmpty()) {
				QLabel picture(&label);
				switch(cell->getPiece()) {
					case WHITE_PAWN : picture.setPixmap(image->getWhitePawn()); break;
					case BLACK_PAWN : picture.setPixmap(image->getBlackPawn()); break;
					case WHITE_QUEEN : picture.setPixmap(image->getWhiteQueen()); break;
					case BLACK_QUEEN : picture.setPixmap(image->getBlackQueen()); break;
				}
			}
			label.move((i%MAX_ROW) * CELL_SIZE, (j%MAX_COL) * CELL_SIZE);
		}
	}
}

Board::~Board() {
	delete image;
	for (int i = 0; i < MAX_ROW; i++) {
		for (int j = 0; j < MAX_COL; j++) {
			delete table[i][j];
		}
		delete[] table[i];
	}
	delete[] table;
}