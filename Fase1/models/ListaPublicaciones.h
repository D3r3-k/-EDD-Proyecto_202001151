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
        std::string correo_autor;
        std::string contenido;
        std::string fecha;
        std::string hora;
        // Constructor
        Publicacion(int id, std::string ca, const std::string &c, const std::string &f, const std::string &h)
            : id(id), correo_autor(ca), contenido(c), fecha(f), hora(h) {}

        // Métodos
        void mostrarPublicacion() const
        {
            std::cout << "|===============[ Publicación ]===============|" << std::endl;
            std::cout << "| Autor: " << correo_autor << std::endl;
            std::cout << "| Fecha: " << fecha << std::endl;
            std::cout << "| Hora: " << hora << std::endl;
            std::cout << "| Contenido: " << contenido << std::endl;
            std::cout << "|============================================|" << std::endl;
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
        void listarPublicaciones() const
        {
            std::cout << "|               [Lista de Publicaciones]               |" << std::endl;
            std::cout << "|======================================================|" << std::endl;
            NodoPublicacion *temp = cabeza;
            while (temp)
            {
                std::cout << "[" << temp->publicacion.id << "]" << std::endl;
                std::cout << "| Autor: " << temp->publicacion.correo_autor << std::endl;
                std::cout << "| Fecha: " << temp->publicacion.fecha << ", Hora: " << temp->publicacion.hora << std::endl;
                std::cout << "| Contenido: " << temp->publicacion.contenido << std::endl;
                temp = temp->siguiente;
                std::cout << "----------------------------------------------------------------" << endl;
            }
            if (!cabeza)
            {
                std::cout << "| No hay publicaciones                                 |" << std::endl;
            }
            std::cout << "----------------------------------------------------------------" << endl;
        }
        void listarPublicacionesInverso() const
        {
            std::cout << "|               [Lista de Publicaciones]               |" << std::endl;
            std::cout << "|======================================================|" << std::endl;
            NodoPublicacion *temp = cola;
            int contador = 1;
            while (temp)
            {
                std::cout << "| " << contador << " |==================================================|" << std::endl;
                std::cout << "| Publicación de: " << temp->publicacion.correo_autor << std::endl;
                std::cout << "| Fecha: " << temp->publicacion.fecha << ", Hora: " << temp->publicacion.hora << std::endl;
                std::cout << "| Contenido: " << temp->publicacion.contenido << std::endl;
                temp = temp->anterior;
                contador++;
            }
            if (!cola)
            {
                std::cout << "| No hay publicaciones                                 |" << std::endl;
            }
            std::cout << "|======================================================|" << std::endl;
        }
        void eliminarPublicaciones(ListaUsuarios::Usuario usuario_logeado)
        {
            NodoPublicacion *temp = cabeza;
            while (temp)
            {
                if (temp->publicacion.correo_autor == usuario_logeado.correo)
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
            // Verificar si la lista está vacía
            if (!cabeza)
            {
                return;
            }

            NodoPublicacion *temp = cabeza;

            // Recorrer la lista buscando la publicación con el ID especificado
            while (temp)
            {
                if (temp->publicacion.id == id) // Encontrar el nodo con el ID
                {
                    // Caso especial: si es el primer nodo
                    if (temp == cabeza)
                    {
                        cabeza = temp->siguiente; // Mover la cabeza al siguiente nodo
                        if (cabeza)
                        {
                            cabeza->anterior = nullptr; // Desconectar el nuevo primer nodo del anterior
                        }
                    }
                    else
                    {
                        temp->anterior->siguiente = temp->siguiente; // Saltar el nodo actual
                    }

                    // Caso especial: si es el último nodo
                    if (temp == cola)
                    {
                        cola = temp->anterior; // Mover la cola al nodo anterior
                        if (cola)
                        {
                            cola->siguiente = nullptr; // Desconectar el nuevo último nodo del siguiente
                        }
                    }
                    else
                    {
                        if (temp->siguiente)
                        {
                            temp->siguiente->anterior = temp->anterior; // Reconectar el nodo siguiente con el anterior
                        }
                    }

                    // Eliminar el nodo encontrado
                    delete temp;
                    return;
                }

                // Avanzar al siguiente nodo
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
        Publicacion ObtenerPublicacion(const int &post)
        {
            NodoPublicacion *temp = cabeza;
            int contador = 1;
            while (temp)
            {
                if (contador == post)
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