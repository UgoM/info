#include "Image.h"

Image::Image() {
	oddCell = new QPixmap("images/oddCell.png");
	evenCell = new QPixmap("images/evenCell.png");
	whitePawn = new QPixmap("images/whitePawn.png");
	blackPawn = new QPixmap("images/blackPawn.png");
	whiteQueen = new QPixmap("images/whiteQueen.png");
	blackQueen = new QPixmap("images/blackQueen.png");
}

Image::~Image() {
	delete oddCell;
	delete evenCell;
	delete whitePawn;
	delete blackPawn;
	delete whiteQueen;
	delete blackQueen;
}