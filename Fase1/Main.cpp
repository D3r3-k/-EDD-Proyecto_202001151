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
    ListaUsuarios::Usuario *admin = new ListaUsuarios::Usuario(0, "Administrador", "Lab EDD", "2001-09-15", "admin@gmail.com", "EDD2S2024", "admin");
    lista_usuarios.agregarUsuario(*admin);
    menuPrincipal();
    return 0;
}
