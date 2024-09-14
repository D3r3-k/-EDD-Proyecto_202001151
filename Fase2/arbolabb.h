#ifndef ARBOLABB_H
#define ARBOLABB_H

#include <ctime>
#include "ListaEnlazada.h"
#include "Structs.h"

class ArbolABB
{
private:
    struct Nodo
    {
        std::tm fecha;
        Nodo *der;
        Nodo *izq;
        ListaEnlazada::ListaEnlazada<Structs::Publicacion> publicaciones;

        Nodo(const std::tm& f) : fecha(f), der(nullptr), izq(nullptr) {}
    };

    Nodo *raiz;

    void destruirArbol(Nodo* nodo);
    Nodo* insertar(Nodo* nodo, const std::tm& fecha, const Structs::Publicacion& publicacion);
    Nodo* buscar(Nodo* nodo, const std::tm& fecha) const;
    // Métodos auxiliares para generar el archivo .dot
    void generarDot(Nodo* nodo, std::ostream& out) const;

public:
    ArbolABB();
    ~ArbolABB();

    void insertar(const std::tm& fecha, const Structs::Publicacion& publicacion);
    Nodo* buscar(const std::tm& fecha) const;

    // Método para graficar el arbol en graphviz
    string graficar();
};

#endif // ARBOLABB_H
