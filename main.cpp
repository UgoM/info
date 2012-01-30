#include <QApplication>
#include <QPushButton>
 
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
 
    QPushButton bouton("Salut les Zéros, la forme ? Test");
    bouton.setText("Pimp mon encore !");
 
    bouton.show();
 
    return app.exec();
}
