#include <QApplication>
#include <QPushButton>
 
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
 
    QPushButton bouton("Salut les Zéros, la forme ?");
    bouton.setText("Pimp mon toto !");
 
    bouton.show();
 
    return app.exec();
}
