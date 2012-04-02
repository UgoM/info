#ifndef DEF_BOARDCONTROLLER
#define DEF_BOARDCONTROLLER

#include <QMap>
#include <QPoint>
#include "Constants.h"
#include "Direction.h"

class BoardController {

	QMap<QPoint, QList<QList<QPoint> > > * clickablePieces;
	bool canQueenCapture;
	bool queenMovementInProgress;
	
	public:
		void calculateClickablePieces(Piece** table, bool current);
		bool isPointClickable(const QPoint & point);
		Move controlMove(Piece** table, const QPoint & point, const QPoint & wanted);
		~BoardController();

	private:
		bool inBounds(int i, int j);
		Piece getOther(Piece piece);
		int findPawnCapture(int i, int j, Piece** table, Piece piece);
		int coordMaxPawnCapture(int i, int j, Piece** table, QList<QList<QPoint> > & coord, int length, int max_rafle, Piece piece);
		int findQueenCapture(int i, int j, Direction dirPrec, Piece** table, Piece piece);
		int coordMaxQueenCapture(int i, int j, Direction dirPrec, Piece** table, QList<QList<QPoint> > & coord, int length, int max_rafle, Piece piece);

};

#endif