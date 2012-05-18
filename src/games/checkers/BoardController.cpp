#include "src/games/checkers/BoardController.h"

/** Implement the < operator for two QPoint. **/
bool operator<(const QPoint & lhs, const QPoint & rhs) {
	return lhs.x() < rhs.x() || (lhs.x() == rhs.x() && lhs.y() < rhs.y());
}

/** Return true if the given point is a clickable, false otherwise. **/
bool BoardController::isPointClickable(const QPoint & point) const {
	return clickablePieces->contains(point);
}

/** Return all of the allowed positions for the current move. **/
QList<QList<QPoint> > BoardController::getAllowedPositions() const {
	return clickablePieces->value(start);
}

/** Set the starting point of the current move. **/
void BoardController::setStartPoint(const QPoint & point) {
	start = point;
}

/** Control the move by the new wanted position of the piece in movement. Return a value of the enum Move accordingly. **/
Move BoardController::controlMove(const QPoint & wanted) {
	QList<QList<QPoint> > newlyAllowedPositions;	//va contenir les nouvelles positions auxquelles que le pion en cours de mouvement aura accès.
	foreach (QList<QPoint> list, getAllowedPositions()) {
		if (list.startsWith(wanted)) {	//est bien voulue la première case légale, les nouvelles positions sont toutes sauf celle-là
			list.removeFirst();
			newlyAllowedPositions << list;
		} else if (list.startsWith(QPoint(-1, -1)) && list.contains(wanted)) {
			//une dame est en mouvement, c'est sa dernière prise possible et le point voulue acceptable
			return SINGLE_CAPTURE;
		} else if (list.startsWith(QPoint(-2, -2)) && list.contains(wanted)) {
			//une dame est en mouvement mais elle ne peut pas prendre
			return SIMPLE;
		}
	}
	if (!newlyAllowedPositions.isEmpty()) {
		if (qAbs(wanted.x() - start.x()) > 1 || qAbs(wanted.y() - start.y()) > 1) {
			if (newlyAllowedPositions.size() == 1 && newlyAllowedPositions[0].isEmpty()) {
				return SINGLE_CAPTURE;
			} else {
				clickablePieces->insert(start, newlyAllowedPositions);
				return MANY_CAPTURE;
			}
		} else {
			return SIMPLE;
		}
	} else {
		return ILLEGAL;
	}
}

/** For the given board which current player is determined by 'current', calculate all pieces that are clickable.
Main rules are :
- pieces can take backwards.
- it is mandatory to play the piece that can take the maximum number of adversary pieces (can be 0).
- if there are more than one piece or the movement has several possibilities leading to this maximum number, a choice is given to the player.
- during a taking process, the player has to go through all the intermediate cell. **/
void BoardController::calculateClickablePieces(int** table, bool current) {
	int maxRafle = 0;
	int targetPiece = current ? BLACK_PAWN : WHITE_PAWN;
	int currentPiece = current ? WHITE_PAWN : BLACK_PAWN;
	int currentOtherPiece = current ? WHITE_QUEEN : BLACK_QUEEN;
	QList<QPoint> possibleList;
	clickablePieces = new QMap<QPoint, QList<QList<QPoint> > >();
	for (int i = 0; i < MAX_COL; i++) {
		for (int j = 0; j < MAX_ROW; j++) {
			int rafle = 0;
			if (table[i][j] == currentPiece) {
				rafle = findPawnCapture(i, j, table, targetPiece);	//combien de pions peut prendre ce pion.
			} else if (table[i][j] == currentOtherPiece) {
				rafle = findQueenCapture(i, j, Direction::UNDEFINED, table, targetPiece);	//combien de pions peut prendre cette dame.
			}
			if (table[i][j] == currentPiece || table[i][j] == currentOtherPiece) {
				if (rafle > maxRafle) {	//si un pion peut prendre plus de pion qu'un autre, l'autre est oublié et seul lui compte.
					possibleList.clear();
					maxRafle = rafle;
					possibleList << QPoint(i, j);
				} else if (rafle == maxRafle) {	//si un pion peut prendre autant de pion qu'un autre (dont 0), on l'ajoute à la liste des possibles.
					possibleList << QPoint(i, j);
				}
			}
		}
	}
	//pour tous les pions possibles, il faut calculer les coordonnées des mouvements respectifs.
	foreach (QPoint point, possibleList) {
		QList<QList<QPoint> > coord;
		if (maxRafle > 0) {	//les mouvements sont des rafles, appel des méthodes spécifiques aux pions et dames pour les coordonnées.
			if (table[point.x()][point.y()] == currentPiece) {
				coordMaxPawnCapture(point.x(), point.y(), table, coord, 0, maxRafle, targetPiece);
			} else {
				coordMaxQueenCapture(point.x(), point.y(), Direction::UNDEFINED, table, coord, 0, maxRafle, targetPiece);
			}
			clickablePieces->insert(point, coord);
		} else {	//les mouvement sont des déplacements simples.
			if (table[point.x()][point.y()] == currentPiece) {	//pion : acceptable = en jeu et une des deux cases voisines vide
				int deltaY = current ? -1 : 1;
				for (int i = -1; i <= 1; i += 2) {
					if (inBounds(point.x() + i, point.y() + deltaY) && table[point.x() + i][point.y() + deltaY] == NONE) {
						QList<QPoint> list;
						list << QPoint(point.x() + i, point.y() + deltaY);
						coord << list;
					}
				}
			} else {	//dame : acceptable = en jeu et toutes les cases des différents diagonales vides
				QList<Direction> dirList = Direction::values();
				for (int p = 0; p < dirList.size(); p++) {
					int l = 1;
					Direction dir = dirList[p];
					QList<QPoint> list;
					while (inBounds(point.x() + l*dir.i(), point.y() + l*dir.j()) && table[point.x() + l*dir.i()][point.y() + l*dir.j()] == NONE) {
						list << QPoint(point.x() + l*dir.i(), point.y() + l*dir.j());
						l++;
					}
					if(!list.isEmpty()) {
						list.prepend(QPoint(-2, -2));
						coord << list;
					}
				}
			}
			if (!coord.isEmpty()) {
				clickablePieces->insert(point, coord);
			}
		}
	}
}

/** True if the given column i and row j are within the bounds of the board. **/
bool BoardController::inBounds(int i, int j) const {
	return i >= 0 && i < MAX_COL && j >= 0 && j < MAX_ROW;
}

/** If the given piece is a pawn, return the correspoding queen (same suit), and inversingly. **/
int BoardController::getOther(int piece) const {
	switch (piece) {
		case WHITE_PAWN : return WHITE_QUEEN;
		case BLACK_PAWN : return BLACK_QUEEN;
		case WHITE_QUEEN : return WHITE_PAWN;
		case BLACK_QUEEN : return BLACK_PAWN;
		default : return EMPTY;
	}
}

/** Find the maximum number of pieces that can be captured by the pawn at column i and row j for the given board configuration.
Argument 'piece' corresponds to the target piece. **/
int BoardController::findPawnCapture(int i, int j, int** table, int piece) const {
	int rafle = 0, maxRafle = 0, lin = 0, col = 0;
	int pieceOther = getOther(piece);
	foreach (Direction dir, Direction::values()) {
		int nextCol = (col = i + dir.i()) + dir.i();
		int nextLin = (lin = j + dir.j()) + dir.j();
		if (inBounds(nextCol, nextLin)) {
			int p = table[col][lin];
			if ((p == piece || p == pieceOther) && table[nextCol][nextLin] == NONE) {
				table[col][lin] = NONE;
				rafle = findPawnCapture(nextCol, nextLin, table, piece) + 1;
				table[col][lin] = p;
				if (rafle > maxRafle) {
					maxRafle = rafle;
				}
			}
		}
	}
	return maxRafle;
}

/** Find the coordinate of the maximum captures for the pawn at column i and row j for the given board configuration.
Argument 'coord' is the list (possibly several captures) of the list of QPoint corresponding to each capture possible.
Argument 'length' has to be 0 when this method is called.
Argument 'maxRafle' is the number returned by 'findPawnCapture'.
Argument 'piece' is the target piece. **/
int BoardController::coordMaxPawnCapture(int i, int j, int** table, QList<QList<QPoint> > & coord, int length, int maxRafle, int piece) const {
	int lin = 0, col = 0, l = 0, rafle = 0, rafleTmp = 0;
	int pieceOther = getOther(piece);
	foreach (Direction dir, Direction::values()) {
		int nextCol = (col = i + dir.i()) + dir.i();
		int nextLin = (lin = j + dir.j()) + dir.j();
		if (inBounds(nextCol, nextLin)) {
			int p = table[col][lin];
			if ((p == piece || p == pieceOther) && table[nextCol][nextLin] == NONE) {
				table[col][lin] = NONE;
				length++;
				rafleTmp = coordMaxPawnCapture(nextCol, nextLin, table, coord, length, maxRafle, piece) + 1;
				table[col][lin] = p;
				length--;
				if (length + rafleTmp == maxRafle) {
					if (length == maxRafle - 1) {
						QList<QPoint> list = QList<QPoint>();
						for (l = 0; l < maxRafle; l++) {
							list << QPoint(-1, -1);
						}
						coord.append(list);
					}
					int nb_chemin = coord.size();
					coord[nb_chemin-1].replace(length, QPoint(nextCol, nextLin));
					l = 0;
					while (nb_chemin > l+1 && coord.at(nb_chemin - 2 - l).at(length) == QPoint(-1, -1)) {
						coord[nb_chemin - 2 - l].replace(length, QPoint(nextCol, nextLin));
						l++;
					}
				}
				if (rafleTmp > rafle) {
					rafle = rafleTmp;
				}
			}
		}
	}
	return rafle;
}

/** Find the maximum number of pieces that can be captured by the queen at column i and row j for the given board configuration.
Argument 'dirPrec' has to be Direction::UNDEFINED when this method is called.
Argument 'piece' corresponds to the target piece. **/
int BoardController::findQueenCapture(int i, int j, Direction dirPrec, int** table, int piece) const {
	int rafle = 0, maxRafle = 0, lin = 0, col = 0;
	int pieceOther = getOther(piece);
	if (dirPrec == Direction::UNDEFINED) {
		foreach (Direction dir, Direction::values()) {
			col = i;
			lin = j;
			bool end = false;
			while (inBounds(col + 2*dir.i(), lin + 2*dir.j()) && !end) {
				int p = table[col + dir.i()][lin + dir.j()];
				if ((p == piece || p == pieceOther) && table[col + 2*dir.i()][lin + 2*dir.j()] == NONE) {
					table[col + dir.i()][lin + dir.j()] = TOOK;
					rafle = findQueenCapture(col + 2*dir.i(), lin + 2*dir.j(), dir, table, piece) + 1;
					if (rafle > maxRafle) {
						maxRafle = rafle;
					}
					end = true;
					table[col + dir.i()][lin + dir.j()] = p;
				}
				if (p != NONE) {
					end = true;
				}
				col = col + dir.i();
				lin = lin + dir.j();
			}
		}
	}
	else {
		col = i;
		lin = j;
		bool end = false;
		while (inBounds(col+2*dirPrec.i(), lin+2*dirPrec.j()) && !end) {
			int p = table[col+dirPrec.i()][lin+dirPrec.j()];
			if ((p == piece || p == pieceOther) && table[col+2*dirPrec.i()][lin+2*dirPrec.j()] == NONE) {
				table[col+dirPrec.i()][lin+dirPrec.j()] = TOOK;
				rafle = findQueenCapture(col+2*dirPrec.i(), lin+2*dirPrec.j(), dirPrec, table, piece) + 1;
				if (rafle > maxRafle) {
					maxRafle = rafle;
				}
				end = true;
				table[col+dirPrec.i()][lin+dirPrec.j()] = p;
			}
			if (p != NONE){
				end = true;
			}
			col = col+dirPrec.i();
			lin = lin+dirPrec.j();
		}
		while (inBounds(i,j) && table[i][j] == NONE) {
			foreach (Direction dir, Direction::getOrthogonalDirections(dirPrec)) {
				col = i;
				lin = j;
				end = false;
				while (inBounds(col+2*dir.i(), lin+2*dir.j()) && !end) {
					int p = table[col+dir.i()][lin+dir.j()];
					if ((p == piece || p == pieceOther) && table[col+2*dir.i()][lin+2*dir.j()] == NONE){
						table[col+dir.i()][lin+dir.j()] = TOOK;
						int rafleTmp = findQueenCapture(col+2*dir.i(), lin+2*dir.j(), dir, table, piece) + 1;
						if (rafleTmp > rafle) {
							rafle = rafleTmp;
						}
						end = true;
						table[col+dir.i()][lin+dir.j()] = p;
					}
					if (p != NONE){
						end = true;
					}
					col = col + dir.i();
					lin = lin + dir.j();
				}
			}
			i = i + dirPrec.i();
			j = j + dirPrec.j();
		}
	}
	if (rafle > maxRafle) {
		maxRafle = rafle;
	}
	return maxRafle;
}

/** Find the coordinate of the maximum captures for the queen at column i and row j for the given board configuration.
Argument 'dirPrec' has to be Direction::UNDEFINED when this method is called.
Argument 'coord' is the list (possibly several captures) of the list of QPoint corresponding to each capture possible.
Argument 'length' has to be 0 when this method is called.
Argument 'maxRafle' is the number returned by 'findQueenCapture'.
Argument 'piece' is the target piece. **/
int BoardController::coordMaxQueenCapture(int i, int j, Direction dirPrec, int** table, QList<QList<QPoint> > & coord, int length, int maxRafle, int piece) const {
	int rafle=0, res=0, col=0, lin=0, l=0;
	int pieceOther = getOther(piece);
	if (dirPrec == Direction::UNDEFINED) {
		foreach (Direction dir, Direction::values()) {
			col = i;
			lin = j;
			bool end = false;
			while (inBounds(col+2*dir.i(),lin+2*dir.j()) && !end) {
				int p = table[col+dir.i()][lin+dir.j()];
				if ((p == piece || p == pieceOther) && table[col+2*dir.i()][lin+2*dir.j()] == NONE) {
					table[col+dir.i()][lin+dir.j()] = TOOK;
					length++;
					rafle = coordMaxQueenCapture(col+2*dir.i(), lin+2*dir.j(), dir, table, coord, length, maxRafle, piece) + 1;
					if(maxRafle == 1){
						QList<QPoint> list;
						list << QPoint(-1, -1);
						coord << list;
						int nb_chemin = coord.size();
						l = 0;
						while (inBounds(col+(l+2)*dir.i(), lin+(l+2)*dir.j()) && table[col+(l+2)*dir.i()][lin+(l+2)*dir.j()] == NONE) {
							coord[nb_chemin - 1] << QPoint(col+(l+2)*dir.i(), lin+(l+2)*dir.j());
							l++;
						}
					}
					end = true;
					table[col+dir.i()][lin+dir.j()] = p;
				}
				if (p != NONE) {
					end = true;
				}
				col = col + dir.i();
				lin = lin + dir.j();
			}
		}
	}
	else {
		col = i;
		lin = j;
		bool end = false;
		while (inBounds(col+2*dirPrec.i(),lin+2*dirPrec.j()) && !end) {
			int p = table[col+dirPrec.i()][lin+dirPrec.j()];
			if ((p == piece || p == pieceOther) && table[col+2*dirPrec.i()][lin+2*dirPrec.j()] == NONE){
				table[col+dirPrec.i()][lin+dirPrec.j()] = TOOK;
				length++;
				res = coordMaxQueenCapture(col+2*dirPrec.i(), lin+2*dirPrec.j(), dirPrec, table, coord, length, maxRafle, piece) + 1;
				length--;
				if (length + res == maxRafle) {
					if (length == maxRafle - 1) {
						QList<QPoint> list = QList<QPoint>();
						for (l = 0; l < maxRafle; l++) {
							list << QPoint(-1, -1);
						}
						coord << list;
						int nb_chemin = coord.size();
						l = 0;
						while (inBounds(col+(l+2)*dirPrec.i(), lin+(l+2)*dirPrec.j()) && table[col+(l+2)*dirPrec.i()][lin+(l+2)*dirPrec.j()] == NONE){
							coord[nb_chemin - 1] << QPoint(col+(l+2)*dirPrec.i(), lin+(l+2)*dirPrec.j());
							l++;
						}
					}
					int nb_chemin = coord.size();
					coord[nb_chemin - 1].replace(length - 1, QPoint(col, lin));
					l = 0;
					while (nb_chemin > l+1 && coord[nb_chemin-2-l][length] == QPoint(-1, -1)){
						coord[nb_chemin - 2 - l].replace(length, QPoint(col, lin));
						l++;
					}
				}
				end = true;
				table[col+dirPrec.i()][lin+dirPrec.j()] = p;
			}
			if (p != NONE) {
				end = true;
			}
			col = col + dirPrec.i();
			lin = lin + dirPrec.j();
		}
		while (inBounds(i, j) && table[i][j] == NONE) {
			foreach (Direction dir, Direction::getOrthogonalDirections(dirPrec)) {
				col = i;
				lin = j;
				end = false;
				while (inBounds(col+2*dir.i(),lin+2*dir.j()) && !end) {
					int p = table[col+dir.i()][lin+dir.j()];
					if ((p == piece || p == pieceOther) && table[col+2*dir.i()][lin+2*dir.j()] == NONE){
						table[col+dir.i()][lin+dir.j()] = TOOK;
						length++;
						int rafleTmp = coordMaxQueenCapture(col+2*dir.i(),lin+2*dir.j(), dir, table, coord, length, maxRafle, piece) + 1;
						length--;
						if (length + rafleTmp == maxRafle) {
							if (length == maxRafle-1) {
								QList<QPoint> list = QList<QPoint>();
								for (l = 0; l < maxRafle; l++) {
									list << QPoint(-1, -1);
								}
								coord << list;
								int nb_chemin = coord.size();
								l = 0;
								while (inBounds(col+(l+2)*dir.i(), lin+(l+2)*dir.j()) && table[col+(l+2)*dir.i()][lin+(l+2)*dir.j()] == NONE) {
									coord[nb_chemin - 1] << QPoint(col+(l+2)*dir.i(), lin+(l+2)*dir.j());
									l++;
								}
							}
							int nb_chemin = coord.size();
							coord[nb_chemin - 1].replace(length - 1, QPoint(i, j));
							l=0;
							while (nb_chemin > l+1 && coord[nb_chemin-2-l][length-1] == QPoint(-1, -1)) {
								coord[nb_chemin - 2 - l].replace(length - 1, QPoint(i, j));
								l++;
							}
						}
						if (rafleTmp > res) {
							res = rafleTmp;
						}
						end = true;
						table[col+dir.i()][lin+dir.j()] = p;
					}
					if (p != NONE){
						end = true;
					}
					col = col + dir.i();
					lin = lin + dir.j();
				}
			}
			i = i + dirPrec.i();
			j = j + dirPrec.j();
		}
	}
	if (res > rafle) {
		rafle = res;
	}
	return rafle;
}

/** Delete this BoardController instance. **/
BoardController::~BoardController() {
	delete clickablePieces;
}
