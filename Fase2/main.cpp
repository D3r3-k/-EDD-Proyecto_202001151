#include "windowmain.h"

#include <QApplication>

#include "globales.h"
#include "Structs.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    Structs::Usuario admin(0, "Administrador", "Lab EDD", "2001-09-15", "admin@gmail.com", "123", "admin");
    lista_usuarios.insertar(admin);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
