#include "MainWindow.h"
#include "Checkers.h"
#include "ServerList.h"

mainwindow::mainwindow() 
{
	QMdiArea *zoneCentrale = new QMdiArea;


	QBrush arrierePlan; // un QBrush
	arrierePlan.setTexture(QPixmap("images/imageFond2.jpg")); 
	zoneCentrale->setBackground(arrierePlan); // et on l'affecte à la zone centrale
	

	/* *zoneTexte1 = new QTextEdit;
    QTextEdit *zoneTexte2 = new QTextEdit;

    QMdiSubWindow *sousFenetre1 = zoneCentrale->addSubWindow(zoneTexte1);
    QMdiSubWindow *sousFenetre2 = zoneCentrale->addSubWindow(zoneTexte2);
	*/ //ouverture plusieurs fenêtre

	setCentralWidget(zoneCentrale); //fenetre ppale
	zoneCentrale->setFixedSize(345,345);




	QMenu *menuFile = menuBar()->addMenu("&Partie");
    QMenu *menuQuestion = menuBar()->addMenu("&?");

	QAction *actionNew = menuFile->addAction("&Nouvelle partie");
	connect(actionNew, SIGNAL(triggered()),this, SLOT(newGameFromMenu()));
	
	QAction *actionWatch = menuFile->addAction("&Regarder une partie");
	QAction *actionCancel = menuFile->addAction("&Annuler");
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
	QPushButton *buttonNew= new QPushButton("Nouvelle partie");
	connect(buttonNew, SIGNAL(clicked()),this, SLOT(newGameFromMenu()));
	QPushButton *buttonSetup=new QPushButton("Configurer joueur");
	
	mainButtonDisp(buttonList);
	mainButtonDisp(buttonNew);
	mainButtonDisp(buttonSetup);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->setAlignment(Qt::AlignJustify);
	layout->addWidget(buttonList);
	layout->addWidget(buttonNew);
	layout->addWidget(buttonSetup);
	zoneCentrale->setLayout(layout);
	//
    
	windowSetup = new QDialog;
	gamerName = new QLineEdit;
	gamerPseudo = new QLineEdit;
    gamerPassword = new QLineEdit;
	gamerStatute = new QCheckBox("Apparaitre en ligne");
	create = new QPushButton("&Créer !");
    cancel = new QPushButton("&Remise à zéros");
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
	windowSetup->setWindowTitle("Configuration joueur");
	//windowSetup->show();

	// connection boutton quit et create
	connect(create, SIGNAL(clicked()), this, SLOT(setupClose()));
	connect(cancel, SIGNAL(clicked()), this, SLOT(setupCancel()));


	///////////////////////:
	connect(buttonSetup, SIGNAL(clicked()), this, SLOT(setupDisp()));
	windowServerWatch = new QDialog;
	
	
	
}
void mainwindow::serverListDisp()
{
	serverListWidget->show();
}
void mainwindow::serverWatchDisp()
{
	windowServerWatch->setWindowIcon(QIcon("images/icone2.png"));
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
	imageBoss->setPixmap(QPixmap("images/imageBoss.png"));
	layout2->addWidget(imageBoss);
    windowBoss->setLayout(layout2);
	windowBoss->exec();
}
void mainwindow::windowAboutDisp()
{
	windowAbout->setWindowIcon(QIcon("images/icone2.png"));
	windowAbout->setWindowTitle("A propos du jeu de dame");
	//QBrush arrierePlan; // un QBrush
	//arrierePlan.setTexture(QPixmap("images/icone2.jpg")); 
	//windowAbout->setBackground(arrierePlan); 
	
	QLabel *label = new QLabel("Ce logiciel est créé par Guillaune, Ugo et Marine", windowAbout);
    label->move(30, 20);

	windowAbout->exec();
}



void mainwindow::mainButtonDisp(QPushButton * button)
{
	button->setFont(QFont("Times",20, QFont::Bold));
	button->setFixedWidth (250);
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
    if (!server)
        server = new Server();
    server->makeNewGame();
}

