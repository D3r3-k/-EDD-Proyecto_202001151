#ifndef LISTAUSUARIOS_H
#define LISTAUSUARIOS_H

#include <iostream>
#include <string>
#include <fstream>

#include "ListaSolicitudes.h"
#include "Pila.h"

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
        PilaSolicitudes::Pila solicitudesRecibidas;               // Solicitudes recibidas -> Pila

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
            return solicitudesRecibidas.existeCorreo(correo);
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
            PilaSolicitudes::Pila tempPila = solicitudesRecibidas;
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

        // Métodos para graficar
        void graficarSolicitudes(string nombre, string formato)
        {
            string path_render = "renders/" + nombre + "." + formato;
            string path_dot = "renders/" + nombre + ".dot";
            ofstream fs(path_dot);
            if (fs.is_open())
            {
                fs << "digraph G {" << endl;
                fs << "node [shape=rectangle];" << endl;
                fs << "rankdir = TB;" << endl;
                fs << "label=\"Lista de Solicitudes de: " << nombres << "\" fontsize = 20 fontname = \"Arial\";" << endl;
                // Solicitudes enviadas
                fs << "subgraph cluster_0 {" << endl;
                fs << "label=\"Solicitudes Enviadas\";" << endl;
                for (int i = 0; i < solicitudesEnviadas.getSize() + 1; i++)
                {
                    string _correo = solicitudesEnviadas.obtenerSolicitud(i);
                    if (solicitudesEnviadas.obtenerSolicitudActual() == "")
                    {
                        continue;
                    }
                    if (i + 1 != solicitudesEnviadas.getSize() + 1)
                    {
                        fs << "enviada" << i << "[label=\"" << _correo << "\"];" << endl;
                    }
                }

                for (int i = 0; i < solicitudesEnviadas.getSize() + 1; i++)
                {
                    if (solicitudesEnviadas.obtenerSolicitudActual() == "")
                    {
                        break;
                    }
                    if (i + 1 < solicitudesEnviadas.getSize())
                    {
                        fs << "enviada" << i << " -> enviada" << i + 1 << " [style=invis];" << endl;
                    }
                }

                fs << "}" << endl;
                // Solicitudes recibidas
                fs << "subgraph cluster_1 {" << endl;
                fs << "label=\"Solicitudes Recibidas\";" << endl;
                PilaSolicitudes::Pila tempPila = solicitudesRecibidas;
                int contador = 0;
                while (!tempPila.empty())
                {
                    fs << "recibida" << contador << "[label=\"" << tempPila.top() << "\"];" << endl;
                    tempPila.pop();
                    contador++;
                }

                for (int i = 0; i < contador; i++)
                {
                    if (i + 1 < contador)
                    {
                        fs << "recibida" << i << " -> recibida" << i + 1 << " [style=invis];" << endl;
                    }
                }

                fs << "}" << endl;

                fs << "}" << endl;
                fs.close();
                // system("dot -Tpng lista_usuarios.dot -o lista_usuarios.png");
                string cmd = "dot -T" + formato + " " + path_dot + " -o " + path_render;
                system(cmd.c_str());
            }
            else
            {
                cout << "Error al abrir el archivo." << endl;
            }
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
                PilaSolicitudes::Pila tempPila = receptor->solicitudesRecibidas;
                PilaSolicitudes::Pila auxPila;
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
                PilaSolicitudes::Pila pilaActual = temp->usuario.solicitudesRecibidas;
                PilaSolicitudes::Pila nuevaPila;

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
        void graficarUsuarios(string nombre, string formato)
        {
            string path_render = "renders/" + nombre + "." + formato;
            string path_dot = "renders/" + nombre + ".dot";
            ofstream fs(path_dot);
            if (fs.is_open())
            {
                fs << "digraph G {" << endl;
                fs << "node [shape=record];" << endl;
                fs << "rankdir = TB;" << endl;
                fs << "label=\"Lista de Usuarios\" fontsize = 20 fontname = \"Arial\";" << endl;
                NodoUsuario *temp = cabeza;
                while (temp)
                {
                    fs << "nodo" << temp->usuario.id << " [label=\"{ID: " << temp->usuario.id << " | Nombres: " << temp->usuario.nombres << " | Apellidos: " << temp->usuario.apellidos << " | Fecha de Nacimiento: " << temp->usuario.fechaNacimiento << " | Correo: " << temp->usuario.correo << "}\"];" << endl;
                    temp = temp->siguiente;
                }
                temp = cabeza;
                while (temp->siguiente)
                {
                    fs << "nodo" << temp->usuario.id << " -> nodo" << temp->siguiente->usuario.id << ";" << endl;
                    temp = temp->siguiente;
                }
                fs << "}" << endl;
                fs.close();
                // system("dot -Tpng lista_usuarios.dot -o lista_usuarios.png");
                string cmd = "dot -T" + formato + " " + path_dot + " -o " + path_render;
                system(cmd.c_str());
            }
            else
            {
                cout << "Error al abrir el archivo." << endl;
            }
        }
    };
}

#endif // LISTAUSUARIOS_H
