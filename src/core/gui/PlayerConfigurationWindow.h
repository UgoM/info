#ifndef DEF_PLAYER_CONFIGURATION_WINDOW
#define DEF_PLAYER_CONFIGURATION_WINDOW

#include <QDialog>
#include <QTextEdit>
#include <QCheckBox>
#include <QLineEdit>
#include <QGroupBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QPushButton>

class PlayerConfigurationWindow : public QDialog {

	Q_OBJECT
	
	QLineEdit * name;
	QLineEdit * pseudo;
	QLineEdit * password;
	QTextEdit * comment;
	QCheckBox * chatStatus;
	QCheckBox * playerStatus;
	
	public:
		PlayerConfigurationWindow();
		
	public slots:
		void setupClose();
		void setupCancel();

};

#endif