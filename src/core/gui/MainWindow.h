#ifndef HEADER_MAINWINDOW
#define HEADER_MAINWINDOW

#include <QtGui>

#include "src/core/gui/ServerListWidget.h"
#include "src/core/gui/PlayerConfigurationWindow.h"
#include "src/games/checkers/Checkers.h"
#include "src/core/network/ServerList.h"
#include "src/core/gui/GameInfoWidget.h"
 
class MainWindow : public QMainWindow {

	Q_OBJECT
	
	QDialog * windowBoss;
	QDialog * windowAbout;
	QDialog * windowServerWatch;

	Server * server;
	ServerListWidget * serverListWidget;
	PlayerConfigurationWindow * playerConfigurationWindow;
	QList<Game *> games;	

    public:
		MainWindow();
	
	public:
		void mainButtonDisp(QPushButton * button);

	public slots:
		
		void serverListDisp();
		void setupDisp();
		void serverWatchDisp();
	    void newGameFromMenu();

		void windowBossDisp();
		void windowAboutDisp();

        void newObserver(QString hostAddress, quint32 id);
        void newGameWindow(QWidget * widget);

};
 
#endif
