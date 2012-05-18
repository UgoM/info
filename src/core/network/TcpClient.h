#include <QObject>
#include <QtNetwork>
#include <QtCore>

class TcpClient : QObject
{
	Q_OBJECT

	public:
		TcpClient();

	private slots:
		void readData();

	private:
		QTcpSocket * tcpSocket;
};
