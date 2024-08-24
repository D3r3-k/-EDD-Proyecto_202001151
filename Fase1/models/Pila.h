#ifndef PILA_H
#define PILA_H

#include <iostream>
#include <string>

using namespace std;

namespace PilaSolicitudes
{
    // clase pila como lista enlazada simple
    class Pila
    {
    private:
        // estructura de nodo
        struct Nodo
        {
            string solicitud;
            Nodo *siguiente;
        };
        // puntero al primer nodo
        Nodo *cima;
        // tamaño de la pila
        int tamano;

    public:
        // constructor
        Pila() : cima(NULL), tamano(0) {}
        // destructor
        ~Pila()
        {
            // nodo auxiliar
            Nodo *aux = cima;
            // recorrer la pila
            while (aux != NULL)
            {
                // nodo auxiliar
                Nodo *aux2 = aux;
                // avanzar al siguiente nodo
                aux = aux->siguiente;
                // eliminar el nodo
                delete aux2;
            }
        }
        // insertar un elemento en la pila
        void push(string solicitud)
        {
            // crear un nuevo nodo
            Nodo *nuevo = new Nodo();
            // asignar el valor al nodo
            nuevo->solicitud = solicitud;
            // el siguiente nodo es la cima
            nuevo->siguiente = cima;
            // la cima es el nuevo nodo
            cima = nuevo;
            // incrementar el tamaño
            tamano++;
        }
        // eliminar un elemento de la pila
        void pop()
        {
            // si la pila está vacía
            if (cima == NULL)
            {
                cout << "La pila está vacía" << endl;
                return;
            }
            // nodo auxiliar
            Nodo *aux = cima;
            // la cima es el siguiente nodo
            cima = cima->siguiente;
            // eliminar el nodo
            delete aux;
            // decrementar el tamaño
            tamano--;
        }
        // obtener el elemento en la cima de la pila
        string top()
        {
            // si la pila está vacía
            if (cima == NULL)
            {
                cout << "La pila está vacía" << endl;
                return "";
            }
            // retornar el valor de la cima
            return cima->solicitud;
        }
        // obtener el tamaño de la pila
        int size()
        {
            return tamano;
        }
        // verificar si la pila está vacía
        bool empty()
        {
            return cima == NULL;
        }
        // imprimir la pila
        void print()
        {
            // nodo auxiliar
            Nodo *aux = cima;
            // recorrer la pila
            while (aux != NULL)
            {
                // imprimir el valor del nodo
                cout << aux->solicitud << endl;
                // avanzar al siguiente nodo
                aux = aux->siguiente;
            }
        }
        // verificar si existe un correo en la pila
        bool existeCorreo(string correo)
        {
            // nodo auxiliar
            Nodo *aux = cima;
            // recorrer la pila
            while (aux != NULL)
            {
                // si el correo es igual al correo del nodo
                if (aux->solicitud == correo)
                {
                    return true;
                }
                // avanzar al siguiente nodo
                aux = aux->siguiente;
            }
            return false;
        }
        
    };
}

#endif // PILA_H
