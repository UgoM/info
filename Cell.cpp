#include <stdexcept>
#include "Cell.h"

Cell::Cell(const QPoint & pos, const QPixmap & picture) {
	this->pos = pos;
	this->picture = picture;
	empty = true;
}

Cell::Cell(const QPoint & pos, Piece piece, const QPixmap & picture) {
	this->pos = pos;
	this->piece = piece;
	this->picture = picture;
	empty = false;
}

Piece Cell::getPiece() const {
	if (empty) {
		throw std::logic_error("This cell does not contain a piece");
	}
	return piece;
}

void Cell::setPiece(Piece piece) {
	this->piece = piece;
}

bool Cell::isEmpty() const {
	return empty;
}

void Cell::setEmpty(bool empty) {
	this->empty = empty;
}

QPoint Cell::getPosition() const {
	return pos;
}

QPixmap Cell::getPicture() const {
	return picture;
}