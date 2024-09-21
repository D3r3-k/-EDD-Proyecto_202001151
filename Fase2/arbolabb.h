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

        Nodo(const std::tm &f) : fecha(f), der(nullptr), izq(nullptr) {}
    };

    Nodo *raiz;

    void destruirArbol(Nodo *nodo);
    Nodo *insertar(Nodo *nodo, const std::tm &fecha, const Structs::Publicacion &publicacion);
    Nodo *buscar(Nodo *nodo, const std::tm &fecha) const;
    // Métodos auxiliares para generar el archivo .dot
    void generarDot(Nodo *nodo, std::ostream &out) const;

    // Métodos auxiliares para obtener las publicaciones de un nodo
    ListaEnlazada::ListaEnlazada<Structs::Publicacion> obtenerPublicaciones(Nodo *nodo, const std::tm &fecha) const;
    ListaEnlazada::ListaEnlazada<Structs::Publicacion> obtenerPublicaciones(Nodo *nodo, const std::tm &fecha, const int orden, int cantidad) const;

    // Métodos auxiliares para retornar una lista de publicaciones (inorder, preorder, postorder)
    void inOrdenRecursivo(Nodo *nodo, ListaEnlazada::ListaEnlazada<Structs::Publicacion> &lista);
    void inOrdenRecursivo(Nodo* nodo, const std::tm& fecha, ListaEnlazada::ListaEnlazada<Structs::Publicacion>& lista);
    void preOrdenRecursivo(Nodo *nodo, ListaEnlazada::ListaEnlazada<Structs::Publicacion> &lista);
    void preOrdenRecursivo(Nodo* nodo, const std::tm& fecha, ListaEnlazada::ListaEnlazada<Structs::Publicacion>& lista);
    void postOrdenRecursivo(Nodo *nodo, ListaEnlazada::ListaEnlazada<Structs::Publicacion> &lista);
    void postOrdenRecursivo(Nodo* nodo, const std::tm& fecha, ListaEnlazada::ListaEnlazada<Structs::Publicacion>& lista);

    // Método auxiliar para obtener las fechas de los nodos
    ListaEnlazada::ListaEnlazada<std::tm> obtenerFechas(Nodo *nodo) const;

public:
    ArbolABB();
    ~ArbolABB();

    void insertar(const std::tm &fecha, const Structs::Publicacion &publicacion);
    Nodo *buscar(const std::tm &fecha) const;

    // Método para graficar el arbol en graphviz
    string graficar();

    // Método para limpiar el arbol
    void limpiar();

    // metodo para obtener la lista de fechas
    ListaEnlazada::ListaEnlazada<std::tm> obtenerFechas() const;

    // Método para obtener la lista de publicaciones de un nodo según la fecha
    ListaEnlazada::ListaEnlazada<Structs::Publicacion> obtenerPublicaciones(const std::tm &fecha) const;

    // Método para obtener la lista de publicaciones de un nodo según la fecha, orden y cantidad
    // Método para obtener la lista de publicaciones de un nodo según la fecha, orden y cantidad
    ListaEnlazada::ListaEnlazada<Structs::Publicacion> obtenerPublicaciones(const std::tm &fecha, const int orden, int cantidad);

    // Métodos para retornar una lista de publicaciones (inorder, preorder, postorder)
    ListaEnlazada::ListaEnlazada<Structs::Publicacion> inorder(const int cantidad);
    ListaEnlazada::ListaEnlazada<Structs::Publicacion> preorder(const int cantidad);
    ListaEnlazada::ListaEnlazada<Structs::Publicacion> postorder(const int cantidad);

    // Método para obtener una lista de nodos ordenadas por cantidad de publicaciones
    ListaEnlazada::ListaEnlazada<Nodo *> obtenerNodosOrdenados() const;
};

#endif // ARBOLABB_H
