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
		
		const QPixmap & getOddCell() {
			return *oddCell;
		}
		const QPixmap & getEvenCell() {
			return *evenCell;
		}
		const QPixmap & getWhitePawn() {
			return *whitePawn;
		}
		const QPixmap & getBlackPawn() {
			return *blackPawn;
		}
		const QPixmap & getWhiteQueen() {
			return *whiteQueen;
		}
		const QPixmap & getBlackQueen() {
			return *blackQueen;
		}

};

#endif