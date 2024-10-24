#include "funciones.h"
#include "windowlogin.h"

#include <QApplication>

#include "globales.h"
#include "Structs.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    std::string pass = "123";
    std::string hashpass = Func::generarHash(pass);
    Structs::Usuario admin(0, "Administrador", "Lab EDD", "2001-09-15", "admin@gmail.com", hashpass,"admin");
    lista_usuarios.insertar(admin);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
