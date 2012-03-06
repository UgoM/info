#ifndef DEF_IMAGE
#define DEF_IMAGE

#include <QPixmap>

class Image {

	QPixmap * oddCell;
	QPixmap * evenCell;
	
	QPixmap * whitePawn;
	QPixmap * blackPawn;
	QPixmap * whiteQueen;
	QPixmap * blackQueen;
	
	public:
		Image();
		~Image();
		QPixmap * getOddCell() {
			return oddCell;
		}
		QPixmap * getEvenCell() {
			return evenCell;
		}
		QPixmap * getWhitePawn() {
			return whitePawn;
		}
		QPixmap * getBlackPawn() {
			return blackPawn;
		}
		QPixmap * getWhiteQueen() {
			return whiteQueen;
		}
		QPixmap * getBlackQueen() {
			return blackQueen;
		}

};

#endif