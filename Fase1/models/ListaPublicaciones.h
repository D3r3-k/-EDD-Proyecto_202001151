#ifndef LISTAPUBLICACIONES_H
#define LISTAPUBLICACIONES_H

#include <iostream>
#include <string>

#include "ListaUsuarios.h"

using namespace std;

namespace ListaPublicaciones
{
    struct Publicacion
    {
        // Atributos
        int id;
        string correo_autor;
        string contenido;
        string fecha;
        string hora;
        // Constructor
        Publicacion(int id, string ca, const string &c, const string &f, const string &h)
            : id(id), correo_autor(ca), contenido(c), fecha(f), hora(h) {}

        // Métodos
        void mostrarPublicacion() const
        {
            cout << "|===============[ Publicación ]===============|" << endl;
            cout << "| Autor: " << correo_autor << endl;
            cout << "| Fecha: " << fecha << endl;
            cout << "| Hora: " << hora << endl;
            cout << "| Contenido: " << contenido << endl;
            cout << "|============================================|" << endl;
        }
    };

    class ListaEnlazadaDoble
    {
    private:
        struct NodoPublicacion
        {
            Publicacion publicacion;
            NodoPublicacion *siguiente;
            NodoPublicacion *anterior;

            NodoPublicacion(const Publicacion &p)
                : publicacion(p), siguiente(nullptr), anterior(nullptr) {}
        };

        NodoPublicacion *cabeza;
        NodoPublicacion *cola;

    public:
        // Constructor
        ListaEnlazadaDoble() : cabeza(nullptr), cola(nullptr) {}
        // Destructor
        ~ListaEnlazadaDoble()
        {
            while (cabeza)
            {
                NodoPublicacion *temp = cabeza;
                cabeza = cabeza->siguiente;
                delete temp;
            }
        }
        // Métodos
        void agregarPublicacion(const Publicacion &p)
        {
            NodoPublicacion *nuevoNodo = new NodoPublicacion(p);
            if (!cabeza)
            {
                cabeza = nuevoNodo;
                cola = nuevoNodo;
            }
            else
            {
                cola->siguiente = nuevoNodo;
                nuevoNodo->anterior = cola;
                cola = nuevoNodo;
            }
        }
        void eliminarPublicaciones(ListaUsuarios::Usuario *usuario_logeado)
        {
            NodoPublicacion *temp = cabeza;
            while (temp)
            {
                if (temp->publicacion.correo_autor == usuario_logeado->correo)
                {
                    if (temp->anterior)
                    {
                        temp->anterior->siguiente = temp->siguiente;
                    }
                    if (temp->siguiente)
                    {
                        temp->siguiente->anterior = temp->anterior;
                    }
                    if (temp == cabeza)
                    {
                        cabeza = temp->siguiente;
                    }
                    if (temp == cola)
                    {
                        cola = temp->anterior;
                    }
                    delete temp;
                }
                temp = temp->siguiente;
            }
        }
        void eliminarPublicacion(int id)
        {
            if (!cabeza)
                return;
            NodoPublicacion *temp = cabeza;
            while (temp)
            {
                if (temp->publicacion.id == id)
                {
                    if (temp == cabeza)
                    {
                        cabeza = temp->siguiente;
                        if (cabeza)
                        {
                            cabeza->anterior = nullptr;
                        }
                    }
                    else
                    {
                        temp->anterior->siguiente = temp->siguiente;
                    }
                    if (temp == cola)
                    {
                        cola = temp->anterior;
                        if (cola)
                        {
                            cola->siguiente = nullptr;
                        }
                    }
                    else
                    {
                        if (temp->siguiente)
                        {
                            temp->siguiente->anterior = temp->anterior;
                        }
                    }
                    delete temp;
                    return;
                }
                temp = temp->siguiente;
            }
        }

        Publicacion buscarPublicacion(const int &id)
        {
            NodoPublicacion *temp = cabeza;
            while (temp)
            {
                if (temp->publicacion.id == id)
                {
                    return temp->publicacion;
                }
                temp = temp->siguiente;
            }
            return Publicacion(-1, "", "", "", "");
        }
        Publicacion obtenerPublicacion(const int &pos)
        {
            NodoPublicacion *temp = cabeza;
            int contador = 1;
            while (temp)
            {
                if (contador == pos)
                {
                    return temp->publicacion;
                }
                temp = temp->siguiente;
                contador++;
            }
            return Publicacion(-1, "", "", "", "");
        }
        int obtenerUltimoId() const
        {
            NodoPublicacion *temp = cabeza;
            int id = 0;
            while (temp)
            {
                id = temp->publicacion.id;
                temp = temp->siguiente;
            }
            return id;
        }
        int getLength() const
        {
            NodoPublicacion *temp = cabeza;
            int contador = 0;
            while (temp)
            {
                contador++;
                temp = temp->siguiente;
            }
            return contador;
        }
    };

}

#endif // LISTAPUBLICACIONES_H