#include <iostream>
#include <string>
#include <windows.h>

#include "models/Globales.h"
#include "models/ListaUsuarios.h"
#include "models/ListaPublicaciones.h"
#include "models/MatrisRelacion.h"
#include "models/Menus.h"

//! Declaración de variables globales
ListaUsuarios::ListaEnlazadaSimple lista_usuarios;
ListaPublicaciones::ListaEnlazadaDoble lista_publicaciones;
MatrisRelacion::MatrisDispersa matris_relacion;
ListaUsuarios::Usuario usuario_logeado = ListaUsuarios::Usuario(-1, "", "", "", "", "", "");

using namespace std;

// TODO: MAIN
int main()
{
    //? Configuración de la consola
    SetConsoleOutputCP(CP_UTF8);

    // Crear un usuario y añadirlo a la lista
    ListaUsuarios::Usuario admin(0, "Administrador", "Local", "2001-09-15", "admin", "123", "admin");
    ListaUsuarios::Usuario user(1, "Derek", "Local", "2000-09-16", "user", "123", "user");
    ListaUsuarios::Usuario user2(2, "Kevin", "Local", "2000-08-07", "user2", "123", "user");
    lista_usuarios.agregarUsuario(admin);
    lista_usuarios.agregarUsuario(user);
    lista_usuarios.agregarUsuario(user2);

    // Crear publicaciones y añadirlas a la lista
    // ListaPublicaciones::Publicacion publicacion1(0, "user", "Hola, soy contenido 1", "19-07-2021", "12:45:15");
    // ListaPublicaciones::Publicacion publicacion2(1, "user", "Hola, soy contenido 2", "22-08-2022", "17:25:15");
    // ListaPublicaciones::Publicacion publicacion3(2, "user", "Hola, soy contenido 3", "03-09-2023", "16:30:15");
    // ListaPublicaciones::Publicacion publicacion4(3, "user", "Hola, soy contenido 4", "14-10-2024", "01:17:15");
    // lista_publicaciones.agregarPublicacion(publicacion1);
    // lista_publicaciones.agregarPublicacion(publicacion2);
    // lista_publicaciones.agregarPublicacion(publicacion3);
    // lista_publicaciones.agregarPublicacion(publicacion4);

    menuPrincipal();
}
