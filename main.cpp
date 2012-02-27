#include <QApplication>
#include <QtCore>

#include "Server.h"
#include "TcpClient.h"

#include <QPushButton>

#include "Board.h"
 
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
	
	CellType even = CellType::EVEN;
	even.setPicture(QPixmap("evenCell.png", "png"));
	
	CellType odd = CellType::ODD;
	odd.setPicture(QPixmap("oddCell.png", "png"));

    Board * board = new Board();
	board->update();
 
    return app.exec();
}
