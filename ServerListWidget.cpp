#include "ServerListWidget.h"
#include <QGridLayout>
#include <QPushButton>
#include <QHeaderView>

ServerListWidget::ServerListWidget()
{
    serverList = new ServerList();
	model = new QStandardItemModel();

    connect(serverList, SIGNAL(newList()), this, SLOT(refreshDisplay()));

    tableView = new QTableView();


    QPushButton * buttonGetList = new QPushButton("Rafraichir");
	connect(buttonGetList, SIGNAL(clicked()), this, SLOT(buttonRefresh()));
    QPushButton * buttonConnect = new QPushButton("Connecter");
    QPushButton * buttonWatch = new QPushButton("Observer");


    
	QGridLayout * gridLayout = new QGridLayout();
    gridLayout->addWidget(tableView, 0, 0, 0, 10);
    gridLayout->addWidget(buttonGetList, 1, 0);
    gridLayout->addWidget(buttonConnect, 1, 1);
    gridLayout->addWidget(buttonWatch, 1, 2);

	this->setLayout(gridLayout);

    this->config();



}
void ServerListWidget::config()
{
	this->setWindowIcon(QIcon("images/icone2.png"));
	this->setWindowTitle("Liste des serveurs disponibles");

    tableView->setModel(model);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->verticalHeader()->hide();
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //resultTableView->resizeColumnsToContents();

}


void ServerListWidget::buttonRefresh()
{
    serverList->run();
}
void ServerListWidget::refreshDisplay()
{
	model = serverList->get();
    tableView->setModel(model);
}
