#ifndef GRAFORELACION_H
#define GRAFORELACION_H

#include "Structs.h"
#include "ListaEnlazada.h"
#include <string>

// Grafo no dirigido
class GrafoRelacion
{
private:
    struct Nodo
    {
        Structs::Usuario *usuario;
        ListaEnlazada::ListaEnlazada<Structs::Usuario> amigos;
        Nodo *siguiente = nullptr;

        Nodo(Structs::Usuario *usuario) : usuario(usuario) {}
        ~Nodo() {}
    };

    Nodo *cabeza = nullptr; // Primer nodo de la lista de nodos (usuarios)
    Nodo *cola = nullptr;   // Último nodo de la lista de nodos (usuarios)

    // Métodos privados
    void insertarNodo(Structs::Usuario &usuario); // Método para insertar un nuevo nodo si no existe
    Nodo *buscarNodo(const std::string &correo) const; // Buscar un nodo por el correo del usuario

public:
    // Constructor
    GrafoRelacion() {}

    // Destructor
    //~GrafoRelacion();

    // Métodos
    void agregarRelacion(Structs::Usuario &user1, Structs::Usuario &user2);
    void eliminarRelacion(const std::string &correo1, const std::string &correo2);
    void eliminarRelacionesUsuario(const std::string &correo);
    ListaEnlazada::ListaEnlazada<Structs::Usuario> obtenerAmigos(const std::string &correo);
    ListaEnlazada::ListaEnlazada<Structs::AmigoSugerido> sugerirAmigos(const std::string &correo);
    bool verificarRelacion(const std::string &correo1, const std::string &correo2);
    bool estaVacio() const;
    std::string graficarGrafo();
    std::string graficarGrafo(const std::string &correo);
    std::string graficarGrafoSugeridos(const std::string &correo);
    std::string tablaAdyacencia();
    void mostrarRelaciones(const std::string &correo);
};

#endif // GRAFORELACION_H
