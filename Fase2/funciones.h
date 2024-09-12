#ifndef FUNCIONES_H
#define FUNCIONES_H

#include "Structs.h"
#include <QTableWidget>
#include <string>

using namespace std;

namespace Func {
// TODO: Metodos Login
void IniciarSesion(string email, string password);
// TODO: Metodos Admin
void ActualizarTabla(QTableWidget* table, ListaEnlazada::ListaEnlazada<Structs::Usuario>& lista);
void CargarUsuarios(string directorio);
void CargarSolicitudes(string directorio);
void CargarPublicaciones(string directorio);
}


#endif // FUNCIONES_H
