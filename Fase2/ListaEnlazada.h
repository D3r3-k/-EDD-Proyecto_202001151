#ifndef LISTAENLAZADA_H
#define LISTAENLAZADA_H

#include <stdexcept> // Para manejar excepciones

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

        // Método auxiliar para clonar nodos
        Nodo *clonar(Nodo *otraCabeza)
        {
            if (!otraCabeza)
                return nullptr;
            Nodo *nuevaCabeza = new Nodo(otraCabeza->data);
            Nodo *actual = nuevaCabeza;
            Nodo *temp = otraCabeza->next;
            while (temp)
            {
                actual->next = new Nodo(temp->data);
                actual = actual->next;
                temp = temp->next;
            }
            return nuevaCabeza;
        }

    public:
        // Constructor
        ListaEnlazada() : cabeza(nullptr) {}

        // Destructor
        ~ListaEnlazada()
        {
            while (cabeza)
            {
                Nodo *temp = cabeza;
                cabeza = cabeza->next;
                delete temp;
            }
        }

        // Constructor de copia
        ListaEnlazada(const ListaEnlazada &otra) : cabeza(clonar(otra.cabeza)) {}

        // Operador de asignación
        ListaEnlazada &operator=(const ListaEnlazada &otra)
        {
            if (this != &otra)
            {
                // Primero limpiar la lista actual
                while (cabeza)
                {
                    Nodo *temp = cabeza;
                    cabeza = cabeza->next;
                    delete temp;
                }
                // Clonar la lista de la otra instancia
                cabeza = clonar(otra.cabeza);
            }
            return *this;
        }

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
            if (estaVacia()) // Validación
            {
                throw std::out_of_range("La lista está vacía, no se puede eliminar.");
            }
            Nodo *temp = cabeza;
            cabeza = cabeza->next;
            delete temp;
        }

        // Eliminar el último nodo de la lista
        void eliminarFinal()
        {
            if (estaVacia()) // Validación
            {
                throw std::out_of_range("La lista está vacía, no se puede eliminar.");
            }

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

        // Eliminar un nodo según su posición
        void eliminar(int pos)
        {
            if (estaVacia()) // Validación
            {
                throw std::out_of_range("La lista está vacía.");
            }

            if (pos < 0 || pos >= size()) // Validación de rango
            {
                throw std::out_of_range("Posición fuera de rango.");
            }

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
                Nodo *temp2 = temp->next;
                temp->next = temp->next->next;
                delete temp2;
            }
        }

        // Obtener el tamaño de la lista
        int size() const
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
            if (estaVacia()) // Validación
            {
                throw std::out_of_range("La lista está vacía.");
            }

            if (pos < 0 || pos >= size()) // Validación de rango
            {
                throw std::out_of_range("Posición fuera de rango.");
            }

            Nodo *temp = cabeza;
            for (int i = 0; i < pos && temp; i++)
            {
                temp = temp->next;
            }

            return &temp->data;
        }

        // Verificar si la lista está vacía
        bool estaVacia() const
        {
            return cabeza == nullptr;
        }

        // Metodo para concatenar dos listas
        void concatenar(const ListaEnlazada &otra)
        {
            if (otra.estaVacia())
                return;

            Nodo *temp = cabeza;
            while (temp->next)
            {
                temp = temp->next;
            }
            temp->next = clonar(otra.cabeza);
        }

        // Método para intercambiar dos nodos
        void intercambiar(int pos1, int pos2)
        {
            if (pos1 < 0 || pos1 >= size() || pos2 < 0 || pos2 >= size())
            {
                throw std::out_of_range("Posición fuera de rango.");
            }

            if (pos1 == pos2)
                return;

            Nodo *temp1 = cabeza;
            Nodo *temp2 = cabeza;
            for (int i = 0; i < pos1 && temp1; i++)
            {
                temp1 = temp1->next;
            }
            for (int i = 0; i < pos2 && temp2; i++)
            {
                temp2 = temp2->next;
            }

            T temp = temp1->data;
            temp1->data = temp2->data;
            temp2->data = temp;
        }

        // TODO: MÉTODOS DE PILA
        // Insertar un nuevo nodo al inicio de la lista (PUSH)
        void push(const T &data)
        {
            insertarInicio(data);
        }

        // Eliminar el primer nodo de la lista (POP)
        void pop()
        {
            if (estaVacia()) // Validación
            {
                throw std::out_of_range("La pila está vacía, no se puede eliminar.");
            }
            eliminarInicio();
        }

        // Obtener el valor del primer nodo (TOPE de la pila)
        T top() const
        {
            if (estaVacia()) // Validación
            {
                throw std::out_of_range("La pila está vacía.");
            }
            return cabeza->data;
        }
    };
}

#endif // LISTAENLAZADA_H
