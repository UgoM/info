#ifndef DEF_BOARDCONTROLLER
#define DEF_BOARDCONTROLLER

#include <QMap>
#include <QPoint>
#include "Constants.h"
#include "Direction.h"

class BoardController {

	/*map de tous les points cliquables : clé = le point en question, valeur = liste (plusieurs chemins peuvent mener à la rafle maximale)
	de la liste des points où ce pion doit aller, dans l'ordre du mouvement. */
	QMap<QPoint, QList<QList<QPoint> > > * clickablePieces;
	QPoint start;
	
	public:
		void calculateClickablePieces(int** table, bool current);
		void setStartPoint(const QPoint & point);
		bool isPointClickable(const QPoint & point);
		Move controlMove(const QPoint & wanted);
		~BoardController();

	private:
		bool inBounds(int i, int j);
		int getOther(int piece);
		int findPawnCapture(int i, int j, int** table, int piece);
		int coordMaxPawnCapture(int i, int j, int** table, QList<QList<QPoint> > & coord, int length, int max_rafle, int piece);
		int findQueenCapture(int i, int j, Direction dirPrec, int** table, int piece);
		int coordMaxQueenCapture(int i, int j, Direction dirPrec, int** table, QList<QList<QPoint> > & coord, int length, int max_rafle, int piece);

};

#endif