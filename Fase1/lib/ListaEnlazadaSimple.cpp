#include "../models/ListaEnlazadaSimple.h"

ListaEnlazadaUsuarios::~ListaEnlazadaUsuarios()
{
    while (cabeza)
    {
        NodoUsuario *temp = cabeza;
        cabeza = cabeza->siguiente;
        delete temp;
    }
}

void ListaEnlazadaUsuarios::agregarUsuario(const Usuario &usuario)
{
    NodoUsuario *nuevoNodo = new NodoUsuario(usuario);
    if (!cabeza)
    {
        cabeza = nuevoNodo;
    }
    else
    {
        NodoUsuario *temp = cabeza;
        while (temp->siguiente)
        {
            temp = temp->siguiente;
        }
        temp->siguiente = nuevoNodo;
    }
}

void ListaEnlazadaUsuarios::imprimirUsuarios() const
{
    NodoUsuario *temp = cabeza;
    while (temp)
    {
        std::cout << "Usuario: " << temp->usuario.nombres << " " << temp->usuario.apellidos << std::endl;
        std::cout << "Correo: " << temp->usuario.correo << std::endl;
        std::cout << "Fecha de Nacimiento: " << temp->usuario.fechaNacimiento << std::endl;
        std::cout << "Solicitudes Enviadas:" << std::endl;
        temp->usuario.solicitudesEnviadas.imprimirSolicitudes();
        std::cout << "Solicitudes Pendientes:" << std::endl;
        std::stack<std::string> pendientes = temp->usuario.solicitudesPendientes;
        while (!pendientes.empty())
        {
            std::cout << " - " << pendientes.top() << std::endl;
            pendientes.pop();
        }
        std::cout << std::endl;
        temp = temp->siguiente;
    }
}
void ListaEnlazadaUsuarios::eliminarUsuarios(const Usuario &logeado)
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

void ListaEnlazadaUsuarios::listarUsuarios()
{
    // recorrer la lista de usuarios y llamar a la función mostrarPerfil
    NodoUsuario *temp = cabeza;
    int contador = 1;
    while (temp)
    {
        
        std::cout << "|===============[ Perfil " << contador << " ]===============|" << std::endl;
        temp->usuario.mostrarPerfil();
        temp = temp->siguiente;
        contador++;
    }
    std::cout << "|==========================================|" << std::endl;
}

Usuario ListaEnlazadaUsuarios::buscarUsuario(const std::string &correo, const std::string &contrasena)
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
    return Usuario("", "", "", "", "", "");
}

Usuario ListaEnlazadaUsuarios::buscarUsuario(const std::string &correo)
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
    return Usuario("", "", "", "", "", "");
}

bool ListaEnlazadaUsuarios::eliminarUsuario(const std::string &correo)
{
    NodoUsuario *temp = cabeza;
    NodoUsuario *prev = nullptr;
    while (temp)
    {
        if (temp->usuario.correo == correo)
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
            return true;
        }
        prev = temp;
        temp = temp->siguiente;
    }
    return false;
}

bool ListaEnlazadaUsuarios::existeUsuario(const std::string &correo)
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
