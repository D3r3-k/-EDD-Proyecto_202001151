#include "Funciones.h"
#include "models/Estructuras.h"
#include "models/Globales.h"

#include "includes/nlohmann/json.hpp"

#include <iostream>
#include <string>
#include <windows.h>
#include <fstream>

using json = nlohmann::json;
using namespace std;

// Funcion para cargar usuarios desde un archivo json
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

            if (listaUsuarios.buscarUsuario(correo).correo == "")
            {
                // Crear Usuario
                Usuario nuevoUsuario(nombres, apellidos, fechaNacimiento, correo, contrasena);
                listaUsuarios.agregarUsuario(nuevoUsuario);
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

// Funcion para cargar relaciones desde un archivo json
void cargarRelaciones()
{
    std::cout << "Cargando relaciones desde un archivo json" << std::endl;
    system("pause");
}

// Funcion para cargar publicaciones desde un archivo json
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

            Usuario autor = listaUsuarios.buscarUsuario(correo);

            if (autor.correo == "")
            {
                cerr << "El usuario no existe" << endl;
                continue;
            }

            // Crear Publicacion
            Publicacion nuevaPublicacion(autor.correo, contenido, fecha, hora);
            listaPublicaciones.agregarPublicacion(nuevaPublicacion);

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

// Funcion para eliminar la cuenta de un usuario logeado
void eliminarMiCuenta(string mail)
{
    // Primero se verifica si el usuario existe
    // Se borra las publicaciones del usuario
    // Se borran las relaciones del usuario (lista de amigos de los usuarios y pila de solicitudes de los usuarios)
    // Se borra de la matriz relacion
    // Se borra el usuario de la lista de usuarios
}

// Función para limpiar la entrada en caso de error
void limpiarEntrada()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
