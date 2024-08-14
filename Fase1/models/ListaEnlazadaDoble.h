#ifndef LISTAENLAZADADOBLE_H
#define LISTAENLAZADADOBLE_H

#include "Estructuras.h"

// Clase para manejar una lista doblemente enlazada de publicaciones
class ListaEnlazadaDoblePublicaciones {
private:
    struct NodoPublicacion {
        Publicacion publicacion;
        NodoPublicacion* siguiente;
        NodoPublicacion* anterior;

        NodoPublicacion(const Publicacion& p)
            : publicacion(p), siguiente(nullptr), anterior(nullptr) {}
    };

    NodoPublicacion* cabeza;
    NodoPublicacion* cola;

public:
    // Constructor
    ListaEnlazadaDoblePublicaciones() : cabeza(nullptr), cola(nullptr) {}
    // Destructor
    ~ListaEnlazadaDoblePublicaciones();

    // Métodos
    void agregarPublicacion(const Publicacion& publicacion);
    void imprimirPublicaciones() const;
    void eliminarPublicacion(const Publicacion& publicacion);
    void listarPublicaciones();
    

};

#endif // LISTAENLAZADADOBLE_H