#include "BoardController.h"

#include <QDebug>

bool operator<(const QPoint & lhs, const QPoint & rhs) {
	return lhs.x() < rhs.x() || (lhs.x() == rhs.x() && lhs.y() < rhs.y());
}

bool BoardController::isPointClickable(const QPoint & point) {
	return clickablePieces->contains(point);
}

Move BoardController::controlMove(int** table, const QPoint & point, const QPoint & wanted) {
	int p = table[point.x()][point.y()];
	if (p == WHITE_QUEEN || p == BLACK_QUEEN) {
		queenMovementInProgress = true;
	}
	if (queenMovementInProgress && !canQueenCapture) {
		foreach (QList<QPoint> list, clickablePieces->value(point)) {
			if (list.contains(wanted)) {
				return SIMPLE;
			}
		}
	} else if (queenMovementInProgress && canQueenCapture) {
		int rafle = findQueenCapture(point.x(), point.y(), Direction::UNDEFINED, table, p == WHITE_QUEEN ? BLACK_PAWN : WHITE_PAWN);
		if (rafle == 1) {
			foreach (QList<QPoint> list, clickablePieces->value(point)) {
				if (list.contains(wanted)) {
					return SINGLE_CAPTURE;
				}
			}
		}
	}
	QList<QList<QPoint> > newlyAllowedPositions;
	foreach (QList<QPoint> list, clickablePieces->value(point)) {
		if (list.startsWith(wanted)) {
			list.removeFirst();
			newlyAllowedPositions << list;
		}
	}
	if (!newlyAllowedPositions.isEmpty()) {
		if (qAbs(wanted.x() - point.x()) > 1 || qAbs(wanted.y() - point.y()) > 1) {
			if (newlyAllowedPositions.size() == 1 && newlyAllowedPositions[0].isEmpty()) {
				return SINGLE_CAPTURE;
			} else {
				clickablePieces->insert(point, newlyAllowedPositions);
				return MANY_CAPTURE;
			}
		} else {
			return SIMPLE;
		}
	} else {
		return ILLEGAL;
	}
}

void BoardController::calculateClickablePieces(int** table, bool current) {
	int max_rafle = 0;
	canQueenCapture = false;
	queenMovementInProgress = false;
	int targetPiece = current ? BLACK_PAWN : WHITE_PAWN;
	int currentPiece = current ? WHITE_PAWN : BLACK_PAWN;
	int currentOtherPiece = current ? WHITE_QUEEN : BLACK_QUEEN;
	QList<QPoint> possibleList;
	clickablePieces = new QMap<QPoint, QList<QList<QPoint> > >();
	for (int i = 0; i < MAX_COL; i++) {
		for (int j = 0; j < MAX_ROW; j++) {
			int rafle = 0;
			if (table[i][j] == currentPiece) {
				rafle = findPawnCapture(i, j, table, targetPiece);
			} else if (table[i][j] == currentOtherPiece) {
				rafle = findQueenCapture(i, j, Direction::UNDEFINED, table, targetPiece);
			}
			if (table[i][j] == currentPiece || table[i][j] == currentOtherPiece) {
				if (rafle > max_rafle) {
					possibleList.clear();
					max_rafle = rafle;
					possibleList << QPoint(i, j);
				} else if (rafle == max_rafle) {
					possibleList << QPoint(i, j);
				}
			}
		}
	}
	foreach (QPoint point, possibleList) {
		QList<QList<QPoint> > coord;
		if (max_rafle > 0) {
			if (table[point.x()][point.y()] == currentPiece) {
				coordMaxPawnCapture(point.x(), point.y(), table, coord, 0, max_rafle, targetPiece);
			} else {
				canQueenCapture = true;
				coordMaxQueenCapture(point.x(), point.y(), Direction::UNDEFINED, table, coord, 0, max_rafle, targetPiece);
			}
			clickablePieces->insert(point, coord);
		} else {
			if (table[point.x()][point.y()] == currentPiece) {
				if (current) {
					if (inBounds(point.x() - 1, point.y() - 1) && table[point.x() - 1][point.y() - 1] == NONE) {
						QList<QPoint> list;
						list << QPoint(point.x() - 1, point.y() - 1);
						coord << list;
					}
					if (inBounds(point.x() + 1, point.y() - 1) && table[point.x() + 1][point.y() - 1] == NONE) {
						QList<QPoint> list;
						list << QPoint(point.x() + 1, point.y() - 1);
						coord << list;
					}
				} else {
					if (inBounds(point.x() - 1, point.y() + 1) && table[point.x() - 1][point.y() + 1] == NONE) {
						QList<QPoint> list;
						list << QPoint(point.x() - 1, point.y() + 1);
						coord << list;
					}
					if (inBounds(point.x() + 1, point.y() + 1) && table[point.x() + 1][point.y() + 1] == NONE) {
						QList<QPoint> list;
						list << QPoint(point.x() + 1, point.y() + 1);
						coord << list;
					}
				}
			} else {
				QList<Direction> dirList = Direction::values();
				for (int p = 0; p < dirList.size(); p++) {
					int l = 1;
					Direction dir = dirList[p];
					QList<QPoint> list;
					while (inBounds(point.x()+l*dir.i(), point.y()+l*dir.j()) && table[point.x()+l*dir.i()][point.y()+l*dir.j()] == NONE) {
						list << QPoint(point.x()+l*dir.i(), point.y()+l*dir.j());
						l++;
					}
					if(!list.isEmpty()) {
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

bool BoardController::inBounds(int i, int j) {
	return i>=0 && i<MAX_ROW && j>=0 && j<MAX_COL;
}

int BoardController::getOther(int piece) {
	switch (piece) {
		case WHITE_PAWN : return WHITE_QUEEN;
		case BLACK_PAWN : return BLACK_QUEEN;
		case WHITE_QUEEN : return WHITE_PAWN;
		case BLACK_QUEEN : return BLACK_PAWN;
		default : return EMPTY;
	}
}

int BoardController::findPawnCapture(int i, int j, int** table, int piece) {
    int rafle = 0, max_rafle = 0, lin = 0, col = 0;
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
                if (rafle > max_rafle) {
                    max_rafle = rafle;
                }
            }
        }
	}
    return max_rafle;
}

int BoardController::coordMaxPawnCapture(int i, int j, int** table, QList<QList<QPoint> > & coord, int length, int max_rafle, int piece) {
    int lin = 0, col = 0, l = 0, rafle = 0, rafle_tmp = 0;
    int pieceOther = getOther(piece);
	QList<Direction> dirs = Direction::values();
    foreach (Direction dir, Direction::values()) {
        int nextCol = (col = i + dir.i()) + dir.i();
		int nextLin = (lin = j + dir.j()) + dir.j();
		if (inBounds(nextCol, nextLin)) {
			int p = table[col][lin];
			if ((p == piece || p == pieceOther) && table[nextCol][nextLin] == NONE) {
				table[col][lin] = NONE;
				length++;
				rafle_tmp = coordMaxPawnCapture(nextCol, nextLin, table, coord, length, max_rafle, piece) + 1;
				table[col][lin] = p;
				length--;
				if (length + rafle_tmp == max_rafle) {
					if (length == max_rafle - 1) {
						QList<QPoint> list = QList<QPoint>();
						for (l = 0; l < max_rafle; l++) {
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
				if (rafle_tmp > rafle) {
					rafle = rafle_tmp;
				}
			}
		}
    }
    return rafle;
}

int BoardController::findQueenCapture(int i, int j, Direction dirPrec, int** table, int piece){
    int rafle = 0, max_rafle = 0, lin = 0, col = 0;
	int pieceOther = getOther(piece);
    if (dirPrec == Direction::UNDEFINED) {
        foreach (Direction dir, Direction::values()) {
			col = i;
			lin = j;
            bool end = false;
			while (inBounds(col+2*dir.i(), lin+2*dir.j()) && !end) {
				int p = table[col+dir.i()][lin+dir.j()];
                if ((p == piece || p == pieceOther) && table[col+2*dir.i()][lin+2*dir.j()] == NONE) {
                    table[col+dir.i()][lin+dir.j()] = TOOK;
                    rafle = findQueenCapture(col+2*dir.i(), lin+2*dir.j(), dir, table, piece) + 1;
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
        while (inBounds(col+2*dirPrec.i(), lin+2*dirPrec.j()) && !end) {
			int p = table[col+dirPrec.i()][lin+dirPrec.j()];
            if ((p == piece || p == pieceOther) && table[col+2*dirPrec.i()][lin+2*dirPrec.j()] == NONE) {
                table[col+dirPrec.i()][lin+dirPrec.j()] = TOOK;
                rafle = findQueenCapture(col+2*dirPrec.i(), lin+2*dirPrec.j(), dirPrec, table, piece) + 1;
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
                        int rafle_tmp = findQueenCapture(col+2*dir.i(), lin+2*dir.j(), dir, table, piece) + 1;
                        if (rafle_tmp > rafle) {
                            rafle = rafle_tmp;
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
    if (rafle > max_rafle) {
        max_rafle = rafle;
    }
    return max_rafle;
}

int BoardController::coordMaxQueenCapture(int i, int j, Direction dirPrec, int** table, QList<QList<QPoint> > & coord, int length, int max_rafle, int piece) {
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
                    rafle = coordMaxQueenCapture(col+2*dir.i(), lin+2*dir.j(), dir, table, coord, length, max_rafle, piece) + 1;
                    if(max_rafle == 1){
						coord << QList<QPoint>();
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
                res = coordMaxQueenCapture(col+2*dirPrec.i(), lin+2*dirPrec.j(), dirPrec, table, coord, length, max_rafle, piece) + 1;
                length--;
                if (length + res == max_rafle) {
                    if (length == max_rafle - 1) {
                        QList<QPoint> list = QList<QPoint>();
						for (l = 0; l < max_rafle; l++) {
							list << QPoint(-1, -1);
						}
						coord << list;
						int nb_chemin = coord.size();
                        l=1;
						coord[nb_chemin - 1].replace(max_rafle - 1, QPoint(col+2*dirPrec.i(), lin+2*dirPrec.j()));
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
                        int rafle_tmp = coordMaxQueenCapture(col+2*dir.i(),lin+2*dir.j(), dir, table, coord, length, max_rafle, piece) + 1;
                        length--;
                        if (length + rafle_tmp == max_rafle) {
                            if (length == max_rafle-1) {
                                QList<QPoint> list = QList<QPoint>();
								for (l = 0; l < max_rafle; l++) {
									list << QPoint(-1, -1);
								}
								coord << list;
								int nb_chemin = coord.size();
                                l=1;
								coord[nb_chemin - 1].replace(max_rafle - 1, QPoint(col+2*dir.i(), lin+2*dir.j()));
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
                        if (rafle_tmp > res) {
                            res = rafle_tmp;
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

BoardController::~BoardController() {
	delete clickablePieces;
}
