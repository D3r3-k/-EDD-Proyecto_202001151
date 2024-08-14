#include "../models/ListaEnlazadaDoble.h"
#include <iostream>

ListaEnlazadaDoblePublicaciones::~ListaEnlazadaDoblePublicaciones()
{
    while (cabeza)
    {
        NodoPublicacion *temp = cabeza;
        cabeza = cabeza->siguiente;
        delete temp;
    }
}

void ListaEnlazadaDoblePublicaciones::agregarPublicacion(const Publicacion &publicacion)
{
    NodoPublicacion *nuevoNodo = new NodoPublicacion(publicacion);
    if (!cabeza)
    {
        cabeza = nuevoNodo;
        cola = nuevoNodo;
    }
    else
    {
        cola->siguiente = nuevoNodo;
        nuevoNodo->anterior = cola;
        cola = nuevoNodo;
    }
}
void ListaEnlazadaDoblePublicaciones::imprimirPublicaciones() const
{
    NodoPublicacion *temp = cabeza;
    int contador = 1;
    while (temp)
    {
        std::cout << "| "<< contador <<" |==================================================|" << std::endl;
        std::cout << "| Publicación de: " << temp->publicacion.correo_autor << std::endl;
        std::cout << "| Fecha: " << temp->publicacion.fecha << ", Hora: " << temp->publicacion.hora << std::endl;
        std::cout << "| Contenido: " << temp->publicacion.contenido << std::endl;
        temp = temp->siguiente;
        contador++;
    }
}

void ListaEnlazadaDoblePublicaciones::eliminarPublicacion(const Publicacion &publicacion)
{
    NodoPublicacion *temp = cabeza;
    while (temp)
    {
        if (temp->publicacion.correo_autor == publicacion.correo_autor &&
            temp->publicacion.contenido == publicacion.contenido &&
            temp->publicacion.fecha == publicacion.fecha &&
            temp->publicacion.hora == publicacion.hora)
        {
            if (temp == cabeza)
            {
                cabeza = temp->siguiente;
                if (cabeza)
                {
                    cabeza->anterior = nullptr;
                }
            }
            else if (temp == cola)
            {
                cola = temp->anterior;
                cola->siguiente = nullptr;
            }
            else
            {
                temp->anterior->siguiente = temp->siguiente;
                temp->siguiente->anterior = temp->anterior;
            }
            delete temp;
            return;
        }
        temp = temp->siguiente;
    }
}

void ListaEnlazadaDoblePublicaciones::listarPublicaciones()
{
    std::cout << "|               [Lista de Publicaciones]               |" << std::endl;
    imprimirPublicaciones();
    std::cout << "|======================================================|" << std::endl;

}