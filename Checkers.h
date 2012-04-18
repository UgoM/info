#ifndef DEF_CHECKERS
#define DEF_CHECKERS

#include <QLabel>
#include <QMouseEvent>
#include "Game.h"
#include "Constants.h"
#include "Image.h"
#include "BoardController.h"

class Checkers : public Game {

	Q_OBJECT
	
	int** table;	//contient une matrice de l'enum Pion.
	QLabel*** screen;	//contient l'image à affichier dans chaque case dans un QLabel.
	Image* image;	//pointeur vers tous les fichiers images
	bool current;	//true si les blancs jouent, false sinon
	BoardController * controller;
	int whiteCount;	//nombre de pions blancs sur le damier
	int blackCount;	//nombre de pions noirs sur le damier
	
	bool moveInProgress;	//true si un mouvement est un cours, false sinon
	QPoint position;	//contient la position courant du pion en cours de mouvement
	
	static const char SEPARATOR;

	public:
		Checkers();
		~Checkers();
		
		void mousePressEvent(QMouseEvent * event);
		int** getPieceTable();
		void decodeBoard(QByteArray byteArray);
        void drawBoard();
		
		void processReceive(QByteArray block);
		void processClick();
		void processKey();

    public slots:
        void reSendData();

	private:
		void handleChangeTurn(int ni, int nj);
		QByteArray encodeBoard() const;
		void setPieceAt(int i, int j, int piece);
		void drawCell(int i, int j);
		void grayAllowedPositions();
		
	signals:
		void moveMade(QByteArray boardEnc);

};

#endif
