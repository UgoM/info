#ifndef DEF_BOARD
#define DEF_BOARD

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include "Constants.h"
#include "Image.h"
#include "BoardController.h"

class Board : public QWidget {

	int** table;
	QLabel*** screen;
	Image* image;
	bool current;	//true if white to play, false otherwhise
	BoardController * controller;
	int whiteCount;
	int blackCount;
	
	QLabel * inPlay;
	QPoint position;
	QPoint start;
	
	static const char SEPARATOR;
	
	public:
		Board();
		~Board();

		void mousePressEvent(QMouseEvent * event);
		int** getPieceTable();
		QByteArray encodeBoard() const;
		void decodeBoard(QByteArray byteArray);
		
	private:
		QLabel * setLabelPicture(QPixmap * pixmap);
		void handleChangeTurn(int ni, int nj);

};

#endif
