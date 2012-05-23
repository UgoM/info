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
HEADERS += 	src/core/type.h \
            src/core/network/TcpServer.h \
            src/core/network/UdpServer.h \
            src/core/network/ServerList.h \
			src/core/network/Server.h \
			src/core/game/Game.h \
			src/core/game/Brain.h \
			src/core/gui/MainWindow.h \
            src/core/gui/ServerListWidget.h \
			src/core/gui/PlayerConfigurationWindow.h \
			src/core/gui/GameInfoWidget.h \
			src/games/checkers/BrainCheckers.h \
			src/games/checkers/BoardController.h \
			src/games/checkers/Direction.h \
            src/games/checkers/Constants.h \
			src/games/checkers/Image.h \
			src/games/checkers/Checkers.h

SOURCES += 	src/core/main.cpp \
			src/core/network/TcpServer.cpp \
            src/core/network/UdpServer.cpp \
            src/core/network/ServerList.cpp \
			src/core/network/Server.cpp \
			src/core/game/Game.cpp \
			src/core/game/Brain.cpp \
			src/core/gui/MainWindow.cpp \
            src/core/gui/ServerListWidget.cpp \
			src/core/gui/PlayerConfigurationWindow.cpp \
			src/core/gui/GameInfoWidget.cpp \
			src/games/checkers/BrainCheckers.cpp \
			src/games/checkers/BoardController.cpp \
			src/games/checkers/Direction.cpp \
			src/games/checkers/Image.cpp \
			src/games/checkers/Checkers.cpp
