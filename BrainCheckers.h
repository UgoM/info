#ifndef DEF_BRAINCHECKERS
#define DEF_BRAINCHECKERS

#include "Brain.h"

class BrainCheckers : public Brain {

	Q_OBJECT

	public slots:
		void handleMove(QByteArray boardEnd);

};

#endif