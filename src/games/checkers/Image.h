#ifndef DEF_IMAGE
#define DEF_IMAGE

#include <QPixmap>

class Image {

	QPixmap * oddCell;
	QPixmap * evenCell;
	QPixmap * grayCell;
	
	QPixmap * whitePawn;
	QPixmap * blackPawn;
	QPixmap * whiteQueen;
	QPixmap * blackQueen;
	
	public:
		Image();
		~Image();
		QPixmap getOddCell() {
			return *oddCell;
		}
		QPixmap getEvenCell() {
			return *evenCell;
		}
		QPixmap getGrayCell() {
			return *grayCell;
		}
		QPixmap getWhitePawn() {
			return *whitePawn;
		}
		QPixmap getBlackPawn() {
			return *blackPawn;
		}
		QPixmap getWhiteQueen() {
			return *whiteQueen;
		}
		QPixmap getBlackQueen() {
			return *blackQueen;
		}

};

#endif