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
    QWidget * windowGameChoice;

	Server * server;
	ServerListWidget * serverListWidget;
	PlayerConfigurationWindow * playerConfigurationWindow;
	QList<Game *> * games;	

    public:
		MainWindow();
		~MainWindow();
	
	public:
		void mainButtonDisp(QPushButton * button);

	public slots:
		
		void serverListDisp();
		void setupDisp();
		void serverWatchDisp();
	    void newGameFromMenu();

		void windowBossDisp();
		void windowAboutDisp();

        void newObserver(QString hostAddress, quint32 id, QString gameName);
        void wantsToPlay(QString hostAddress, quint32 id, QString gameName);
        void newGameWindow(QWidget * widget);

    private slots:
        void windowGameChoiceClicked(const QString & gameName);

};
 
#endif
