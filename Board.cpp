#include "Board.h"

Board::Board() : QWidget() {
	setFixedSize(MAX_ROW * CELL_SIZE, MAX_COL * CELL_SIZE);
	image = new Image();
	table = new Piece*[MAX_COL];
	screen = new QLabel**[MAX_COL];
	for (int i = 0; i < MAX_COL ; i++) {
		table[i] = new Piece[MAX_ROW];
		screen[i] = new QLabel*[MAX_ROW];
	}
	for (int i = 0; i < MAX_COL; i++) {
		for (int j = 0; j < MAX_ROW; j++) {
			if ( (i + j) % 2 == 0) {
				table[i][j] = EMPTY;
				screen[i][j] = setLabelPicture(image->getEvenCell());
			} else {
				if (j <= 3) {
					table[i][j] = BLACK_PAWN;
					screen[i][j] = setLabelPicture(image->getBlackPawn());
				} else if (j >= 6) {
					table[i][j] = WHITE_PAWN;
					screen[i][j] = setLabelPicture(image->getWhitePawn());
				} else {
					table[i][j] = NONE;
					screen[i][j] = setLabelPicture(image->getOddCell());
				}
			}
			screen[i][j]->move(i*CELL_SIZE, j*CELL_SIZE);
		}
	}
	inPlay = NULL;
	current = true;
	controller = new BoardController();
	controller->calculateClickablePieces(table, current);
	whiteCount = blackCount = (MAX_COL / 2) * (MAX_ROW / 2 - 1);
}

QLabel * Board::setLabelPicture(QPixmap * pixmap) {
	QLabel * label = new QLabel(this);
	label->setPixmap(*pixmap);
	return label;
}

void Board::mousePressEvent(QMouseEvent *ev) {
    if (!inPlay) {
		inPlay = static_cast<QLabel*>(childAt(ev->pos()));
		position.setX((inPlay->pos()).x() / CELL_SIZE);
		position.setY((inPlay->pos()).y() / CELL_SIZE);
		start = position;
		if (!controller->isPointClickable(position)) {
			inPlay = NULL;
		}
	} else {
		QPoint wanted((ev->pos()).x() / CELL_SIZE, (ev->pos()).y() / CELL_SIZE);
		Move movePerformed = controller->controlMove(start, wanted);
		if (movePerformed != ILLEGAL) {
			int ni = wanted.x(), nj = wanted.y(), i = position.x(), j = position.y();
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
			if (movePerformed == SIMPLE) {
				inPlay = NULL;
				current = !current;
				controller->calculateClickablePieces(table, current);
			} else {
				table[i + (ni - i) / 2][j + (nj - j) / 2] = NONE;
				screen[i + (ni - i) / 2][j + (nj - j) / 2]->setPixmap(*image->getOddCell());
				if (current) {
					blackCount--;
				} else {
					whiteCount--;
				}
				if (movePerformed == SINGLE_CAPTURE) {
					inPlay = NULL;
					current = !current;
					controller->calculateClickablePieces(table, current);
				} else {
					position += QPoint(ni - i, nj - j);
				}
			}
		}
	}
}

Piece** Board::getPieceTable() {
	return table;
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
	delete inPlay;
}