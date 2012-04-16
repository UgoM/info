#ifndef DEF_CHECKERS
#define DEF_CHECKERS

#include <QLabel>
#include <QMouseEvent>
#include "Game.h"
#include "Constants.h"
#include "Image.h"
#include "BoardController.h"

class Checkers : public Game {

	Q_OBJECT
	
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
		Checkers();
		~Checkers();
		
		void mousePressEvent(QMouseEvent * event);
		int** getPieceTable();
		void decodeBoard(QByteArray byteArray);
        void drawBoard();
		
		void processReceive(QByteArray data);
		void processClick();
		void processKey();
		
	private:
		QLabel * setLabelPicture(QPixmap * pixmap);
		void handleChangeTurn(int ni, int nj);
		QByteArray encodeBoard() const;
		
	signals:
		void moveMade(QByteArray boardEnc);

};

#endif
