#ifndef DEF_SERVERLISTWIDGET
#define DEF_SERVERLISTWIDGET

#include <QWidget>
#include <QStandardItemModel>
#include <QTableView>
#include <QGridLayout>
#include <QPushButton>
#include <QHeaderView>
#include "ServerList.h"

class ServerListWidget : public QWidget {

    Q_OBJECT

    ServerList * serverList;
    QTableView * tableView;

    public:
        ServerListWidget();

    private:
        void config();

    public slots:
        void buttonRefresh();
        void refreshDisplay();
        void buttonObserv();

	signals:
        void newObserver(QString hostAddress, quint32 id);

};

#endif