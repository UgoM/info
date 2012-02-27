#ifndef DEF_PIECE
#define DEF_PIECE

#include "Picture.h"

class Piece : public Picture {

	public:
		static const Piece WHITE_PAWN;
		static const Piece BLACK_PAWN;
		static const Piece WHITE_QUEEN;
		static const Piece BLACK_QUEEN;
		//static const Piece NONE; <- utile ?
		
	private:
		Piece() {}

};

#endif