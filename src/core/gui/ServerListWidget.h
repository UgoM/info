#pragma once

#include <QWidget>

class QTableView;
class QLineEdit;
class ServerList;

/** \class ServerListWidget
  * \brief Display the window to look for servers
  *
  * \todo comment ServerListWidget
  */
class ServerListWidget : public QWidget {

    Q_OBJECT

    ServerList * serverList;
    QTableView * tableView;
    QLineEdit * fieldBroadcastAddress;

    public:
        ServerListWidget();

    private:
        void config();

    private slots:
        void buttonRefresh();
        void refreshDisplay();
        void buttonObserv();
        void buttonConnectAction();
        void textBroadcastAddressChanged(QString);

	signals:
        void newObserver(QString hostAddress, quint32 id, QString gameName);
        void wantsToPlay(QString hostAddress, quint32 id, QString gameName);

};

