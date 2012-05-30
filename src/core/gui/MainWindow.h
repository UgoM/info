#ifndef HEADER_MAINWINDOW
#define HEADER_MAINWINDOW

#include <QtGui>

class Server;
class ServerListWidget;
class PlayerConfigurationWindow;
class Game;
 
class MainWindow : public QMainWindow {

	Q_OBJECT
	
	QDialog * windowBoss;
	QDialog * windowAbout;
	QDialog * windowServerWatch;

	Server * server;
	ServerListWidget * serverListWidget;
	PlayerConfigurationWindow * playerConfigurationWindow;
	QList<Game *> * games;	

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
        void wantsToPlay(QString hostAddress, quint32 id);
        void newGameWindow(QWidget * widget);

};
 
#endif
