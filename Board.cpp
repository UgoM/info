#include "Board.h"
#include <QLabel>

Board::Board() {
	table = new Cell**[MAX_COL];
	for(int i = 0; i < MAX_COL ; i++) {
		table[i] = new Cell*[MAX_ROW];
	}
	for(int i = 0; i < MAX_ROW; i++) {
		for(int j = 0; j < MAX_COL; j++) {
			if ( (i + j) % 2 == 0) {
				table[i][j] = new Cell(QPoint(i, j), CellType::EVEN);
			} else {
				if (i <= 3) {
					table[i][j] = new Cell(QPoint(i, j), Piece::BLACK_PAWN, CellType::ODD);
				} else if (i >= 6) {
					table[i][j] = new Cell(QPoint(i, j), Piece::WHITE_PAWN, CellType::ODD);
				} else {
					table[i][j] = new Cell(QPoint(i, j), CellType::ODD);
				}
			}
		}
	}
}

Cell * Board::getCellAt(const QPoint & pos) const {
	return table[pos.x()][pos.y()];
}

void Board::update() const {
	for(int i = 0; i < MAX_ROW; i++) {
		for(int j = 0; j < MAX_COL; j++) {
			QLabel label;
			table[i][j]->draw(label);
			label.move(i * 20, j * 20);
		}
	}
}