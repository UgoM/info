#include "Image.h"

Image::Image() {
	oddCell = new QPixmap("oddCell.png");
	evenCell = new QPixmap("evenCell.png");
	whitePawn = new QPixmap("whitePawn.png");
	blackPawn = new QPixmap("blackPawn.png");
	//TODO:
	whiteQueen = new QPixmap("whitePawn.png");
	blackQueen = new QPixmap("blackPawn.png");
}

Image::~Image() {
	delete oddCell;
	delete evenCell;
	delete whitePawn;
	delete blackPawn;
	delete whiteQueen;
	delete blackQueen;
}