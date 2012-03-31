#ifndef DEF_CONSTANTS
#define DEF_CONSTANTS

#define MAX_ROW 10
#define MAX_COL 10
#define CELL_COUNT MAX_ROW*MAX_COL
#define CELL_SIZE 60

enum Move {
	ILLEGAL,
	SIMPLE,	//mouvement simple (pas de prise)
	SINGLE_CAPTURE, //prise d'un seul pion
	MANY_CAPTURE	//prise de plusieurs pions
};

enum Piece {
	WHITE_PAWN,
	BLACK_PAWN,
	WHITE_QUEEN,
	BLACK_QUEEN,
	NONE, //n'a pas de pièces
	EMPTY //n'en aura jamais (case paire)
};

#endif