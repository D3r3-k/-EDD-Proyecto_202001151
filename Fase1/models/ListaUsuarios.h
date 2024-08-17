#ifndef LISTAUSUARIOS_H
#define LISTAUSUARIOS_H

#include <iostream>
#include <string>

using namespace std;

namespace ListaUsuarios
{

    struct Usuario
    {
        // Atributos
        int id;
        std::string nombres;
        std::string apellidos;
        std::string fechaNacimiento;
        std::string correo;
        std::string contrasena;
        std::string rol;
        // ListaEnlazadaSolicitudes solicitudesEnviadas; // Solicitudes enviadas -> Lista Enlazada Simple
        // std::stack<Solicitud> solicitudesRecibidas; // Solicitudes recibidas -> Pila
        // Constructor
        Usuario(const int &id,
                const std::string &nombres,
                const std::string &apellidos,
                const std::string &fechaNacimiento,
                const std::string &correo,
                const std::string &contrasena,
                const std::string &rol = "user")
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
            std::cout << "| ID:                  " << id << std::endl;
            std::cout << "| Nombres:             " << nombres << std::endl;
            std::cout << "| Apellidos:           " << apellidos << std::endl;
            std::cout << "| Fecha de Nacimiento: " << fechaNacimiento << std::endl;
            std::cout << "| Correo:              " << correo << std::endl;
        }
    };

    class ListaEnlazadaSimple
    {
    private:
        struct NodoUsuario
        {
            // Atributos
            Usuario usuario;
            NodoUsuario *siguiente;
            // Constructor
            NodoUsuario(const Usuario &u) : usuario(u), siguiente(nullptr) {}
        };

        NodoUsuario *cabeza;

    public:
        // Constructor
        ListaEnlazadaSimple() : cabeza(nullptr) {}
        // Destructor
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
        // Métodos
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
                std::cout << "|======================================================|" << std::endl;
                aux = aux->siguiente;
            }
        }

        Usuario buscarUsuario(const int &pos)
        {
            NodoUsuario *temp = cabeza;
            int contador = 1;
            while (temp)
            {
                if (contador == pos)
                {
                    return temp->usuario;
                }
                temp = temp->siguiente;
                contador++;
            }
            return Usuario(-1, "", "", "", "", "", "");
        }

        Usuario buscarUsuario(const std::string &correo, const std::string &contrasena)
        {
            NodoUsuario *temp = cabeza;
            while (temp)
            {
                if (temp->usuario.correo == correo && temp->usuario.contrasena == contrasena)
                {
                    return temp->usuario;
                }
                temp = temp->siguiente;
            }
            return Usuario(-1, "", "", "", "", "", "");
        }

        Usuario buscarUsuario(const std::string &correo)
        {
            NodoUsuario *temp = cabeza;
            while (temp)
            {
                if (temp->usuario.correo == correo)
                {
                    return temp->usuario;
                }
                temp = temp->siguiente;
            }
            return Usuario(-1, "", "", "", "", "", "");
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

        bool existeUsuario(const std::string &correo)
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
            // eliminar usuarios excepto el usuario logeado
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
    };
}
#endif // LISTAUSUARIOS_H