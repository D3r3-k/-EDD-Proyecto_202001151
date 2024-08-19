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
        NodoFeed *actual;

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
            if (actual->publicacion.id < 10)
            {
                cout << "| [" << actual->publicacion.id << "]                                                                     |" << endl;
            }
            else
            {
                cout << "| [" << actual->publicacion.id << "]                                                                    |" << endl;
            }
            cout << "|                Fecha: " << actual->publicacion.fecha << "          Hora: " << actual->publicacion.hora << "                 |" << endl;
            cout << "| Autor: " << actual->publicacion.correo_autor << endl;
            cout << "| Contenido: " << endl
                 << "| " << actual->publicacion.contenido << endl;
            cout << "|=========================================================================|" << endl;
        }
        int obtenerIdActual()
        {
            if (!cabeza)
                return -1;
            return actual->publicacion.id;
        }
        void avanzarPublicacion()
        {
            if (!cabeza)
                return;
            actual = actual->siguiente;
        }
        void retrocederPublicacion()
        {
            if (!cabeza)
                return;
            actual = actual->anterior;
        }
        void vaciarLista()
        {
            if (!cabeza)
                return;
            NodoFeed *aux = cabeza;
            if (cabeza == cola)
            {
                delete cabeza;
                cabeza = nullptr;
                cola = nullptr;
                actual = nullptr;
                return;
            }
            do
            {
                NodoFeed *temp = aux;
                aux = aux->siguiente;
                delete temp;
            } while (aux != cabeza);
            cabeza = nullptr;
            cola = nullptr;
            actual = nullptr;
        }

        void graficarPublicacionesFeed(string nombre, string formato)
        {
            string path_render = "renders/" + nombre + "." + formato;
            string path_dot = "renders/" + nombre + ".dot";
            ofstream fs(path_dot);
            if (!fs.is_open())
            {
                cout << "Error al abrir el archivo" << endl;
                return;
            }
            fs << "digraph G {" << endl;
            fs << "rankdir = TB;" << endl;
            fs << "node [shape=record];" << endl;
            fs << "label=\"Publicaciones del Feed de" << usuario_logeado->nombres << " \" fontsize = 20 fontname = \"Arial\";" << endl;
            NodoFeed *temp = cabeza;
            if (!temp)
            {
                fs << "}" << endl;
                fs.close();
                string comando = "dot -T" + formato + " " + path_dot + " -o " + path_render;
                system(comando.c_str());
                return;
            }
            do
            {
                fs << "node" << temp->publicacion.id << " [label=\"{ID: " << temp->publicacion.id << " | Autor: " << temp->publicacion.correo_autor << " | Fecha: " << temp->publicacion.fecha << " | Hora: " << temp->publicacion.hora << " | Contenido: " << temp->publicacion.contenido << "}\"];" << endl;
                temp = temp->siguiente;
            } while (temp != cabeza);
            temp = cabeza;
            do
            {
                fs << "node" << temp->publicacion.id << " -> node" << temp->siguiente->publicacion.id << ";" << endl;
                fs << "node" << temp->siguiente->publicacion.id << " -> node" << temp->publicacion.id << ";" << endl;
                temp = temp->siguiente;
            } while (temp != cabeza);
            fs << "}" << endl;
            fs.close();
            string cmd = "dot -T" + formato + " " + path_dot + " -o " + path_render;
            system(cmd.c_str());
        }
    };

}

#endif // LISTAPUBLICACIONESFEED_H