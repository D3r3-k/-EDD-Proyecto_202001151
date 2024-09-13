#ifndef STRUCTS_H
#define STRUCTS_H

#include <iostream>
#include <string>

#include "ListaEnlazada.h"
// #include "../models/ArbolB5.h"

using namespace std;

namespace Structs
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
        ListaEnlazada::ListaEnlazada<Usuario> solicitudesEnviadas;  // Solicitudes Enviadas -> Lista Simple
        ListaEnlazada::ListaEnlazada<Usuario> solicitudesRecibidas; // Solicitudes recibidas -> Lista Simple

        // Constructor
        Usuario(int id, string nombres,
                string apellidos,
                string fechaNacimiento,
                string correo,
                string contrasena,
                string rol = "user") : id(id),
                                       nombres(nombres),
                                       apellidos(apellidos),
                                       fechaNacimiento(fechaNacimiento),
                                       correo(correo),
                                       contrasena(contrasena),
                                       rol(rol) {}

        // TODO: Metodos
        void mostrarPerfil() const
        {
            cout << "| ID:                  " << id << endl;
            cout << "| Nombres:             " << nombres << endl;
            cout << "| Apellidos:           " << apellidos << endl;
            cout << "| Fecha de Nacimiento: " << fechaNacimiento << endl;
            cout << "| Correo:              " << correo << endl;
        }
        void modificar(string nombres, string apellidos, string fecha, string contrasena)
        {
            this->nombres = nombres;
            this->apellidos = apellidos;
            this->fechaNacimiento = fecha;
            this->contrasena = contrasena;
        }

        // Enviar Solicitud
        void enviarSolicitud(const Usuario& u){
            this->solicitudesEnviadas.insertar(u);
        }
        void cancelarSolicitud(const string correo){
            for (int i = 0; i < this->solicitudesEnviadas.size(); ++i) {
                Usuario *temp = this->solicitudesEnviadas.obtener(i);
                if (temp) {
                    if (temp->correo == correo) {
                        this->solicitudesEnviadas.eliminar(i);
                        break;
                    }
                }
            }
        }

        // Recibir Solicitud
        void recibirSolicitud(const Usuario& u){
            this->solicitudesRecibidas.insertar(u);
        }
        void aceptarSolicitudRec(const string correo){
            for (int i = 0; i < this->solicitudesRecibidas.size(); ++i) {
                Usuario *temp = this->solicitudesRecibidas.obtener(i);
                if (temp) {
                    if (temp->correo == correo) {
                        this->solicitudesRecibidas.eliminar(i);
                        break;
                    }
                }
            }
        }
        void aceptarSolicitudEnv(const string correo){
            for (int i = 0; i < this->solicitudesEnviadas.size(); ++i) {
                Usuario *temp = this->solicitudesEnviadas.obtener(i);
                if (temp) {
                    if (temp->correo == correo) {
                        this->solicitudesEnviadas.eliminar(i);
                        break;
                    }
                }
            }
        }
        void rechazarSolicitud(const string correo){
            for (int i = 0; i < this->solicitudesRecibidas.size(); ++i) {
                Usuario *temp = this->solicitudesRecibidas.obtener(i);
                if (temp) {
                    if (temp->correo == correo) {
                        this->solicitudesRecibidas.eliminar(i);
                        break;
                    }
                }
            }
        }
        
        
    };

    struct Publicacion
    {
        // Atributos
        int id;
        string correo_autor;
        string contenido;
        string fecha;
        string hora;
        // ArbolB5::ArbolB5 comentarios; // Lista de comentarios -> Arbol B5
        // Constructor
        Publicacion(int id, string ca, const string &c, const string &f, const string &h)
            : id(id), correo_autor(ca), contenido(c), fecha(f), hora(h) {}

        // Destructor
        ~Publicacion() {}

        // TODO: Metodos
        void mostrarPublicacion() const
        {
            cout << "|===============[ Publicación ]===============|" << endl;
            cout << "| Autor: " << correo_autor << endl;
            cout << "| Fecha: " << fecha << endl;
            cout << "| Hora: " << hora << endl;
            cout << "| Contenido: " << contenido << endl;
            cout << "|============================================|" << endl;
        }
    };
}

#endif // STRUCTS_H
