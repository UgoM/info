TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .
QT += network core testlib
OBJECTS_DIR = objects/
MOC_DIR = moc_dir/
DESTDIR = ./
QMAKE_CXXFLAGS += -Wshadow 

# Input
HEADERS += 	TcpServer.h \
			TcpClient.h \
            UdpServer.h \
            ServerList.h \
			Server.h \
			Game.h \
			Brain.h \
			BrainCheckers.h \
			BoardController.h \
			Direction.h \
            Constants.h \
			Image.h \
			Checkers.h \
			MainWindow.h \
            ServerListWidget.h \
			PlayerConfigurationWindow.h \
            QTcpSocketTest.h

SOURCES += 	main.cpp \
			TcpServer.cpp \
			TcpClient.cpp \
            UdpServer.cpp \
            ServerList.cpp \
			Server.cpp \
			Game.cpp \
			Brain.cpp \
			BrainCheckers.cpp \
			BoardController.cpp \
			Direction.cpp \
			Image.cpp \
			Checkers.cpp \
			MainWindow.cpp \
            ServerListWidget.cpp \
			PlayerConfigurationWindow.cpp \
            QTcpSocketTest.cpp
