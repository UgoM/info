#include <stdexcept>
#include "Cell.h"

Cell::Cell(const QPoint & pos, const CellType & type) {
	this->pos = pos;
	*this->type = type;
	empty = true;
}

Cell::Cell(const QPoint & pos, Piece * piece, const CellType & type) {
	this->pos = pos;
	this->piece = piece;
	*this->type = type;
	empty = false;
}

Piece * Cell::getPiece() const {
	if (empty) {
		throw std::logic_error("This cell does not contain a piece");
	}
	return piece;
}

bool Cell::isEmpty() const {
	return empty;
}

QPoint Cell::getPosition() const {
	return pos;
}