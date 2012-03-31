#ifndef DEF_BOARDCONTROLLER
#define DEF_BOARDCONTROLLER

#include <QMap>
#include <QPoint>
#include "Constants.h"
#include "Direction.h"

class BoardController {

	QMap<QPoint, QList<QList<QPoint> > > * clickablePieces;
	
	public:
		void calculateClickablePieces(Piece** table, bool current);
		bool isPointClickable(const QPoint & point);
		Move controlMove(const QPoint & point, const QPoint & wanted);
		~BoardController();
		
	private:
		bool inBounds(int i, int j);
		Piece getOther(Piece piece);
		int findPawnCapture(int i, int j, Piece** table, Piece piece);
		int coordMaxPawnCapture(int i, int j, Piece** table, QList<QList<QPoint> > & coord, int length, int max_rafle, Piece piece);


};

#endif