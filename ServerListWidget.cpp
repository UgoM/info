#include "ServerListWidget.h"
#include <iostream>

ServerListWidget::ServerListWidget() {
    serverList = new ServerList();
	tableView = new QTableView();
    connect(serverList, SIGNAL(newList()), this, SLOT(refreshDisplay()));
    QPushButton * buttonGetList = new QPushButton(tr("Rafraîchir"));
    QPushButton * buttonConnect = new QPushButton(tr("Connecter"));
    QPushButton * buttonWatch = new QPushButton(tr("Observer"));
	connect(buttonGetList, SIGNAL(clicked()), this, SLOT(buttonRefresh()));
	connect(buttonWatch, SIGNAL(clicked()), this, SLOT(buttonObserv()));
	QGridLayout * gridLayout = new QGridLayout();
    gridLayout->addWidget(tableView, 0, 0, 0, 10);
    gridLayout->addWidget(buttonGetList, 1, 0);
    gridLayout->addWidget(buttonConnect, 1, 1);
    gridLayout->addWidget(buttonWatch, 1, 2);
	setLayout(gridLayout);
    config();
}
void ServerListWidget::config() {
	setWindowIcon(QIcon("images/icone2.png"));
	setWindowTitle(tr("Liste des serveurs disponibles"));
    tableView->setModel(new QStandardItemModel());
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->verticalHeader()->hide();
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //resultTableView->resizeColumnsToContents();
}

void ServerListWidget::buttonRefresh() {
    serverList->run();
}

void ServerListWidget::refreshDisplay() {
    tableView->setModel(serverList->get());
}

void ServerListWidget::buttonObserv() {
    std::cout << "ServerListWidget::buttonObserv()" << std::endl;
    QModelIndex index = tableView->currentIndex();
    QString hostAddress = index.data(Qt::UserRole).toString();
    quint32 port = index.data(Qt::UserRole + 1).toInt();
    emit newObserver(hostAddress, port);
}
