#pragma once

#include "ServerList.h"
#include <QWidget>
#include <QStandardItemModel>
#include <QTableView>

class ServerListWidget : public QWidget
{
    Q_OBJECT

    ServerList * serverList;
    QStandardItemModel * model;
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
        void newObserver( QString hostAddress, quint32 id );

};
