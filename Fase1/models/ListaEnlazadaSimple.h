#ifndef LISTA_ENLAZADA_SIMPLE_H
#define LISTA_ENLAZADA_SIMPLE_H

#include <iostream>
#include "Estructuras.h"

//? Clas Lista Simple Enlazada para almacenar usuarios
class ListaEnlazadaUsuarios {
private:
    struct NodoUsuario {
        // Atributos
        Usuario usuario;
        NodoUsuario* siguiente;
        // Constructor
        NodoUsuario(const Usuario& u) : usuario(u), siguiente(nullptr) {}
    };

    NodoUsuario* cabeza;

public:
    // Constructor
    ListaEnlazadaUsuarios() : cabeza(nullptr) {}
    // Destructor
    ~ListaEnlazadaUsuarios();
    // Métodos
    void agregarUsuario(const Usuario& usuario);
    void imprimirUsuarios() const;
    void eliminarUsuarios(const Usuario& usuario);
    void listarUsuarios();

    // Métodos con retorno
    Usuario buscarUsuario(const std::string &correo);
    Usuario buscarUsuario(const std::string &correo, const std::string &contrasena);
    bool eliminarUsuario(const std::string &correo);
    bool existeUsuario(const std::string &correo);

};

#endif // LISTA_ENLAZADA_SIMPLE_H
