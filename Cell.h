#ifndef DEF_CELL
#define DEF_CELL

#include <QPoint>
#include "Piece.h"
#include "CellType.h"

class Cell {

	Piece * piece;
	bool empty;
	QPoint pos;
	CellType * type;
	
	public:
		Cell(const QPoint & pos, const CellType & type);
		Cell(const QPoint & pos, Piece * piece, const CellType & type);
		
		Piece * getPiece() const;
		bool isEmpty() const;
		QPoint getPosition() const;
		void draw() const;

};

#endif