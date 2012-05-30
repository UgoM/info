#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include "src/core/gui/MainWindow.h"

void initConfig();

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);

    // traduction en français ds touches
    QString locale = QLocale::system().name().section('_', 0, 0);
    QTranslator translator;
    translator.load(QString("qt_") + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&translator);

    // options pour QSettings
    QCoreApplication::setOrganizationName("projetInfo");
    QCoreApplication::setOrganizationDomain("projetInfo.fr");
    QCoreApplication::setApplicationName("info");
    initConfig();

    MainWindow window;
    window.show();

return app.exec();
}

/** initialize, and check integrity of the configuration file,
  * if a key is missing, it adds it with default value
  * default keys/values are in default.conf
  */
void initConfig()
{
    // program configuration
    QSettings settings;

    // default configuration
    QSettings defSettings("./default.conf", QSettings::IniFormat);
    
    QStringList allDefaultKeys = defSettings.allKeys();

    QString key;
    foreach (key, allDefaultKeys) {
        if (!settings.contains(key)) {
            settings.setValue(key, defSettings.value(key));
            qDebug() << "put default value (" << defSettings.value(key) << ")"
                        << "for missing key (" << key << ")"
                        << " in configuration file";
        }
    }
}
