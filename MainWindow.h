#ifndef HEADER_MAINWINDOW
#define HEADER_MAINWINDOW


#include <QtGui>
#include <QVBoxLayout>

#include <QApplication>
#include <QWidget>
#include <QPushButton>



#include <QMessageBox>
 
class mainwindow : public QMainWindow
{
	Q_OBJECT

    public:
		mainwindow();
	
	public:
		void log(const char * message, int);
		void mainButtonDisp(QPushButton * button);

	public slots:
		
		void serverListDisp();
		void setupDisp();
		void serverWatchDisp();
		void setupClose();
		void setupCancel();
	

		
		void windowBossDisp();
		void windowAboutDisp();

		

	private:
		
		QDialog *windowBoss;
		QDialog *windowAbout;

		QDialog *windowServerList;


		QDialog *windowSetup;

		QString SgamerName;
		QString SgamerPseudo;
		QString SgamerPassword;
		QLineEdit *gamerName;
		QLineEdit *gamerPassword;
		QLineEdit *gamerPseudo;




		QTextEdit *comment;
		QGroupBox *groupComment;
		QCheckBox *gamerStatute;
		QCheckBox *chatStatute;
		QPushButton *create;
        QPushButton *cancel;

		QDialog *windowServerWatch;


		
		

};
 
#endif