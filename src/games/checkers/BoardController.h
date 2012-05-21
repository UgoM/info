#ifndef DEF_BOARDCONTROLLER
#define DEF_BOARDCONTROLLER

#include <QMap>
#include <QPoint>
#include <QList>

#include "src/games/checkers/Constants.h"
class Direction;

class BoardController {

	/*map de tous les points cliquables : cl� = le point en question, valeur = liste (plusieurs chemins peuvent mener � la rafle maximale)
	de la liste des points o� ce pion doit aller, dans l'ordre du mouvement. */
	QMap<QPoint, QList<QList<QPoint> > > * clickablePieces;
	QPoint start;	//point de d�part du mouvement en cours
	
	public:
		void calculateClickablePieces(int** table, bool current);
		void setStartPoint(const QPoint & point);
		bool isPointClickable(const QPoint & point) const;
		bool inBounds(int i, int j) const;
		QList<QList<QPoint> > getAllowedPositions() const;
		Move controlMove(const QPoint & wanted);
		~BoardController();

	private:
		int getOther(int piece) const;
		int findPawnCapture(int i, int j, int** table, int piece) const;
		int coordMaxPawnCapture(int i, int j, int** table, QList<QList<QPoint> > & coord, int length, int max_rafle, int piece) const;
		int findQueenCapture(int i, int j, Direction dirPrec, int** table, int piece) const;
		int coordMaxQueenCapture(int i, int j, Direction dirPrec, int** table, QList<QList<QPoint> > & coord, int length, int max_rafle, int piece) const;

};

#endif
