#ifndef LISTASOLICITUDES_H
#define LISTASOLICITUDES_H

#include <iostream>
#include <string>

using namespace std;

namespace ListaSolicitudes
{

    struct NodoSolicitud
    {
        string correo;
        NodoSolicitud *siguiente;
        NodoSolicitud *anterior;

        NodoSolicitud(const string &u)
            : correo(u), siguiente(nullptr), anterior(nullptr) {}
    };
    class ListaCircularDoble
    {
    private:
        NodoSolicitud *cabeza;
        NodoSolicitud *cola;
        NodoSolicitud *actual;

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

            NodoSolicitud *aux = cabeza;

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
                NodoSolicitud *temp = aux;
                aux = aux->siguiente;
                delete temp;
            } while (aux != cabeza); // El bucle se detiene cuando volvemos a la cabeza

            // Establecemos los punteros a nullptr después de la eliminación
            cabeza = nullptr;
            cola = nullptr;
            actual = nullptr;
        }

        // Métodos
        void agregarSolicitud(const string &u)
        {
            NodoSolicitud *nuevo = new NodoSolicitud(u);
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

        void mostrarSolicitudActual()
        {
            if (!cabeza)
            {
                cout << "|                          No hay Solicitudes                             |" << endl;
                return;
            }
            cout << "| " << actual->correo;
            cout << "|=========================================================================|" << endl;
        }
        string obtenerSolicitudActual()
        {
            if (!cabeza)
            {
                return "";
            }
            return actual->correo;
        }
        void avanzarSolicitud()
        {
            if (!cabeza)
                return;
            actual = actual->siguiente;
        }
        void retrocederSolicitud()
        {
            if (!cabeza)
                return;
            actual = actual->anterior;
        }
        void vaciarLista()
        {
            if (!cabeza)
                return;
            NodoSolicitud *aux = cabeza;
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
                NodoSolicitud *temp = aux;
                aux = aux->siguiente;
                delete temp;
            } while (aux != cabeza);
            cabeza = nullptr;
            cola = nullptr;
            actual = nullptr;
        }
        void eliminarSolicitud()
        {
            if (!cabeza)
                return;

            // Si solo hay un nodo
            if (cabeza == cola)
            {
                delete cabeza;
                cabeza = nullptr;
                cola = nullptr;
                actual = nullptr;
                return;
            }

            // Caso general
            NodoSolicitud *temp = actual;
            if (actual == cabeza)
            {
                cabeza = actual->siguiente;
            }
            if (actual == cola)
            {
                cola = actual->anterior;
            }

            actual->anterior->siguiente = actual->siguiente;
            actual->siguiente->anterior = actual->anterior;

            // Mover el puntero actual al siguiente nodo
            actual = actual->siguiente;

            delete temp;
        }
        bool existeSolicitud(const string &correo)
        {
            if (!cabeza)
                return false;

            NodoSolicitud *aux = cabeza;
            do
            {
                if (aux->correo == correo)
                {
                    return true;
                }
                aux = aux->siguiente;
            } while (aux != cabeza);

            return false;
        }
        void mostrarSolicitudes()
        {
            if (!cabeza)
            {
                cout << "|                          No hay Solicitudes                             |" << endl;
                return;
            }
            NodoSolicitud *aux = cabeza;
            do
            {
                cout << "| " << aux->correo;
                cout << "|=========================================================================|" << endl;
                aux = aux->siguiente;
            } while (aux != cabeza);
        }
    };
}

#endif // LISTASOLICITUDES_H