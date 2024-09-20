#ifndef FUNCIONES_H
#define FUNCIONES_H

#include "Structs.h"
#include <QTableWidget>
#include <QScrollArea>
#include <string>
#include <QComboBox>
#include <QSpinBox>

using namespace std;

namespace Func {
// GLOBAL PARA ADMIN
extern QTableWidget* adminTablaUsuarios;
// GLOBAL PARA SOLICITUDES - USER
extern QTableWidget* userTablaUsuarios;
extern QTableWidget* userTablaEnviadas;
extern QTableWidget* userTablaRecibidas;
// GLOBAL PARA PUBLICACIONES - USER
extern QScrollArea* userPostFeed;
extern QComboBox* selectedDate;
extern QComboBox* selectedOrder;
extern QSpinBox* countPost;



// TODO: Metodos Login
void IniciarSesion(string email, string password);
// TODO: Metodos Admin
void ActualizarTablaUsuariosAdmin(QTableWidget* table,ListaEnlazada::ListaEnlazada<Structs::Usuario> &lista);
void CargarUsuarios(string directorio);
void CargarSolicitudes(string directorio);
void CargarPublicaciones(string directorio);
// TODO: Metodos Usuario
void ActualizarTablaUsuarios(QTableWidget* table);
void ActualizarTablaRecibidos(QTableWidget* table);
void ActualizarTablaEnviados(QTableWidget* table);
void EliminarMiCuenta();
void EliminarCuenta(string correo);


// TODO: Metodos extras
ListaEnlazada::ListaEnlazada<Structs::Usuario> obtenerListaUsuariosLogeado();
void ActualizarTablaAdmin(int opcion);
void ActualizarTablas();
// Publicaciones
Structs::Publicacion *buscarPost(int id);
void modificarPublicacion(int id, std::string contenido, std::string pathImg);
void eliminarPublicacion(int id);
void eliminarPublicacionUsuario(std::string correo);
int obtenerPostID();
string graficarPublicaciones();
void ComentarPublicacion(int id, StructsComment::Comentario comentario);

void ActualizarFeed();
string convertirFecha(const std::string& fechaOriginal);
string convertirFecha(const std::tm &fecha);
tm convertirFechaTm(const std::string& fechaOriginal);
string convertirFechayHora(const std::string& fechaOriginal);
string convertirFechayHora(const std::tm &fecha);
tm convertirFechayHoraTm(const std::string& fechaOriginal);
}


#endif // FUNCIONES_H
