#ifndef LISTADOBLE_H
#define LISTADOBLE_H

namespace ListaDoble
{

    template <typename T>
    class ListaDoble
    {
    private:
        struct Nodo
        {
            T dato;
            Nodo *siguiente;
            Nodo *anterior;
            Nodo(const T &valor) : dato(valor), siguiente(nullptr), anterior(nullptr) {}
        };

        Nodo *cabeza;
        Nodo *cola;

        // Metodo auxiliar para clonar nodos
        void clonar(const ListaDoble &otra)
        {
            Nodo *temp = otra.cabeza;
            while (temp)
            {
                agregar(temp->dato);
                temp = temp->siguiente;
            }
        }

    public:
        // Constructor
        ListaDoble() : cabeza(nullptr), cola(nullptr) {}
        // Destructor
        ~ListaDoble()
        {
            while (cabeza)
            {
                Nodo *temp = cabeza;
                cabeza = cabeza->siguiente;
                delete temp;
            }
        }
        // Constructor para clonar
        ListaDoble(const ListaDoble &otra) : cabeza(nullptr), cola(nullptr)
        {
            clonar(otra);
        }
        // Operador de asignacion
        ListaDoble &operator=(const ListaDoble &otra)
        {
            if (this != &otra)
            {
                while (cabeza)
                {
                    Nodo *temp = cabeza;
                    cabeza = cabeza->siguiente;
                    delete temp;
                }
                clonar(otra);
            }
            return *this;
        }
        // insertar al final de la lista (insertar)
        void insertar(const T &valor)
        {
            Nodo *nuevo = new Nodo(valor);
            if (!cabeza)
            {
                cabeza = nuevo;
                cola = nuevo;
            }
            else
            {
                cola->siguiente = nuevo;
                nuevo->anterior = cola;
                cola = nuevo;
            }
        }
        // insertar al inicio de la lista
        void insertarInicio(const T &valor)
        {
            Nodo *nuevo = new Nodo(valor);
            if (!cabeza)
            {
                cabeza = nuevo;
                cola = nuevo;
            }
            else
            {
                nuevo->siguiente = cabeza;
                cabeza->anterior = nuevo;
                cabeza = nuevo;
            }
        }
        // eliminar al inicio
        void eliminarInicio()
        {
            if (cabeza)
            {
                Nodo *temp = cabeza;
                cabeza = cabeza->siguiente;
                if (cabeza)
                {
                    cabeza->anterior = nullptr;
                }
                else
                {
                    cola = nullptr;
                }
                delete temp;
            }
        }
        // eliminar al final
        void eliminarFinal()
        {
            if (cola)
            {
                Nodo *temp = cola;
                cola = cola->anterior;
                if (cola)
                {
                    cola->siguiente = nullptr;
                }
                else
                {
                    cabeza = nullptr;
                }
                delete temp;
            }
        }
        // eliminar segun su posicion
        void eliminarPosicion(int pos)
        {
            if (pos < 0)
            {
                return;
            }
            if (pos == 0)
            {
                eliminarInicio();
                return;
            }
            Nodo *temp = cabeza;
            for (int i = 0; i < pos - 1; i++)
            {
                if (!temp)
                {
                    return;
                }
                temp = temp->siguiente;
            }
            if (!temp || !temp->siguiente)
            {
                return;
            }
            Nodo *eliminar = temp->siguiente;
            temp->siguiente = eliminar->siguiente;
            if (eliminar->siguiente)
            {
                eliminar->siguiente->anterior = temp;
            }
            else
            {
                cola = temp;
            }
            delete eliminar;
        }
        // obtener el tamaño de la lista
        int size() const
        {
            int tam = 0;
            Nodo *temp = cabeza;
            while (temp)
            {
                tam++;
                temp = temp->siguiente;
            }
            return tam;
        }
        // obtener el nodo segun su posicion
        T *obtener(int pos) const
        {
            if (pos < 0)
            {
                return nullptr;
            }
            Nodo *temp = cabeza;
            for (int i = 0; i < pos; i++)
            {
                if (!temp)
                {
                    return nullptr;
                }
                temp = temp->siguiente;
            }
            if (!temp)
            {
                return nullptr;
            }
            return &temp->dato;
        }

        // esta vacia
        bool vacia() const
        {
            return cabeza == nullptr;
        }
        
    };
}
#endif // LISTADOBLE_H
