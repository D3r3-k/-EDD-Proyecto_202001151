#ifndef MATRIZRELACION_H
#define MATRIZRELACION_H

#include <iostream>
#include <iomanip>

#include "Globales.h"
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
            if (!cabeza)
            {
                cout << "No hay relaciones para mostrar" << endl;
                return;
            }

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
            if (!cabeza)
            {
                return;
            }
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

        bool existeRelacion(string correo1, string correo2)
        {
            if (!cabeza)
            {
                return false;
            }
            NodoRelacion *fila = cabeza->abajo;
            while (fila)
            {
                NodoRelacion *current = fila->siguiente;
                while (current)
                {
                    if (current->usuario1->correo == correo1 && current->usuario2->correo == correo2)
                    {
                        return true;
                    }
                    current = current->siguiente;
                }
                fila = fila->abajo;
            }
            return false;
        }
        
        ListaUsuarios::ListaEnlazadaSimple obtenerAmigos(string correo)
        {
            // obtener una lista simple de los amigos del usuario con el correo dado, siendo una matriz, se recorre la fila del usuario y se obtienen los amigos de este
            // y quitando cualquier nulo, repetido o usuario que no sea amigo
            ListaUsuarios::ListaEnlazadaSimple amigos;
            if (!cabeza)
            {
                return amigos;
            }
            NodoRelacion *fila = cabeza->abajo;
            while (fila)
            {
                if (fila->usuario1->correo == correo)
                {
                    NodoRelacion *current = fila->siguiente;
                    while (current)
                    {
                        if (current->usuario2 && current->usuario2->correo != correo && !amigos.existeUsuario(current->usuario2->correo))
                        {
                            amigos.agregarUsuario(*current->usuario2);
                        }
                        current = current->siguiente;
                    }
                    break;
                }
                fila = fila->abajo;
            }
            return amigos;
        }

        void top5UsuariosConMenosAmigos()
        {
            // obtener los 5 usuarios con menos amigos, se recorre la matriz y se cuenta la cantidad de amigos que tiene cada usuario
            // se almacenara en arreglos string e int de tamaño 5, los 5 usuarios con menos amigos
            string usuarios[5];
            int cantidad[5];
            for (int i = 0; i < 5; i++)
            {
                cantidad[i] = 999999;
            }
            if (!cabeza)
            {
                cout << "No hay relaciones para mostrar" << endl;
                return;
            }
            NodoRelacion *fila = cabeza->abajo;
            while (fila)
            {
                int amigos = 0;
                NodoRelacion *current = fila->siguiente;
                while (current)
                {
                    if (current->usuario2)
                    {
                        amigos++;
                    }
                    current = current->siguiente;
                }
                for (int i = 0; i < 5; i++)
                {
                    if (amigos < cantidad[i])
                    {
                        for (int j = 4; j > i; j--)
                        {
                            cantidad[j] = cantidad[j - 1];
                            usuarios[j] = usuarios[j - 1];
                        }
                        cantidad[i] = amigos;
                        usuarios[i] = fila->usuario1->nombres;
                        break;
                    }
                }
                fila = fila->abajo;
            }
            for (int i = 0; i < 5; i++)
            {
                cout << "| " << i + 1 << ". " << usuarios[i] << " con " << cantidad[i] << " amigos" << endl;
            }
        }

        void graficarMatrizRelaciones(string nombre, string formato)
        {
            // Ver si la matriz tiene elementos
            if (!cabeza)
            {
                cout << "No hay relaciones para graficar" << endl;
                return;
            }

            // Crear el archivo .dot
            string path_render = "renders/" + nombre + "." + formato;
            string path_dot = "renders/" + nombre + ".dot";
            ofstream fs(path_dot);

            // Escribir el encabezado del archivo
            fs << "digraph G {" << endl;
            fs << "node [shape=box];" << endl;
            fs << "rankdir=TB;" << endl;
            fs << "label=\"Matriz de Relaciones de Usuarios\" fontsize = 20 fontname = \"Arial\";" << endl;

            // Definir encabezados de filas
            fs << "// Definir encabezados de filas" << endl;
            NodoRelacion *fila = cabeza->abajo;
            while (fila)
            {
                fs << "fila" << fila->usuario1->id << " [label=\"" << fila->usuario1->nombres << "\", group=1];" << endl;
                fila = fila->abajo;
            }

            // Definir encabezados de columnas
            fs << "// Definir encabezados de columnas" << endl;
            NodoRelacion *columna = cabeza->siguiente;
            while (columna)
            {
                fs << "col" << columna->usuario2->id << " [label=\"" << columna->usuario2->nombres << "\", group=" << columna->usuario2->id + 1 << "];" << endl;
                columna = columna->siguiente;
            }

            // Mantener los encabezados de las columnas en el mismo rango horizontal
            fs << "{ rank=same;";
            columna = cabeza->siguiente;
            while (columna)
            {
                fs << " col" << columna->usuario2->id << ";";
                columna = columna->siguiente;
            }
            fs << " }" << endl;

            // Definir nodos en las coordenadas especificadas
            fs << "// Definir nodos en las coordenadas especificadas" << endl;
            fila = cabeza->abajo;
            while (fila)
            {
                NodoRelacion *current = fila->siguiente;
                while (current)
                {
                    fs << "nodo" << current->usuario1->id << "_" << current->usuario2->id
                       << " [label=\"\", color=green, style=filled, group=" << current->usuario2->id + 1 << "];" << endl;
                    current = current->siguiente;
                }
                fila = fila->abajo;
            }

            // Conectar encabezados de filas con nodos y los nodos entre ellos horizontalmente
            fs << "// Conectar encabezados de filas con nodos y los nodos entre ellos horizontalmente" << endl;
            fila = cabeza->abajo;
            while (fila)
            {
                NodoRelacion *current = fila->siguiente;
                NodoRelacion *prev = nullptr;
                if (current)
                {
                    // Conectar el encabezado de la fila con el primer nodo de la fila
                    fs << "fila" << fila->usuario1->id << " -> nodo" << current->usuario1->id << "_" << current->usuario2->id
                       << " [dir=both, minlen=2];" << endl;

                    // Conectar los nodos horizontalmente
                    prev = current;
                    current = current->siguiente;
                    while (current)
                    {
                        fs << "nodo" << prev->usuario1->id << "_" << prev->usuario2->id
                           << " -> nodo" << current->usuario1->id << "_" << current->usuario2->id
                           << " [dir=both, minlen=2];" << endl;
                        prev = current;
                        current = current->siguiente;
                    }
                }
                fila = fila->abajo;
            }

            // Conectar encabezados de columnas con nodos y los nodos entre ellos verticalmente
            fs << "// Conectar encabezados de columnas con nodos y los nodos entre ellos verticalmente" << endl;
            columna = cabeza->siguiente;
            while (columna)
            {
                NodoRelacion *current = columna->abajo;
                NodoRelacion *prev = nullptr;
                if (current)
                {
                    // Conectar el encabezado de la columna con el primer nodo de la columna
                    fs << "col" << columna->usuario2->id << " -> nodo" << current->usuario1->id << "_" << current->usuario2->id
                       << " [dir=both, minlen=2];" << endl;

                    // Conectar los nodos verticalmente
                    prev = current;
                    current = current->abajo;
                    while (current)
                    {
                        fs << "nodo" << prev->usuario1->id << "_" << prev->usuario2->id
                           << " -> nodo" << current->usuario1->id << "_" << current->usuario2->id
                           << " [dir=both, minlen=2];" << endl;
                        prev = current;
                        current = current->abajo;
                    }
                }
                columna = columna->siguiente;
            }

            // Mantener los encabezados de filas alineados verticalmente
            fs << "// Mantener los encabezados de filas alineados verticalmente" << endl;
            fila = cabeza->abajo;
            while (fila && fila->abajo)
            {
                fs << "fila" << fila->usuario1->id << " -> fila" << fila->abajo->usuario1->id
                   << " [style=invis];" << endl;
                fila = fila->abajo;
            }

            // Mantener los nodos de la misma fila en el mismo rango horizontal
            fs << "// Mantener los nodos de la misma fila en el mismo rango horizontal" << endl;
            fila = cabeza->abajo;
            while (fila)
            {
                NodoRelacion *current = fila->siguiente;
                while (current)
                {
                    fs << "{ rank=same; fila" << fila->usuario1->id << "; nodo" << current->usuario1->id
                       << "_" << current->usuario2->id << " }" << endl;
                    current = current->siguiente;
                }
                fila = fila->abajo;
            }

            // Conectar las columnas entre sí de forma invisible para mantener el orden
            fs << "// Conectar las columnas entre sí de forma invisible para mantener el orden" << endl;
            columna = cabeza->siguiente;
            while (columna && columna->siguiente)
            {
                fs << "col" << columna->usuario2->id << " -> col" << columna->siguiente->usuario2->id
                   << " [style=invis];" << endl;
                columna = columna->siguiente;
            }

            // Cerrar el archivo
            fs << "}" << endl;
            fs.close();

            // Generar la imagen con el comando dot
            string cmd = "dot -T" + formato + " " + path_dot + " -o " + path_render;
            system(cmd.c_str());
        }
    };
};

#endif // MATRIZRELACION_H
