#ifndef DEF_BOARD
#define DEF_BOARD

#include <QWidget>
#include <QPoint>
#include "Constants.h"
#include "Cell.h"
#include "Image.h"

class Board : public QWidget {

	Cell*** table;
	Image* image;
	
	public:
		Board();
		~Board();
		
		Cell * getCellAt(const QPoint & pos) const;
		void update();

};

#endif