#ifndef GLOBALES_H
#define GLOBALES_H

#include "ListaUsuarios.h"
#include "ListaPublicaciones.h"
#include "MatrizRelacion.h"

extern ListaUsuarios::Usuario *usuario_logeado;

extern ListaUsuarios::ListaEnlazadaSimple lista_usuarios;
extern ListaPublicaciones::ListaEnlazadaDoble lista_publicaciones;
extern MatrizRelacion::MatrizDispersa matriz_relacion;

#endif // GLOBALES_H
