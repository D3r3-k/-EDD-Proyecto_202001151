#include "windowmain.h"

#include <QApplication>

#include "globales.h"
#include "Structs.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    Structs::Usuario admin(0, "Administrador", "Lab EDD", "2001-09-15", "admin@gmail.com", "123", "admin");
    lista_usuarios.insertar(admin);
    Structs::Usuario user1(1, "Usuario", "Lab EDD", "2001-09-15", "user@gmail.com", "123", "user");
    lista_usuarios.insertar(user1);
    Structs::Usuario user2(2, "Usuario2", "Lab EDD", "2001-09-15", "user2@gmail.com", "123", "user");
    lista_usuarios.insertar(user2);
    Structs::Usuario user3(3, "Usuario3", "Lab EDD", "2001-09-15", "user3@gmail.com", "123", "user");
    lista_usuarios.insertar(user3);
    Structs::Usuario user4(4, "Usuario4", "Lab EDD", "2001-09-15", "user4@gmail.com", "123", "user");
    lista_usuarios.insertar(user4);
    Structs::Usuario user5(5, "Usuario5", "Lab EDD", "2001-09-15", "user5@gmail.com", "123", "user");
    lista_usuarios.insertar(user5);

    // Usuario 1 envía solicitud a usuario 2, 3 y 4
    bool enviar1 = lista_usuarios.enviarSolicitud("user@gmail.com", "user2@gmail.com");
    bool enviar2 = lista_usuarios.enviarSolicitud("user@gmail.com", "user3@gmail.com");
    bool enviar3 = lista_usuarios.enviarSolicitud("user@gmail.com", "user4@gmail.com");

    // Usuario 1 recibe solicitud de usuario 5 y admin
    bool enviar4 = lista_usuarios.enviarSolicitud("user5@gmail.com", "user@gmail.com");
    bool enviar5 = lista_usuarios.enviarSolicitud("admin@gmail.com", "user@gmail.com");


    qInfo() << "1 Solicitud enviada: " << enviar1;
    qInfo() << "2 Solicitud enviada: " << enviar2;
    qInfo() << "3 Solicitud enviada: " << enviar3;
    qInfo() << "4 Solicitud enviada: " << enviar4;
    qInfo() << "5 Solicitud enviada: " << enviar5;



    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
