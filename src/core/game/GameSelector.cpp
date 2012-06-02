#include "src/core/game/GameSelector.h"

#include <QDir>
#include <QDebug>

#include "src/games/checkers/Checkers.h"
#include "src/games/checkers/BrainCheckers.h"

/** \brief Get list of available games
  *
  * Get list of available games by taking folders name in src/games/
  */
QList<QString> GameSelector::getGameList()
{
    QList<QString> ret;

    QDir dir("src/games/");
    dir.setFilter(QDir::Dirs | QDir::NoSymLinks | QDir::NoDot | QDir::NoDotDot);
    dir.setSorting(QDir::Name);
    
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        ret << fileInfo.fileName();
    }
    qDebug() << "Game List : " << ret;
    return ret;
}

/** \brief Start a new game a return the pointer
  */
Game * GameSelector::startNewGame(QString gameName)
{
    if (gameName == "checkers" || gameName == "Jeu de dames") {
        Checkers * newGame = new Checkers();
        return newGame;
    } else {
        qDebug() << "Wrong gameName";
        return NULL;
    }
}

/** \brief Start a new Brain a return the pointer
  */
Brain * GameSelector::startNewBrain(QString gameName)
{
    if (gameName == "checkers" || gameName == "Jeu de dames") {
        BrainCheckers * newGame = new BrainCheckers();
        return newGame;
    } else {
        qDebug() << "Wrong gameName" << gameName;
        return NULL;
    }
}
