#include "src/core/gui/MainWindow.h"
#include "src/core/gui/ServerListWidget.h"
#include "src/core/gui/PlayerConfigurationWindow.h"
#include "src/core/network/Server.h"
#include "src/core/gui/GameInfoWidget.h"
#include "src/core/gui/ChatWidget.h"
#include "src/core/type.h"
#include "src/core/game/Game.h"
#include "src/core/game/GameSelector.h"

MainWindow::MainWindow() {
	QMdiArea * centralZone = new QMdiArea;
	centralZone->setBackground(QBrush(QPixmap("images/imageFond2.jpg")));	//rajout image de fond d'�cran
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
	QAction *actionAbout = menuQuestion->addAction(tr("&� propos"));
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
    windowGameChoice = NULL;
    server = NULL;
    games = new QList<Game *>;
}
MainWindow::~MainWindow()
{
	delete windowBoss;
	delete windowAbout;
	delete windowServerWatch;
	delete windowGameChoice;
	delete server;
	delete serverListWidget;
	delete playerConfigurationWindow;

    QList<Game *>::iterator itr;
    for (itr = games->begin(); itr != games->end(); itr++)
    {
        Game * toto = *itr;
        delete toto;
        toto = NULL;
    }
	delete games;
}
void MainWindow::serverListDisp() {
	if (!serverListWidget) {
		serverListWidget = new ServerListWidget();
		connect(serverListWidget, SIGNAL(newObserver(QString, quint32, QString)), 
                this, SLOT(newObserver(QString, quint32, QString)));
		connect(serverListWidget, SIGNAL(wantsToPlay(QString, quint32, QString)), 
                this, SLOT(wantsToPlay(QString, quint32, QString)));
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
		windowAbout->setWindowTitle(tr("� propos du jeu de dame"));
		//windowAbout->setBackground(QBrush(QPixmap("images/icone2.jpg"))); 
		QLabel * label = new QLabel(tr("Ce logiciel est cr�� par Guillaune, Ugo et Marine"), windowAbout);
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

/** \brief start a new game
  *
  * Will start a new Brain (game server), managed by Server, and a client (Game)
  * Will connect the client to the Brain as a player
  */
void MainWindow::newGameFromMenu()
{
    // Display game choice
    if (!windowGameChoice)
    {
        windowGameChoice = new QWidget();
        QVBoxLayout * layoutGameChoice = new QVBoxLayout();
        QList<QString> gameList = GameSelector::getGameList();
        QSignalMapper *signalMapper = new QSignalMapper(this);
        for (int i = 0; i < gameList.size(); ++i)
        {
            QPushButton *button = new QPushButton(gameList[i]);
            connect(button, SIGNAL(clicked()), signalMapper, SLOT(map()));
            signalMapper->setMapping(button, gameList[i]);
            layoutGameChoice->addWidget(button);
        }
        connect(signalMapper, SIGNAL(mapped(const QString &)),
                this, SLOT(windowGameChoiceClicked(const QString &)));

        windowGameChoice->setLayout(layoutGameChoice);
    }
    windowGameChoice->show();
}

void MainWindow::windowGameChoiceClicked(const QString & gameName)
{
    qDebug() << "windowGameChoiceClicked" << gameName ;
    windowGameChoice->hide();

    // if no server started, start it    
    if (!server) {
        server = new Server();
	}

    // start the game server
    quint32 port = server->makeNewBrain(gameName);

    // start the game client
    Game * newGame = GameSelector::startNewGame(gameName);
    newGame->setServer("127.0.0.1", port);
    newGame->setClientType(ClientType::PLAYER);

    // create the display
    newGameWindow(newGame);
}

void MainWindow::newObserver(QString hostAddress, quint32 port, QString gameName) {
    qDebug() << "MainWindow::newObserver";
    qDebug() << "hostAddress : " << hostAddress << ", port : " << port;
    Game * newGame = GameSelector::startNewGame(gameName);
    newGame->setServer(hostAddress, port);
    newGame->setClientType(ClientType::OBSERVER);
    games->append(newGame);

    newGameWindow(newGame);
}

void MainWindow::wantsToPlay(QString hostAddress, quint32 port, QString gameName)
{
    qDebug() << "MainWindow::wantsToPlay";
    qDebug() << "hostAddress : " << hostAddress << ", port : " << port;
    Game * newGame = GameSelector::startNewGame(gameName);
    newGame->setServer(hostAddress, port);
    newGame->setClientType(ClientType::PLAYER);
    games->append(newGame);

    newGameWindow(newGame);
    /// \todo regroup newObserver and wantsToPlay in a single function
}

// Make a new window, where the game will be (for players AND observers)
// Will someday contain other widget like a chat, or game infos
void MainWindow::newGameWindow(QWidget * newGame)
{
    // Widget that will be the window
    QWidget * newWindow = new QWidget();
    
    // other widget in the window (arg widget is the game's widget)
    GameInfoWidget * giw = new GameInfoWidget();
    ChatWidget * cw = new ChatWidget();

    // Formating
    QFrame * gif = new QFrame();
    QGridLayout * gig = new QGridLayout();
    gif->setFrameStyle(QFrame::Box | QFrame::Raised);
    gif->setLineWidth(2);
    gig->addWidget(giw);
    gif->setLayout(gig);

    QFrame * cf = new QFrame();
    QGridLayout * cg = new QGridLayout();
    cf->setFrameStyle(QFrame::Box | QFrame::Raised);
    cf->setLineWidth(2);
    cg->addWidget(cw);
    cf->setLayout(cg);

    // Positioning
	QGridLayout * gridLayout = new QGridLayout();
    gridLayout->addWidget(newGame, 0, 0, 10, 10);
    gridLayout->addWidget(gif, 0, 11, 1, 3);
    gridLayout->addWidget(cf, 3, 11, 1, 7);
	newWindow->setLayout(gridLayout);

    newWindow->show();

    // Connecting Game to GameInfoWidget to refresh infos
    connect(newGame, SIGNAL(nConnectedChanged(int, int)), 
        giw, SLOT(nConnectedChanged(int, int)) );
    connect(newGame, SIGNAL(newStatus(QString)), 
        giw, SLOT(statusChanged(QString)) );
    // Connecting Game to ChatWidget to refresh chat
    connect(newGame, SIGNAL(newChatData(QString)), 
        cw, SLOT(newChatData(QString)) );
    connect(cw, SIGNAL(sendDataToServer(QString)),
        newGame, SLOT(sendChatData(QString)) );
}
