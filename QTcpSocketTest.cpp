#include <iostream>
#include "QTcpSocketTest.h"

QTcpSocketTest::QTcpSocketTest (QObject *parent): QTcpSocket(parent)
{
    std::cout << "Constructeur QTcpSocketTest" << std::endl;
}

QTcpSocketTest::~QTcpSocketTest()
{
    std::cout << "Destructeur QTcpSocketTest" << std::endl;
}
