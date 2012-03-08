#include <stdexcept>
#include <sstream>
#include "Board.h"

Board::Board() : QWidget() {
	setFixedSize(MAX_ROW * CELL_SIZE, MAX_COL * CELL_SIZE);
	setAcceptDrops(true);
	image = new Image();
	table = new Piece*[MAX_COL];
	screen = new QLabel**[MAX_COL];
	for (int i = 0; i < MAX_COL ; i++) {
		table[i] = new Piece[MAX_ROW];
		screen[i] = new QLabel*[MAX_ROW];
	}
	for (int i = 0; i < MAX_ROW; i++) {
		for (int j = 0; j < MAX_COL; j++) {
			if ( (i + j) % 2 == 0) {
				table[i][j] = EMPTY;
				screen[i][j] = setLabelPicture(image->getEvenCell());
			} else {
				if (i <= 3) {
					table[i][j] = BLACK_PAWN;
					screen[i][j] = setLabelPicture(image->getBlackPawn());
				} else if (i >= 6) {
					table[i][j] = WHITE_PAWN;
					screen[i][j] = setLabelPicture(image->getWhitePawn());
				} else {
					table[i][j] = NONE;
					screen[i][j] = setLabelPicture(image->getOddCell());
				}
			}
			screen[i][j]->move(j*CELL_SIZE, i*CELL_SIZE);
		}
	}
	inPlay = NULL;
	current = true;
}

QLabel * Board::setLabelPicture(QPixmap * pixmap) {
	QLabel * label = new QLabel(this);
	label->setPixmap(*pixmap);
	return label;
}

void Board::mousePressEvent(QMouseEvent *ev) {
    if (!inPlay) {
		inPlay = static_cast<QLabel*>(childAt(ev->pos()));
		//TODO: contrôler que l'on clique un bon pion
		QPoint pt = inPlay->pos();
		int i = pt.y()/CELL_SIZE, j = pt.x()/CELL_SIZE;
		if (!isPieceMoveable(i, j)) {
			inPlay = NULL;
		}
	} else {
		//TODO: contrôler que le mouvement est possible
		QPoint pt = inPlay->pos();
		int i = pt.y()/CELL_SIZE, j = pt.x()/CELL_SIZE;
		int ni = ev->pos().y()/CELL_SIZE, nj = ev->pos().x()/CELL_SIZE;
		table[ni][nj] = table[i][j];
		switch(table[i][j]) {
			case WHITE_PAWN : screen[ni][nj]->setPixmap(*image->getWhitePawn()); break;
			case BLACK_PAWN : screen[ni][nj]->setPixmap(*image->getBlackPawn()); break;
			case WHITE_QUEEN : screen[ni][nj]->setPixmap(*image->getWhiteQueen()); break;
			case BLACK_QUEEN : screen[ni][nj]->setPixmap(*image->getBlackQueen()); break;
			default : break;
		}
		table[i][j] = NONE;
		screen[i][j]->setPixmap(*image->getOddCell());
		inPlay = NULL;
		current = !current;
	}
}

// QMap<Piece, int> calculatePositionCount(...) {
	// calculer pour chaque pion le nombre de case sur lesquels il peut aller.
// }

bool Board::isPieceMoveable(int i, int j) {
	if(table[i][j] == NONE || table[i][j] == EMPTY ||
		(current && table[i][j] == BLACK_PAWN) || (current && table[i][j] == BLACK_QUEEN) ||
		(!current && table[i][j] == WHITE_PAWN) || (!current && table[i][j] == WHITE_QUEEN)) {
		//clic sur un pion blanc alors que les noirs jouent et inversement
		return false;
	}
	if (current && table[i][j] == WHITE_PAWN) {
		//clic sur un pion blanc qui n'a pas de pion devant
		if (inBounds(i-1,j-1) && table[i-1][j-1] == NONE) {
			return true;
		} else if (inBounds(i-1,j+1) && table[i-1][j+1] == NONE) {
			return true;
		} else {
			return false;
		}
	}
	if (!current && table[i][j] == BLACK_PAWN) {
		//clic sur un pion noir qui n'a pas de pion devant
		if (inBounds(i+1,j-1) && table[i+1][j-1] == NONE) {
			return true;
		} else if (inBounds(i+1,j+1) && table[i+1][j+1] == NONE) {
			return true;
		} else {
			return false;
		}
	}
	return false;
}

bool Board::inBounds(int i, int j) {
	return i>=0 && i<MAX_ROW && j>=0 && j<MAX_COL;
}

Board::~Board() {
	delete image;
	for (int i = 0; i < MAX_ROW; i++) {
		for (int j = 0; j < MAX_COL; j++) {
			delete screen[i][j];
		}
		delete[] table[i];
		delete[] screen[i];
	}
	delete[] table;
	delete[] screen;
}