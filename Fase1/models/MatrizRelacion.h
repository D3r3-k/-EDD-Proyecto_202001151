#ifndef MATRIZRELACION_H
#define MATRIZRELACION_H

#include <iostream>
#include <iomanip>
#include "ListaUsuarios.h"

using namespace std;

namespace MatrizRelacion
{
    class MatrizDispersa
    {
    private:
        struct NodoRelacion
        {
            ListaUsuarios::Usuario *usuario1, *usuario2;
            NodoRelacion *siguiente;
            NodoRelacion *anterior;
            NodoRelacion *arriba;
            NodoRelacion *abajo;

            NodoRelacion(ListaUsuarios::Usuario *u1 = nullptr, ListaUsuarios::Usuario *u2 = nullptr)
                : usuario1(u1), usuario2(u2), siguiente(nullptr), anterior(nullptr), arriba(nullptr), abajo(nullptr) {}
        };

        NodoRelacion *cabeza;

    public:
        // Constructor
        MatrizDispersa() : cabeza(nullptr) {}

        // Destructor
        ~MatrizDispersa()
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
        void agregarRelacion(ListaUsuarios::Usuario *user1, ListaUsuarios::Usuario *user2)
        {
            if (!cabeza)
            {
                cabeza = new NodoRelacion(nullptr, nullptr);
            }
            NodoRelacion *fila1 = buscarFila(user1);
            NodoRelacion *columna1 = buscarColumna(user2);

            NodoRelacion *fila2 = buscarFila(user2);
            NodoRelacion *columna2 = buscarColumna(user1);

            if (!fila1)
            {
                fila1 = insertarFilaCabecera(user1);
            }
            if (!columna1)
            {
                columna1 = insertarColumnaCabecera(user2);
            }

            if (!fila2)
            {
                fila2 = insertarFilaCabecera(user2);
            }
            if (!columna2)
            {
                columna2 = insertarColumnaCabecera(user1);
            }

            if (!NodoRelacionExiste(fila1, user2))
            {
                NodoRelacion *nuevo1 = new NodoRelacion(user1, user2);
                insertarEnFila(nuevo1, fila1);
                insertarEnColumna(nuevo1, columna1);
            }

            if (!NodoRelacionExiste(fila2, user1))
            {
                NodoRelacion *nuevo2 = new NodoRelacion(user2, user1);
                insertarEnFila(nuevo2, fila2);
                insertarEnColumna(nuevo2, columna2);
            }
        }

        NodoRelacion *buscarFila(ListaUsuarios::Usuario *u1)
        {
            NodoRelacion *current = cabeza->abajo;
            while (current)
            {
                if (current->usuario1 == u1)
                {
                    return current;
                }
                current = current->abajo;
            }
            return nullptr;
        }

        NodoRelacion *buscarColumna(ListaUsuarios::Usuario *u2)
        {
            NodoRelacion *current = cabeza->siguiente;
            while (current)
            {
                if (current->usuario2 == u2)
                {
                    return current;
                }
                current = current->siguiente;
            }
            return nullptr;
        }

        bool NodoRelacionExiste(NodoRelacion *fila, ListaUsuarios::Usuario *u2)
        {
            NodoRelacion *current = fila->siguiente;
            while (current)
            {
                if (current->usuario2 == u2)
                {
                    return true;
                }
                current = current->siguiente;
            }
            return false;
        }

        NodoRelacion *insertarFilaCabecera(ListaUsuarios::Usuario *user)
        {
            NodoRelacion *newRowHeader = new NodoRelacion(user, nullptr);

            if (!cabeza->abajo)
            {
                cabeza->abajo = newRowHeader;
            }
            else
            {
                NodoRelacion *current = cabeza;
                while (current->abajo && current->abajo->usuario1->id < user->id)
                {
                    current = current->abajo;
                }
                newRowHeader->abajo = current->abajo;
                if (current->abajo)
                {
                    current->abajo->arriba = newRowHeader;
                }
                current->abajo = newRowHeader;
                newRowHeader->arriba = current;
            }

            return newRowHeader;
        }

        NodoRelacion *insertarColumnaCabecera(ListaUsuarios::Usuario *user)
        {
            NodoRelacion *newColumnHeader = new NodoRelacion(nullptr, user);

            if (!cabeza->siguiente)
            {
                cabeza->siguiente = newColumnHeader;
            }
            else
            {
                NodoRelacion *current = cabeza;
                while (current->siguiente && current->siguiente->usuario2->id < user->id)
                {
                    current = current->siguiente;
                }
                newColumnHeader->siguiente = current->siguiente;
                if (current->siguiente)
                {
                    current->siguiente->anterior = newColumnHeader;
                }
                current->siguiente = newColumnHeader;
                newColumnHeader->anterior = current;
            }

            return newColumnHeader;
        }

        void insertarEnFila(NodoRelacion *nuevo, NodoRelacion *filaCabecera)
        {
            NodoRelacion *current = filaCabecera;
            while (current->siguiente)
            {
                if (nuevo->usuario2->id < current->siguiente->usuario2->id)
                {
                    break;
                }
                current = current->siguiente;
            }
            nuevo->siguiente = current->siguiente;
            if (current->siguiente)
            {
                current->siguiente->anterior = nuevo;
            }
            current->siguiente = nuevo;
            nuevo->anterior = current;
        }

        void insertarEnColumna(NodoRelacion *nuevo, NodoRelacion *columnaCabecera)
        {
            NodoRelacion *current = columnaCabecera;
            while (current->abajo)
            {
                if (nuevo->usuario1->id < current->abajo->usuario1->id)
                {
                    break;
                }
                current = current->abajo;
            }
            nuevo->abajo = current->abajo;
            if (current->abajo)
            {
                current->abajo->arriba = nuevo;
            }
            current->abajo = nuevo;
            nuevo->arriba = current;
        }

        void imprimir() const
        {
            NodoRelacion *rowHeader = cabeza->abajo;
            cout << setw(10) << " ";
            NodoRelacion *colHeader = cabeza->siguiente;

            // Imprimir cabeceras de columna
            while (colHeader)
            {
                cout << setw(10) << colHeader->usuario2->nombres;
                colHeader = colHeader->siguiente;
            }
            cout << endl;

            // Imprimir las filas
            while (rowHeader)
            {
                cout << setw(10) << rowHeader->usuario1->nombres;
                NodoRelacion *current = rowHeader->siguiente;
                colHeader = cabeza->siguiente;

                while (colHeader)
                {
                    if (current && current->usuario2 == colHeader->usuario2)
                    {
                        cout << setw(10) << "[ X ]";
                        current = current->siguiente;
                    }
                    else
                    {
                        cout << setw(10) << "[   ]";
                    }
                    colHeader = colHeader->siguiente;
                }
                cout << endl;
                rowHeader = rowHeader->abajo;
            }
        }

        void eliminarRelacionesUsuario(string correo_usuario)
        {
            // borrando todas las relaciones que tengan relacion con el correo del usuario tanto en filas como en columnas
            NodoRelacion *fila = cabeza->abajo;
            while (fila)
            {
                NodoRelacion *temp = fila;
                fila = fila->abajo;
                if (temp->usuario1->correo == correo_usuario)
                {
                    if (temp->arriba)
                    {
                        temp->arriba->abajo = temp->abajo;
                    }
                    if (temp->abajo)
                    {
                        temp->abajo->arriba = temp->arriba;
                    }
                    if (temp == cabeza->abajo)
                    {
                        cabeza->abajo = temp->abajo;
                    }
                    delete temp;
                }
            }

            NodoRelacion *columna = cabeza->siguiente;
            while (columna)
            {
                NodoRelacion *temp = columna;
                columna = columna->siguiente;
                if (temp->usuario2->correo == correo_usuario)
                {
                    if (temp->anterior)
                    {
                        temp->anterior->siguiente = temp->siguiente;
                    }
                    if (temp->siguiente)
                    {
                        temp->siguiente->anterior = temp->anterior;
                    }
                    if (temp == cabeza->siguiente)
                    {
                        cabeza->siguiente = temp->siguiente;
                    }
                    delete temp;
                }
            }
        }
    };
};

#endif // MATRIZRELACION_H
