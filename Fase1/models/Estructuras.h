#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

#include <string>
#include <stack>
#include <iostream>

//? Estructura para almacenar una solicitud
struct Solicitud
{
    // Atributos
    std::string descripcion;
    Solicitud *siguiente;
    // Constructor
    Solicitud(const std::string &desc) : descripcion(desc), siguiente(nullptr) {}
};

//? Clase Lista Simple Enlazada para almacenar solicitudes
class ListaEnlazadaSolicitudes
{
private:
    // Atributos
    Solicitud *cabeza;

public:
    // Constructor
    ListaEnlazadaSolicitudes() : cabeza(nullptr) {}
    // Métodos
    void agregarSolicitud(const std::string &descripcion);
    void imprimirSolicitudes() const;
    // Destructor
    ~ListaEnlazadaSolicitudes();
};

//? Estructura Usuario
struct Usuario
{
    // Atributos
    std::string nombres;
    std::string apellidos;
    std::string fechaNacimiento;
    std::string correo;
    std::string contrasena;
    std::string rol;
    ListaEnlazadaSolicitudes solicitudesEnviadas;
    std::stack<std::string> solicitudesPendientes;
    // Constructor
    Usuario(const std::string &n, const std::string &a, const std::string &fn, const std::string &c, const std::string &cont, const std::string &r = "user")
        : nombres(n), apellidos(a), fechaNacimiento(fn), correo(c), contrasena(cont), rol(r) {}

    // Métodos
    void mostrarPerfil() const{
        std::cout << "| Nombres: " << nombres << std::endl;
        std::cout << "| Apellidos: " << apellidos << std::endl;
        std::cout << "| Fecha de Nacimiento: " << fechaNacimiento << std::endl;
        std::cout << "| Correo: " << correo << std::endl;
        std::cout << "| Rol: " << rol << std::endl;
    }
};

//? Estructura Publicacion
struct Publicacion
{
    // Atributos
    std::string correo_autor;
    std::string contenido;
    std::string fecha;
    std::string hora;
    // Constructor
    Publicacion(std::string ca, const std::string &c, const std::string &f, const std::string &h)
        : correo_autor(ca), contenido(c), fecha(f), hora(h) {}

    // Métodos
    void mostrarPublicacion() const{
        std::cout << "|===============[ Publicación ]===============|" << std::endl;
        std::cout << "| Autor: " << correo_autor << std::endl;
        std::cout << "| Fecha: " << fecha << std::endl;
        std::cout << "| Hora: " << hora << std::endl;
        std::cout << "| Contenido: " << contenido << std::endl;
        std::cout << "|============================================|" << std::endl;
    }
};

#endif // ESTRUCTURAS_H