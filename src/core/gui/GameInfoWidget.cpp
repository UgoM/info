#include "src/core/gui/GameInfoWidget.h"

GameInfoWidget::GameInfoWidget()
{
    QLabel * label1 = new QLabel(tr("Game Infos :"));
    QLabel * label2 = new QLabel(tr("Players :"));
    labelNPlayers = new QLabel("#");
    QLabel * label3 = new QLabel(tr("Obs :"));
    labelNObs = new QLabel("#");
    QLabel * label4 = new QLabel(tr("Status :"));
    labelStatus = new QLabel("#");

	QGridLayout * gridLayout = new QGridLayout();
    gridLayout->addWidget(label1, 0, 0, 1, 2);
    gridLayout->addWidget(label2, 1, 0);
    gridLayout->addWidget(labelNPlayers, 1, 1);
    gridLayout->addWidget(label3, 2, 0);
    gridLayout->addWidget(labelNObs, 2, 1);
    gridLayout->addWidget(label4, 3, 0);
    gridLayout->addWidget(labelStatus, 4, 0);
	setLayout(gridLayout);
}

void GameInfoWidget::nConnectedChanged(int nPlayers, int nObs)
{
    labelNPlayers->setText(QString::number(nPlayers));
    labelNObs->setText(QString::number(nObs));
}
void GameInfoWidget::statusChanged(QString label)
{
    labelStatus->setText(label);
}
