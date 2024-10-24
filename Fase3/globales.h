#ifndef _GLOBALES_H
#define _GLOBALES_H

#include "ListaDoble.h"
#include "Structs.h"
#include "arbolavl.h"
#include "graforelacion.h"

extern Structs::Usuario *usuario_logeado;
extern ArbolAVL lista_usuarios;
extern ListaDoble::ListaDoble<Structs::Publicacion> lista_publicaciones;
extern GrafoRelacion relaciones_amistad;

#endif // _GLOBALES_H
