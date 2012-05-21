#include "src/games/checkers/Checkers.h"
#include "src/games/checkers/Constants.h"
#include "src/games/checkers/Image.h"
#include "src/games/checkers/BoardController.h"

/** Séparateur utilisé dans l'encodage du damier **/
const char Checkers::SEPARATOR = ';';

/** Construit un nouveau damier. **/
Checkers::Checkers() : Game() {
	setFixedSize(MAX_ROW * CELL_SIZE, MAX_COL * CELL_SIZE);
	controller = new BoardController();
	image = new Image();
	table = new int*[MAX_COL];
	screen = new QLabel**[MAX_COL];
	//initialisation du damier (pion et affichage)
	for (int i = 0; i < MAX_COL; i++) {
		table[i] = new int[MAX_ROW];
		screen[i] = new QLabel*[MAX_ROW];
	}
	for (int i = 0; i < MAX_COL; i++) {
		for (int j = 0; j < MAX_ROW; j++) {
			screen[i][j] = new QLabel(this);
			if ( (i + j) % 2 == 0) {
				setPieceAt(i, j, EMPTY);
			} else {
				if (j <= 3) {
					setPieceAt(i, j, BLACK_PAWN);
				} else if (j >= 6) {
					setPieceAt(i, j, WHITE_PAWN);
				} else {
					setPieceAt(i, j, NONE);
				}
			}
			screen[i][j]->move(i*CELL_SIZE, j*CELL_SIZE);
		}
	}
	moveInProgress = false;
	current = true;	//les blancs commencent le jeu
	controller->calculateClickablePieces(table, current);
	whiteCount = blackCount = (MAX_COL / 2) * (MAX_ROW / 2 - 1);
	show();
}

/** Set the given piece at column i et row j. Update the screen accordingly. **/
void Checkers::setPieceAt(int i, int j, int piece) {
	if (controller->inBounds(i, j)) {
		table[i][j] = piece;
		drawCell(i, j);
	}
}

/** Set the correct display image corresponding to the piece at the given column i and row j. **/
void Checkers::drawCell(int i, int j) {
	switch(table[i][j]) {
		case WHITE_PAWN : screen[i][j]->setPixmap(image->getWhitePawn()); break;
		case BLACK_PAWN : screen[i][j]->setPixmap(image->getBlackPawn()); break;
		case WHITE_QUEEN : screen[i][j]->setPixmap(image->getWhiteQueen()); break;
		case BLACK_QUEEN : screen[i][j]->setPixmap(image->getBlackQueen()); break;
		case NONE : screen[i][j]->setPixmap(image->getOddCell()); break;
		case EMPTY : screen[i][j]->setPixmap(image->getEvenCell()); break;
		case GRAY : screen[i][j]->setPixmap(image->getGrayCell()); break;
		default : break;
	}
}

/** Draw the whole board. **/
void Checkers::drawBoard() {
	for (int j = 0; j < MAX_ROW; j++) {
		for (int i = 0; i < MAX_COL; i++) {
			drawCell(i, j);
		}
	}
}

/** Handle mouse press event. **/
void Checkers::mousePressEvent(QMouseEvent *ev) {
	if (clientType == ClientType::OBSERVER) {
		return;	//si c'est un observeur qui clique, on le coupe
	}
	if (!moveInProgress) {	//aucun mouvement en cours
		QLabel * inPlay = static_cast<QLabel*>(childAt(ev->pos()));	//récupération de l'image cliquée
		position.setX((inPlay->pos()).x() / CELL_SIZE);
		position.setY((inPlay->pos()).y() / CELL_SIZE);
		controller->setStartPoint(position);
		if (controller->isPointClickable(position)) {	//on vérifie si le pion est bien cliquable
			moveInProgress = true;
			grayAllowedPositions();
		} else {
			int pion = table[position.x()][position.y()];
			if ((current && (pion == WHITE_PAWN || pion == WHITE_QUEEN)) ||
				(!current && (pion == BLACK_PAWN|| pion == BLACK_QUEEN))) {
				//le pion cliqué n'est pas le pion qui prend le plus de pion adverse
				QMessageBox::information(this, tr("Rafle maximale"), tr("Vous devez prendre le plus de pions adverses !"));
			}
		}
		// moveInProgress = controller->isPointClickable(position);
	} else {	//mouvement en cours
		QPoint wanted((ev->pos()).x() / CELL_SIZE, (ev->pos()).y() / CELL_SIZE);	//position voulue
		Move movePerformed = controller->controlMove(wanted);	//cette position est-elle acceptable ?
		if (movePerformed != ILLEGAL) {	//oui, elle est acceptable (sinon, il ne se passe rien)
			int ni = wanted.x(), nj = wanted.y(), i = position.x(), j = position.y();
			//transfert du pion et mise à jour de l'affichage
			setPieceAt(ni, nj, table[i][j]);
			setPieceAt(i, j, NONE);
			if (movePerformed == SIMPLE) {	//pas de prise
				handleChangeTurn(ni, nj);
			} else {	//une prise
				/*afin de s'affranchir des subtilités de la dame, tous les pions situés sur la diagonale
				reliant la position finale à la position initiale sont enlevés. En fait, il ne peut y en avoir qu'un seul (mais on se pas à priori où)*/
				for (int k = 1; k < qAbs(ni - i); k++) {
					int deltaY = nj-j > 0 ? k : -k;
					int deltaX = ni-i > 0 ? k : -k;
					if (table[i + deltaX][j + deltaY] != NONE) {
						setPieceAt(i + deltaX, j + deltaY, NONE);
					}
				}
				//mise à jour du nombre de pion
				if (current) {
					blackCount--;
				} else {
					whiteCount--;
				}
				if (movePerformed == SINGLE_CAPTURE) {	//prise simple (au sens où il ne peut pas y en avoir une autre et le mouvement est terminé)
					handleChangeTurn(ni, nj);
				} else {	//il reste des pions à prendre
					position = QPoint(ni, nj);
					grayAllowedPositions();
				}
			}
			emit moveMade(encodeBoard());	//on prévient qu'un coup est joué à tous ceux qui écoutent
		}
	}
}

/** Reset gray cell on screen back to NONE. **/
void Checkers::resetGrayPositions() {
	//remise à zéro des aides de localisation
	for (int i = 0; i < MAX_COL; i++) {
		for (int j = 0; j < MAX_ROW; j++) {
			if (table[i][j] == GRAY) {
				setPieceAt(i, j, NONE);
			}
		}
	}
}

/** Gray on the screen all positions that are allowed for the current move. **/
void Checkers::grayAllowedPositions() {
	//les cases accepatbles sont grisées
	foreach (QList<QPoint> list, controller->getAllowedPositions()) {
		for (int i = 0; i < list.size(); i++) {
			QPoint point = list[i];
			setPieceAt(point.x(), point.y(), GRAY);
		}
	}
}

/** Handle the change in turn. **/
void Checkers::handleChangeTurn(int ni, int nj) {
	if (whiteCount == 0 || blackCount == 0) {
		return endGame();
	}
	moveInProgress = false;
	resetGrayPositions();
	//fait la promotion des pions en dames, s'il y en a
	if (current && nj == 0) {
		setPieceAt(ni, nj, WHITE_QUEEN);
	} else if (!current && nj == MAX_ROW - 1) {
		setPieceAt(ni, nj, BLACK_QUEEN);
	}
	current = !current;	//le tour change
	controller->calculateClickablePieces(table, current);	//il faut recalculer les pions cliquables en conséquence
}

void Checkers::endGame() {
	QString endText("Les ");
	endText.append(whiteCount == 0 ? "noirs " : "blancs ");
	endText.append("ont gagné la partie !");
	QMessageBox endMessageBox(QMessageBox::Information, tr("Fin de la partie !"), endText);
	endMessageBox.setWindowModality(Qt::WindowModal);
	endMessageBox.addButton(tr("Fermer la partie"), QMessageBox::AcceptRole);
	endMessageBox.exec();
	hide();
}

/** Encode the board into a QByteArray to prepare transmission. **/
QByteArray Checkers::encodeBoard() const {
	QByteArray byteArray;
	for (int j = 0; j < MAX_ROW; j++) {
		for (int i = 0; i < MAX_COL; i++) {
			byteArray.append(QString::number(table[i][j]) + SEPARATOR);
		}
	}
	byteArray.append(QString::number(current));
	return byteArray;
}

/** Decode the board according to the given QByteArray. **/
void Checkers::decodeBoard(QByteArray byteArray) {
	QList<QByteArray> tokens = byteArray.split(SEPARATOR);
	for (int k = 0; k < tokens.size() - 1; k++) {
		table[k % MAX_COL][k / MAX_COL] = tokens[k].toInt();
	}
	current = tokens.last().toInt();
}

/** Return the matrix of pieces of the board. **/
int** Checkers::getPieceTable() {
	return table;
}

/** Delete this checkers instance. **/
Checkers::~Checkers() {
	delete image;
	for (int i = 0; i < MAX_ROW; i++) {
		for (int j = 0; j < MAX_COL; j++) {
			delete screen[i][j];
		}
		delete[] table[i];
		delete[] screen[i];
	}
	delete[] table;
	delete[] screen;
}


void Checkers::processReceive(QByteArray block) {
	decodeBoard(block);
	drawBoard();
}

void Checkers::processClick() {

}

void Checkers::processKey() {

}

void Checkers::reSendData() {
	emit moveMade(encodeBoard());
}
