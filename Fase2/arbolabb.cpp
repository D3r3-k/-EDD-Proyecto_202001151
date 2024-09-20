#include "arbolabb.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>

// Constructor
ArbolABB::ArbolABB() : raiz(nullptr) {}

// Destructor
ArbolABB::~ArbolABB() {
    destruirArbol(raiz);
}

// Función recursiva para destruir el árbol
void ArbolABB::destruirArbol(Nodo* nodo) {
    if (nodo != nullptr) {
        destruirArbol(nodo->izq);
        destruirArbol(nodo->der);
        delete nodo;
    }
}

// TODO: Metodos privados
// Función recursiva para insertar un nodo en el árbol
ArbolABB::Nodo* ArbolABB::insertar(Nodo* nodo, const std::tm& fecha, const Structs::Publicacion& publicacion) {
    if (nodo == nullptr) {
        Nodo* nuevoNodo = new Nodo(fecha);
        nuevoNodo->publicaciones.insertar(publicacion);
        return nuevoNodo;
    }

    if (mktime(const_cast<std::tm*>(&fecha)) < mktime(&nodo->fecha)) {
        nodo->izq = insertar(nodo->izq, fecha, publicacion);
    } else if (mktime(const_cast<std::tm*>(&fecha)) > mktime(&nodo->fecha)) {
        nodo->der = insertar(nodo->der, fecha, publicacion);
    } else {
        nodo->publicaciones.insertar(publicacion);
    }

    return nodo;
}
// Función recursiva para buscar un nodo en el árbol
ArbolABB::Nodo* ArbolABB::buscar(Nodo* nodo, const std::tm& fecha) const {
    if (nodo == nullptr) {
        return nullptr;
    }
    if (mktime(const_cast<std::tm*>(&fecha)) < mktime(&nodo->fecha)) {
        return buscar(nodo->izq, fecha);
    } else if (mktime(const_cast<std::tm*>(&fecha)) > mktime(&nodo->fecha)) {
        return buscar(nodo->der, fecha);
    }
    return nodo;
}

// Funcion recursiva para obtener las publicaciones de un nodo
ListaEnlazada::ListaEnlazada<Structs::Publicacion> ArbolABB::obtenerPublicaciones(Nodo* nodo, const std::tm& fecha) const {
    if (nodo == nullptr) {
        return ListaEnlazada::ListaEnlazada<Structs::Publicacion>();
    }
    if (mktime(const_cast<std::tm*>(&fecha)) < mktime(&nodo->fecha)) {
        return obtenerPublicaciones(nodo->izq, fecha);
    } else if (mktime(const_cast<std::tm*>(&fecha)) > mktime(&nodo->fecha)) {
        return obtenerPublicaciones(nodo->der, fecha);
    }
    return nodo->publicaciones;
}

// Funcion recursiva para obtener las fechas de los nodos
ListaEnlazada::ListaEnlazada<std::tm> ArbolABB::obtenerFechas(Nodo* nodo) const {
    ListaEnlazada::ListaEnlazada<std::tm> lista;
    if (nodo != nullptr) {
        lista.insertar(nodo->fecha);
        ListaEnlazada::ListaEnlazada<std::tm> izq = obtenerFechas(nodo->izq);
        ListaEnlazada::ListaEnlazada<std::tm> der = obtenerFechas(nodo->der);
        lista.concatenar(izq);
        lista.concatenar(der);
    }
    return lista;
}

// Función recursiva auxiliar para generar el archivo .dot
void ArbolABB::generarDot(Nodo* nodo, std::ostream& out) const {
    if (nodo == nullptr) {
        return;
    }

    // Crear un identificador único basado en la fecha
    std::ostringstream id;
    id << "nodo_" << nodo->fecha.tm_year + 1900 << "_" << nodo->fecha.tm_mon + 1 << "_" << nodo->fecha.tm_mday;
    
    // Escribir el nodo actual
    out << "    \"" << id.str() << "\" [label=\"" << nodo->fecha.tm_mday << "/" << nodo->fecha.tm_mon + 1 << "/" << (nodo->fecha.tm_year + 1900) << "\"];\n";
    
    // Llamar a la función recursivamente para los hijos
    if (nodo->izq != nullptr) {
        std::ostringstream idIzq;
        idIzq << "nodo_" << nodo->izq->fecha.tm_year + 1900 << "_" << nodo->izq->fecha.tm_mon + 1 << "_" << nodo->izq->fecha.tm_mday;
        out << "    \"" << id.str() << "\" -> \"" << idIzq.str() << "\";\n";
        generarDot(nodo->izq, out);
    }
    if (nodo->der != nullptr) {
        std::ostringstream idDer;
        idDer << "nodo_" << nodo->der->fecha.tm_year + 1900 << "_" << nodo->der->fecha.tm_mon + 1 << "_" << nodo->der->fecha.tm_mday;
        out << "    \"" << id.str() << "\" -> \"" << idDer.str() << "\";\n";
        generarDot(nodo->der, out);
    }
}

// Método para recorrer el árbol in-order
void ArbolABB::inOrdenRecursivo(Nodo* nodo, ListaEnlazada::ListaEnlazada<Structs::Publicacion>& lista) {
    if (nodo != nullptr) {
        inOrdenRecursivo(nodo->izq, lista);
        for (int i = 0; i < nodo->publicaciones.size(); ++i) {
            Structs::Publicacion *p = nodo->publicaciones.obtener(i);
            if (p) {
                lista.insertar(*p);
            }
        }
        inOrdenRecursivo(nodo->der, lista);
    }
}

void ArbolABB::preOrdenRecursivo(Nodo* nodo, ListaEnlazada::ListaEnlazada<Structs::Publicacion>& lista) {
    if (nodo != nullptr) {
        for (int i = 0; i < nodo->publicaciones.size(); ++i) {
            Structs::Publicacion *p = nodo->publicaciones.obtener(i);
            if (p) {
                lista.insertar(*p);
            }
        }
        preOrdenRecursivo(nodo->izq, lista);
        preOrdenRecursivo(nodo->der, lista);
    }
}

void ArbolABB::postOrdenRecursivo(Nodo* nodo, ListaEnlazada::ListaEnlazada<Structs::Publicacion>& lista) {
    if (nodo != nullptr) {
        postOrdenRecursivo(nodo->izq, lista);
        postOrdenRecursivo(nodo->der, lista);
        for (int i = 0; i < nodo->publicaciones.size(); ++i) {
            Structs::Publicacion *p = nodo->publicaciones.obtener(i);
            if (p) {
                lista.insertar(*p);
            }
        }
    }
}


// TODO: Metodos publicos
// Función para buscar un nodo en el árbol
ArbolABB::Nodo* ArbolABB::buscar(const std::tm& fecha) const {
    return buscar(raiz, fecha);
}

// Función para insertar un nodo en el árbol o añadir una publicación
void ArbolABB::insertar(const std::tm& fecha, const Structs::Publicacion& publicacion) {
    raiz = insertar(raiz, fecha, publicacion);
}

// Método para generar el archivo .dot y graficar el árbol
std::string ArbolABB::graficar() {
    // Crear el archivo .dot
    std::ofstream file("arbolABB.dot");
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo para graficar." << std::endl;
        return "";
    }

    // Comenzar a escribir el contenido del archivo .dot
    file << "digraph ArbolABB {\n";
    file << "    node [shape=circle];\n";
    
    // Llamar al método auxiliar para generar los nodos
    if (raiz != nullptr) {
        generarDot(raiz, file);
    }

    file << "}\n";
    file.close();

    // Generar la imagen con Graphviz usando system()
    std::string command = "dot -Tpng arbolABB.dot -o arbolABB.png";
    system(command.c_str());

    // Devolver la ruta de la imagen generada
    return "arbolABB.png";
}

// Método para limpiar el árbol
void ArbolABB::limpiar() {
    destruirArbol(raiz);
    raiz = nullptr;
}

// Metodo para obtener las publicaciones de un nodo según la fecha
ListaEnlazada::ListaEnlazada<Structs::Publicacion> ArbolABB::obtenerPublicaciones(const std::tm& fecha) const {
    return obtenerPublicaciones(raiz, fecha);
}

// Método para obtener la lista de fechas
ListaEnlazada::ListaEnlazada<std::tm> ArbolABB::obtenerFechas() const {
    return obtenerFechas(raiz);
}

// Método para obtener una lista enlazada de publicaciones en orden in-order
ListaEnlazada::ListaEnlazada<Structs::Publicacion> ArbolABB::inorder(const int cantidad) {
    ListaEnlazada::ListaEnlazada<Structs::Publicacion> lista;
    inOrdenRecursivo(raiz, lista);
    // dejar solo la cantidad de publicaciones en la lista
    for (int i = lista.size(); i > cantidad; --i) {
        lista.eliminarFinal();
    }
    return lista;
}

ListaEnlazada::ListaEnlazada<Structs::Publicacion> ArbolABB::preorder(const int cantidad) {
    ListaEnlazada::ListaEnlazada<Structs::Publicacion> lista;
    preOrdenRecursivo(raiz, lista);
    // dejar solo la cantidad de publicaciones en la lista
    for (int i = lista.size(); i > cantidad; --i) {
        lista.eliminarFinal();
    }
    return lista;
}

ListaEnlazada::ListaEnlazada<Structs::Publicacion> ArbolABB::postorder(const int cantidad) {
    ListaEnlazada::ListaEnlazada<Structs::Publicacion> lista;
    postOrdenRecursivo(raiz, lista);
    // dejar solo la cantidad de publicaciones en la lista
    for (int i = lista.size(); i > cantidad; --i) {
        lista.eliminarFinal();
    }
    return lista;
}

// Método para obtener la lista de publicaciones de un nodo según la fecha, orden y cantidad
ListaEnlazada::ListaEnlazada<Structs::Publicacion> ArbolABB::obtenerPublicaciones(const std::tm &fecha, const int orden, int cantidad) {
    ListaEnlazada::ListaEnlazada<Structs::Publicacion> lista;
    if (orden == 0)
    {
        inOrdenRecursivo(raiz, lista);
    }
    else if (orden == 1)
    {
        preOrdenRecursivo(raiz, lista);
    }
    else if (orden == 2)
    {
        postOrdenRecursivo(raiz, lista);
    }

    // dejar solo la cantidad de publicaciones en la lista
    for (int i = lista.size(); i > cantidad; --i) {
        lista.eliminarFinal();
    }


    return lista;
}

