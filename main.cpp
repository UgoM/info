#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include "MainWindow.h"

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);

	// traduction en français ds touches 
	QString locale = QLocale::system().name().section('_', 0, 0);
    QTranslator translator;
    translator.load(QString("qt_") + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&translator);

    MainWindow window;
    window.show();

	return app.exec();
}
