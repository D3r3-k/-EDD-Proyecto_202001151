#include "models/Menus.h"
#include "models/Globales.h"
#include "models/ListaUsuarios.h"
#include "models/ListaPublicacionesFeed.h"
#include "models/Funciones.h"

#include <iostream>
#include <string>

using namespace std;

// TODO: Menú Principal
void menuPrincipal()
{
    int opcion = 0;
    while (opcion != 4)
    {
        system("cls");
        cout << "|========== [Menu] ==========|" << endl;
        cout << "| 1. Iniciar Sesión          |" << endl;
        cout << "| 2. Registrarse             |" << endl;
        cout << "| 3. Información             |" << endl;
        cout << "| 4. Salir                   |" << endl;
        cout << "|========== [Menu] ==========|" << endl;
        cout << "[-] Ingrese una opcion: ";
        cin >> opcion;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore();
            cout << "Entrada invalida. Por favor, ingrese un número." << endl;
            system("pause");
            continue;
        }

        string names, lastnames, birthday, mail, password;

        switch (opcion)
        {
        case 1:
            // Iniciar Sesión
            menuIniciarSesion();
            break;
        case 2:
            // Registrarse
            menuRegistrarse();
            break;
        case 3:
            // Información
            menuMostrarInformacion();
            break;
        case 4:
            // Salir
            cout << "Saliendo..." << endl;
            break;
        default:
            cout << "Opcion no valida" << endl;
            system("pause");
            break;
        }
    }
}
//? Submenús
void menuIniciarSesion()
{
    string mail, password;
    system("cls");
    cout << "|=========== [ Iniciar Sesión ] =========|" << endl;
    cout << "|               c. Cancelar              |" << endl;
    cout << "|========================================|" << endl;
    cout << "|Ingrese su usuario:                     |" << endl;
    cout << "|+| ";
    cin >> mail;
    if (mail == "c")
    {
        return;
    }
    cout << "|Ingrese su contraseña:                  |" << endl;
    cout << "|+| ";
    cin >> password;
    if (password == "c")
    {
        return;
    }
    usuario_logeado = lista_usuarios.buscarUsuario(mail, password);
    if (usuario_logeado != nullptr)
    {
        if (usuario_logeado->rol == "admin")
        {
            menuAdministrador();
        }
        else
        {
            menuUsuario();
        }
    }
    else
    {
        cout << "Usuario o contraseña incorrectos" << endl;
        system("pause");
    }
}

void menuRegistrarse()
{
    string names, lastnames, birthday, mail, password;
    system("cls");
    cout << "|========== [ Registrarse ] ==========|" << endl;
    cout << "|            c. Cancelar              |" << endl;
    cout << "|=====================================|" << endl;
    cout << "|Ingrese sus nombres:" << endl;
    cin.ignore();
    cout << "|+| ";
    getline(cin, names);
    if (names == "c")
    {
        return;
    }
    cout << "|Ingrese sus apellidos:" << endl;
    cout << "|+| ";
    getline(cin, lastnames);
    if (lastnames == "c")
    {
        return;
    }
    cout << "|Ingrese su fecha de nacimiento:" << endl;
    cout << "|+| ";
    getline(cin, birthday);
    if (birthday == "c")
    {
        return;
    }
    cout << "|Ingrese su correo:" << endl;
    cout << "|+| ";
    getline(cin, mail);
    if (mail == "c")
    {
        return;
    }
    cout << "|Ingrese su contraseña:" << endl;
    cout << "|+| ";
    getline(cin, password);
    if (password == "c")
    {
        return;
    }
    cout << "|=====================================|" << endl;

    Func::RegistrarUsuario(names, lastnames, birthday, mail, password);
    system("pause");
}

void menuMostrarInformacion()
{
    system("cls");
    cout << "|========== [ Información ] ==========|" << endl;
    cout << "| Laboratorio de Estructuras de Datos |" << endl;
    cout << "| Sección C                           |" << endl;
    cout << "| Derek Francisco Orellana Ibáñez     |" << endl;
    cout << "| 202001151                           |" << endl;
    cout << "|========== [ Información ] ==========|" << endl;
    system("pause");
}

// TODO: Menú Administrador
void menuAdministrador()
{
    int opcion;
    while (opcion != 6)
    {
        system("cls");
        cout << "|========== [Menu Administrador] ==========|" << endl;
        cout << "| 1. Carga de usuarios                     |" << endl;
        cout << "| 2. Carga de relaciones                   |" << endl;
        cout << "| 3. Carga de publicaciones                |" << endl;
        cout << "| 4. Gestionar usuarios                    |" << endl;
        cout << "| 5. Reportes                              |" << endl;
        cout << "| 6. Cerrar Sesión                         |" << endl;
        cout << "|========== [Menu Administrador] ==========|" << endl;
        cout << "[-] Ingrese una opcion: ";
        cin >> opcion;

        // Verificación de errores de entrada
        if (cin.fail())
        {
            cin.clear();
            cin.ignore();
            cout << "Entrada invalida. Por favor, ingrese un número." << endl;
            system("pause");
            continue;
        }

        switch (opcion)
        {
        case 1:
            // Carga de usuarios
            Func::cargarUsuarios();
            break;
        case 2:
            // Carga de relaciones
            Func::cargarRelaciones();
            break;
        case 3:
            // Carga de publicaciones
            Func::cargarPublicaciones();
            break;
        case 4:
            // Gestionar usuarios
            adminGestionarUsuarios();
            break;
        case 5:
            // Reportes (a implementar)
            adminGestionarReportes();
            break;
        case 6:
            // Cerrar sesión
            usuario_logeado = nullptr;
            break;
        default:
            cout << "Opcion no valida" << endl;
            system("pause");
            break;
        }
    }
}
//? Submenús
void adminGestionarUsuarios()
{
    int opcionGestion = 0;
    while (opcionGestion != 3)
    {
        system("cls");
        cout << "|========== [Gestionar Usuarios] ==========|" << endl;
        cout << "| 1. Listar Usuarios                       |" << endl;
        cout << "| 2. Eliminar Usuarios                     |" << endl;
        cout << "| 3. Regresar                              |" << endl;
        cout << "|========== [Gestionar Usuarios] ==========|" << endl;
        cout << "[-] Ingrese una opcion: ";
        cin >> opcionGestion;

        // Verificación de errores de entrada
        if (cin.fail())
        {
            cin.clear();
            cin.ignore();
            cout << "Entrada invalida. Por favor, ingrese un número." << endl;
            system("pause");
            continue;
        }

        switch (opcionGestion)
        {
        case 1:
            // Listar Usuarios
            system("cls");
            cout << "|================ [ Lista de Usuarios ] ===============|" << endl;
            lista_usuarios.listarUsuarios();
            system("pause");
            break;
        case 2:
            // Eliminar Usuarios
            system("cls");
            cout << "|================ [ Eliminar Usuarios ] ===============|" << endl;
            cout << "| Eliminando usuarios..." << endl;
            lista_usuarios.eliminarUsuarios(*usuario_logeado);
            cout << "| Usuarios eliminados correctamente" << endl;
            cout << "|======================================================|" << endl;
            system("pause");
            break;
        case 3:
            // Regresar
            break;
        default:
            cout << "Opcion no valida" << endl;
            system("pause");
            break;
        }
    }
}

void adminGestionarReportes()
{
    // TODO: Implementar
    // Usuarios -> grafico de la lista de usuarios
    // Publicaciones -> grafico de la lista doble enlazada
    // Relaciones -> grafico de la matriz dispersa
    // top:
    //      1. 5 Usuarios con más publicaciones
    //      2. 5 Usuarios con menos amigos
}

// TODO: Menú Usuario
void menuUsuario()
{
    int opcion = 0;
    while (opcion != 5)
    {
        system("cls");
        cout << "|========== [Menu Usuario] ==========|" << endl;
        cout << "| 1. Perfil                          |" << endl;
        cout << "| 2. Solicitudes                     |" << endl;
        cout << "| 3. Publicaciones                   |" << endl;
        cout << "| 4. Reportes                        |" << endl;
        cout << "| 5. Cerrar Sesión                   |" << endl;
        cout << "|========== [Menu Usuario] ==========|" << endl;
        cout << "[-] Ingrese una opcion: ";
        cin >> opcion;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore();
            cout << "Entrada invalida. Por favor, ingrese un número." << endl;
            system("pause");
            continue;
        }

        switch (opcion)
        {
        case 1:
            // Perfil
            userGestionarPerfil();
            break;
        case 2:
            // Solicitudes
            userGestionarSolicitudes();
            break;
        case 3:
            // Publicaciones
            userGestionarPublicaciones();
            break;
        case 4:
            // Reportes
            userGestionarReportes();
            break;
        case 5:
            // Cerrar sesión
            usuario_logeado = nullptr;
            break;
        default:
            cout << "Opcion no valida" << endl;
            system("pause");
            break;
        }
    }
}
//? Submenús
void userGestionarPerfil()
{
    int opcionPerfil = 0;
    while (opcionPerfil != 3)
    {
        system("cls");
        cout << "|========== [Perfil] ==========|" << endl;
        cout << "| 1. Ver Perfil                |" << endl;
        cout << "| 2. Eliminar Perfil           |" << endl;
        cout << "| 3. Regresar                  |" << endl;
        cout << "|========== [Perfil] ==========|" << endl;
        cout << "[-] Ingrese una opcion: ";
        cin >> opcionPerfil;

        // Verificación de errores de entrada
        if (cin.fail())
        {
            cin.clear();
            cin.ignore();
            cout << "Entrada invalida. Por favor, ingrese un número." << endl;
            system("pause");
            continue;
        }

        switch (opcionPerfil)
        {
        case 1:
            // Perfil
            system("cls");
            cout << "|========================= [ Perfil ] =========================|" << endl;
            usuario_logeado->mostrarPerfil();
            cout << "|==============================================================|" << endl;
            system("pause");
            break;
        case 2:
            // Eliminar Perfil
            menuEliminarPerfil();
            break;
        case 3:
            break; // Regresar
        default:
            cout << "Opcion no valida" << endl;
            system("pause");
            break;
        }
    }
}

void userGestionarPublicaciones()
{
    int opcionPublicaciones = 0;
    while (opcionPublicaciones != 4)
    {
        system("cls");
        cout << "|========== [Publicaciones] ==========|" << endl;
        cout << "| 1. Ver Publicaciones                |" << endl;
        cout << "| 2. Crear Publicación                |" << endl;
        cout << "| 3. Eliminar Publicación             |" << endl;
        cout << "| 4. Regresar                         |" << endl;
        cout << "|========== [Publicaciones] ==========|" << endl;
        cout << "[-] Ingrese una opcion: ";
        cin >> opcionPublicaciones;

        // Verificación de errores de entrada
        if (cin.fail())
        {
            cin.clear();
            cin.ignore();
            cout << "Entrada invalida. Por favor, ingrese un número." << endl;
            system("pause");
            continue;
        }

        switch (opcionPublicaciones)
        {
        case 1:
            // Ver Publicaciones
            menuMostrarPublicaciones();
            break;
        case 2:
            // Crear Publicación
            menuCrearPublicacion();
            break;
        case 3:
            // Eliminar Publicación
            menuEliminarPublicacion();
            break;
        case 4:
            break; // Regresar
        default:
            cout << "Opcion no valida" << endl;
            system("pause");
            break;
        }
    }
}

void userGestionarSolicitudes()
{
    // TODO: Implementar
    // ver solicitudes
    //      1. aceptar/rechazar
    // enviar solicitud
}

void userGestionarReportes()
{
    // TODO: Implementar
    // Solicitudes enviadas y recibidas: grafico de la lista de solicitudes enviadas y la pila de solicitudes recibidas
    // Relaciones: grafico de la matriz dispersa
    // Publicaciones: grafico de la lista circular doble del usuario y de amigos
    // mis amigos: listado de amigos
}

void menuEliminarPerfil()
{
    int opcionEliminar = 0;
    while (opcionEliminar != 2)
    {
        system("cls");
        cout << "|========================= [ Eliminar Perfil ] =========================|" << endl;
        cout << "|              ¿Está seguro que desea eliminar su perfil?               |" << endl;
        cout << "|                          [1] Si       [2] No                          |" << endl;
        cout << "|=======================================================================|" << endl;
        cout << "[+] Ingrese una opcion: ";
        cin >> opcionEliminar;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore();
            cout << "Entrada invalida. Por favor, ingrese un número." << endl;
            system("pause");
            continue;
        }
        switch (opcionEliminar)
        {
        case 1:
            // Eliminar publicaciones del usuario
            // Eliminar relaciones del usuario
            // Eliminar usuario
            // Cerrar sesión
            opcionEliminar = 2;
            break;
        case 2:
            break; // No
        default:
            cout << "Opcion no valida" << endl;
            system("pause");
            break;
        }
    }
}

void menuMostrarPublicaciones()
{
    ListaPublicacionesFeed::ListaCircularDoble lista_publicaciones_feed = Func::cargarPublicacionesFeed();
    int opcionGestion = 0;
    while (opcionGestion != 3)
    {
        system("cls");
        cout << "|============================== [ Mi Feed ] ==============================|" << endl;
        cout << "|                  [1] Siguiente [3] Regresar [2] Anterior                |" << endl;
        cout << "|=========================================================================|" << endl;
        lista_publicaciones_feed.mostrarPublicacionActual();
        cout << "[+] Ingrese una opcion: ";
        cin >> opcionGestion;

        // Verificación de errores de entrada
        if (cin.fail())
        {
            cin.clear();
            cin.ignore();
            cout << "Entrada invalida. Por favor, ingrese un número." << endl;
            system("pause");
            continue;
        }

        switch (opcionGestion)
        {
        case 1:
            // Siguiente
            lista_publicaciones_feed.avanzarPublicacion();
            break;
        case 2:
            // Anterior
            lista_publicaciones_feed.retrocederPublicacion();
            break;
        case 3:
            lista_publicaciones_feed.vaciarLista();
            break; // Regresar
        default:
            cout << "Opcion no valida" << endl;
            system("pause");
            break;
        }
    }
}

void menuCrearPublicacion()
{
    string contenido, fecha, hora;
    fecha = Func::obtenerFecha();
    hora = Func::obtenerHora();
    system("cls");
    cout << "|========== [ Crear Publicación ] ==========|" << endl;
    cout << "|    Fecha: " << fecha << "     Hora: " << hora << "    |" << endl;
    cout << "| Contenido:                                |" << endl;
    cout << "|===========================================|" << endl;
    cout << "|                  c. Cancelar              |" << endl;
    cout << "|===========================================|" << endl;
    cout << "|+| ";
    cin.ignore();
    getline(cin, contenido);
    if (contenido == "c")
    {
        return;
    }
    Func::crearPublicacion(*usuario_logeado, contenido, fecha, hora);
}

void menuEliminarPublicacion()
{
    ListaPublicacionesFeed::ListaCircularDoble lista_publicaciones_feed = Func::cargarPublicacionesFeed();
    int opcionGestion = 0;
    while (opcionGestion != 3)
    {
        system("cls");
        cout << "|========================= [ Mis Publicaciones ] =========================|" << endl;
        cout << "|                  [1] Siguiente [3] Regresar [2] Anterior                |" << endl;
        cout << "|                                [0] Eliminar                             |" << endl;
        cout << "|=========================================================================|" << endl;
        lista_publicaciones_feed.mostrarPublicacionActual();
        cout << "[+] Ingrese una opcion: ";
        cin >> opcionGestion;

        // Verificación de errores de entrada
        if (cin.fail())
        {
            cin.clear();
            cin.ignore();
            cout << "Entrada invalida. Por favor, ingrese un número." << endl;
            system("pause");
            continue;
        }

        int id = -1;
        switch (opcionGestion)
        {
        case 0:
            // Eliminar Publicación
            id = lista_publicaciones_feed.obtenerIdActual();
            if (id != -1)
            {
                lista_publicaciones.eliminarPublicacion(id);
                cout << "Publicación eliminada correctamente" << endl;
            }
            else
            {
                cout << "No hay una publicación para eliminar" << endl;
                system("pause");
            }
            opcionGestion = 3;
            break;
        case 1:
            // Siguiente
            lista_publicaciones_feed.avanzarPublicacion();
            break;
        case 2:
            // Anterior
            lista_publicaciones_feed.retrocederPublicacion();
            break;
        case 3:
            lista_publicaciones_feed.vaciarLista();
            break; // Regresar
        default:
            cout << "Opcion no valida" << endl;
            system("pause");
            break;
        }
    }
}
