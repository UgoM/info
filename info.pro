######################################################################
# Automatically generated by qmake (2.01a) lun. f�vr. 6 13:38:26 2012
######################################################################

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
            Board.h \
            Constants.h \
			Piece.h \
			Image.h \
			Checkers.h \
			MainWindow.h

SOURCES += 	main.cpp \
			TcpServer.cpp \
			TcpClient.cpp \
            UdpServer.cpp \
            ServerList.cpp \
			Server.cpp \
			Game.cpp \
			Brain.cpp \
            Board.cpp \
			Image.cpp \
			Checkers.cpp \
			MainWindow.cpp