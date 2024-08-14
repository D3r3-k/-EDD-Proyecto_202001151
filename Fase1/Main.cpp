#include "models/ListaEnlazadaSimple.h"
#include "models/ListaEnlazadaDoble.h"
#include "models/Globales.h"
#include "Funciones.h"

#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

// Crear una lista de usuarios
ListaEnlazadaUsuarios listaUsuarios;
ListaEnlazadaDoblePublicaciones listaPublicaciones;
// MatrizDispersa matrizRelaciones;

//! Variables globales
Usuario usuario_logeado = Usuario("", "", "", "", "", "");

// TODO: MAIN
int main()
{
    //? Configuración de la consola
    SetConsoleOutputCP(CP_UTF8);

    // Crear un usuario y añadirlo a la lista
    Usuario admin("Administrador", "Local", "2001-09-15", "admin", "123", "admin");
    listaUsuarios.agregarUsuario(admin);

    menuPrincipal();
}
