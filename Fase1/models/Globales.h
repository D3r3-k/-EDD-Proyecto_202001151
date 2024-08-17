#ifndef GLOBALES_H
#define GLOBALES_H

#include "ListaUsuarios.h"
#include "ListaPublicaciones.h"
// #include "ListaPublicacionesFeed.h"
#include "MatrisRelacion.h"

extern ListaUsuarios::Usuario *usuario_logeado;

extern ListaUsuarios::ListaEnlazadaSimple lista_usuarios;
extern ListaPublicaciones::ListaEnlazadaDoble lista_publicaciones;
extern MatrisRelacion::MatrisDispersa matris_relacion;

#endif // GLOBALES_H
