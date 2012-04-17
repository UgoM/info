#include "Checkers.h"

const char Checkers::SEPARATOR = ';';

Checkers::Checkers() : Game() {
	setFixedSize(MAX_ROW * CELL_SIZE, MAX_COL * CELL_SIZE);
	image = new Image();
	table = new int*[MAX_COL];
	screen = new QLabel**[MAX_COL];
	for (int i = 0; i < MAX_COL; i++) {
		table[i] = new int[MAX_ROW];
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
	show();
}

QLabel * Checkers::setLabelPicture(QPixmap * pixmap) {
	QLabel * label = new QLabel(this);
	label->setPixmap(*pixmap);
	return label;
}

void Checkers::drawBoard()
{
	for (int j = 0; j < MAX_ROW; j++) {
		for (int i = 0; i < MAX_COL; i++) {
			switch(table[i][j]) {
				case WHITE_PAWN : screen[i][j]->setPixmap(*image->getWhitePawn()); break;
				case BLACK_PAWN : screen[i][j]->setPixmap(*image->getBlackPawn()); break;
				case WHITE_QUEEN : screen[i][j]->setPixmap(*image->getWhiteQueen()); break;
				case BLACK_QUEEN : screen[i][j]->setPixmap(*image->getBlackQueen()); break;
                case NONE : screen[i][j]->setPixmap(*image->getOddCell());
				default : break;
			}
        }
    }
            
}

void Checkers::mousePressEvent(QMouseEvent *ev) {
    if (clientType == ClientType::OBSERVER) {
		return;
	}
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
		Move movePerformed = controller->controlMove(table, start, wanted);
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
				handleChangeTurn(ni, nj);
			} else {
				for (int k = 1; k < qAbs(ni - i); k++) {
					int deltaY = nj-j > 0 ? k : -k;
					int deltaX = ni-i > 0 ? k : -k;
					if (table[i+deltaX][j+deltaY] != NONE) {
						table[i+deltaX][j+deltaY] = NONE;
						screen[i+deltaX][j+deltaY]->setPixmap(*image->getOddCell());
					}
				}
				if (current) {
					blackCount--;
				} else {
					whiteCount--;
				}
				if (movePerformed == SINGLE_CAPTURE) {
					handleChangeTurn(ni, nj);
				} else {
					position = QPoint(ni, nj);
				}
			}
			emit moveMade(encodeBoard());
		}
	}
}

void Checkers::handleChangeTurn(int ni, int nj) {
	inPlay = NULL;
	if (current && nj == 0) {
		table[ni][nj] = WHITE_QUEEN;
		screen[ni][nj]->setPixmap(*image->getWhiteQueen());
	} else if (!current && nj == MAX_ROW - 1) {
		table[ni][nj] = BLACK_QUEEN;
		screen[ni][nj]->setPixmap(*image->getBlackQueen());
	}
	current = !current;
	controller->calculateClickablePieces(table, current);
}

QByteArray Checkers::encodeBoard() const {
	QByteArray byteArray;
	for (int j = 0; j < MAX_ROW; j++) {
		for (int i = 0; i < MAX_COL; i++) {
			byteArray.append(QString::number(table[i][j]) + SEPARATOR);
		}
	}
	byteArray.append(QString::number(current));
	return byteArray;
}

void Checkers::decodeBoard(QByteArray byteArray) {
	QList<QByteArray> tokens = byteArray.split(SEPARATOR);
	for (int k = 0; k < tokens.size() - 1; k++) {
        table[k % MAX_COL][k / MAX_COL] = tokens[k].toInt();
	}
	current = tokens.last().toInt();
}

int** Checkers::getPieceTable() {
	return table;
}

Checkers::~Checkers() {
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


void Checkers::processReceive(QByteArray block)
{
    decodeBoard(block);
    drawBoard();
}

void Checkers::processClick() {

}

void Checkers::processKey() {

}
