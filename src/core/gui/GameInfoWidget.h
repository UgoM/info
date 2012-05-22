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
        void nConnectedChanged(int nPlayers, int nObs);


};

