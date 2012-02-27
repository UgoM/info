#ifndef DEF_BOARD
#define DEF_BOARD

#include <QWidget>
#include <QPoint>
#include "Constants.h"
#include "Cell.h"

class Board : public QWidget {

	Cell*** table;
	
	public:
		Board();
		
		Cell * getCellAt(const QPoint & pos) const;
		void update() const;

};

#endif