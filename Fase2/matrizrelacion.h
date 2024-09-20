#ifndef MATRIZRELACION_H
#define MATRIZRELACION_H

#include "Structs.h"

class MatrizRelacion
{
private:
    struct Nodo
    {
        Structs::Usuario *usuario1, *usuario2;
        Nodo *siguiente;
        Nodo *anterior;
        Nodo *arriba;
        Nodo *abajo;

        Nodo(Structs::Usuario *u1 = nullptr, Structs::Usuario *u2 = nullptr)
            : usuario1(u1), usuario2(u2), siguiente(nullptr), anterior(nullptr), arriba(nullptr), abajo(nullptr) {}
    };

    Nodo *cabeza;
    // Métodos privados para gestión interna de la matriz
    Nodo *buscarFila(Structs::Usuario *u1);
    Nodo *buscarColumna(Structs::Usuario *u2);
    bool NodoRelacionExiste(Nodo*fila, Structs::Usuario *u2);
    Nodo *insertarFilaCabecera(Structs::Usuario *user);
    Nodo *insertarColumnaCabecera(Structs::Usuario *user);
    void insertarEnFila(Nodo*nuevo, Nodo*filaCabecera);
    void insertarEnColumna(Nodo *nuevo, Nodo *columnaCabecera);

public:
    // Constructor
    MatrizRelacion() : cabeza(nullptr) {}

    // Destructor
    ~MatrizRelacion();

    // Métodos
    void agregarRelacion(Structs::Usuario *user1, Structs::Usuario *user2);
    void eliminarRelacionesUsuario(string correo_usuario);
    bool verificarRelacion(string correo1, string correo2);
    std::string graficar();
    ListaEnlazada::ListaEnlazada<Structs::Usuario> obtenerAmigos(string correo);
    bool estaVacio();
};

#endif // MATRIZRELACION_H
