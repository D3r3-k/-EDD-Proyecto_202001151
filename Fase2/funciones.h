#ifndef FUNCIONES_H
#define FUNCIONES_H

#include "Structs.h"
#include <QTableWidget>
#include <string>

using namespace std;

namespace Func {
extern QTableWidget* adminTablaUsuarios;
extern QTableWidget* userTablaUsuarios;
extern QTableWidget* userTablaEnviadas;
extern QTableWidget* userTablaRecibidas;

// TODO: Metodos Login
void IniciarSesion(string email, string password);
// TODO: Metodos Admin
void ActualizarTabla(QTableWidget* table, ListaEnlazada::ListaEnlazada<Structs::Usuario>& lista);
void CargarUsuarios(string directorio);
void CargarSolicitudes(string directorio);
void CargarPublicaciones(string directorio);
// TODO: Metodos Usuario
void ActualizarTablaUsuarios(QTableWidget* table);
void ActualizarTablaRecibidos(QTableWidget* table);
void ActualizarTablaEnviados(QTableWidget* table);


// TODO: Metodos extras
void enviarSolicitud(Structs::Usuario *emisor, Structs::Usuario *receptor);
ListaEnlazada::ListaEnlazada<Structs::Usuario> obtenerListaUsuariosLogeado();
void ActualizarTablas();
}


#endif // FUNCIONES_H
