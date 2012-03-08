#include "MainWindow.h"
#include "Checkers.h"
#include "ServerList.h"

mainwindow::mainwindow() 
{
	QMdiArea *zoneCentrale = new QMdiArea;

	/* *zoneTexte1 = new QTextEdit;
    QTextEdit *zoneTexte2 = new QTextEdit;

    QMdiSubWindow *sousFenetre1 = zoneCentrale->addSubWindow(zoneTexte1);
    QMdiSubWindow *sousFenetre2 = zoneCentrale->addSubWindow(zoneTexte2);
	*/ //ouverture plusieurs fenêtre

	setCentralWidget(zoneCentrale); //fenetre ppale

	QMenu *menuFile = menuBar()->addMenu("&Partie");
	QMenu *menuEdition = menuBar()->addMenu("&Edition");
    QMenu *menuQuestion = menuBar()->addMenu("&?");

	QAction *actionNew = menuFile->addAction("&Nouvelle partie");
	connect(actionNew, SIGNAL(triggered()),this, SLOT(newGameFromMenu()));
	QAction *actionWatch = menuFile->addAction("&Regarder une partie");
	QAction *actionCancel = menuFile->addAction("&Annuler");
	QAction *actionStat = menuFile->addAction("&Statistique");
	QAction *actionLeave = menuFile->addAction("&Abandonner");
	QAction *actionHide = menuFile->addAction("&Caches toi de ton Boss");
	actionHide->setShortcut(QKeySequence("Ctrl+H"));
	windowBoss = new QDialog;
	connect(actionHide, SIGNAL(triggered()),this, SLOT(windowBossDisp()));
	QAction *actionQuit = menuFile->addAction("&Quitter");
	connect(actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
	actionQuit->setShortcut(QKeySequence("Ctrl+Q"));
	QAction *actionHelp=menuQuestion->addAction("&Afficher l'aide");
	QAction *actionAbout=menuQuestion->addAction("&A propos");
	windowAbout = new QDialog;
	connect(actionAbout, SIGNAL(triggered()),this, SLOT(windowAboutDisp()));
	
   
	//boutons principaux
    QPushButton *buttonList = new QPushButton("Liste serveurs");
    serverListWidget = new ServerListWidget();
	connect(buttonList, SIGNAL(clicked()), this, SLOT(serverListDisp()));
	QPushButton *buttonWatch= new QPushButton("observation parties en cours");
	QPushButton *buttonSetup=new QPushButton("configurer joueur");
	
	mainButtonDisp(buttonList);
	mainButtonDisp(buttonWatch);
	mainButtonDisp(buttonSetup);
	


	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(buttonList);
	layout->addWidget(buttonWatch);
	layout->addWidget(buttonSetup);
	zoneCentrale->setLayout(layout);
	//
    
	windowSetup = new QDialog;
	gamerName = new QLineEdit;
	gamerPseudo = new QLineEdit;
    gamerPassword = new QLineEdit;
	gamerStatute = new QCheckBox("Apparaitre en ligne");
	create = new QPushButton("&Créer !");
    cancel = new QPushButton("&Annuler");
	chatStatute = new QCheckBox("Chatter avec l'adversaire");
	comment= new QTextEdit;
	groupComment = new QGroupBox("Ajouter des commentaires");
	
/////////////////////////:
	gamerPassword->setEchoMode(QLineEdit::Password);

	// Groupe caractéristique profile (nom....)
	QFormLayout *profileLayout = new QFormLayout;
    profileLayout->addRow("&Nom :", gamerName);
	profileLayout->addRow("&Pseudo :", gamerPseudo);
	profileLayout->addRow("&Mot de passe :", gamerPassword);
	QGroupBox *groupProfile = new QGroupBox("Nom joeur");
    groupProfile->setLayout(profileLayout);

	// Groupe Option
	
    gamerStatute->setChecked(true);
   
	chatStatute->setChecked(true);
    QVBoxLayout *optionsLayout = new QVBoxLayout;
    optionsLayout->addWidget(gamerStatute);
    optionsLayout->addWidget(chatStatute);
    QGroupBox *groupOptions = new QGroupBox("Options");
    groupOptions->setLayout(optionsLayout);

	// Groupe commentaire
	
	QFormLayout *commentLayout = new QFormLayout;
	commentLayout->addRow("&Commentaires :", comment);

	
    groupComment->setCheckable(true);
    groupComment->setChecked(false);
    groupComment->setLayout(commentLayout);

	//bouton create et quit
	
	QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->setAlignment(Qt::AlignRight);
	buttonsLayout->addWidget(create);
    buttonsLayout->addWidget(cancel);

	// Layout principal fenetre Setup
	QVBoxLayout *layoutPrincipal = new QVBoxLayout;
    layoutPrincipal->addWidget(groupProfile);
	layoutPrincipal->addWidget(groupOptions);
	layoutPrincipal->addWidget(groupComment);
	layoutPrincipal->addLayout(buttonsLayout);
    windowSetup->setLayout(layoutPrincipal);
	windowSetup->setModal(true);
	windowSetup->setWindowIcon(QIcon("icone2.png"));
	windowSetup->setWindowTitle("configuration joueur");
	//windowSetup->show();

	// connection boutton quit et create
	connect(create, SIGNAL(clicked()), this, SLOT(setupClose()));
	connect(cancel, SIGNAL(clicked()), this, SLOT(setupCancel()));


	///////////////////////:



	connect(buttonSetup, SIGNAL(clicked()), this, SLOT(setupDisp()));
	windowServerWatch = new QDialog;
	connect(buttonWatch, SIGNAL(clicked()), this, SLOT(serverWatchDisp()));
	
	
}
void mainwindow::serverListDisp()
{
	serverListWidget->show();
}
void mainwindow::serverWatchDisp()
{
	windowServerWatch->setWindowIcon(QIcon("icone2.png"));
	windowServerWatch->setWindowTitle("observation parties en cours");
	windowServerWatch->setModal(false);
	windowServerWatch->show();

}
void mainwindow::setupDisp()
{	
	windowSetup->show();
}

void mainwindow::setupClose()
{
	if ( (gamerName->text()).isEmpty() )
    {	
	QMessageBox::critical(windowSetup, "Nom", "Vous n'avez pas entré de nom...snif");
    }
	else if ( (gamerPseudo->text()).isEmpty() )
	{
	QMessageBox::critical(windowSetup, "Pseudo", "Vous n'avez pas entré de pseudo...snif");
	}
	else if ( (gamerPassword->text()).isEmpty() )
	{
	QMessageBox::critical(windowSetup, "Mot de passe", "Vous n'avez pas entré de mot de passe...snif");
	}
	else
	{
	
	SgamerName=gamerName->text();
	SgamerPseudo=gamerPseudo->text();
	SgamerPassword=gamerPassword->text();
	windowSetup->hide();
	}
}
void mainwindow::setupCancel()
{	
	gamerName->clear();
	gamerPseudo->clear();
	gamerPassword->clear();
	
}
void mainwindow::windowBossDisp()
{
	QVBoxLayout *layout2 = new QVBoxLayout;
	QLabel *imageBoss= new QLabel(windowBoss);
	imageBoss->setPixmap(QPixmap("imageBoss.png"));
	layout2->addWidget(imageBoss);
    windowBoss->setLayout(layout2);
	windowBoss->exec();
}
void mainwindow::windowAboutDisp()
{
	windowAbout->setWindowIcon(QIcon("icone2.png"));
	windowAbout->setWindowTitle("A propos du jeu de dame");
	windowAbout->exec();
}



void mainwindow::mainButtonDisp(QPushButton * button)
{
	button->setFont(QFont("Arial",60, QFont::Bold));
}
void mainwindow::log(const char * message,int linenumber)
{

FILE *fichier;
fichier=fopen("erreur.txt","at");
	fprintf(fichier,"Ligne %d, %s\n",linenumber,message);
fclose(fichier);

}


void mainwindow::newGameFromMenu()
{
    /// When Beginning a new game :
    /// - start Server if down
    /// - launch a Brain
    /// - launch a Game
    /// - connect Game to Brain
    /// For now : just start a Game object
    //Checkers * checkers = new Checkers();
    Server * s = new Server();
    s->makeNewGame();
}
