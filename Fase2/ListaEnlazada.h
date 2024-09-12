#ifndef LISTAENLAZADA_H
#define LISTAENLAZADA_H

#include <iostream>
#include <string>

using namespace std;

namespace ListaEnlazada
{
template <class T>
class ListaEnlazada
{
private:
    struct Nodo
    {
        T data;
        Nodo *next;
        Nodo(const T &data) : data(data), next(nullptr) {}
    };
    Nodo *cabeza;

public:
    // Constructor de la lista simple
    ListaEnlazada() : cabeza(nullptr) {}
    // Destructor de la lista simple
    ~ListaEnlazada()
    {
        while (cabeza)
        {
            Nodo *temp = cabeza;
            cabeza = cabeza->next;
            delete temp;
        }
    }

    // TODO: METODOS
    // Insertar un nuevo nodo al final de la lista
    void insertar(const T &data)
    {
        Nodo *nuevo = new Nodo(data);
        if (!cabeza)
        {
            cabeza = nuevo;
        }
        else
        {
            Nodo *temp = cabeza;
            while (temp->next)
            {
                temp = temp->next;
            }
            temp->next = nuevo;
        }
    }
    // Insertar un nuevo nodo al inicio de la lista
    void insertarInicio(const T &data)
    {
        Nodo *nuevo = new Nodo(data);
        nuevo->next = cabeza;
        cabeza = nuevo;
    }
    // Eliminar el primer nodo de la lista
    void eliminarInicio()
    {
        if (cabeza)
        {
            Nodo *temp = cabeza;
            cabeza = cabeza->next;
            delete temp;
        }
    }
    // Eliminar el último nodo de la lista
    void eliminarFinal()
    {
        if (cabeza)
        {
            if (!cabeza->next)
            {
                delete cabeza;
                cabeza = nullptr;
            }
            else
            {
                Nodo *temp = cabeza;
                while (temp->next && temp->next->next)
                {
                    temp = temp->next;
                }
                delete temp->next;
                temp->next = nullptr;
            }
        }
    }
    // Eliminar un nodo según su posición
    void eliminar(int pos)
    {
        if (cabeza)
        {
            if (pos == 0)
            {
                Nodo *temp = cabeza;
                cabeza = cabeza->next;
                delete temp;
            }
            else
            {
                Nodo *temp = cabeza;
                for (int i = 0; i < pos - 1 && temp->next; i++)
                {
                    temp = temp->next;
                }
                if (temp->next)
                {
                    Nodo *temp2 = temp->next;
                    temp->next = temp->next->next;
                    delete temp2;
                }
            }
        }
    }
    
    // Obtener el tamaño de la lista
    int size()
    {
        int size = 0;
        Nodo *temp = cabeza;
        while (temp)
        {
            size++;
            temp = temp->next;
        }
        return size;
    }

    // Obtener el nodo según su posición
    T *obtener(int pos)
    {
        if (cabeza)
        {
            Nodo *temp = cabeza;
            for (int i = 0; i < pos && temp; i++)
            {
                temp = temp->next;
            }
            if (temp)
            {
                return &temp->data; // Retorna un puntero al dato del nodo
            }
        }
        return nullptr; // Si no se encuentra la posición, devuelve nullptr
    }
    T* getHead() const {
        return cabeza;
    }
    // Verificar si la lista está vacía
    bool estaVacia()
    {
        return cabeza == nullptr;
    }

    // TODO: METODOS DE PILA
    // Insertar un nuevo nodo al inicio de la lista
    void push(const T &data)
    {
        insertarInicio(data);
    }
    // Eliminar el primer nodo de la lista
    void pop()
    {
        eliminarFinal();
    }
    // Obtener el último nodo de la lista (tope de la pila)
    T top()
    {
        if (!estaVacia())
        {
            return cabeza->data;
        }
        throw std::out_of_range("La pila está vacía");
    }
};
}

#endif // LISTAENLAZADA_H
