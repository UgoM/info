#ifndef DEF_SERVERLISTWIDGET
#define DEF_SERVERLISTWIDGET

#include <QtGui>
#include <QSettings>
#include "src/core/network/ServerList.h"

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
