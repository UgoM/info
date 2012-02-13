#include "Board.h"

Board::Board() {
	// table = new Cell**[MAX_COL];
	// table = new Cell*[CELL_COUNT];
	// for(int i = 0; i < CELL_COUNT; i++) {
		// if (i % 2 == 0) {
			// table[i] = new Cell(QPoint(i / MAX_COL, i % MAX_COL));
		// } else {
			// table[i] = new Cell(QPoint(i / MAX_COL, i % MAX_COL), new Piece());
		// }
	// }
}

Cell * Board::getCellAt(const QPoint & pos) const {
	//return table[pos.x() * MAX_COL + pos.y()];
	return NULL;
}