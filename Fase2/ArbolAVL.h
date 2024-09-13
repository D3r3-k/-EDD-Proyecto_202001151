#ifndef ARBOLAVL_H
#define ARBOLAVL_H

#include <string>
#include "Structs.h"

class ArbolAVL
{
private:
    // Estructura del nodo
    struct Nodo
    {
        Structs::Usuario usuario;
        Nodo *izquierda;
        Nodo *derecha;
        int altura;

        // Constructor del nodo
        Nodo(const Structs::Usuario &u)
            : usuario(u), izquierda(nullptr), derecha(nullptr), altura(1) {}
    };

    Nodo *raiz;
    // Metodo para el destructor
    void destruirArbolRecursivo(Nodo *nodo);
    // Métodos privados (helper)
    int obtenerAltura(Nodo *nodo);
    int obtenerBalance(Nodo *nodo);
    Nodo *insertarNodo(Nodo *nodo, const Structs::Usuario &usuario);
    Nodo *eliminarNodo(Nodo *nodo, const string &correo);
    Nodo *buscarNodo(Nodo *nodo, const string &correo);
    Nodo *rotacionIzquierda(Nodo *nodo);
    Nodo *rotacionDerecha(Nodo *nodo);
    Nodo *balancearNodo(Nodo *nodo);
    Nodo *nodoConValorMinimo(Nodo *nodo);

    // Métodos privados de recorrido
    void inOrdenRecursivo(Nodo *nodo, ListaEnlazada::ListaEnlazada<Structs::Usuario> &lista);
    void preOrdenRecursivo(Nodo *nodo, ListaEnlazada::ListaEnlazada<Structs::Usuario> &lista);
    void postOrdenRecursivo(Nodo *nodo, ListaEnlazada::ListaEnlazada<Structs::Usuario> &lista);

    // Método privado para graficar el árbol en formato Graphviz
    void graficarNodo(Nodo *nodo, std::ofstream &archivoDot);
    // Método privado para obtener el usuario con el mayor ID
    Structs::Usuario obtenerUsuarioConMayorID();
public:
    // Constructor y destructor
    ArbolAVL();
    ~ArbolAVL();

    // Funciones públicas
    bool insertar(const Structs::Usuario &usuario);
    bool eliminar(const string &correo);
    bool modificar(const std::string &correo, std::string nombres, std::string apellidos, std::string fecha, std::string contrasena);
    Structs::Usuario *buscar(const string &correo);
    int obtenerId();

    // Métodos para retornar una lista de usuarios
    ListaEnlazada::ListaEnlazada<Structs::Usuario> InOrder();
    ListaEnlazada::ListaEnlazada<Structs::Usuario> PreOrder();
    ListaEnlazada::ListaEnlazada<Structs::Usuario> PostOrder();

    // Método para graficar el arbol en graphviz
    string graficar();

    // Método para eliminar una cuenta de usuario logeado
    bool eliminarSolicitudes(const std::string &correo);
    // Definir futuros métodos (sin implementación)
    bool enviarSolicitud(const std::string &correoEmisor, const std::string &correoReceptor);
    bool cancelarSolicitud(const std::string &correoEmisor, const std::string &correoReceptor);
    bool rechazarSolicitud(const std::string &correoEmisor, const std::string &correoReceptor);
    bool aceptarSolicitud(const std::string &correoEmisor, const std::string &correoReceptor);

};

#endif // ARBOLAVL_H
