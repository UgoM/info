#ifndef DEF_SERVERLISTWIDGET
#define DEF_SERVERLISTWIDGET

#include <QWidget>
#include <QStandardItemModel>
#include <QTableView>
#include <QGridLayout>
#include <QPushButton>
#include <QHeaderView>
#include <QLineEdit>
#include <QLabel>
#include <QSettings>
#include "ServerList.h"

class ServerListWidget : public QWidget {

    Q_OBJECT

    ServerList * serverList;
    QTableView * tableView;

    QLineEdit * fieldBroadcastAddress;

    public:
        ServerListWidget();

    private:
        void config();

    public slots:
        void buttonRefresh();
        void refreshDisplay();
        void buttonObserv();
        void textBroadcastAddressChanged(QString);

	signals:
        void newObserver(QString hostAddress, quint32 id);

};

#endif