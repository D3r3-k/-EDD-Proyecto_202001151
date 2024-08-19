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
        // Verificación de errores de entrada, si no es un número, se limpia el buffer y se ignora la entrada, se muestra un mensaje de error y se reinicia la variable opcion
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
    int opcionReportes = 0;
    while (opcionReportes != 5)
    {
        system("cls");
        cout << "|========== [Reportes] ==========|" << endl;
        cout << "| 1. Usuarios                    |" << endl;
        cout << "| 2. Relaciones de Amistad       |" << endl;
        cout << "| 3. Publicaciones               |" << endl;
        cout << "| 4. Top                         |" << endl;
        cout << "| 5. Regresar                    |" << endl;
        cout << "|========== [Reportes] ==========|" << endl;
        cout << "[-] Ingrese una opcion: ";
        cin >> opcionReportes;

        // Verificación de errores de entrada
        if (cin.fail())
        {
            cin.clear();
            cin.ignore();
            cout << "Entrada invalida. Por favor, ingrese un número." << endl;
            system("pause");
            continue;
        }

        switch (opcionReportes)
        {
        case 1:
            // Usuarios
            system("cls");
            cout << "|================== [ Reporte de Usuarios ] ==================|" << endl;
            cout << "| Generando reporte de usuarios...                            |" << endl;
            lista_usuarios.graficarUsuarios("lista_usuarios_admin", "pdf");
            cout << "| Reporte generado correctamente                              |" << endl;
            cout << "|=============================================================|" << endl;
            system("pause");
            break;
        case 2:
            // Relaciones
            system("cls");
            cout << "|================ [ Reporte de Relaciones ] ==================|" << endl;
            cout << "| Generando reporte de relaciones...                          |" << endl;
            matriz_relacion.graficarMatrizRelaciones("matriz_relacion_admin", "pdf");
            cout << "| Reporte generado correctamente                              |" << endl;
            cout << "|=============================================================|" << endl;
            system("pause");
            break;
        case 3:
            // Publicaciones
            system("cls");
            cout << "|================ [ Reporte de Publicaciones ] ===============|" << endl;
            cout << "| Generando reporte de publicaciones...                       |" << endl;
            lista_publicaciones.graficarPublicaciones("lista_publicaciones_admin", "pdf");
            cout << "| Reporte generado correctamente                              |" << endl;
            cout << "|=============================================================|" << endl;
            system("pause");
            break;
        case 4:
            // Top
            adminGestionarTop();
            break;
        case 5:
            // Regresar
            break;
        default:
            cout << "Opcion no valida" << endl;
            system("pause");
            break;
        }
    }
}

void adminGestionarTop()
{
    int opcionTop = 0;
    while (opcionTop != 3)
    {
        system("cls");
        cout << "|==================== [Top 5] ====================|" << endl;
        cout << "| 1. Usuarios con mas publicaciones               |" << endl;
        cout << "| 2. Usuarios con menos amigos                    |" << endl;
        cout << "| 3. Regresar                                     |" << endl;
        cout << "|=================================================|" << endl;
        cout << "[-] Ingrese una opcion: ";
        cin >> opcionTop;

        // Verificación de errores de entrada
        if (cin.fail())
        {
            cin.clear();
            cin.ignore();
            cout << "Entrada invalida. Por favor, ingrese un número." << endl;
            system("pause");
            continue;
        }

        switch (opcionTop)
        {
        case 1:
            // Top 5 Publicaciones
            system("cls");
            cout << "|================ [ Top 5 Publicaciones ] ===============|" << endl;
            lista_publicaciones.top5UsuariosConMasPublicaciones();
            system("pause");
            break;
        case 2:
            // Top 5 Amigos
            system("cls");
            cout << "|================ [ Top 5 Usuarios ] ===============|" << endl;
            matriz_relacion.top5UsuariosConMenosAmigos();
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
            if (Func::verificarSesion())
            {
                return;
            }
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
            if (Func::verificarSesion())
            {
                return;
            }
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

void userGestionarSolicitudes()
{
    int opcionSolicitudes = 0;
    while (opcionSolicitudes != 4)
    {
        system("cls");
        cout << "|========== [Solicitudes] ==========|" << endl;
        cout << "| 1. Ver Solicitudes                |" << endl;
        cout << "| 2. Ver Solicitudes Enviadas       |" << endl;
        cout << "| 3. Enviar Solicitudes             |" << endl;
        cout << "| 4. Regresar                       |" << endl;
        cout << "|========== [Solicitudes] ==========|" << endl;
        cout << "[-] Ingrese una opcion: ";
        cin >> opcionSolicitudes;

        // Verificación de errores de entrada
        if (cin.fail())
        {
            cin.clear();
            cin.ignore();
            cout << "Entrada invalida. Por favor, ingrese un número." << endl;
            system("pause");
            continue;
        }

        switch (opcionSolicitudes)
        {
        case 1:
            // Ver Solicitudes
            system("cls");
            menuSolicitudesRecibidas();
            break;
        case 2:
            // Ver Solicitudes Enviadas
            system("cls");
            cout << "|========================= [ Solicitudes Enviadas ] ==========================|" << endl;
            usuario_logeado->mostrarSolicitudesEnviadas();
            cout << "|=============================================================================|" << endl;
            system("pause");
            break;
        case 3:
            // Enviar Solicitudes
            menuEnviarSolicitud();
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

void userGestionarReportes()
{
    // Solicitudes Enviadas y Recibidas: Gráfico de la lista de solicitudes enviadas y de
    // la pila de solicitudes recibidas
    // Relaciones de Amistad: Gráfico de la matriz dispersa
    // Publicaciones: Gráfico de la lista circular de publicaciones del usuario y de amigos
    // Mis Amigos: Listado de amigos
    int opcionReportes = 0;
    while (opcionReportes != 5)
    {
        system("cls");
        cout << "|==================== [Reportes] ====================|" << endl;
        cout << "| 1. Solicitudes enviadas y recibidas                |" << endl;
        cout << "| 2. Relaciones de Amistad                           |" << endl;
        cout << "| 3. Publicaciones                                   |" << endl;
        cout << "| 4. Mis Amigos                                      |" << endl;
        cout << "| 5. Regresar                                        |" << endl;
        cout << "|====================================================|" << endl;
        cout << "[-] Ingrese una opcion: ";
        cin >> opcionReportes;

        // Verificación de errores de entrada
        if (cin.fail())
        {
            cin.clear();
            cin.ignore();
            cout << "Entrada invalida. Por favor, ingrese un número." << endl;
            system("pause");
            continue;
        }

        switch (opcionReportes)
        {
        case 1:
            // Solicitudes
            system("cls");
            cout << "|================ [ Reporte de Solicitudes ] ===============|" << endl;
            usuario_logeado->graficarSolicitudes("solicitudes_user", "pdf");
            system("pause");
            break;
        case 2:
            // Relaciones
            system("cls");
            cout << "|================ [ Reporte de Relaciones ] ===============|" << endl;
            system("pause");
            break;
        case 3:
            // Publicaciones
            system("cls");
            cout << "|================ [ Reporte de Publicaciones ] ===============|" << endl;
            Func::graficarMiFeed();
            system("pause");
            break;
        case 4:
            // Mis Amigos
            system("cls");
            cout << "|==================== [ Mis Amigos ] ==================|" << endl;
            Func::mostrarAmigos();
            system("pause");
            break;
        case 5:
            // Regresar
            break;
        default:
            cout << "Opcion no valida" << endl;
            system("pause");
            break;
        }
    }
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
            Func::eliminarMiPerfil();
            usuario_logeado = nullptr;
            if (Func::verificarSesion())
            {
                return;
            }
            break;
        case 2:
            // No
            break;
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

void menuEnviarSolicitud()
{
    string correo_destinatario;
    while (true)
    {
        system("cls");
        cout << "|================ [ Enviar Solicitud ] ================|" << endl;
        lista_usuarios.listarUsuarios();
        cout << "|                      [c] Regresar                    |" << endl;
        cout << "|======================================================|" << endl;
        cout << "|+| Correo del usuario: ";
        cin >> correo_destinatario;

        // Verificar si el usuario quiere regresar
        if (correo_destinatario == "c")
        {
            return;
        }

        // Buscar el usuario por correo
        ListaUsuarios::Usuario *usuario = lista_usuarios.buscarUsuario(correo_destinatario);

        if (usuario == nullptr)
        {
            cout << "Usuario no encontrado" << endl;
            system("pause");
            continue;
        }

        // Verificar si el usuario es el mismo que el que está logueado
        if (usuario->correo == usuario_logeado->correo)
        {
            cout << "No puedes enviarte una solicitud a ti mismo" << endl;
            system("pause");
            continue;
        }

        // Enviar la solicitud
        Func::enviarSolicitud(usuario);
    }
}

void menuSolicitudesRecibidas()
{
    int opcion = 0;
    while (opcion != 3)
    {
        system("cls");
        cout << "|========================= [ Solicitudes Recibidas ] =========================|" << endl;
        usuario_logeado->mostrarSolicitudRecibida();
        cout << "|                    [1] Aceptar   [3] Regresar    [2] Rechazar               |" << endl;
        cout << "|=============================================================================|" << endl;
        cout << "|+|: ";
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
            // Aceptar
            Func::aceptarSolicitud(usuario_logeado);
            break;
        case 2:
            // Rechazar
            Func::rechazarSolicitud(usuario_logeado);
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
