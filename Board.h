#ifndef DEF_BOARD
#define DEF_BOARD

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include "Constants.h"
#include "Piece.h"
#include "Image.h"

#include <QDebug>

class Board : public QWidget {

	Piece** table;
	QLabel*** screen;
	Image* image;
	bool current;	//true if white to play, false otherwhise
	
	QLabel * inPlay;
	
	public:
		Board();
		~Board();

		void mousePressEvent(QMouseEvent * event);
		
	private:
		QLabel * setLabelPicture(QPixmap * pixmap);
		bool inBounds(int i, int j);
		bool isPieceMoveable(int i, int j);

};

#endif