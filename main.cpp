#include <QApplication>
#include <QPushButton>
 
 
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
 
    QPushButton bouton("Salut les Zéros, la forme ?");
    bouton.setText("Pimp mon bouton !");
 
    bouton.show();
 
    return app.exec();
}

