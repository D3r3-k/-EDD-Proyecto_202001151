#ifndef LISTAUSUARIOS_H
#define LISTAUSUARIOS_H

#include <iostream>
#include <string>
#include <stack>
#include <fstream>


#include "ListaSolicitudes.h"

using namespace std;

namespace ListaUsuarios
{
    struct Usuario
    {
        // Atributos
        int id;
        string nombres;
        string apellidos;
        string fechaNacimiento;
        string correo;
        string contrasena;
        string rol;
        ListaSolicitudes::ListaCircularDoble solicitudesEnviadas; // Solicitudes enviadas -> Lista Circular Doble
        stack<string> solicitudesRecibidas;                       // Solicitudes recibidas -> Pila

        // Constructor
        Usuario(const int &id,
                const string &nombres,
                const string &apellidos,
                const string &fechaNacimiento,
                const string &correo,
                const string &contrasena,
                const string &rol = "user")
            : id(id),
              nombres(nombres),
              apellidos(apellidos),
              fechaNacimiento(fechaNacimiento),
              correo(correo),
              contrasena(contrasena),
              rol(rol) {}

        // Métodos
        void mostrarPerfil() const
        {
            cout << "| ID:                  " << id << endl;
            cout << "| Nombres:             " << nombres << endl;
            cout << "| Apellidos:           " << apellidos << endl;
            cout << "| Fecha de Nacimiento: " << fechaNacimiento << endl;
            cout << "| Correo:              " << correo << endl;
        }
        // Métodos de la pila
        bool solicitudRecibidaExiste(const string &correo)
        {
            stack<string> tempPila = solicitudesRecibidas;
            while (!tempPila.empty())
            {
                if (tempPila.top() == correo)
                {
                    return true;
                }
                tempPila.pop();
            }
            return false;
        }
        void agregarSolicitudRecibida(const string &correo)
        {
            if (!solicitudRecibidaExiste(correo))
            {
                solicitudesRecibidas.push(correo);
                cout << "Solicitud de " << correo << " agregada exitosamente." << endl;
            }
            else
            {
                cout << "La solicitud de " << correo << " ya existe." << endl;
            }
        }
        void mostrarSolicitudesRecibidas()
        {
            if (solicitudesRecibidas.empty())
            {
                cout << "|                          No hay Solicitudes                                 |" << endl;
                return;
            }
            stack<string> tempPila = solicitudesRecibidas;
            while (!tempPila.empty())
            {
                cout << "| " << tempPila.top() << endl;
                tempPila.pop();
            }
        }
        void mostrarSolicitudRecibida()
        {
            if (solicitudesRecibidas.empty())
            {
                cout << "|                          No hay Solicitudes                                 |" << endl;
                return;
            }
            cout << "| " << solicitudesRecibidas.top() << endl;
        }

        // Métodos de la lista circular doble
        bool solicitudEnviadaExiste(const string &correo)
        {
            return solicitudesEnviadas.existeSolicitud(correo);
        }
        void agregarSolicitudEnviada(const string &correo)
        {
            if (!solicitudEnviadaExiste(correo))
            {
                solicitudesEnviadas.agregarSolicitud(correo);
                cout << "Solicitud a " << correo << " enviada exitosamente." << endl;
            }
            else
            {
                cout << "La solicitud a " << correo << " ya existe." << endl;
            }
        }
        void mostrarSolicitudesEnviadas()
        {
            solicitudesEnviadas.mostrarSolicitudes();
        }
    };

    class ListaEnlazadaSimple
    {
    private:
        struct NodoUsuario
        {
            Usuario usuario;
            NodoUsuario *siguiente;
            NodoUsuario(const Usuario &u) : usuario(u), siguiente(nullptr) {}
        };

        NodoUsuario *cabeza;

    public:
        ListaEnlazadaSimple() : cabeza(nullptr) {}

        ~ListaEnlazadaSimple()
        {
            NodoUsuario *aux = cabeza;
            while (aux != nullptr)
            {
                NodoUsuario *temp = aux;
                aux = aux->siguiente;
                delete temp;
            }
        }

        void agregarUsuario(const Usuario &u)
        {
            NodoUsuario *nuevo = new NodoUsuario(u);
            if (cabeza == nullptr)
            {
                cabeza = nuevo;
            }
            else
            {
                NodoUsuario *aux = cabeza;
                while (aux->siguiente != nullptr)
                {
                    aux = aux->siguiente;
                }
                aux->siguiente = nuevo;
            }
        }

        void listarUsuarios() const
        {
            NodoUsuario *aux = cabeza;
            while (aux != nullptr)
            {
                aux->usuario.mostrarPerfil();
                cout << "|======================================================|" << endl;
                aux = aux->siguiente;
            }
        }

        Usuario *buscarUsuario(const int &pos)
        {
            NodoUsuario *temp = cabeza;
            int contador = 1;
            while (temp)
            {
                if (contador == pos)
                {
                    return &(temp->usuario);
                }
                temp = temp->siguiente;
                contador++;
            }
            return nullptr; // Usuario no encontrado
        }

        Usuario *buscarUsuario(const string &correo, const string &contrasena)
        {
            NodoUsuario *temp = cabeza;
            while (temp)
            {
                if (temp->usuario.correo == correo && temp->usuario.contrasena == contrasena)
                {
                    return &(temp->usuario);
                }
                temp = temp->siguiente;
            }
            return nullptr; // Usuario no encontrado
        }

        Usuario *buscarUsuario(const string &correo)
        {
            NodoUsuario *temp = cabeza;
            while (temp)
            {
                if (temp->usuario.correo == correo)
                {
                    return &(temp->usuario);
                }
                temp = temp->siguiente;
            }
            return nullptr; // Usuario no encontrado
        }

        int getSize()
        {
            NodoUsuario *temp = cabeza;
            int contador = 0;
            while (temp)
            {
                contador++;
                temp = temp->siguiente;
            }
            return contador;
        }

        int getUltimoId()
        {
            NodoUsuario *temp = cabeza;
            int id = 0;
            while (temp)
            {
                id = temp->usuario.id;
                temp = temp->siguiente;
            }
            return id;
        }

        bool existeUsuario(const string &correo)
        {
            NodoUsuario *temp = cabeza;
            while (temp)
            {
                if (temp->usuario.correo == correo)
                {
                    return true;
                }
                temp = temp->siguiente;
            }
            return false;
        }

        void eliminarUsuarios(const Usuario &logeado)
        {
            NodoUsuario *temp = cabeza;
            NodoUsuario *prev = nullptr;
            while (temp)
            {
                if (temp->usuario.correo != logeado.correo)
                {
                    if (prev)
                    {
                        prev->siguiente = temp->siguiente;
                    }
                    else
                    {
                        cabeza = temp->siguiente;
                    }
                    delete temp;
                    temp = prev->siguiente;
                }
                else
                {
                    prev = temp;
                    temp = temp->siguiente;
                }
            }
        }

        void eliminarUsuario(const Usuario *usuario)
        {
            NodoUsuario *temp = cabeza;
            NodoUsuario *prev = nullptr;
            while (temp)
            {
                if (temp->usuario.correo == usuario->correo)
                {
                    if (prev)
                    {
                        prev->siguiente = temp->siguiente;
                    }
                    else
                    {
                        cabeza = temp->siguiente;
                    }
                    delete temp;
                    break;
                }
                prev = temp;
                temp = temp->siguiente;
            }
        }

        void agregarSolicitudRecibida(const string &correoEmisor, const string &correoReceptor)
        {
            Usuario *receptor = buscarUsuario(correoReceptor);
            if (receptor != nullptr)
            {
                receptor->agregarSolicitudRecibida(correoEmisor);
            }
            else
            {
                cout << "Usuario receptor no encontrado." << endl;
            }
        }

        void agregarSolicitudEnviada(const string &correoEmisor, const string &correoReceptor)
        {
            Usuario *emisor = buscarUsuario(correoEmisor);
            if (emisor != nullptr)
            {
                emisor->agregarSolicitudEnviada(correoReceptor);
            }
            else
            {
                cout << "Usuario emisor no encontrado." << endl;
            }
        }

        void eliminarSolicitudEnviada(const string &correoEmisor, const string &correoReceptor)
        {
            Usuario *emisor = buscarUsuario(correoEmisor);
            if (emisor != nullptr)
            {
                emisor->solicitudesEnviadas.eliminarSolicitud(correoReceptor);
            }
            else
            {
                cout << "Usuario emisor no encontrado." << endl;
            }
        }
        void eliminarSolicitudRecibida(const string &correoEmisor, const string &correoReceptor)
        {
            Usuario *receptor = buscarUsuario(correoReceptor);
            if (receptor != nullptr)
            {
                stack<string> tempPila = receptor->solicitudesRecibidas;
                stack<string> auxPila;
                while (!tempPila.empty())
                {
                    if (tempPila.top() != correoEmisor)
                    {
                        auxPila.push(tempPila.top());
                    }
                    tempPila.pop();
                }
                receptor->solicitudesRecibidas = auxPila;
            }
            else
            {
                cout << "Usuario receptor no encontrado." << endl;
            }
        }

        void eliminarSolicitudes(const string &correo)
        {
            NodoUsuario *temp = cabeza;
            while (temp)
            {
                // Eliminar de la lista de solicitudes enviadas
                temp->usuario.solicitudesEnviadas.eliminarSolicitud(correo);

                // Procesar la pila de solicitudes recibidas
                stack<string> pilaActual = temp->usuario.solicitudesRecibidas;
                stack<string> nuevaPila;

                // Recorrer la pila original
                while (!pilaActual.empty())
                {
                    if (pilaActual.top() != correo)
                    {
                        nuevaPila.push(pilaActual.top());
                    }
                    pilaActual.pop();
                }

                // Asignar la nueva pila (con las solicitudes filtradas)
                temp->usuario.solicitudesRecibidas = nuevaPila;

                // Pasar al siguiente usuario
                temp = temp->siguiente;
            }
        }

        // Método para graficar con Graphviz
        void graficarUsuarios()
        {
            ofstream archivo("lista_usuarios.dot");
            if (archivo.is_open())
            {
                archivo << "digraph G {" << endl;
                archivo << "node [shape=record];" << endl;
                NodoUsuario *temp = cabeza;
                while (temp)
                {
                    archivo << "nodo" << temp->usuario.id << " [label=\"{ID: " << temp->usuario.id << " | Nombres: " << temp->usuario.nombres << " | Apellidos: " << temp->usuario.apellidos << " | Fecha de Nacimiento: " << temp->usuario.fechaNacimiento << " | Correo: " << temp->usuario.correo << "}\"];" << endl;
                    temp = temp->siguiente;
                }
                temp = cabeza;
                while (temp->siguiente)
                {
                    archivo << "nodo" << temp->usuario.id << " -> nodo" << temp->siguiente->usuario.id << ";" << endl;
                    temp = temp->siguiente;
                }
                archivo << "}" << endl;
                archivo.close();
                system("dot -Tpng lista_usuarios.dot -o lista_usuarios.png");
            }
            else
            {
                cout << "Error al abrir el archivo." << endl;
            }
        }
    };
}

#endif // LISTAUSUARIOS_H
