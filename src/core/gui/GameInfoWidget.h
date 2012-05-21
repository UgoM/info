#pragma once

#include <QtGui>

class GameInfoWidget : public QWidget {

    Q_OBJECT

    QLabel * labelNPlayers;
    QLabel * labelNObs;

    public:
        GameInfoWidget();

    private:

    public slots:
        void nPlayersChanged(int n);
        void nObsChanged(int n);


};

