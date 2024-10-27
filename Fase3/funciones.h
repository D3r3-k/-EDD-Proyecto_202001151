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
// METODOS DE CARGA
std::string leerArchivo(const std::string& path);
void cargarEstructuras(const std::string& data);
// GLOBAL PARA ADMIN
extern QTableWidget* adminTablaUsuarios;
extern QScrollArea* adminBlockchain;
// GLOBAL PARA SOLICITUDES - USER
extern QTableWidget* userTablaUsuarios;
extern QTableWidget* userTablaEnviadas;
extern QTableWidget* userTablaRecibidas;
extern QTableWidget* userTablaSugerencias;
// GLOBAL PARA PUBLICACIONES - USER
extern QScrollArea* userPostFeed;
extern QScrollArea* userFriends;
extern QComboBox* selectedDate;
extern QComboBox* selectedOrder;
extern QSpinBox* countPost;

// TODO: Metodos Login
void IniciarSesion(string email, string password);
bool RegistrarUsuario(QString nombres, QString apellidos, QString fechaNacimiento, QString correo, QString contrasena);
bool ModificarUsuario(QString nombres, QString apellidos, QString fechaNacimiento, QString correo, std::string contrasena, std::string rol);
void backupSerializacion();
void CerrarSesion();
// TODO: Metodos Admin
void ActualizarTablaUsuariosAdmin(QTableWidget* table,ListaEnlazada::ListaEnlazada<Structs::Usuario> &lista);
void CargarUsuarios(string directorio);
void CargarSolicitudes(string directorio);
void CargarPublicaciones(string directorio);
void EliminarCuenta(string correo);
void ActualizarTablaAdmin(int opcion);
string graficarPublicaciones();
// TODO: Metodos usuario / Publicaciones
int obtenerPostID();
void eliminarPublicacion(int id);
Structs::Publicacion *buscarPost(int id);
void modificarPublicacion(int id, std::string contenido, std::string pathImg);
void ComentarPublicacion(int id, StructsComment::Comentario comentario);
void actualizarListaFechas();
void actualizarArbolPost();
void ActualizarFeed();
void agregarSeguridad();
int obtenerCommentID(int postid);
bool existeComentario(int postid, int cid);

// TODO: Metodos Usuario / Solicitudes
void ActualizarTablaUsuarios(QTableWidget* table);
void ActualizarTablaSugeridos(QTableWidget* table);
void ActualizarTablaRecibidos(QTableWidget* table);
void ActualizarTablaEnviados(QTableWidget* table);
void ActualizarTablas();

// TODO: Metodos usuario / reportes
ListaEnlazada::ListaEnlazada<Structs::ReportePosts> obtenerReporteFechasPost();
ListaEnlazada::ListaEnlazada<Structs::ReportePosts> obtenerReporteFechasPostFriends();
ListaEnlazada::ListaEnlazada<Structs::Publicacion> obtenerMisPosts();
void bubbleSort(ListaEnlazada::ListaEnlazada<Structs::ReportePosts> &lista);
void bubbleSort(ListaEnlazada::ListaEnlazada<Structs::Publicacion> &lista);

// TODO: metodos Usuario / Perfil
void EliminarMiCuenta();
void ActualizarListaAmigos();

// TODO: metodos Blockchain
void ActualizarBlockchains();

// TODO: Metodos extras
ListaEnlazada::ListaEnlazada<Structs::Usuario> obtenerListaUsuariosLogeado();
void eliminarPublicacionUsuario(std::string correo);
string convertirFecha(const std::string& fechaOriginal);
string convertirFecha(const std::tm &fecha);
tm convertirFechaTm(const std::string& fechaOriginal);
string convertirFechayHora(const std::string& fechaOriginal);
string convertirFechayHora(const std::tm &fecha);
tm convertirFechayHoraTm(const std::string& fechaOriginal);
string generarHash(std::string contrasena);
std::string generateTimestamp();
}


#endif // FUNCIONES_H
