#ifndef DEF_CELL
#define DEF_CELL

#include <QPoint>
#include <QLabel>
#include "Piece.h"

class Cell {

	Piece piece;
	bool empty;
	QPoint pos;
	QPixmap picture;
	
	public:
		Cell(const QPoint & pos, const QPixmap & picture);
		Cell(const QPoint & pos, Piece piece, const QPixmap & picture);
		
		Piece getPiece() const;
		void setPiece(Piece piece);
		bool isEmpty() const;
		void setEmpty(bool empty);
		QPoint getPosition() const;
		QPixmap getPicture() const;

};

#endif