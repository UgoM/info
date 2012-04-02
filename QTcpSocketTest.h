#pragma once

#include <QTcpSocket>

class QTcpSocketTest : public QTcpSocket
{
	Q_OBJECT

	public:
        QTcpSocketTest(QObject * parent);
        ~QTcpSocketTest();
};
