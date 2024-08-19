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
        void eliminarSolicitud(const string &correo)
        {
            if (!cabeza)
                return;

            NodoSolicitud *aux = cabeza;

            do
            {
                if (aux->correo == correo)
                {
                    // Caso 1: Si la lista tiene un solo nodo
                    if (cabeza == cola && aux == cabeza)
                    {
                        delete cabeza;
                        cabeza = nullptr;
                        cola = nullptr;
                        actual = nullptr;
                        return;
                    }

                    // Caso 2: Si se elimina la cabeza
                    if (aux == cabeza)
                    {
                        cabeza = cabeza->siguiente;
                        cabeza->anterior = cola;
                        cola->siguiente = cabeza;
                        if (actual == aux)
                        {
                            actual = cabeza;
                        }
                        delete aux;
                        return;
                    }

                    // Caso 3: Si se elimina la cola
                    if (aux == cola)
                    {
                        cola = cola->anterior;
                        cola->siguiente = cabeza;
                        cabeza->anterior = cola;
                        if (actual == aux)
                        {
                            actual = cabeza;
                        }
                        delete aux;
                        return;
                    }

                    // Caso 4: Si se elimina un nodo intermedio
                    aux->anterior->siguiente = aux->siguiente;
                    aux->siguiente->anterior = aux->anterior;
                    if (actual == aux)
                    {
                        actual = aux->siguiente;
                    }
                    delete aux;
                    return;
                }
                aux = aux->siguiente;
            } while (aux != cabeza);
        }
        int getSize()
        {
            if (!cabeza)
                return 0;
            int size = 0;
            NodoSolicitud *aux = cabeza;
            do
            {
                size++;
                aux = aux->siguiente;
            } while (aux != cabeza);
            return size;
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
                cout << "|                          No hay Solicitudes                                 |" << endl;
                return;
            }
            NodoSolicitud *aux = cabeza;
            do
            {
                cout << "|+| " << aux->correo << endl;
                aux = aux->siguiente;
            } while (aux != cabeza);
        }
    };
}

#endif // LISTASOLICITUDES_H