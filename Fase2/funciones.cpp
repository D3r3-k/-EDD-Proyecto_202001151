#include "funciones.h"
#include "arbolabb.h"
#include "globales.h"
#include "Structs.h"
#include "nlohmann/json.hpp"
#include "dialogmodificar.h"
#include "widgetpost.h"

#include <QMessageBox>
#include <QTableWidget>
#include <QPushButton>
#include <qboxlayout.h>
#include <QScrollArea>

#include <fstream>
#include <cstdlib> // Para system()

namespace Func
{
    // globales
    QTableWidget *adminTablaUsuarios = nullptr;
    QTableWidget *userTablaUsuarios = nullptr;
    QTableWidget *userTablaEnviadas = nullptr;
    QTableWidget *userTablaRecibidas = nullptr;
    QScrollArea *userPostFeed = nullptr;

    // TODO: metodos carga masiva
    void CargarUsuarios(string directorio)
    {
        try
        {
            // Abrir el archivo JSON
            ifstream archivo(directorio);
            if (!archivo.is_open())
            {
                throw runtime_error("No se pudo abrir el archivo para leer.");
            }

            // Parsear el archivo JSON
            int contador = 0;
            int repetidos = 0;
            int conerror = 0;
            nlohmann::json usuarios;
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

                    if (lista_usuarios.buscar(correo) == nullptr)
                    {
                        // Crear Usuario
                        int id = lista_usuarios.obtenerId() + 1;
                        Structs::Usuario nuevoUsuario(id, nombres, apellidos, fechaNacimiento, correo, contrasena);
                        lista_usuarios.insertar(nuevoUsuario);
                        contador++;
                    }
                    else
                    {
                        repetidos++;
                    }
                }
                else
                {
                    conerror++;
                }
            }
            archivo.close();

            QString stdcontador = QString::number(contador);
            QString stdrepetidos = QString::number(repetidos);
            QString stderrores = QString::number(conerror);
            QMessageBox::information(nullptr, "Información", "Usuarios agregados: " + stdcontador + "\nUsuarios repetidos: " + stdrepetidos + "\nErrores: " + stderrores);
        }
        catch (const ifstream::failure &e)
        {
            QMessageBox::information(nullptr, "Error", "Error de archivo: " + QString::fromStdString(e.what()));
        }
        catch (const nlohmann::json::exception &e)
        {
            QMessageBox::information(nullptr, "Error", "Error de JSON: " + QString::fromStdString(e.what()));
        }
        catch (const std::exception &e)
        {
            QMessageBox::information(nullptr, "Error", "Error inesperado: " + QString::fromStdString(e.what()));
        }
        catch (...)
        {
            QMessageBox::information(nullptr, "Error", "Error desconocido.");
        }
    }

    void CargarSolicitudes(string directorio)
    {
        try
        {
            // Abrir el archivo JSON
            ifstream archivo(directorio);
            if (!archivo.is_open())
            {
                throw runtime_error("No se pudo abrir el archivo para leer.");
            }

            // Parsear el archivo JSON
            int contador = 0;
            int conerror = 0;
            int noexisten = 0;
            nlohmann::json post;
            archivo >> post;

            // Recorrer el JSON
            for (const auto &p : post)
            {
                // Verificar si las claves existen
                if (p.contains("emisor") && p.contains("receptor") && p.contains("estado"))
                {
                    string emisor = p["emisor"].get<string>();
                    string receptor = p["receptor"].get<string>();
                    string estado = p["estado"].get<string>();
                    transform(estado.begin(), estado.end(), estado.begin(), ::tolower);

                    Structs::Usuario *usuario_emisor = lista_usuarios.buscar(emisor);
                    Structs::Usuario *usuario_receptor = lista_usuarios.buscar(receptor);

                    // Verificar si los usuarios existen y especificar cuál es el emisor y cuál es el receptor
                    if (usuario_emisor == nullptr)
                    {
                        noexisten++;
                        continue;
                    }
                    if (usuario_receptor == nullptr)
                    {
                        noexisten++;
                        continue;
                    }

                    // Si el estado convertido en minúsculas es pendiente
                    if (estado == "pendiente")
                    {
                        // Al usuario emisor, en la lista de solicitudes se agregará el correo del usuario receptor
                        lista_usuarios.enviarSolicitud(emisor, receptor);
                    }
                    else if (estado == "aceptada" || estado == "aceptado")
                    {
                        // Se crea la relación de amistad entre los dos usuarios en la matriz dispersa
                        relaciones_amistad.agregarRelacion(usuario_emisor, usuario_receptor);
                    }

                    contador++;
                }
                else
                {
                    conerror++;
                }
            }
            archivo.close();

            QString stdagregados = QString::number(contador);
            QString stderrores = QString::number(conerror);
            QString stdnoexisten = QString::number(noexisten);
            QMessageBox::information(nullptr, "Información", "Solicitudes agregadas: " + stdagregados + "\nErrores: " + stderrores + "\nNo Existen emisores o receptores: " + stdnoexisten);
        }
        catch (const ifstream::failure &e)
        {
            QMessageBox::information(nullptr, "Error", "Error de archivo: " + QString::fromStdString(e.what()));
        }
        catch (const nlohmann::json::exception &e)
        {
            QMessageBox::information(nullptr, "Error", "Error de JSON: " + QString::fromStdString(e.what()));
        }
        catch (const std::exception &e)
        {
            QMessageBox::information(nullptr, "Error", "Error inesperado: " + QString::fromStdString(e.what()));
        }
        catch (...)
        {
            QMessageBox::information(nullptr, "Error", "Error desconocido.");
        }
    }

    void CargarPublicaciones(string directorio) {
        try {
            // Abrir el archivo JSON
            ifstream archivo(directorio);
            if (!archivo.is_open()) {
                throw runtime_error("No se pudo abrir el archivo para leer.");
            }

            // Parsear el archivo JSON
            int contador = 0;
            int conerror = 0;
            int noexisten = 0;
            nlohmann::json post;
            archivo >> post;

            // Recorrer el JSON
            for (const auto& p : post) {
                // Verificar si las claves existen
                if (p.contains("correo") && p.contains("contenido") &&
                    p.contains("fecha") && p.contains("hora")) {

                    string correo = p["correo"].get<string>();
                    string contenido = p["contenido"].get<string>();
                    string _fecha = p["fecha"].get<string>();
                    string hora = p["hora"].get<string>();

                    Structs::Usuario* autor = lista_usuarios.buscar(correo);

                    if (autor == nullptr) {
                        noexisten++;
                        continue;
                    }

                    // Convertir la fecha
                    string fechaConvertida;
                    try {
                        fechaConvertida = convertirFecha(_fecha);
                    } catch (const std::runtime_error& e) {
                        conerror++;
                        continue;
                    }

                    // Crear Publicacion
                    int newid = Func::obtenerIdPublicaciones() + 1;
                    Structs::Publicacion nuevaPublicacion(newid, autor->correo, contenido, fechaConvertida, hora);
                    lista_publicaciones.insertar(nuevaPublicacion);
                    contador++;
                } else {
                    conerror++;
                }
            }
            archivo.close();

            QString stdagregados = QString::number(contador);
            QString stderrores = QString::number(conerror);
            QString stdnoexisten = QString::number(noexisten);
            QMessageBox::information(nullptr, "Información", "Publicaciones agregadas: " + stdagregados + "\nErrores: " + stderrores + "\nNo Existen autores: " + stdnoexisten);
        }
        catch (const ifstream::failure& e) {
            QMessageBox::information(nullptr, "Error", "Error de archivo: " + QString::fromStdString(e.what()));
        }
        catch (const nlohmann::json::exception& e) {
            QMessageBox::information(nullptr, "Error", "Error de JSON: " + QString::fromStdString(e.what()));
        }
        catch (const std::exception& e) {
            QMessageBox::information(nullptr, "Error", "Error inesperado: " + QString::fromStdString(e.what()));
        }
        catch (...) {
            QMessageBox::information(nullptr, "Error", "Error desconocido.");
        }
    }


    // TODO: Metodos Login
    void IniciarSesion(std::string email, std::string password)
    {
        Structs::Usuario *user = lista_usuarios.buscar(email);
        if (user != nullptr && user->contrasena == password)
        {
            usuario_logeado = user;
        }
        else
        {
            QMessageBox::information(nullptr, "Iniciar Sesión", "Credenciales no válidas.");
        }
    }

    // TODO: Metodos Admin
    void ActualizarTablaUsuariosAdmin(QTableWidget *table, ListaEnlazada::ListaEnlazada<Structs::Usuario> &lista)
    {
        // Limpiar la tabla antes de agregar nuevos elementos
        table->clearContents();
        table->setRowCount(lista.size()); // Ajustar el número de filas según el tamaño de la lista

        int row = 0;
        // Recorremos la lista para agregar los elementos a la tabla
        for (int i = 0; i < lista.size(); ++i)
        {
            Structs::Usuario *temp = lista.obtener(i);
            if (temp)
            {
                QTableWidgetItem *nombreItem = new QTableWidgetItem(QString::fromStdString(temp->nombres));
                QTableWidgetItem *apellidoItem = new QTableWidgetItem(QString::fromStdString(temp->apellidos));
                QTableWidgetItem *emailItem = new QTableWidgetItem(QString::fromStdString(temp->correo));
                QTableWidgetItem *fechaItem = new QTableWidgetItem(QString::fromStdString(temp->fechaNacimiento));

                // Agregamos los datos a la tabla
                table->setItem(row, 0, nombreItem);
                table->setItem(row, 1, apellidoItem);
                table->setItem(row, 2, emailItem);
                table->setItem(row, 3, fechaItem);

                // Capturar el id por valor en las lambdas
                string correo = temp->correo;

                // Crear botones de modificar y eliminar
                QPushButton *modificarButton = new QPushButton("Modificar");
                QPushButton *eliminarButton = new QPushButton("Eliminar");

                // Conectar los botones a sus slots
                QObject::connect(modificarButton, &QPushButton::clicked, [correo, row]()
                                 {
                                     DialogModificar dialog(correo);
                                     dialog.exec();
                                     Func::ActualizarTablaAdmin(0); });

                QObject::connect(eliminarButton, &QPushButton::clicked, [correo, row, table]()
                                 {
                    if (usuario_logeado->correo == correo) {
                        QMessageBox::warning(nullptr,"Eliminar usuario","No se puede eliminar el usuario logeado.");
                        return;
                    }else{
                        Func::EliminarCuenta(correo);
                        Func::ActualizarTablaAdmin(0);
                    } });

                // Crear contenedores de botones separados para cada columna
                QWidget *modifyButtonContainer = new QWidget();
                QHBoxLayout *modifyButtonLayout = new QHBoxLayout();
                modifyButtonLayout->addWidget(modificarButton);
                modifyButtonLayout->setAlignment(Qt::AlignCenter);
                modifyButtonLayout->setContentsMargins(0, 0, 0, 0); // Eliminar márgenes
                modifyButtonContainer->setLayout(modifyButtonLayout);
                modifyButtonContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // Expandir para llenar la celda

                QWidget *deleteButtonContainer = new QWidget();
                QHBoxLayout *deleteButtonLayout = new QHBoxLayout();
                deleteButtonLayout->addWidget(eliminarButton);
                deleteButtonLayout->setAlignment(Qt::AlignCenter);
                deleteButtonLayout->setContentsMargins(0, 0, 0, 0); // Eliminar márgenes
                deleteButtonContainer->setLayout(deleteButtonLayout);
                deleteButtonContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // Expandir para llenar la celda

                // Agregar los contenedores de botones a la tabla
                table->setCellWidget(row, 4, modifyButtonContainer); // Botón de modificar en columna 5
                table->setCellWidget(row, 5, deleteButtonContainer); // Botón de eliminar en columna 6

                row++; // Avanzamos a la siguiente fila
            }
        }
    }

    // TODO: Metodos User
    void ActualizarTablaUsuarios(QTableWidget *table)
    {
        // Limpiar la tabla antes de agregar nuevos elementos
        table->clearContents();
        ListaEnlazada::ListaEnlazada<Structs::Usuario> lista = obtenerListaUsuariosLogeado();
        table->setRowCount(lista.size()); // Ajustar el número de filas según el tamaño de la lista

        int row = 0;
        // Recorremos la lista para agregar los elementos a la tabla
        for (int i = 0; i < lista.size(); ++i)
        {
            Structs::Usuario *temp = lista.obtener(i);
            if (temp)
            {
                QTableWidgetItem *nombreItem = new QTableWidgetItem(QString::fromStdString(temp->nombres));
                QTableWidgetItem *apellidoItem = new QTableWidgetItem(QString::fromStdString(temp->apellidos));
                QTableWidgetItem *emailItem = new QTableWidgetItem(QString::fromStdString(temp->correo));
                QTableWidgetItem *fechaItem = new QTableWidgetItem(QString::fromStdString(temp->fechaNacimiento));

                // Agregamos los datos a la tabla
                table->setItem(row, 0, nombreItem);
                table->setItem(row, 1, apellidoItem);
                table->setItem(row, 2, emailItem);
                table->setItem(row, 3, fechaItem);

                // Capturar el id por valor en las lambdas
                string correo = temp->correo;

                // Crear botones de modificar y eliminar
                QPushButton *enviarButton = new QPushButton("Enviar Solicitud");

                // Conectar los botones a sus slots
                QObject::connect(enviarButton, &QPushButton::clicked, [correo, row, table]()
                                 {
                    lista_usuarios.enviarSolicitud(usuario_logeado->correo,correo);
                    Func::ActualizarTablas(); });

                // Crear contenedores de botones separados para cada columna
                QWidget *sendButtonContainer = new QWidget();
                QHBoxLayout *modifyButtonLayout = new QHBoxLayout();
                modifyButtonLayout->addWidget(enviarButton);
                modifyButtonLayout->setAlignment(Qt::AlignCenter);
                modifyButtonLayout->setContentsMargins(0, 0, 0, 0); // Eliminar márgenes
                sendButtonContainer->setLayout(modifyButtonLayout);
                sendButtonContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // Expandir para llenar la celda

                // Agregar los contenedores de botones a la tabla
                table->setCellWidget(row, 4, sendButtonContainer); // Botón de modificar en columna 5

                row++; // Avanzamos a la siguiente fila
            }
        }
    }
    void ActualizarTablaRecibidos(QTableWidget *table)
    {
        // Limpiar la tabla antes de agregar nuevos elementos
        table->clearContents();
        table->setRowCount(0); // Restablecer el número de filas a 0

        // Suponiendo que ListaEnlazada es una plantilla de lista enlazada y tiene los métodos size() y obtener()
        ListaEnlazada::ListaEnlazada<Structs::Usuario> lista = usuario_logeado->solicitudesRecibidas;
        table->setRowCount(lista.size()); // Ajustar el número de filas según el tamaño de la lista

        int row = 0;
        // Recorremos la lista para agregar los elementos a la tabla
        for (int i = 0; i < lista.size(); ++i)
        {
            Structs::Usuario *temp = lista.obtener(i); // Obtenemos un puntero al usuario
            if (temp)
            {
                // Crear los items de la tabla con los datos del usuario
                QTableWidgetItem *correoItem = new QTableWidgetItem(QString::fromStdString(temp->correo));

                // Agregamos los datos a la tabla
                table->setItem(row, 0, correoItem);

                // Capturar el correo y fila por valor en las lambdas
                string correo = temp->correo;

                // Crear botones de aceptar y rechazar solicitud
                QPushButton *aceptarButton = new QPushButton("Aceptar");
                QPushButton *declineButton = new QPushButton("Rechazar");

                // Conectar el botón aceptar a su slot con el correo y la fila capturados
                QObject::connect(aceptarButton, &QPushButton::clicked, [correo, row]()
                                 {
                    lista_usuarios.aceptarSolicitud(usuario_logeado->correo, correo);
                    Func::ActualizarTablas(); });

                // Conectar el botón rechazar a su slot con el correo y la fila capturados
                QObject::connect(declineButton, &QPushButton::clicked, [correo, row]()
                                 {
                    lista_usuarios.rechazarSolicitud(usuario_logeado->correo,correo);
                    Func::ActualizarTablas(); });

                // Crear contenedores de botones para colocarlos en la tabla
                QWidget *acceptButtonContainer = new QWidget();
                QHBoxLayout *acceptButtonLayout = new QHBoxLayout();
                acceptButtonLayout->addWidget(aceptarButton);
                acceptButtonLayout->setAlignment(Qt::AlignCenter);
                acceptButtonLayout->setContentsMargins(0, 0, 0, 0);
                acceptButtonContainer->setLayout(acceptButtonLayout);
                acceptButtonContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

                QWidget *declineButtonContainer = new QWidget();
                QHBoxLayout *declineButtonLayout = new QHBoxLayout();
                declineButtonLayout->addWidget(declineButton);
                declineButtonLayout->setAlignment(Qt::AlignCenter);
                declineButtonLayout->setContentsMargins(0, 0, 0, 0);
                declineButtonContainer->setLayout(declineButtonLayout);
                declineButtonContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

                // Agregar los contenedores de botones a la tabla
                table->setCellWidget(row, 1, acceptButtonContainer);
                table->setCellWidget(row, 2, declineButtonContainer);

                row++; // Avanzamos a la siguiente fila
            }
        }
    }
    void ActualizarTablaEnviados(QTableWidget *table)
    {
        // Limpiar la tabla antes de agregar nuevos elementos
        table->clearContents();
        table->setRowCount(0); // Restablecer el número de filas a 0

        // Suponiendo que ListaEnlazada es una plantilla de lista enlazada y tiene los métodos size() y obtener()
        ListaEnlazada::ListaEnlazada<Structs::Usuario> lista = usuario_logeado->solicitudesEnviadas;
        table->setRowCount(lista.size()); // Ajustar el número de filas según el tamaño de la lista

        int row = 0;
        // Recorremos la lista para agregar los elementos a la tabla
        for (int i = 0; i < lista.size(); ++i)
        {
            Structs::Usuario *temp = lista.obtener(i); // Obtenemos un puntero al usuario
            if (temp)
            {
                // Crear los items de la tabla con los datos del usuario
                QTableWidgetItem *correoItem = new QTableWidgetItem(QString::fromStdString(temp->correo));

                // Agregamos los datos a la tabla
                table->setItem(row, 0, correoItem);

                // Capturar el correo y fila por valor en las lambdas
                string correo = temp->correo;

                // Crear botones de aceptar y rechazar solicitud
                QPushButton *cancelButton = new QPushButton("Cancelar");

                // Conectar el botón aceptar a su slot con el correo y la fila capturados
                QObject::connect(cancelButton, &QPushButton::clicked, [correo, row, table]()
                                 {
                    lista_usuarios.cancelarSolicitud(usuario_logeado->correo,correo);
                    Func::ActualizarTablas(); });

                // Crear contenedores de botones para colocarlos en la tabla
                QWidget *cancelButtonContainer = new QWidget();
                QHBoxLayout *cancelButtonLayout = new QHBoxLayout();
                cancelButtonLayout->addWidget(cancelButton);
                cancelButtonLayout->setAlignment(Qt::AlignCenter);
                cancelButtonLayout->setContentsMargins(0, 0, 0, 0);
                cancelButtonContainer->setLayout(cancelButtonLayout);
                cancelButtonContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

                // Agregar los contenedores de botones a la tabla
                table->setCellWidget(row, 1, cancelButtonContainer);

                row++; // Avanzamos a la siguiente fila
            }
        }
    }

    void EliminarMiCuenta()
    {
        // Eliminar todas las relaciones de amistad del usuario logueado
        relaciones_amistad.eliminarRelacionesUsuario(usuario_logeado->correo);
        // Eliminar las solicitudes enviadas y recibidas del usuario logueado
        lista_usuarios.eliminarSolicitudes(usuario_logeado->correo);
        // Eliminar al usuario logueado de la lista de usuarios
        lista_usuarios.eliminar(usuario_logeado->correo);
        // Desloguear al usuario
        usuario_logeado = nullptr;
    }
    void EliminarCuenta(string correo)
    {
        // Eliminar todas las relaciones de amistad del usuario
        relaciones_amistad.eliminarRelacionesUsuario(correo);
        // Eliminar las solicitudes enviadas y recibidas del usuario
        lista_usuarios.eliminarSolicitudes(correo);
        // Eliminar al usuario de la lista de usuarios
        lista_usuarios.eliminar(correo);
    }
    // TODO: Metodos Extras

    ListaEnlazada::ListaEnlazada<Structs::Usuario> obtenerListaUsuariosLogeado()
    {
        ListaEnlazada::ListaEnlazada<Structs::Usuario> lista = lista_usuarios.InOrder();
        ListaEnlazada::ListaEnlazada<Structs::Usuario> amigos = relaciones_amistad.obtenerAmigos(usuario_logeado->correo);
        ListaEnlazada::ListaEnlazada<Structs::Usuario> enviadas = usuario_logeado->solicitudesEnviadas;
        ListaEnlazada::ListaEnlazada<Structs::Usuario> recibidas = usuario_logeado->solicitudesRecibidas;

        for (int i = 0; i < lista.size(); ++i)
        {
            Structs::Usuario *u = lista.obtener(i);
            if (u)
            {
                // Eliminar si es el usuario logueado
                if (u->correo == usuario_logeado->correo)
                {
                    lista.eliminar(i);
                    --i; // Ajustar el índice después de la eliminación
                    continue;
                }

                // Eliminar si ya es amigo del usuario logueado
                bool esAmigo = false;
                for (int j = 0; j < amigos.size(); ++j)
                {
                    Structs::Usuario *u2 = amigos.obtener(j);
                    if (u2 && u->correo == u2->correo)
                    {
                        esAmigo = true;
                        break;
                    }
                }

                if (esAmigo)
                {
                    lista.eliminar(i);
                    --i; // Ajustar el índice después de la eliminación
                    continue;
                }

                // Eliminar si ya se le ha enviado una solicitud
                bool solicitudEnviada = false;
                for (int k = 0; k < enviadas.size(); ++k)
                {
                    Structs::Usuario *u3 = enviadas.obtener(k);
                    if (u3 && u->correo == u3->correo)
                    {
                        solicitudEnviada = true;
                        break;
                    }
                }

                if (solicitudEnviada)
                {
                    lista.eliminar(i);
                    --i; // Ajustar el índice después de la eliminación
                    continue;
                }

                // Eliminar si ya ha enviado una solicitud al usuario logueado
                bool solicitudRecibida = false;
                for (int l = 0; l < recibidas.size(); ++l)
                {
                    Structs::Usuario *u4 = recibidas.obtener(l);
                    if (u4 && u->correo == u4->correo)
                    {
                        solicitudRecibida = true;
                        break;
                    }
                }

                if (solicitudRecibida)
                {
                    lista.eliminar(i);
                    --i; // Ajustar el índice después de la eliminación
                }
            }
        }

        return lista;
    }

    void ActualizarTablas()
    {
        Func::ActualizarTablaUsuarios(userTablaUsuarios);
        Func::ActualizarTablaEnviados(userTablaEnviadas);
        Func::ActualizarTablaRecibidos(userTablaRecibidas);
    }

    void ActualizarTablaAdmin(int opcion)
    {
        if (opcion == 0)
        {
            ListaEnlazada::ListaEnlazada<Structs::Usuario> temp = lista_usuarios.InOrder();
            Func::ActualizarTablaUsuariosAdmin(adminTablaUsuarios, temp);
        }
        else if (opcion == 1)
        {
            ListaEnlazada::ListaEnlazada<Structs::Usuario> temp = lista_usuarios.PreOrder();
            Func::ActualizarTablaUsuariosAdmin(adminTablaUsuarios, temp);
        }
        else if (opcion == 2)
        {
            ListaEnlazada::ListaEnlazada<Structs::Usuario> temp = lista_usuarios.PostOrder();
            Func::ActualizarTablaUsuariosAdmin(adminTablaUsuarios, temp);
        }
    }

    int obtenerIdPublicaciones()
    {
        if (lista_publicaciones.size() == 0)
        {
            return -1;
        }
        int id_mas_grande = -1;
        for (int i = 0; i < lista_publicaciones.size(); i++)
        {
            Structs::Publicacion *p = lista_publicaciones.obtener(i);
            if (p && p->id > id_mas_grande)
            {
                id_mas_grande = p->id;
            }
        }
        return id_mas_grande;
    }

    void eliminarPublicacion(int id){
        for (int i = 0; i < lista_publicaciones.size(); ++i) {
            Structs::Publicacion *p = lista_publicaciones.obtener(i);
            if (p) {
                if (p->id == id) {
                    lista_publicaciones.eliminarPosicion(i);
                }
            }
        }
    }

    Structs::Publicacion *buscarPost(int id){
        for (int i = 0; i < lista_publicaciones.size(); ++i) {
            Structs::Publicacion *p = lista_publicaciones.obtener(i);
            if (p) {
                if (p->id == id) {
                    return p;
                }
            }
        }
        return nullptr;
    }

    void ActualizarFeed() {
        QWidget *contentWidget = userPostFeed->widget();
        QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(contentWidget->layout());
        if (!layout) {
            layout = new QVBoxLayout(contentWidget);
            contentWidget->setLayout(layout);
        }
        while (QLayoutItem *item = layout->takeAt(0)) {
            if (item->widget()) {
                item->widget()->deleteLater();
            }
            delete item;
        }
        ArbolABB arbolabb;
        for (int i = 0; i < lista_publicaciones.size(); ++i) {
            Structs::Publicacion *p = lista_publicaciones.obtener(i);
            if (p) {
                p->mostrarPublicacion();
                std::tm fecha = {};
                std::istringstream ss(p->fecha);
                ss >> std::get_time(&fecha, "%d-%m-%Y %H:%M:%S");
                if (ss.fail()) {
                    throw std::runtime_error("Error al parsear la fecha.");
                }
                arbolabb.insertar(fecha, *p);

                WidgetPost *newPost = new WidgetPost(p->id);
                layout->addWidget(newPost);
            }
        }
        std::string path_grafica = arbolabb.graficar();
        qInfo()<<"Graficado en: "+path_grafica;
    }

    string convertirFecha(const std::string& fechaOriginal) {
        std::tm fecha = {};
        std::istringstream ss(fechaOriginal);
        ss >> std::get_time(&fecha, "%Y-%m-%d"); // Formato de entrada: aaaa-mm-dd
        if (ss.fail()) {
            throw std::runtime_error("Error al parsear la fecha.");
        }

        std::ostringstream oss;
        oss << std::put_time(&fecha, "%d-%m-%Y"); // Formato de salida: dd-mm-aaaa
        return oss.str();
    }
}
