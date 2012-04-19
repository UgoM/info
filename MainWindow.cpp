#include "MainWindow.h"
#include <iostream>

MainWindow::MainWindow() {
	QMdiArea * centralZone = new QMdiArea;
	centralZone->setBackground(QBrush(QPixmap("images/imageFond2.jpg")));	//rajout image de fond d'écran
	centralZone->setFixedSize(345, 345);
	setCentralWidget(centralZone); //fenetre ppale

	QMenu * menuFile = menuBar()->addMenu(tr("&Partie"));
    QMenu * menuQuestion = menuBar()->addMenu(tr("&?"));

	QAction *actionNew = menuFile->addAction(tr("&Nouvelle partie"));
	connect(actionNew, SIGNAL(triggered()), this, SLOT(newGameFromMenu()));
	/*QAction *actionWatch = */menuFile->addAction(tr("&Regarder une partie"));
	/*QAction *actionCancel = */menuFile->addAction(tr("&Annuler"));
	QAction * actionHide = menuFile->addAction(tr("&Cache toi de ton Boss"));
	actionHide->setShortcut(QKeySequence("Ctrl+H"));
	connect(actionHide, SIGNAL(triggered()), this, SLOT(windowBossDisp()));
	QAction * actionQuit = menuFile->addAction(tr("&Quitter"));
	actionQuit->setShortcut(QKeySequence("Ctrl+Q"));
	connect(actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
	/*QAction *actionHelp=*/menuQuestion->addAction(tr("&Afficher l'aide"));
	QAction *actionAbout = menuQuestion->addAction(tr("&À propos"));
	connect(actionAbout, SIGNAL(triggered()),this, SLOT(windowAboutDisp()));
	
	//boutons principaux
    QPushButton * buttonList = new QPushButton(tr("Liste des serveurs"));
	connect(buttonList, SIGNAL(clicked()), this, SLOT(serverListDisp()));
	QPushButton * buttonNew = new QPushButton(tr("Nouvelle partie"));
	connect(buttonNew, SIGNAL(clicked()),this, SLOT(newGameFromMenu()));
	QPushButton * buttonSetup = new QPushButton(tr("Configurer joueur"));
	connect(buttonSetup, SIGNAL(clicked()), this, SLOT(setupDisp()));
	mainButtonDisp(buttonList);
	mainButtonDisp(buttonNew);
	mainButtonDisp(buttonSetup);
	QVBoxLayout * layoutP = new QVBoxLayout;
	layoutP->setAlignment(Qt::AlignJustify);
	layoutP->addWidget(buttonList);
	layoutP->addWidget(buttonNew);
	layoutP->addWidget(buttonSetup);
	centralZone->setLayout(layoutP);
	
	serverListWidget = NULL;
	playerConfigurationWindow = NULL;
	windowBoss = NULL;
	windowAbout = NULL;
	windowServerWatch = NULL;
    server = NULL;
}

void MainWindow::serverListDisp() {
	if (!serverListWidget) {
		serverListWidget = new ServerListWidget();
		connect(serverListWidget, SIGNAL(newObserver(QString, quint32)), this, SLOT(newObserver(QString, quint32)));
	}
	serverListWidget->show();
}

void MainWindow::serverWatchDisp() {
	if (!windowServerWatch) {
		windowServerWatch = new QDialog();
		windowServerWatch->setWindowIcon(QIcon("images/icone2.png"));
		windowServerWatch->setWindowTitle(tr("Observation des parties en cours"));
		windowServerWatch->setModal(false);
	}
	windowServerWatch->show();
}

void MainWindow::setupDisp() {	
	if (!playerConfigurationWindow) {
		playerConfigurationWindow = new PlayerConfigurationWindow();
	}
	playerConfigurationWindow->show();
}

void MainWindow::windowBossDisp() {
	if (!windowBoss) {
		windowBoss = new QDialog();
		QVBoxLayout * layoutBoss = new QVBoxLayout;
		QLabel * imageBoss = new QLabel(windowBoss);
		imageBoss->setPixmap(QPixmap("images/imageBoss.png"));
		layoutBoss->addWidget(imageBoss);
		windowBoss->setLayout(layoutBoss);
	}
	windowBoss->show();
}

void MainWindow::windowAboutDisp() {
	if (!windowAbout) {
		windowAbout = new QDialog();
		QVBoxLayout * layoutAbout = new QVBoxLayout;
		windowAbout->setWindowIcon(QIcon("images/icone2.png"));
		windowAbout->setWindowTitle(tr("À propos du jeu de dame"));
		//windowAbout->setBackground(QBrush(QPixmap("images/icone2.jpg"))); 
		QLabel * label = new QLabel(tr("Ce logiciel est créé par Guillaune, Ugo et Marine"), windowAbout);
		layoutAbout->addWidget(label);
		windowAbout->setLayout(layoutAbout);
		label->move(30, 20);
	}
	windowAbout->show();
}

void MainWindow::mainButtonDisp(QPushButton * button) {
	button->setFont(QFont("Times", 20, QFont::Bold));
	button->setFixedWidth(250);
}

void MainWindow::newGameFromMenu() {
    if (!server) {
        server = new Server();
	}
    server->makeNewGame();
}

void MainWindow::newObserver(QString hostAddress, quint32 id) {
    std::cout << "MainWindow::newObserver" << std::endl;
    std::cout << "hostAddress : " << hostAddress.toStdString()
                << ", id : " << id << std::endl;
    Checkers * newGame = new Checkers();
    newGame->setServer(hostAddress, id);
    newGame->setClientType(ClientType::OBSERVER);
    games << newGame;
}