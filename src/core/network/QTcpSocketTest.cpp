#include <iostream>
#include "src/core/network/QTcpSocketTest.h"

QTcpSocketTest::QTcpSocketTest (QObject *p): QTcpSocket(p)
{
    std::cout << "Constructeur QTcpSocketTest" << std::endl;
}

QTcpSocketTest::~QTcpSocketTest()
{
    std::cout << "Destructeur QTcpSocketTest" << std::endl;
}
