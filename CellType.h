#ifndef DEF_CELL_TYPE
#define DEF_CELL_TYPE

#include "Picture.h"

class CellType : public Picture {

	public:
		static const CellType EVEN;
		static const CellType ODD;
		
	private:
		CellType() {}

};
	
#endif
