#ifndef FUNCIONES_H
#define FUNCIONES_H

#include "Globales.h"
#include "ListaUsuarios.h"
#include "ListaPublicaciones.h"
#include "ListaPublicacionesFeed.h"
#include "../includes/nlohmann/json.hpp"

#include <iostream>
#include <string>
#include <fstream>

using json = nlohmann::json;
using namespace std;

namespace Func
{
    void RegistrarUsuario(string names, string lastnames, string birthday, string mail, string password)
    {
        int newid = lista_usuarios.getUltimoId() + 1;
        if (lista_usuarios.existeUsuario(mail))
        {
            cout << "El usuario ya existe" << endl;
        }
        else
        {
            ListaUsuarios::Usuario user(newid, names, lastnames, birthday, mail, password, "user");
            lista_usuarios.agregarUsuario(user);
            cout << "Usuario registrado correctamente" << endl;
        }
    }

    //* Funcion para cargar usuarios desde un archivo json
    void cargarUsuarios()
    {
        system("cls");
        int contador = 0;
        string directorio;
        cout << "|========== [Carga de Usuarios] ==========|" << endl;
        cout << "|      Directorio del archivo Json        |" << endl;
        cout << "|=========================================|" << endl;
        cin.ignore();
        cout << "[-] ";
        getline(cin, directorio);

        // Abrir el archivo JSON
        ifstream archivo(directorio);
        if (!archivo.is_open())
        {
            cerr << "No se pudo abrir el archivo: " << directorio << endl;
            system("pause");
            return;
        }

        // Parsear el archivo JSON
        json usuarios;
        archivo >> usuarios;

        // Recorrer el JSON
        for (const auto &usuario : usuarios)
        {
            // Verificar si las claves existen
            if (usuario.contains("nombres") && usuario.contains("apellidos") &&
                usuario.contains("correo") && usuario.contains("contraseña") &&
                usuario.contains("fecha_de_nacimiento"))
            {

                string nombres = usuario["nombres"].get<string>();
                string apellidos = usuario["apellidos"].get<string>();
                string correo = usuario["correo"].get<string>();
                string contrasena = usuario["contraseña"].get<string>();
                string fechaNacimiento = usuario["fecha_de_nacimiento"].get<string>();

                if (lista_usuarios.buscarUsuario(correo) == nullptr)
                {
                    // Crear Usuario
                    int newid = lista_usuarios.getUltimoId() + 1;
                    ListaUsuarios::Usuario nuevoUsuario(newid, nombres, apellidos, fechaNacimiento, correo, contrasena);
                    lista_usuarios.agregarUsuario(nuevoUsuario);
                    contador++;
                }
                else
                {
                    cerr << "El usuario ya existe" << endl;
                }
            }
            else
            {
                cerr << "Faltan campos en el JSON" << endl;
            }
        }
        archivo.close();
        cout << "[+] " << contador << " Usuarios cargados correctamente" << endl;
        system("pause");
    }

    void cargarRelaciones()
    {
        system("cls");
        int contador = 0;
        string directorio;
        cout << "|========== [Carga de Relaciones] ==========|" << endl;
        cout << "|       Directorio del archivo Json         |" << endl;
        cout << "|===========================================|" << endl;
        cin.ignore();
        cout << "[-] ";
        getline(cin, directorio);

        // Abrir el archivo JSON
        ifstream archivo(directorio);
        if (!archivo.is_open())
        {
            cerr << "No se pudo abrir el archivo: " << directorio << endl;
            system("pause");
            return;
        }

        // Parsear el archivo JSON
        json post;
        archivo >> post;

        // Recorrer el JSON
        for (const auto &post : post)
        {
            // Verificar si las claves existen
            if (post.contains("emisor") && post.contains("receptor") && post.contains("estado"))
            {

                string emisor = post["emisor"].get<string>();
                string receptor = post["receptor"].get<string>();
                string estado = post["estado"].get<string>();

                ListaUsuarios::Usuario *usuario_emisor = lista_usuarios.buscarUsuario(emisor);
                ListaUsuarios::Usuario *usuario_receptor = lista_usuarios.buscarUsuario(receptor);
                // verificar si los usuarios existen y especificar cual es el emisor y cual es el receptor
                if (usuario_emisor == nullptr)
                {
                    cerr << "El usuario emisor no existe" << endl;
                    continue;
                }
                if (usuario_receptor == nullptr)
                {
                    cerr << "El usuario receptor no existe" << endl;
                    continue;
                }
                // si el estado convertido en minusculas es pendiente
                if (estado == "pendiente")
                {
                    // al usuario emisor, en la lista de solicitudes se agregara el correo del usuario receptor
                    // al usuario receptor, en la pilas de solicitudes se agregara el correo del usuario emisor
                }
                else if (estado == "aceptada" || estado == "aceptado")
                {
                    // se crea la relacion de amistad entre los dos usuarios en la matriz dispersa
                }

                contador++;
            }
            else
            {
                cerr << "Faltan campos en el JSON" << endl;
            }
        }
        archivo.close();
        cout << "[+] " << contador << " Relaciones cargadas correctamente" << endl;
        system("pause");
    }

    void cargarPublicaciones()
    {
        system("cls");
        int contador = 0;
        string directorio;
        cout << "|========== [Carga de Publicaciones] ==========|" << endl;
        cout << "|          Directorio del archivo Json         |" << endl;
        cout << "|==============================================|" << endl;
        cin.ignore();
        cout << "[-] ";
        getline(cin, directorio);

        // Abrir el archivo JSON
        ifstream archivo(directorio);
        if (!archivo.is_open())
        {
            cerr << "No se pudo abrir el archivo: " << directorio << endl;
            system("pause");
            return;
        }

        // Parsear el archivo JSON
        json post;
        archivo >> post;

        // Recorrer el JSON
        for (const auto &post : post)
        {
            // Verificar si las claves existen
            if (post.contains("correo") && post.contains("contenido") &&
                post.contains("fecha") && post.contains("hora"))
            {

                string correo = post["correo"].get<string>();
                string contenido = post["contenido"].get<string>();
                string fecha = post["fecha"].get<string>();
                string hora = post["hora"].get<string>();

                ListaUsuarios::Usuario *autor = lista_usuarios.buscarUsuario(correo);

                if (autor == nullptr)
                {
                    cerr << "El usuario no existe" << endl;
                    continue;
                }

                // Crear Publicacion
                int newid = lista_publicaciones.obtenerUltimoId() + 1;
                ListaPublicaciones::Publicacion nuevaPublicacion(newid, autor->correo, contenido, fecha, hora);
                lista_publicaciones.agregarPublicacion(nuevaPublicacion);
                contador++;
            }
            else
            {
                cerr << "Faltan campos en el JSON" << endl;
            }
        }
        archivo.close();
        cout << "[+] " << contador << " Publicaciones cargados correctamente" << endl;
        system("pause");
    }

    void crearPublicacion(ListaUsuarios::Usuario usuario, string contenido, string fecha, string hora)
    {
        int newid = lista_publicaciones.obtenerUltimoId() + 1;
        ListaPublicaciones::Publicacion nuevaPublicacion(newid, usuario.correo, contenido, fecha, hora);
        lista_publicaciones.agregarPublicacion(nuevaPublicacion);
        cout << "Publicacion creada correctamente" << endl;
        system("pause");
    }

    string obtenerFecha()
    {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        string fecha = to_string(ltm->tm_mday) + "-" + to_string(1 + ltm->tm_mon) + "-" + to_string(1900 + ltm->tm_year);
        return fecha;
    }

    string obtenerHora()
    {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        string hora = to_string(ltm->tm_hour) + ":" + to_string(ltm->tm_min) + ":" + to_string(ltm->tm_sec);
        return hora;
    }

    ListaPublicacionesFeed::ListaCircularDoble cargarPublicacionesFeed()
    {
        // crear una lista circular temporal para retornar al final
        ListaPublicacionesFeed::ListaCircularDoble lista_temporal;
        for (int i = 0; i < lista_publicaciones.getLength() + 1; i++)
        {
            ListaPublicaciones::Publicacion publicacion = lista_publicaciones.ObtenerPublicacion(i);
            if (usuario_logeado->correo == publicacion.correo_autor)
            {
                lista_temporal.agregarPublicacion(publicacion);
            }
        }
        return lista_temporal;
    }

}

#endif // FUNCIONES_H