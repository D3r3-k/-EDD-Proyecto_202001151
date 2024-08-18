#include <iostream>
#include <string>
#include <windows.h>

#include "models/Globales.h"
#include "models/ListaUsuarios.h"
#include "models/ListaPublicaciones.h"
#include "models/MatrizRelacion.h"
#include "models/Menus.h"

//! Declaración de variables globales
ListaUsuarios::ListaEnlazadaSimple lista_usuarios;
ListaPublicaciones::ListaEnlazadaDoble lista_publicaciones;
MatrizRelacion::MatrizDispersa matriz_relacion;
ListaUsuarios::Usuario *usuario_logeado = nullptr;

using namespace std;

// TODO: MAIN
int main()
{
    //? Configuración de la consola
    SetConsoleOutputCP(CP_UTF8);

    // Crear un usuario y añadirlo a la lista
    ListaUsuarios::Usuario *admin = new ListaUsuarios::Usuario(0, "Administrador", "Local", "2001-09-15", "admin", "123", "admin");
    lista_usuarios.agregarUsuario(*admin);

    ListaUsuarios::Usuario *user = new ListaUsuarios::Usuario(1, "Derek", "Local", "2000-09-16", "kai", "123", "user");
    lista_usuarios.agregarUsuario(*user);
    ListaUsuarios::Usuario *user2 = new ListaUsuarios::Usuario(2, "Kevin", "Local", "2000-08-07", "chanko", "123", "user");
    lista_usuarios.agregarUsuario(*user2);

    // matriz_relacion.agregarRelacion(user, user2);

    menuPrincipal();

    return 0;
}
