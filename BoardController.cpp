#include "BoardController.h"

bool operator<(const QPoint & lhs, const QPoint & rhs) {
	return lhs.x() < rhs.x() || (lhs.x() == rhs.x() && lhs.y() < rhs.y());
}

bool BoardController::isPointClickable(const QPoint & point) {
	return clickablePieces->contains(point);
}

Move BoardController::controlMove(const QPoint & point, const QPoint & wanted) {
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

void BoardController::calculateClickablePieces(Piece** table, bool current) {
	int max_rafle = 0;
	Piece targetPiece = current ? BLACK_PAWN : WHITE_PAWN;
	Piece currentPiece = current ? WHITE_PAWN : BLACK_PAWN;
	QList<QPoint> possibleList;
	clickablePieces = new QMap<QPoint, QList<QList<QPoint> > >();
	for (int i = 0; i < MAX_COL; i++) {
		for (int j = 0; j < MAX_ROW; j++) {
			if (table[i][j] == currentPiece || table[i][j] == getOther(currentPiece)) {
				int rafle = findPawnCapture(i, j, table, targetPiece);
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
			coordMaxPawnCapture(point.x(), point.y(), table, coord, 0, max_rafle, targetPiece);
			clickablePieces->insert(point, coord);
		} else {
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
			if (!coord.isEmpty()) {
				clickablePieces->insert(point, coord);
			}
		}
		
	}
}

bool BoardController::inBounds(int i, int j) {
	return i>=0 && i<MAX_ROW && j>=0 && j<MAX_COL;
}

Piece BoardController::getOther(Piece piece) {
	switch (piece) {
		case WHITE_PAWN : return WHITE_QUEEN;
		case BLACK_PAWN : return BLACK_QUEEN;
		case WHITE_QUEEN : return WHITE_PAWN;
		case BLACK_QUEEN : return BLACK_PAWN;
		default : return EMPTY;
	}
}

int BoardController::findPawnCapture(int i, int j, Piece** table, Piece piece) {
    int rafle = 0, max_rafle = 0, lin = 0, col = 0;
	Piece pieceOther = getOther(piece);
	foreach (Direction dir, Direction::values()) {
		int nextCol = (col = i + dir.i()) + dir.i();
		int nextLin = (lin = j + dir.j()) + dir.j();
        if (inBounds(nextCol, nextLin)) {
			Piece p = table[col][lin];
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

int BoardController::coordMaxPawnCapture(int i, int j, Piece** table, QList<QList<QPoint> > & coord, int length, int max_rafle, Piece piece) {
    int lin = 0, col = 0, l = 0, rafle = 0, rafle_tmp = 0;
    Piece pieceOther = getOther(piece);
	QList<Direction> dirs = Direction::values();
    foreach (Direction dir, Direction::values()) {
        int nextCol = (col = i + dir.i()) + dir.i();
		int nextLin = (lin = j + dir.j()) + dir.j();
		if (inBounds(nextCol, nextLin)) {
			Piece p = table[col][lin];
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

/* j'ai écris le code qui suit quand j'avais fais le jeu de dames en C.
je suis sur à 100 % qu'il marche et fait ce qui est demandé (même si le code est ultra long et compliqué).
en gros, c'est la même chose que pour les pions au dessus sauf que ici, c'est pour les dames
ce qu'il faut retenir est que
i - colonne de départ, j - ligne de départ
k_prec - mettre -1 à l'appel puis ne plus s'en soucier
carte[][NB_TOTAL_CASE] - damier (contient enum VIDE, PION_NOIR, etc).
couleur - la couleur cible énoncé en pion (si blanc joue, couleur = PION_NOIR, et inversement)

int trouver_rafle_dame(int i, int j, int k_prec, int carte[][NB_TOTAL_CASE], int couleur){
    int rafle=0,max_rafle=0,rafle_tmp=0,k=0,col=0,lin=0,fin=0,carte_m=0,cl=0;
    Direc dir[4]={{1,1},{1,-1},{-1,-1},{-1,1}};
    if(couleur==PION_NOIR){
        cl=DAME_NOIRE;
    }
    else{
        cl=DAME_BLANCHE;
    }
    if(k_prec==-1){
        for(k=0;k<4;k++){
            col=i;
            lin=j;
            fin=0;
            while(en_jeu(col+2*dir[k].i,lin+2*dir[k].j) && !fin){
                if((carte[col+dir[k].i][lin+dir[k].j]==couleur || carte[col+dir[k].i][lin+dir[k].j]==cl) && carte[col+2*dir[k].i][lin+2*dir[k].j]==VIDE){
                    carte_m=carte[col+dir[k].i][lin+dir[k].j];
                    carte[col+dir[k].i][lin+dir[k].j]=PRIS;
                    rafle=trouver_rafle_dame(col+2*dir[k].i,lin+2*dir[k].j,k,carte,couleur)+1;
                    fin=1;
                    carte[col+dir[k].i][lin+dir[k].j]=carte_m;
                }
                if(carte[col+dir[k].i][lin+dir[k].j]!=VIDE){
                    fin=1;
                }
                col=col+dir[k].i;
                lin=lin+dir[k].j;
            }
        }
    }
    else{
        col=i;
        lin=j;
        fin=0;
        while(en_jeu(col+2*dir[k_prec].i,lin+2*dir[k_prec].j) && !fin){
            if((carte[col+dir[k_prec].i][lin+dir[k_prec].j]==couleur || carte[col+dir[k_prec].i][lin+dir[k_prec].j]==cl) && carte[col+2*dir[k_prec].i][lin+2*dir[k_prec].j]==VIDE){
                carte_m=carte[col+dir[k_prec].i][lin+dir[k_prec].j];
                carte[col+dir[k_prec].i][lin+dir[k_prec].j]=PRIS;
                rafle=trouver_rafle_dame(col+2*dir[k_prec].i,lin+2*dir[k_prec].j,k_prec,carte,couleur)+1;
                fin=1;
                carte[col+dir[k_prec].i][lin+dir[k_prec].j]=carte_m;
            }
            if(carte[col+dir[k_prec].i][lin+dir[k_prec].j]!=VIDE){
                fin=1;
            }
            col=col+dir[k_prec].i;
            lin=lin+dir[k_prec].j;
        }
        while(en_jeu(i,j) && carte[i][j]==VIDE){
            for(k=min((k_prec+1)%4,(k_prec+3)%4);k<=max((k_prec+1)%4,(k_prec+3)%4);k+=2){
                col=i;
                lin=j;
                fin=0;
                while(en_jeu(col+2*dir[k].i,lin+2*dir[k].j) && !fin){
                    if((carte[col+dir[k].i][lin+dir[k].j]==couleur || carte[col+dir[k_prec].i][lin+dir[k_prec].j]==cl) && carte[col+2*dir[k].i][lin+2*dir[k].j]==VIDE){
                        carte_m=carte[col+dir[k].i][lin+dir[k].j];
                        carte[col+dir[k].i][lin+dir[k].j]=PRIS;
                        rafle_tmp=trouver_rafle_dame(col+2*dir[k].i,lin+2*dir[k].j,k,carte,couleur)+1;
                        if(rafle_tmp>rafle){
                            rafle=rafle_tmp;
                        }
                        fin=1;
                        carte[col+dir[k].i][lin+dir[k].j]=carte_m;
                    }
                    if(carte[col+dir[k].i][lin+dir[k].j]!=VIDE){
                        fin=1;
                    }
                    col=col+dir[k].i;
                    lin=lin+dir[k].j;
                }
            }
            i=i+dir[k_prec].i;
            j=j+dir[k_prec].j;
        }
    }
    if(rafle>max_rafle){
        max_rafle=rafle;
    }
    return max_rafle;
}

Ici, explication :
i, j, k_prec, carte et couleur comme précédemment
coord - coordonnées rafle(s) possible(s). (structure contennant un champ x et y)
longueur - vaut 0 au début et ne plus s'en occuper
max_rafle - le résultat de la fonction ci-dessus
nb_chemin - pointeur vers un int qui vaut 0 et ne pas s'en occuper
l_total - tableau de int qui donne pour chaque rafle, le nombre total de cases possibles (max_rafle + choix final - 1)
(c'est tordu mais lors d'une rafle par une dame, la dernière case peut être laissé au choix ex:
....
....
.o..
x...
x = dame, o=adverse
La dame peut aller sur les 2 points restants de la diagonale donc l_total[0 (car une seule rafle)] = 1 + 2 - 1

int coordonnee_rafle_max_dame(int i, int j, int k_prec, int carte[][10], Coord **coord, int longueur, int max_rafle, int *nb_chemin, int *l_total, int couleur){
    int rafle=0,rafle_tmp=0,res=0,k=0,col=0,lin=0,fin=0,p=0,carte_m=0,cl=0;
    Direc dir[4]={{1,1},{1,-1},{-1,-1},{-1,1}};
    if(couleur==PION_NOIR){
        cl=DAME_NOIRE;
    }
    else{
        cl=DAME_BLANCHE;
    }
    if(k_prec==-1){
        for(k=0;k<4;k++){
            col=i;
            lin=j;
            fin=0;
            while(en_jeu(col+2*dir[k].i,lin+2*dir[k].j) && !fin){
                if((carte[col+dir[k].i][lin+dir[k].j]==couleur || carte[col+dir[k].i][lin+dir[k].j]==cl) && carte[col+2*dir[k].i][lin+2*dir[k].j]==VIDE){
                    carte_m=carte[col+dir[k].i][lin+dir[k].j];
                    carte[col+dir[k].i][lin+dir[k].j]=PRIS;
                    longueur++;
                    rafle=coordonnee_rafle_max_dame(col+2*dir[k].i,lin+2*dir[k].j,k,carte,coord,longueur,max_rafle,nb_chemin,l_total,couleur)+1;
                    if(max_rafle==1){
                        (*nb_chemin)++;
                        if(*nb_chemin>1){
                            for(p=0;p<*nb_chemin;p++){
                                coord=(Coord**)realloc(coord,(*nb_chemin)*sizeof(Coord*));
                                if(coord==NULL) exit(0);
                            }
                            coord[*nb_chemin-1]=malloc(max_rafle*sizeof(Coord));
                            if(coord[*nb_chemin-1]==NULL) exit(0);
                            for(p=0;p<max_rafle;p++){
                                coord[*nb_chemin-1][p].x=-1;
                                coord[*nb_chemin-1][p].y=-1;
                            }
                        }
                        p=0;
                        while(en_jeu(col+(p+2)*dir[k].i,lin+(p+2)*dir[k].j) && carte[col+(p+2)*dir[k].i][lin+(p+2)*dir[k].j]==VIDE){
                            coord[*nb_chemin-1]=(Coord*)realloc(coord[*nb_chemin-1],(max_rafle+p+1)*sizeof(Coord));
                            coord[*nb_chemin-1][max_rafle+p-1].x=col+(p+2)*dir[k].i;
                            coord[*nb_chemin-1][max_rafle+p-1].y=lin+(p+2)*dir[k].j;
                            l_total[*nb_chemin-1]=max_rafle+p;
                            p++;
                        }
                    }
                    fin=1;
                    carte[col+dir[k].i][lin+dir[k].j]=carte_m;
                }
                if(carte[col+dir[k].i][lin+dir[k].j]!=VIDE){
                    fin=1;
                }
                col=col+dir[k].i;
                lin=lin+dir[k].j;
            }
        }
    }
    else{
        col=i;
        lin=j;
        fin=0;
        while(en_jeu(col+2*dir[k_prec].i,lin+2*dir[k_prec].j) && !fin){
            if((carte[col+dir[k_prec].i][lin+dir[k_prec].j]==couleur || carte[col+dir[k_prec].i][lin+dir[k_prec].j]==cl) && carte[col+2*dir[k_prec].i][lin+2*dir[k_prec].j]==VIDE){
                carte_m=carte[col+dir[k_prec].i][lin+dir[k_prec].j];
                carte[col+dir[k_prec].i][lin+dir[k_prec].j]=PRIS;
                longueur++;
                res=coordonnee_rafle_max_dame(col+2*dir[k_prec].i,lin+2*dir[k_prec].j,k_prec,carte,coord,longueur,max_rafle,nb_chemin,l_total,couleur)+1;
                longueur--;
                if(longueur+res==max_rafle){
                    if(longueur==max_rafle-1){
                        (*nb_chemin)++;
                        if(*nb_chemin>1){
                            for(p=0;p<*nb_chemin;p++){
                                coord=(Coord**)realloc(coord,(*nb_chemin)*sizeof(Coord*));
                                if(coord==NULL) exit(0);
                            }
                            coord[*nb_chemin-1]=malloc(max_rafle*sizeof(Coord));
                            if(coord[*nb_chemin-1]==NULL) exit(0);
                            for(p=0;p<max_rafle;p++){
                                coord[*nb_chemin-1][p].x=-1;
                                coord[*nb_chemin-1][p].y=-1;
                            }
                        }
                        p=0;
                        while(en_jeu(col+(p+2)*dir[k_prec].i,lin+(p+2)*dir[k_prec].j) && carte[col+(p+2)*dir[k_prec].i][lin+(p+2)*dir[k_prec].j]==VIDE){
                            coord[*nb_chemin-1]=(Coord*)realloc(coord[*nb_chemin-1],(max_rafle+p+1)*sizeof(Coord));
                            coord[*nb_chemin-1][max_rafle+p-1].x=col+(p+2)*dir[k_prec].i;
                            coord[*nb_chemin-1][max_rafle+p-1].y=lin+(p+2)*dir[k_prec].j;
                            l_total[*nb_chemin-1]=max_rafle+p;
                            p++;
                        }
                    }
                    coord[*nb_chemin-1][longueur-1].x=col;
                    coord[*nb_chemin-1][longueur-1].y=lin;
                    p=0;
                    while(*nb_chemin>p+1 && coord[*nb_chemin-2-p][longueur].x==-1 && coord[*nb_chemin-2-p][longueur].y==-1){
                        coord[*nb_chemin-2-p][longueur].x=col;
                        coord[*nb_chemin-2-p][longueur].y=lin;
                        p++;
                    }
                }
                fin=1;
                carte[col+dir[k_prec].i][lin+dir[k_prec].j]=carte_m;
            }
            if(carte[col+dir[k_prec].i][lin+dir[k_prec].j]!=VIDE){
                fin=1;
            }
            col=col+dir[k_prec].i;
            lin=lin+dir[k_prec].j;
        }
        while(en_jeu(i,j) && carte[i][j]==VIDE){
            for(k=min((k_prec+1)%4,(k_prec+3)%4);k<=max((k_prec+1)%4,(k_prec+3)%4);k+=2){
                col=i;
                lin=j;
                fin=0;
                while(en_jeu(col+2*dir[k].i,lin+2*dir[k].j) && !fin){
                    if((carte[col+dir[k].i][lin+dir[k].j]==couleur || carte[col+dir[k].i][lin+dir[k].j]==cl) && carte[col+2*dir[k].i][lin+2*dir[k].j]==VIDE){
                        carte_m=carte[col+dir[k].i][lin+dir[k].j];
                        carte[col+dir[k].i][lin+dir[k].j]=PRIS;
                        longueur++;
                        rafle_tmp=coordonnee_rafle_max_dame(col+2*dir[k].i,lin+2*dir[k].j,k,carte,coord,longueur,max_rafle,nb_chemin,l_total,couleur)+1;
                        longueur--;
                        if(longueur+rafle_tmp==max_rafle){
                            if(longueur==max_rafle-1){
                                (*nb_chemin)++;
                                if(*nb_chemin>1){
                                    for(p=0;p<*nb_chemin;p++){
                                        coord=(Coord**)realloc(coord,(*nb_chemin)*sizeof(Coord*));
                                        if(coord==NULL) exit(0);
                                    }
                                    coord[*nb_chemin-1]=malloc(max_rafle*sizeof(Coord));
                                    if(coord[*nb_chemin-1]==NULL) exit(0);
                                    for(p=0;p<max_rafle;p++){
                                        coord[*nb_chemin-1][p].x=-1;
                                        coord[*nb_chemin-1][p].y=-1;
                                    }
                                }
                                p=0;
                                while(en_jeu(col+(p+2)*dir[k].i,lin+(p+2)*dir[k].j) && carte[col+(p+2)*dir[k].i][lin+(p+2)*dir[k].j]==VIDE){
                                    coord[*nb_chemin-1]=(Coord*)realloc(coord[*nb_chemin-1],(max_rafle+p+1)*sizeof(Coord));
                                    coord[*nb_chemin-1][max_rafle+p-1].x=col+(p+2)*dir[k].i;
                                    coord[*nb_chemin-1][max_rafle+p-1].y=lin+(p+2)*dir[k].j;
                                    l_total[*nb_chemin-1]=max_rafle+p;
                                    p++;
                                }
                            }
                            coord[*nb_chemin-1][longueur-1].x=i;
                            coord[*nb_chemin-1][longueur-1].y=j;
                            p=0;
                            while(*nb_chemin>p+1 && coord[*nb_chemin-2-p][longueur-1].x==-1 && coord[*nb_chemin-2-p][longueur-1].y==-1){
                                coord[*nb_chemin-2-p][longueur-1].x=i;
                                coord[*nb_chemin-2-p][longueur-1].y=j;
                                p++;
                            }
                        }
                        if(rafle_tmp>res){
                            res=rafle_tmp;
                        }
                        fin=1;
                        carte[col+dir[k].i][lin+dir[k].j]=carte_m;
                    }
                    if(carte[col+dir[k].i][lin+dir[k].j]!=VIDE){
                        fin=1;
                    }
                    col=col+dir[k].i;
                    lin=lin+dir[k].j;
                }
            }
            i=i+dir[k_prec].i;
            j=j+dir[k_prec].j;
        }
    }
    if(res>rafle){
        rafle=res;
    }
    return rafle;
} */

BoardController::~BoardController() {
	delete clickablePieces;
}