#ifndef LISTAPUBLICACIONESFEED_H
#define LISTAPUBLICACIONESFEED_H

#include <iostream>
#include <string>

#include "Globales.h"

using namespace std;

namespace ListaPublicacionesFeed
{

    struct NodoFeed
    {
        ListaPublicaciones::Publicacion publicacion;
        NodoFeed *siguiente;
        NodoFeed *anterior;

        NodoFeed(const ListaPublicaciones::Publicacion &p)
            : publicacion(p), siguiente(nullptr), anterior(nullptr) {}
    };

    class ListaCircularDoble
    {
    private:
        NodoFeed *cabeza;
        NodoFeed *cola;
        NodoFeed *actual; // Puntero adicional

    public:
        // Constructor
        ListaCircularDoble() : cabeza(nullptr), cola(nullptr), actual(nullptr) {}

        // Destructor
        ~ListaCircularDoble()
        {
            if (!cabeza)
            {
                return;
            }

            NodoFeed *aux = cabeza;

            // Si la lista tiene un solo nodo
            if (cabeza == cola)
            {
                delete cabeza;
                cabeza = nullptr;
                cola = nullptr;
                return;
            }

            // Recorremos y eliminamos todos los nodos en la lista
            do
            {
                NodoFeed *temp = aux;
                aux = aux->siguiente;
                delete temp;
            } while (aux != cabeza); // El bucle se detiene cuando volvemos a la cabeza

            // Establecemos los punteros a nullptr después de la eliminación
            cabeza = nullptr;
            cola = nullptr;
            actual = nullptr;
        }

        // Métodos
        void agregarPublicacion(const ListaPublicaciones::Publicacion &p)
        {
            NodoFeed *nuevo = new NodoFeed(p);
            if (!cabeza)
            {
                cabeza = nuevo;
                cola = nuevo;
                cabeza->siguiente = cabeza;
                cabeza->anterior = cabeza;
                actual = cabeza; // Apuntar al primer nodo
            }
            else
            {
                cola->siguiente = nuevo;
                nuevo->anterior = cola;
                nuevo->siguiente = cabeza;
                cabeza->anterior = nuevo;
                cola = nuevo;
            }
        }
        void eliminarPublicacion(int id)
        {
            if (!cabeza)
            {
                return;
            }

            NodoFeed *aux = cabeza;
            do
            {
                if (aux->publicacion.id == id)
                {
                    if (aux == cabeza && aux == cola) // Caso especial: solo un nodo
                    {
                        delete aux;
                        cabeza = nullptr;
                        cola = nullptr;
                        actual = nullptr; // Ya no hay publicaciones
                        return;
                    }

                    if (aux == cabeza)
                    {
                        cabeza = cabeza->siguiente;
                    }
                    if (aux == cola)
                    {
                        cola = cola->anterior;
                    }

                    // Reconectar los nodos
                    aux->anterior->siguiente = aux->siguiente;
                    aux->siguiente->anterior = aux->anterior;

                    // Actualizar el puntero "actual" si estaba apuntando al nodo eliminado
                    if (aux == actual)
                    {
                        actual = aux->siguiente; // Avanzar a la siguiente publicación
                    }

                    delete aux;
                    return;
                }
                aux = aux->siguiente;
            } while (aux != cabeza);
        }

        void mostrarPublicacionActual()
        {
            if (!cabeza)
            {
                cout << "|                          No hay publicaciones                           |" << endl;
                return;
            }

            // si el id tiene 1 digito
            if (actual->publicacion.id < 10)
            {
                cout << "| [" << actual->publicacion.id << "]                                                                     |" << endl;
            }
            else
            {
                cout << "| [" << actual->publicacion.id << "]                                                                   |" << endl;
            }
            cout << "|                Fecha: " << actual->publicacion.fecha << "          Hora: " << actual->publicacion.hora << "                |" << endl;
            cout << "| Autor: " << actual->publicacion.correo_autor << endl;
            cout << "| Contenido: " << endl
                 << "| " << actual->publicacion.contenido << endl;
            cout << "|=========================================================================|" << endl;
        }
        int obtenerIdActual()
        {
            // Si no hay publicaciones
            if (!cabeza)
            {
                return -1;
            }
            return actual->publicacion.id;
        }
        void avanzarPublicacion()
        {
            if (!cabeza)
            {
                return;
            }

            actual = actual->siguiente;
        }
        void retrocederPublicacion()
        {
            if (!cabeza)
            {
                return;
            }

            actual = actual->anterior;
        }
        void vaciarLista()
        {
            if (!cabeza)
            {
                return; // La lista ya está vacía
            }

            NodoFeed *aux = cabeza;

            // Si la lista tiene solo un nodo
            if (cabeza == cola)
            {
                delete cabeza;
                cabeza = nullptr;
                cola = nullptr;
                actual = nullptr;
                return;
            }

            // Recorremos la lista y eliminamos todos los nodos
            do
            {
                NodoFeed *temp = aux;
                aux = aux->siguiente;
                delete temp;
            } while (aux != cabeza);

            // Una vez eliminados todos los nodos, establecemos los punteros a nullptr
            cabeza = nullptr;
            cola = nullptr;
            actual = nullptr;
        }
    };

}

#endif // LISTAPUBLICACIONESFEED_H