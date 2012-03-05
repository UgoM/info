#include <stdexcept>
#include "Cell.h"

Cell::Cell(const QPoint & position, const QPixmap & pixmap, QWidget * b) {
	pos = position;
	picture = new QLabel(b);
	picture->setPixmap(pixmap);
	empty = true;
}

Cell::Cell(const QPoint & position, Piece p, const QPixmap & pixmap, QWidget * b) {
	pos = position;
	piece = p;
	picture = new QLabel(b);
	picture->setPixmap(pixmap);
	empty = false;
}

Piece Cell::getPiece() const {
	if (empty) {
		throw std::logic_error("This cell does not contain a piece");
	}
	return piece;
}

void Cell::setPiece(Piece p) {
	piece = p;
}

bool Cell::isEmpty() const {
	return empty;
}

void Cell::setEmpty(bool emp) {
	empty = emp;
}

QPoint Cell::getPosition() const {
	return pos;
}

QLabel* Cell::getPicture() const {
	return picture;
}

void Cell::setPicture(QLabel * label){
	picture = label;
}