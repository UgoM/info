#ifndef DEF_CELL
#define DEF_CELL

#include <QPoint>
#include <QLabel>
#include "Piece.h"

class Cell {

	Piece piece;
	bool empty;
	QPoint pos;
	QLabel * picture;
	
	public:
		Cell(const QPoint & position, const QPixmap & pixmap, QWidget * b);
		Cell(const QPoint & position, Piece piece, const QPixmap & pixmap, QWidget * b);
		
		Piece getPiece() const;
		void setPiece(Piece piece);
		bool isEmpty() const;
		void setEmpty(bool empty);
		QPoint getPosition() const;
		QLabel * getPicture() const;
		void setPicture(QLabel * label);

};

#endif