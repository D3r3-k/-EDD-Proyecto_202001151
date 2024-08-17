#ifndef MATRISRELACION_H
#define MATRISRELACION_H

#include <iostream>
#include <iomanip>
#include "ListaUsuarios.h"

using namespace std;

namespace MatrisRelacion
{
    struct NodeVal
    {
        bool exists;
        ListaUsuarios::Usuario *value;
        NodeVal(bool e = false, ListaUsuarios::Usuario *v = nullptr) : exists(e), value(v) {}
    };

    class MatrisDispersa
    {
    private:
        struct NodoRelacion
        {
            int i = -1, j = -1;
            ListaUsuarios::Usuario *usuario;

            NodoRelacion *siguiente;
            NodoRelacion *anterior;
            NodoRelacion *arriba;
            NodoRelacion *abajo;

            NodoRelacion(int i = -1, int j = -1, ListaUsuarios::Usuario *u = nullptr)
                : i(i), j(j), usuario(u), siguiente(nullptr), anterior(nullptr), arriba(nullptr), abajo(nullptr) {}
        };

        NodoRelacion *cabeza;
        int width = 0;
        int height = 0;

    public:
        // Destructor
        ~MatrisDispersa()
        {
            NodoRelacion *aux = cabeza;
            while (aux != nullptr)
            {
                NodoRelacion *aux2 = aux;
                aux = aux->abajo;
                while (aux2 != nullptr)
                {
                    NodoRelacion *aux3 = aux2;
                    aux2 = aux2->siguiente;
                    delete aux3;
                }
            }
        }
        // Métodos
        void agregarRelacion(int i, int j, ListaUsuarios::Usuario *user)
        {
            NodoRelacion *nuevo = new NodoRelacion(i, j, user);

            if (!cabeza)
            {
                cabeza = new NodoRelacion(-1, -1);
            }
            NodoRelacion *fila = buscarFila(i);
            NodoRelacion *columna = buscarColumna(j);

            if (j > width)
                width = j;
            if (i > height)
                height = i;

            if (!NodoRelacionExiste(nuevo))
            {
                if (!columna)
                {
                    columna = insertarColumnaCabecera(j);
                }
                if (!fila)
                {
                    fila = insertarFilaCabecera(i);
                }
                insertarEnColumna(nuevo, fila);
                insertarEnFila(nuevo, columna);
            }
        }

        NodoRelacion *buscarFila(int i)
        {
            NodoRelacion *current = cabeza;

            while (current)
            {
                if (current->i == i)
                    return current;
                current = current->abajo;
            }
            return nullptr;
        }

        NodoRelacion *buscarColumna(int j)
        {
            NodoRelacion *current = cabeza;

            while (current)
            {
                if (current->j == j)
                    return current;
                current = current->siguiente;
            }
            return nullptr;
        }

        bool NodoRelacionExiste(NodoRelacion *nuevo)
        {
            NodoRelacion *rowHeader = cabeza;
            while (rowHeader)
            {
                if (rowHeader->i == nuevo->i)
                {
                    NodoRelacion *column = rowHeader;
                    while (column)
                    {
                        if (column->j == nuevo->j)
                        {
                            column->usuario = nuevo->usuario;
                            return true;
                        }
                        column = column->siguiente;
                    }
                    return false;
                }
                rowHeader = rowHeader->abajo;
            }
            return false;
        }

        NodoRelacion *insertarFilaCabecera(int i)
        {
            NodoRelacion *newRowHeader = new NodoRelacion(i, -1);
            insertarEnFila(newRowHeader, cabeza);
            return newRowHeader;
        }
        NodoRelacion *insertarColumnaCabecera(int j)
        {
            NodoRelacion *newColumnHeader = new NodoRelacion(-1, j);
            insertarEnColumna(newColumnHeader, cabeza);
            return newColumnHeader;
        }

        void insertarEnFila(NodoRelacion *nuevo, NodoRelacion *filaCabecera)
        {
            NodoRelacion *current = filaCabecera;

            while (current->abajo)
            {
                if (nuevo->i < current->abajo->i && nuevo->i > current->i)
                {
                    nuevo->abajo = current->abajo;
                    nuevo->arriba = current;
                    current->abajo->arriba = nuevo;
                    current->abajo = nuevo;
                    return;
                }
                current = current->abajo;
            }

            current->abajo = nuevo;
            nuevo->arriba = current;
        }

        void insertarEnColumna(NodoRelacion *nuevo, NodoRelacion *columnaCabecera)
        {
            NodoRelacion *current = columnaCabecera;

            while (current->siguiente)
            {
                if (nuevo->j < current->siguiente->j && nuevo->j > current->j)
                {
                    nuevo->siguiente = current->siguiente;
                    nuevo->anterior = current;
                    current->siguiente->anterior = nuevo;
                    current->siguiente = nuevo;
                    return;
                }
                current = current->siguiente;
            }

            current->siguiente = nuevo;
            nuevo->anterior = current;
        }

        void imprimir() const
        {
            imprimirColumnasCabecera();

            for (int i = 0; i <= height; ++i)
            {
                cout << "\n"
                     << setw(3) << i;
                for (int j = 0; j <= width; ++j)
                {
                    NodeVal val = getValue(i, j);
                    if (!val.exists)
                    {
                        cout << setw(3) << " ";
                    }
                    else
                    {
                        cout << setw(3) << "T";
                    }
                }
            }
            cout << endl;
        }

        void imprimirColumnasCabecera() const
        {
            for (int j = -1; j <= width; ++j)
            {
                cout << setw(3) << j;
            }
        }

        NodeVal getValue(int i, int j) const
        {
            NodeVal val;
            NodoRelacion *rowHeader = cabeza;

            while (rowHeader)
            {
                if (rowHeader->i == i)
                {
                    NodoRelacion *columna = rowHeader;
                    while (columna)
                    {
                        if (columna->j == j)
                        {
                            val.value = columna->usuario;
                            val.exists = true;
                            return val;
                        }
                        columna = columna->siguiente;
                    }
                    return val;
                }
                rowHeader = rowHeader->abajo;
            }
            return val;
        }
    };
};

#endif // MATRIX_H