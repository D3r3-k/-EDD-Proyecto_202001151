#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <iostream>
#include "models/Estructuras.h"
#include "models/Globales.h"

//* Declaraciones de funciones
void cargarUsuarios();
void cargarRelaciones();
void cargarPublicaciones();
void eliminarMiCuenta();

// Funciones de Menú
void limpiarEntrada();
// TODO: Funciones de Menú Administrador
void menuAdministrador();
void adminGestionarUsuarios();

// TODO: Funciones de Menú Usuario
void menuUsuario();
void userGestionarPerfil();
void userGestionarSolicitudes();

// TODO: Funciones de Menú principal
void menuPrincipal();
void menuIniciarSesion();
void menuRegistrarse();
void menuMostrarInformacion();



#endif // FUNCIONES_H
