#ifndef LISTAUSUARIOS_H
#define LISTAUSUARIOS_H

#include <iostream>
#include <string>
#include <stack>

#include "ListaSolicitudes.h"

using namespace std;

namespace ListaUsuarios
{
    struct Usuario
    {
        // Atributos
        int id;
        string nombres;
        string apellidos;
        string fechaNacimiento;
        string correo;
        string contrasena;
        string rol;
        // ListaSolicitudes::<tipo> solicitudesEnviadas; // Solicitudes enviadas -> <tipo de lista>
        stack<string> solicitudesRecibidas;              // Solicitudes recibidas -> Pila

        // Constructor
        Usuario(const int &id,
                const string &nombres,
                const string &apellidos,
                const string &fechaNacimiento,
                const string &correo,
                const string &contrasena,
                const string &rol = "user")
            : id(id),
              nombres(nombres),
              apellidos(apellidos),
              fechaNacimiento(fechaNacimiento),
              correo(correo),
              contrasena(contrasena),
              rol(rol) {}

        // Métodos
        void mostrarPerfil() const
        {
            cout << "| ID:                  " << id << endl;
            cout << "| Nombres:             " << nombres << endl;
            cout << "| Apellidos:           " << apellidos << endl;
            cout << "| Fecha de Nacimiento: " << fechaNacimiento << endl;
            cout << "| Correo:              " << correo << endl;
        }
    };

    class ListaEnlazadaSimple
    {
    private:
        struct NodoUsuario
        {
            Usuario usuario;
            NodoUsuario *siguiente;
            NodoUsuario(const Usuario &u) : usuario(u), siguiente(nullptr) {}
        };

        NodoUsuario *cabeza;

    public:
        ListaEnlazadaSimple() : cabeza(nullptr) {}

        ~ListaEnlazadaSimple()
        {
            NodoUsuario *aux = cabeza;
            while (aux != nullptr)
            {
                NodoUsuario *temp = aux;
                aux = aux->siguiente;
                delete temp;
            }
        }

        void agregarUsuario(const Usuario &u)
        {
            NodoUsuario *nuevo = new NodoUsuario(u);
            if (cabeza == nullptr)
            {
                cabeza = nuevo;
            }
            else
            {
                NodoUsuario *aux = cabeza;
                while (aux->siguiente != nullptr)
                {
                    aux = aux->siguiente;
                }
                aux->siguiente = nuevo;
            }
        }

        void listarUsuarios() const
        {
            NodoUsuario *aux = cabeza;
            while (aux != nullptr)
            {
                aux->usuario.mostrarPerfil();
                cout << "|======================================================|" << endl;
                aux = aux->siguiente;
            }
        }

        Usuario *buscarUsuario(const int &pos)
        {
            NodoUsuario *temp = cabeza;
            int contador = 1;
            while (temp)
            {
                if (contador == pos)
                {
                    return &(temp->usuario);
                }
                temp = temp->siguiente;
                contador++;
            }
            return nullptr; // Usuario no encontrado
        }

        Usuario *buscarUsuario(const string &correo, const string &contrasena)
        {
            NodoUsuario *temp = cabeza;
            while (temp)
            {
                if (temp->usuario.correo == correo && temp->usuario.contrasena == contrasena)
                {
                    return &(temp->usuario);
                }
                temp = temp->siguiente;
            }
            return nullptr; // Usuario no encontrado
        }

        Usuario *buscarUsuario(const string &correo)
        {
            NodoUsuario *temp = cabeza;
            while (temp)
            {
                if (temp->usuario.correo == correo)
                {
                    return &(temp->usuario);
                }
                temp = temp->siguiente;
            }
            return nullptr; // Usuario no encontrado
        }

        int getSize()
        {
            NodoUsuario *temp = cabeza;
            int contador = 0;
            while (temp)
            {
                contador++;
                temp = temp->siguiente;
            }
            return contador;
        }

        int getUltimoId()
        {
            NodoUsuario *temp = cabeza;
            int id = 0;
            while (temp)
            {
                id = temp->usuario.id;
                temp = temp->siguiente;
            }
            return id;
        }

        bool existeUsuario(const string &correo)
        {
            NodoUsuario *temp = cabeza;
            while (temp)
            {
                if (temp->usuario.correo == correo)
                {
                    return true;
                }
                temp = temp->siguiente;
            }
            return false;
        }

        void eliminarUsuarios(const Usuario &logeado)
        {
            NodoUsuario *temp = cabeza;
            NodoUsuario *prev = nullptr;
            while (temp)
            {
                if (temp->usuario.correo != logeado.correo)
                {
                    if (prev)
                    {
                        prev->siguiente = temp->siguiente;
                    }
                    else
                    {
                        cabeza = temp->siguiente;
                    }
                    delete temp;
                    temp = prev->siguiente;
                }
                else
                {
                    prev = temp;
                    temp = temp->siguiente;
                }
            }
        }
    };
}

#endif // LISTAUSUARIOS_H
