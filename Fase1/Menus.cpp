#include "Funciones.h"
#include "models/Estructuras.h"
#include "models/Globales.h"

#include <iostream>
#include <string>
#include <windows.h>

using namespace std;
//? Funcion para registrar un usuario
void registrarUsuario(string names, string lastnames, string birthday, string mail, string password)
{
    if (listaUsuarios.existeUsuario(mail))
    {
        cout << "El usuario ya existe" << endl;
    }
    else
    {
        Usuario user(names, lastnames, birthday, mail, password);
        listaUsuarios.agregarUsuario(user);
        cout << "Usuario registrado correctamente" << endl;
    }
}


// TODO: Menú Administrador
void menuAdministrador()
{
    int opcion;
    do
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
            limpiarEntrada();
            cout << "Entrada invalida. Por favor, ingrese un número." << endl;
            system("pause");
            continue; // Repite el ciclo si la entrada es inválida
        }

        switch (opcion)
        {
        case 1:
            // Carga de usuarios
            cargarUsuarios();
            break;
        case 2:
            // Carga de relaciones
            cargarRelaciones();
            break;
        case 3:
            // Carga de publicaciones
            cargarPublicaciones();
            break;
        case 4:
            // Gestionar usuarios
            adminGestionarUsuarios();
            break;
        case 5:
            // Reportes (a implementar)
            break;
        case 6:
            // Cerrar sesión
            usuario_logeado = Usuario("", "", "", "", "", ""); // Ajustar según implementación
            break;
        default:
            cout << "Opcion no valida" << endl;
            system("pause");
            break;
        }
    } while (opcion != 6);
}
//? Submenús Administrador
void adminGestionarUsuarios()
{
    int opcionGestion;
    do
    {
        system("cls");
        cout << "|========== [Gestionar Usuarios] ==========|" << endl;
        cout << "| 1. Eliminar Usuarios                     |" << endl;
        cout << "| 2. Listar Usuarios                       |" << endl;
        cout << "| 3. Regresar                              |" << endl;
        cout << "|========== [Gestionar Usuarios] ==========|" << endl;
        cout << "[-] Ingrese una opcion: ";
        cin >> opcionGestion;

        // Verificación de errores de entrada
        if (cin.fail())
        {
            limpiarEntrada();
            cout << "Entrada invalida. Por favor, ingrese un número." << endl;
            system("pause");
            continue; // Repite el ciclo si la entrada es inválida
        }

        switch (opcionGestion)
        {
        case 1:
            // Eliminar Usuarios
            cout << "==========[" << usuario_logeado.correo << "]==========" << endl;
            cout << "Eliminando usuarios..." << endl;
            listaUsuarios.eliminarUsuarios(usuario_logeado);
            cout << "Usuarios eliminados correctamente" << endl;
            system("pause");
            break;
            break;
        case 2:
            // Listar Usuarios
            system("cls");
            listaUsuarios.listarUsuarios();
            system("pause");
            break;
            break;
        case 3:
            break; // Regresar
        default:
            cout << "Opcion no valida" << endl;
            system("pause");
            break;
        }
    } while (opcionGestion != 3);
}

// TODO: Menú Usuario
void menuUsuario()
{
    int opcion;
    do
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
            system("cls");
            listaPublicaciones.listarPublicaciones();
            system("pause");
            break;
        case 4:
            // Reportes
            break;
        case 5:
            usuario_logeado = Usuario("", "", "", "", "", "");
            break;
        default:
            cout << "Opcion no valida" << endl;
            break;
        }
    } while (opcion != 5);
}
//? Submenús Usuario
void userGestionarPerfil(){
    int opcionPerfil;
    do
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
            limpiarEntrada();
            cout << "Entrada invalida. Por favor, ingrese un número." << endl;
            system("pause");
            continue; // Repite el ciclo si la entrada es inválida
        }

        switch (opcionPerfil)
        {
        case 1:
            // Perfil
            system("cls");
            std::cout << "|===============[ Perfil ]===============|" << std::endl;
            usuario_logeado.mostrarPerfil();
            std::cout << "|========================================|" << std::endl;
            system("pause");
            break;
        case 2:
            // Eliminar Perfil
            
            system("pause");
            break;
        case 3:
            break; // Regresar
        default:
            cout << "Opcion no valida" << endl;
            break;
        }
    } while (opcionPerfil != 3);
    
}

void userGestionarSolicitudes(){
    int opcionSolicitud;
    do
    {
        system("cls");
        cout << "|========== [Solicitudes] ==========|" << endl;
        cout << "| 1. Ver Solicitudes               |" << endl;
        cout << "| 2. Enviar Solicitud              |" << endl;
        cout << "| 3. Regresar                      |" << endl;
        cout << "|========== [Solicitudes] ==========|" << endl;
        cout << "[-] Ingrese una opcion: ";
        cin >> opcionSolicitud;

        // Verificación de errores de entrada
        if (cin.fail())
        {
            limpiarEntrada();
            cout << "Entrada invalida. Por favor, ingrese un número." << endl;
            system("pause");
            continue; // Repite el ciclo si la entrada es inválida
        }

        switch (opcionSolicitud)
        {
        case 1:
            // Ver Solicitudes
            break;
        case 2:
            // Enviar Solicitud
            break;
        case 3:
            break; // Regresar
        default:
            cout << "Opcion no valida" << endl;
            break;
        }
    } while (opcionSolicitud != 3);
    
}

// TODO: Menú Principal
void menuPrincipal(){
    int opcion;
    do
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

        // Verificación de errores de entrada
        if (cin.fail())
        {
            limpiarEntrada();
            cout << "Entrada invalida. Por favor, ingrese un número." << endl;
            system("pause");
            continue; // Repite el ciclo si la entrada es inválida
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
            break;
        default:
            cout << "Opcion no valida" << endl;
            system("pause");
            break;
        }
    } while (opcion != 4);
}
//? Submenús principal
void menuIniciarSesion() {
    string mail, password;
    system("cls");
    cout << "|========== [ Iniciar Sesión ] ==========|" << endl;
    cout << "|Ingrese su usuario:                     |" << endl;
    cout << "|-| ";
    cin >> mail;
    cout << "|Ingrese su contraseña:                  |" << endl;
    cout << "|-| ";
    cin >> password;
    
    usuario_logeado = listaUsuarios.buscarUsuario(mail, password);
    if (usuario_logeado.correo != "" && usuario_logeado.rol == "admin") {
        menuAdministrador();
    } else if (usuario_logeado.correo != "" && usuario_logeado.rol == "user") {
        menuUsuario();
    } else {
        cout << "Usuario o contraseña incorrectos" << endl;
    }
    system("pause");
}

void menuRegistrarse() {
    string names, lastnames, birthday, mail, password;
    system("cls");
    cout << "|========== [ Registrarse ] ==========|" << endl;
    cout << "|Ingrese sus nombres:" << endl;
    cin.ignore();
    cout << "|-| ";
    getline(cin, names);
    cout << "|Ingrese sus apellidos:" << endl;
    cout << "|-| ";
    getline(cin, lastnames);
    cout << "|Ingrese su fecha de nacimiento:" << endl;
    cout << "|-| ";
    getline(cin, birthday);
    cout << "|Ingrese su correo:" << endl;
    cout << "|-| ";
    getline(cin, mail);
    cout << "|Ingrese su contraseña:" << endl;
    cout << "|-| ";
    getline(cin, password);
    cout << "|=====================================|" << endl;
    
    registrarUsuario(names, lastnames, birthday, mail, password);
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