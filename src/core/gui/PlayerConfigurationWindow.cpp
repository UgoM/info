#include "src/core/gui/PlayerConfigurationWindow.h"

PlayerConfigurationWindow::PlayerConfigurationWindow() : QDialog() {
    // Message d'information
    QLabel * infoMessage = new QLabel(tr("Pour l'instant, seule la case pseudo est utilisée"));
	// Groupe caractéristique profile (nom....)
	name = new QLineEdit();
	pseudo = new QLineEdit();
	password = new QLineEdit();
	comment = new QTextEdit();
	QGroupBox * groupComment = new QGroupBox(tr("Ajouter des commentaires"));
	password->setEchoMode(QLineEdit::Password);
	QFormLayout * profileLayout = new QFormLayout;
	profileLayout->addRow(tr("&Nom :"), name);
	profileLayout->addRow(tr("&Pseudo :"), pseudo);
	profileLayout->addRow(tr("&Mot de passe :"), password);
	QGroupBox * groupProfile = new QGroupBox(tr("Nom joueur"));
	groupProfile->setLayout(profileLayout);
	// Groupe Option
	chatStatus = new QCheckBox(tr("Chatter avec l'adversaire"));
	playerStatus = new QCheckBox(tr("Apparaître en ligne"));
	playerStatus->setChecked(true);
	chatStatus->setChecked(true);
	QVBoxLayout * optionsLayout = new QVBoxLayout();
	optionsLayout->addWidget(playerStatus);
	optionsLayout->addWidget(chatStatus);
	QGroupBox * groupOptions = new QGroupBox(tr("Options"));
	groupOptions->setLayout(optionsLayout);
	// Groupe commentaire
	QFormLayout * commentLayout = new QFormLayout();
	commentLayout->addRow(tr("&Commentaires :"), comment);
	groupComment->setCheckable(true);
	groupComment->setChecked(false);
	groupComment->setLayout(commentLayout);
	//bouton création et quit
	QPushButton * createButton = new QPushButton(tr("&Créer !"));
	QPushButton * cancelButton = new QPushButton(tr("&Remise à zéros"));
	connect(createButton, SIGNAL(clicked()), this, SLOT(setupClose()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(setupCancel()));
	QHBoxLayout * buttonsLayout = new QHBoxLayout();
	buttonsLayout->setAlignment(Qt::AlignRight);
	buttonsLayout->addWidget(createButton);
	buttonsLayout->addWidget(cancelButton);

    // remplissage des champs avec les valeurs existantes
    QSettings settings;
    pseudo->setText(settings.value("global/username").toString());

	QVBoxLayout * layoutPrincipal = new QVBoxLayout();
	layoutPrincipal->addWidget(infoMessage);
	layoutPrincipal->addWidget(groupProfile);
	layoutPrincipal->addWidget(groupOptions);
	layoutPrincipal->addWidget(groupComment);
	layoutPrincipal->addLayout(buttonsLayout);

	setLayout(layoutPrincipal);
	setModal(true);
	setWindowIcon(QIcon("icone2.png"));
	setWindowTitle(tr("Configuration joueur"));
}

void PlayerConfigurationWindow::setupClose() {
	//if (name->text().isEmpty()) {	
	//	QMessageBox::critical(this, tr("Nom"), tr("Vous n'avez pas entré de nom..."));
    //} else if (pseudo->text().isEmpty()) {
    if (pseudo->text().isEmpty()) {
		QMessageBox::critical(this, tr("Pseudo"), tr("Vous n'avez pas entré de pseudo..."));
	//} else if (password->text().isEmpty()) {
	//	QMessageBox::critical(this, tr("Mot de passe"), tr("Vous n'avez pas entré de mot de passe..."));
	} else {
		//mettre ici ce qu'il faut faire des données
        QSettings settings;
        settings.setValue("global/username", pseudo->text());
		hide();
	}
}
void PlayerConfigurationWindow::setupCancel() {	
	name->clear();
	pseudo->clear();
	password->clear();
}
