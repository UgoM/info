#include <stdexcept>
#include "Cell.h"

Cell::Cell(const QPoint & pos, const CellType & type) {
	this->pos = pos;
	*this->type = type;
	empty = true;
}

Cell::Cell(const QPoint & pos, const Piece & piece, const CellType & type) {
	this->pos = pos;
	*this->piece = piece;
	*this->type = type;
	empty = false;
}

Piece & Cell::getPiece() const {
	if (empty) {
		throw std::logic_error("This cell does not contain a piece");
	}
	return *piece;
}

void Cell::setPiece(const Piece & piece) {
	*this->piece = piece;
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

void Cell::draw(QLabel & parent) const {
	parent.setPixmap(type->getPicture());
	if (!empty) {
		QLabel picture(&parent);
		picture.setPixmap(piece->getPicture());
	}
}