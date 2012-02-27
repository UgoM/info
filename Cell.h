#ifndef DEF_CELL
#define DEF_CELL

#include <QPoint>
#include <QLabel>
#include "Piece.h"
#include "CellType.h"

class Cell {

	Piece * piece;
	bool empty;
	QPoint pos;
	CellType * type;
	
	public:
		Cell(const QPoint & pos, const CellType & type);
		Cell(const QPoint & pos, const Piece & piece, const CellType & type);
		
		Piece & getPiece() const;
		void setPiece(const Piece & piece);
		bool isEmpty() const;
		void setEmpty(bool empty);
		QPoint getPosition() const;
		void draw(QLabel & parent) const;

};

#endif