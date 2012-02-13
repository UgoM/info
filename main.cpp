#include <QApplication>
#include <QtCore>

#include "Server.h"
#include "TcpClient.h"

#include <QPushButton>

 
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
 
    QPushButton bouton("Salut les Zéros, la forme ? Test");
    bouton.setText("Pimp mon encore !");
 
    bouton.show();

	Server * server = new Server();

	TcpClient * tcpClient = new TcpClient();

	
    return app.exec();
}
