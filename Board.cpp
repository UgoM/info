#include <stdexcept>
#include <sstream>
#include "Board.h"


#include <iostream>

Board::Board() : QWidget() {
	setFixedSize(MAX_ROW * CELL_SIZE, MAX_COL * CELL_SIZE);
	image = new Image();
	table = new Cell**[MAX_COL];
	for (int i = 0; i < MAX_COL ; i++) {
		table[i] = new Cell*[MAX_ROW];
	}
	for (int i = 0; i < MAX_ROW; i++) {
		for (int j = 0; j < MAX_COL; j++) {
			if ( (i + j) % 2 == 0) {
				table[i][j] = new Cell(QPoint(i, j), image->getEvenCell(), this);
			} else {
				if (i <= 3) {
					table[i][j] = new Cell(QPoint(i, j), BLACK_PAWN, image->getOddCell(), this);
				} else if (i >= 6) {
					table[i][j] = new Cell(QPoint(i, j), WHITE_PAWN, image->getOddCell(), this);
				} else {
					table[i][j] = new Cell(QPoint(i, j), image->getOddCell(), this);
				}
			}
		}
	}
}

Cell * Board::getCellAt(const QPoint & posi) const {
	if (posi.x() < 0 || posi.x() >= MAX_COL || posi.y() < 0 || posi.y() >= MAX_ROW) {
		std::stringstream sstm;
		sstm << "Index out of bounds : (" << posi.x() << "," << posi.y() + ")";
		throw std::logic_error(sstm.str());
	}
	return table[posi.x()][posi.y()];
}

void Board::update() {
	for (int i = 0; i < MAX_ROW; i++) {
		for (int j = 0; j < MAX_COL; j++) {
			Cell *cell = table[i][j];
			QLabel *label = cell->getPicture();
			if (!cell->isEmpty()) {
				QLabel picture(label);
				picture.setText("toto");
				switch(cell->getPiece()) {
					case WHITE_PAWN : picture.setPixmap(image->getWhitePawn()); break;
					case BLACK_PAWN : picture.setPixmap(image->getBlackPawn()); break;
					case WHITE_QUEEN : picture.setPixmap(image->getWhiteQueen()); break;
					case BLACK_QUEEN : picture.setPixmap(image->getBlackQueen()); break;
				}
				cell->setPicture(&picture);
			}
			label->move((i%MAX_ROW) * CELL_SIZE, (j%MAX_COL) * CELL_SIZE);
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