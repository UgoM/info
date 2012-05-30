#include <QtGui>
#include <QSettings>

#include "src/core/gui/ServerListWidget.h"
#include "src/core/network/ServerList.h"

ServerListWidget::ServerListWidget() {
    serverList = new ServerList();
	tableView = new QTableView();
    connect(serverList, SIGNAL(newList()), this, SLOT(refreshDisplay()));
    QPushButton * buttonGetList = new QPushButton(tr("Rafraîchir"));
    QPushButton * buttonConnect = new QPushButton(tr("Connecter"));
    QPushButton * buttonWatch = new QPushButton(tr("Observer"));
    QLabel      * labelBroadcast = new QLabel(tr("Broadcast Address :"));
    fieldBroadcastAddress = new QLineEdit();
	connect(buttonGetList, SIGNAL(clicked()), this, SLOT(buttonRefresh()));
	connect(buttonConnect, SIGNAL(clicked()), this, SLOT(buttonConnectAction()));
	connect(buttonWatch, SIGNAL(clicked()), this, SLOT(buttonObserv()));
    connect(fieldBroadcastAddress, SIGNAL(textChanged(QString)),
                    this, SLOT(textBroadcastAddressChanged(QString)));
	QGridLayout * gridLayout = new QGridLayout();
    gridLayout->addWidget(tableView, 0, 0, 1, 10);
    gridLayout->addWidget(buttonGetList, 1, 0);
    gridLayout->addWidget(buttonConnect, 1, 1);
    gridLayout->addWidget(buttonWatch, 1, 2);
    gridLayout->addWidget(labelBroadcast, 2, 0);
    gridLayout->addWidget(fieldBroadcastAddress, 2, 1, 1, 2);
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

    // import setting
    QSettings settings;
    QStringList broadcastAddresses = settings.value("network/broadcastAdresses").toStringList();
    QString text = broadcastAddresses.join(", ");
    fieldBroadcastAddress->setText(text);
}

// When the Broadcast Addresses are manually changed, it is saved to 
// the configuration file
void ServerListWidget::textBroadcastAddressChanged(QString text)
{
    QSettings settings;
    settings.setValue("network/broadcastAdresses", text.split(","));
}

void ServerListWidget::buttonRefresh() {
    serverList->run();
}

void ServerListWidget::refreshDisplay() {
    tableView->setModel(serverList->get());
}

void ServerListWidget::buttonObserv() {
    qDebug() << "ServerListWidget::buttonObserv()";
    QModelIndex index = tableView->currentIndex();
    QString hostAddress = index.data(Qt::UserRole).toString();
    quint32 port = index.data(Qt::UserRole + 1).toInt();
    emit newObserver(hostAddress, port);
}

void ServerListWidget::buttonConnectAction()
{
    qDebug() << "ServerListWidget::buttonConnectAction()";
    QModelIndex index = tableView->currentIndex();
    QString hostAddress = index.data(Qt::UserRole).toString();
    quint32 port = index.data(Qt::UserRole + 1).toInt();
    emit wantsToPlay(hostAddress, port);
}
