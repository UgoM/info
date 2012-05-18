#include "src/games/checkers/Image.h"

Image::Image() {
	oddCell = new QPixmap("images/checkers/oddCell.png");
	evenCell = new QPixmap("images/checkers/evenCell.png");
	grayCell = new QPixmap("images/checkers/grayCell.png");
	whitePawn = new QPixmap("images/checkers/whitePawn.png");
	blackPawn = new QPixmap("images/checkers/blackPawn.png");
	whiteQueen = new QPixmap("images/checkers/whiteQueen.png");
	blackQueen = new QPixmap("images/checkers/blackQueen.png");
}

Image::~Image() {
	delete oddCell;
	delete evenCell;
	delete grayCell;
	delete whitePawn;
	delete blackPawn;
	delete whiteQueen;
	delete blackQueen;
}
