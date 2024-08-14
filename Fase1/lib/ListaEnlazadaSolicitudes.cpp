#include "../models/Estructuras.h"
#include <iostream>


void ListaEnlazadaSolicitudes::agregarSolicitud(const std::string& descripcion) {
    Solicitud* nuevaSolicitud = new Solicitud(descripcion);
    if (!cabeza) {
        cabeza = nuevaSolicitud;
    } else {
        Solicitud* temp = cabeza;
        while (temp->siguiente) {
            temp = temp->siguiente;
        }
        temp->siguiente = nuevaSolicitud;
    }
}

void ListaEnlazadaSolicitudes::imprimirSolicitudes() const {
    Solicitud* temp = cabeza;
    while (temp) {
        std::cout << "Solicitud: " << temp->descripcion << std::endl;
        temp = temp->siguiente;
    }
}

ListaEnlazadaSolicitudes::~ListaEnlazadaSolicitudes() {
    while (cabeza) {
        Solicitud* temp = cabeza;
        cabeza = cabeza->siguiente;
        delete temp;
    }
}
