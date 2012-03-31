#ifndef DEF_BOARD
#define DEF_BOARD

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include "Constants.h"
#include "Piece.h"
#include "Image.h"
#include "BoardController.h"

class Board : public QWidget {

	Piece** table;
	QLabel*** screen;
	Image* image;
	bool current;	//true if white to play, false otherwhise
	BoardController * controller;
	int whiteCount;
	int blackCount;
	
	QLabel * inPlay;
	QPoint position;
	QPoint start;
	
	public:
		Board();
		~Board();

		void mousePressEvent(QMouseEvent * event);
		Piece** getPieceTable();
		
	private:
		QLabel * setLabelPicture(QPixmap * pixmap);

};

#endif