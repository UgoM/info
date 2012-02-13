#include <QObject>

#include <QTcpSocket>

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
