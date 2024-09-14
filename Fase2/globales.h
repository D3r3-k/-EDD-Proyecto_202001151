#ifndef _GLOBALES_H
#define _GLOBALES_H

#include "ListaDoble.h"
#include "Structs.h"
#include "arbolavl.h"
#include "matrizrelacion.h"

extern Structs::Usuario *usuario_logeado;
extern ArbolAVL lista_usuarios;
extern ListaDoble::ListaDoble<Structs::Publicacion> lista_publicaciones;
extern MatrizRelacion relaciones_amistad;


#endif // _GLOBALES_H
