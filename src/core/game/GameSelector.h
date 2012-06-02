#pragma once

#include <QList>
#include <QString>

class Game;
class Brain;

/** Goal is not to have to modify any files from core when adding a new game
  * to this framework. For now, don't really have an idea on how to achieve it.
  * But this class still simplify adding a new game because, you only have some
  * lines to modify in GameSelector.cpp and no other ones anywhere.
  */
class GameSelector
{
    public:
        GameSelector();
        
        static QList<QString> getGameList();

        static Game * startNewGame(QString gameName);
        static Brain * startNewBrain(QString gameName);
};
