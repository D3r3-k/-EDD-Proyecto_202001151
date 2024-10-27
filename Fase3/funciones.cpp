#include "funciones.h"
#include "Tree/merkle.h"
#include "Trees/huffman.h"
#include "arbolabb.h"
#include "globales.h"
#include "Structs.h"
#include "nlohmann/json.hpp"
#include "dialogmodificar.h"
#include "widgetfriend.h"
#include "widgetpost.h"

#include <QMessageBox>
#include <QTableWidget>
#include <QPushButton>
#include <qboxlayout.h>
#include <QScrollArea>

#include <fstream>
#include <cstdlib>

#include <cstring>
#include <fstream>
#include <iostream>
#include <qdir.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>

#include "hash_functions.h"

const unsigned int hash_functions::hash_keys[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1,
    0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786,
    0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
    0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b,
    0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a,
    0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

void hash_functions::compress(const unsigned char *message,
                              unsigned int block_nb)
{
    register_32 w[64];
    register_32 buffer[8];
    register_32 t1, t2;
    const unsigned char *sub_block;
    int m;
    int n;
    for (m = 0; m < (int)block_nb; m++)
    {
        sub_block = message + (m << 6);
        for (n = 0; n < 16; n++)
        {
            SHAF_PACK32(&sub_block[n << 2], &w[n]);
        }
        for (n = 16; n < 64; n++)
        {
            w[n] = SHAF_4(w[n - 2]) + w[n - 7] + SHAF_3(w[n - 15]) + w[n - 16];
        }
        for (n = 0; n < 8; n++)
        {
            buffer[n] = s_r[n];
        }
        for (n = 0; n < 64; n++)
        {
            t1 = buffer[7] + SHAF_2(buffer[4]) +
                 CHOICE_OF(buffer[4], buffer[5], buffer[6]) + hash_keys[n] + w[n];
            t2 = SHAF_1(buffer[0]) + MAJORITY_OF(buffer[0], buffer[1], buffer[2]);
            buffer[7] = buffer[6];
            buffer[6] = buffer[5];
            buffer[5] = buffer[4];
            buffer[4] = buffer[3] + t1;
            buffer[3] = buffer[2];
            buffer[2] = buffer[1];
            buffer[1] = buffer[0];
            buffer[0] = t1 + t2;
        }
        for (n = 0; n < 8; n++)
        {
            s_r[n] += buffer[n];
        }
    }
}

void hash_functions::stateregister()
{
    s_r[0] = 0x6a09e667;
    s_r[1] = 0xbb67ae85;
    s_r[2] = 0x3c6ef372;
    s_r[3] = 0xa54ff53a;
    s_r[4] = 0x510e527f;
    s_r[5] = 0x9b05688c;
    s_r[6] = 0x1f83d9ab;
    s_r[7] = 0x5be0cd19;
    s_r_len = 0;
    s_r_totlen = 0;
}

void hash_functions::adjust_digest(const unsigned char *text,
                                   unsigned int text_len)
{
    unsigned int block_nb;
    unsigned int new_len, rem_len, tmp_len;
    const unsigned char *shifted_message;
    tmp_len = BLOCK_SIZE_of_256 - s_r_len;
    rem_len = text_len < tmp_len ? text_len : tmp_len;
    memcpy(&s_r_block[s_r_len], text, rem_len);
    if (s_r_len + text_len < BLOCK_SIZE_of_256)
    {
        s_r_len += text_len;
        return;
    }
    new_len = text_len - rem_len;
    block_nb = new_len / BLOCK_SIZE_of_256;
    shifted_message = text + rem_len;
    compress(s_r_block, 1);
    compress(shifted_message, block_nb);
    rem_len = new_len % BLOCK_SIZE_of_256;
    memcpy(s_r_block, &shifted_message[block_nb << 6], rem_len);
    s_r_len = rem_len;
    s_r_totlen += (block_nb + 1) << 6;
}

void hash_functions::digest_final(unsigned char *digest)
{
    unsigned int block_nb;
    unsigned int pm_len;
    unsigned int len_b;
    int i;
    block_nb = (1 + ((BLOCK_SIZE_of_256 - 9) < (s_r_len % BLOCK_SIZE_of_256)));
    len_b = (s_r_totlen + s_r_len) << 3;
    pm_len = block_nb << 6;
    memset(s_r_block + s_r_len, 0, pm_len - s_r_len);
    s_r_block[s_r_len] = 0x80;
    SHAF_UNPACK32(len_b, s_r_block + pm_len - 4);
    compress(s_r_block, block_nb);
    for (i = 0; i < 8; i++)
    {
        SHAF_UNPACK32(s_r[i], &digest[i << 2]);
    }
}

std::string sha256(std::string input)
{
    unsigned char digest[hash_functions::PADD_SIZE];
    memset(digest, 0, hash_functions::PADD_SIZE);

    hash_functions obj = hash_functions();
    obj.stateregister();
    obj.adjust_digest((unsigned char *)input.c_str(), input.length());
    obj.digest_final(digest);

    char buf[2 * hash_functions::PADD_SIZE + 1];
    buf[2 * hash_functions::PADD_SIZE] = 0;
    for (int i = 0; i < hash_functions::PADD_SIZE; i++)
        sprintf(buf + i * 2, "%02x", digest[i]);
    return std::string(buf);
}

namespace Func
{
    // globales
    QTableWidget *adminTablaUsuarios = nullptr;
    QTableWidget *userTablaUsuarios = nullptr;
    QTableWidget *userTablaEnviadas = nullptr;
    QTableWidget *userTablaRecibidas = nullptr;
    QTableWidget *userTablaSugerencias = nullptr;
    QScrollArea *userPostFeed = nullptr;
    QScrollArea *userFriends = nullptr;
    QComboBox *selectedDate = nullptr;
    QComboBox *selectedOrder = nullptr;
    QSpinBox *countPost = nullptr;
    // Para las publicaciones
    ArbolABB posts;
    tm fecha;

    // TODO: METODOS GENERALES

    std::string generarHash(const std::string data)
    {
        std::string final = sha256(data);
        return final;
    }

    std::string leerArchivo(const std::string &path)
    {
        std::ifstream archivo(path);
        if (!archivo.is_open())
        {
            std::cerr << "Error: No se pudo abrir el archivo: " << path << std::endl;
            return "";
        }
        std::string contenido((std::istreambuf_iterator<char>(archivo)), std::istreambuf_iterator<char>());
        archivo.close();
        return contenido;
    }

    void cargarEstructuras(const std::string &data)
    {
        try
        {
            nlohmann::json j = nlohmann::json::parse(data);
            // Cargar usuarios
            if (j.contains("usuarios") && j["usuarios"].is_array())
            {
                for (const auto &usuario : j["usuarios"])
                {
                    if (usuario.contains("nombres") && usuario.contains("apellidos") &&
                        usuario.contains("fecha_de_nacimiento") && usuario.contains("correo") &&
                        usuario.contains("contrasena") && usuario.contains("rol"))
                    {
                        int id = usuario["id"];
                        std::string nombres = usuario["nombres"];
                        std::string apellidos = usuario["apellidos"];
                        std::string fechaNacimiento = usuario["fecha_de_nacimiento"];
                        std::string correo = usuario["correo"];
                        std::string contrasena = usuario["contrasena"];
                        std::string rol = usuario["rol"];

                        Structs::Usuario nuevo(id, nombres, apellidos, fechaNacimiento, correo, contrasena, rol);
                        lista_usuarios.insertar(nuevo);
                    }
                    else
                    {
                        std::cerr << "Error: Usuario incompleto en el JSON.\n";
                    }
                }
            }

            // Cargar solicitudes
            if (j.contains("solicitudes") && j["solicitudes"].is_array())
            {
                for (const auto &solicitud : j["solicitudes"])
                {
                    if (solicitud.contains("emisor") && solicitud.contains("receptor"))
                    {
                        std::string emisor = solicitud["emisor"];
                        std::string receptor = solicitud["receptor"];
                        lista_usuarios.enviarSolicitud(emisor, receptor);
                    }
                    else
                    {
                        std::cerr << "Error: Solicitud incompleta en el JSON.\n";
                    }
                }
            }

            // Cargar amigos
            if (j.contains("amigos") && j["amigos"].is_array())
            {
                for (const auto &amistad : j["amigos"])
                {
                    if (amistad.contains("emisor") && amistad.contains("receptor"))
                    {
                        Structs::Usuario *usuario1 = lista_usuarios.buscar(amistad["emisor"]);
                        Structs::Usuario *usuario2 = lista_usuarios.buscar(amistad["receptor"]);
                        if (usuario1 && usuario2)
                        {
                            if (!relaciones_amistad.verificarRelacion(usuario1->correo, usuario2->correo))
                            {
                                relaciones_amistad.agregarRelacion(*usuario1, *usuario2);
                            }
                            
                        }
                        else
                        {
                            std::cerr << "Error: No se encontraron usuarios para la amistad.\n";
                        }
                    }
                    else
                    {
                        std::cerr << "Error: Amistad incompleta en el JSON.\n";
                    }
                }
            }

            std::cout << "Estructuras cargadas correctamente.\n";
        }
        catch (const nlohmann::json::exception &e)
        {
            std::cerr << "Error al cargar las estructuras: " << e.what() << "\n";
        }
    }

    // TODO: Metodos Login
    void IniciarSesion(std::string email, std::string password)
    {
        Structs::Usuario *user = lista_usuarios.buscar(email);
        std::string hashpass = generarHash(password);
        if (user != nullptr && user->contrasena == hashpass)
        {
            usuario_logeado = user;
        }
    }

    bool RegistrarUsuario(QString nombres, QString apellidos, QString fechaNacimiento, QString correo, QString contrasena)
    {
        int id = lista_usuarios.obtenerId() + 1;
        std::string passHash = Func::generarHash(contrasena.toStdString());
        Structs::Usuario nuevo(id, nombres.toStdString(), apellidos.toStdString(), fechaNacimiento.toStdString(), correo.toStdString(), passHash);
        return lista_usuarios.insertar(nuevo);
    }

    bool ModificarUsuario(QString nombres, QString apellidos, QString fechaNacimiento, QString correo, std::string contrasena, std::string rol)
    {
        Structs::Usuario *user = lista_usuarios.buscar(correo.toStdString());
        if (user)
        {
            if (user->contrasena == contrasena)
            {
                return lista_usuarios.modificar(correo.toStdString(), nombres.toStdString(), apellidos.toStdString(), fechaNacimiento.toStdString(), contrasena, rol);
            }
            else
            {
                std::string hashpashh = generarHash(contrasena);
                return lista_usuarios.modificar(correo.toStdString(), nombres.toStdString(), apellidos.toStdString(), fechaNacimiento.toStdString(), hashpashh, rol);
            }
        }
        else
        {
            return false;
        }
    }

    void backupSerializacion()
    {
        Huffman::Huffman huff;
        QJsonObject objs;
        QJsonArray arr_users, arr_solicitudes, arr_amigos, arr_freqs;
        // Serializar Usuarios
        ListaEnlazada::ListaEnlazada<Structs::Usuario> usuarios = lista_usuarios.InOrder();
        for (int i = 0; i < usuarios.size(); ++i)
        {
            Structs::Usuario *user = usuarios.obtener(i);
            if (user)
            {
                QJsonObject userObj;
                userObj["id"] = user->id;
                userObj["nombres"] = QString::fromStdString(user->nombres);
                userObj["apellidos"] = QString::fromStdString(user->apellidos);
                userObj["fecha_de_nacimiento"] = QString::fromStdString(user->fechaNacimiento);
                userObj["correo"] = QString::fromStdString(user->correo);
                userObj["contrasena"] = QString::fromStdString(user->contrasena);
                userObj["rol"] = QString::fromStdString(user->rol);
                arr_users.append(userObj);
            }
        }
        // Serializar Solicitudes enviadas y recibidas
        for (int i = 0; i < usuarios.size(); i++)
        {
            Structs::Usuario *user = usuarios.obtener(i);
            if (user)
            {
                for (int j = 0; j < user->solicitudesEnviadas.size(); j++)
                {
                    Structs::Usuario *receptor = user->solicitudesEnviadas.obtener(j);
                    if (receptor)
                    {
                        QJsonObject solicitud;
                        solicitud["emisor"] = QString::fromStdString(user->correo);
                        solicitud["receptor"] = QString::fromStdString(receptor->correo);
                        solicitud["estado"] = "PENDIENTE";
                        arr_solicitudes.append(solicitud);
                    }
                }
                
            }
        }
        // Serializar Amigos
        for (int i = 0; i < usuarios.size(); ++i) {
            Structs::Usuario *user = usuarios.obtener(i);
            if (user) {
                ListaEnlazada::ListaEnlazada<Structs::Usuario> amigos = relaciones_amistad.obtenerAmigos(user->correo);
                for (int j = 0; j < amigos.size(); ++j) {
                    Structs::Usuario *amigo = amigos.obtener(j);
                    if (amigo) {
                        QJsonObject amistad;
                        amistad["emisor"] = QString::fromStdString(user->correo);
                        amistad["receptor"] = QString::fromStdString(amigo->correo);
                        amistad["estado"] = "ACEPTADA";
                        arr_amigos.append(amistad);
                    }
                }
            }
        }


        objs["usuarios"] = arr_users;
        objs["solicitudes"] = arr_solicitudes;
        objs["amigos"] = arr_amigos;
        QJsonDocument jsonDoc(objs);
        QString jsonString = jsonDoc.toJson(QJsonDocument::Indented);
        std::string json_str = jsonString.toStdString();

        std::string comprimido = huff.compress(json_str);
        huff.exportTree(".backups/_frecuencias.json");
        // Comprimir datos en backup.edd
        QFile file2(".backups/_backup.edd");
        if (file2.open(QIODevice::WriteOnly) | QIODevice::Text)
        {
            QTextStream out(&file2);
            out << QString::fromStdString(comprimido);
            file2.close();
        }
    }

    void CerrarSesion()
    {
        try
        {
            //backupSerializacion();
            adminTablaUsuarios = nullptr;
            userTablaUsuarios = nullptr;
            userTablaEnviadas = nullptr;
            userTablaRecibidas = nullptr;
            userPostFeed = nullptr;
            userFriends = nullptr;
            selectedDate = nullptr;
            selectedOrder = nullptr;
            countPost = nullptr;
            // Para las publicaciones
            posts.limpiar();
            usuario_logeado = nullptr;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
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
                    std::string hashpass = generarHash(contrasena);
                    string fechaNacimiento = usuario["fecha_de_nacimiento"].get<string>();

                    if (lista_usuarios.buscar(correo) == nullptr)
                    {
                        // Crear Usuario
                        int id = lista_usuarios.obtenerId() + 1;
                        Structs::Usuario nuevoUsuario(id, nombres, apellidos, fechaNacimiento, correo, hashpass);
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
            ifstream archivo(directorio);
            if (!archivo.is_open())
            {
                throw runtime_error("No se pudo abrir el archivo para leer.");
            }
            int contador = 0;
            int conerror = 0;
            int noexisten = 0;
            nlohmann::json post;
            archivo >> post;
            for (const auto &p : post)
            {
                if (p.contains("emisor") && p.contains("receptor") && p.contains("estado"))
                {
                    string emisor = p["emisor"].get<string>();
                    string receptor = p["receptor"].get<string>();
                    string estado = p["estado"].get<string>();
                    transform(estado.begin(), estado.end(), estado.begin(), ::tolower);

                    Structs::Usuario *usuario_emisor = lista_usuarios.buscar(emisor);
                    Structs::Usuario *usuario_receptor = lista_usuarios.buscar(receptor);

                    if (usuario_emisor == nullptr)
                    {
                        noexisten++;
                        continue;
                    }
                    else
                    {
                        if (usuario_emisor->verificarSolicitudEnviada(receptor))
                        {
                            conerror++;
                            continue;
                        }
                    }
                    if (usuario_receptor == nullptr)
                    {
                        noexisten++;
                        continue;
                    }
                    else
                    {
                        if (usuario_receptor->verificarSolicitudEnviada(emisor))
                        {
                            conerror++;
                            continue;
                        }
                    }

                    if (relaciones_amistad.verificarRelacion(emisor, receptor))
                    {
                        conerror++;
                        continue;
                    }

                    if (estado == "pendiente")
                    {
                        lista_usuarios.enviarSolicitud(emisor, receptor);
                    }
                    else if (estado == "aceptada" || estado == "aceptado")
                    {
                        relaciones_amistad.agregarRelacion(*usuario_emisor, *usuario_receptor);
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
    void CargarPublicaciones(string directorio)
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
                if (p.contains("correo") && p.contains("contenido") &&
                    p.contains("fecha") && p.contains("hora"))
                {

                    string correo = p["correo"].get<string>();
                    string contenido = p["contenido"].get<string>();
                    string _fecha = p["fecha"].get<string>();
                    string hora = p["hora"].get<string>();

                    Structs::Usuario *autor = lista_usuarios.buscar(correo);

                    if (autor == nullptr)
                    {
                        noexisten++;
                        continue;
                    }

                    // Convertir la fecha
                    string fechaConvertida;
                    try
                    {
                        fechaConvertida = convertirFecha(_fecha);
                    }
                    catch (const std::runtime_error &e)
                    {
                        conerror++;
                        continue;
                    }

                    // Crear Publicacion
                    int newid = Func::obtenerPostID() + 1;
                    Structs::Publicacion nuevaPublicacion(newid, autor->correo, contenido, fechaConvertida, hora);

                    // Verifica si 'comentarios' es un array
                    if (p.contains("comentarios") && p["comentarios"].is_array())
                    {
                        nlohmann::json comentariosJson = p["comentarios"]; // Aquí solo asignamos el array
                        for (const auto &comentario : comentariosJson)
                        {
                            if (comentario.contains("correo") && comentario.contains("comentario") &&
                                comentario.contains("fecha") && comentario.contains("hora"))
                            {
                                int idComentario = Func::obtenerCommentID(newid)+1;
                                string correoComentario = comentario["correo"].get<string>();
                                string contenidoComentario = comentario["comentario"].get<string>();
                                string fechaComentario = comentario["fecha"].get<string>();
                                string horaComentario = comentario["hora"].get<string>();

                                // Convertir la fecha
                                string fechaConvertidaComentario;
                                try
                                {
                                    fechaConvertidaComentario = convertirFechayHora(fechaComentario + " " + horaComentario);
                                }
                                catch (const std::runtime_error &e)
                                {
                                    conerror++;
                                    continue;
                                }

                                StructsComment::Comentario nuevoComentario(idComentario,fechaConvertidaComentario, correoComentario, contenidoComentario);
                                nuevaPublicacion.comentarios->insertar(nuevoComentario);
                            }
                            else
                            {
                                conerror++;
                            }
                        }
                    }
                    else
                    {
                        conerror++; // Maneja el caso en que no hay comentarios o no es un array
                    }

                    lista_publicaciones.insertar(nuevaPublicacion);
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
            QMessageBox::information(nullptr, "Información", "Publicaciones agregadas: " + stdagregados + "\nErrores: " + stderrores + "\nNo Existen autores: " + stdnoexisten);
            Func::agregarSeguridad();
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
    void EliminarCuenta(string correo)
    {
        // Eliminar las publicaciones del usuario
        eliminarPublicacionUsuario(correo);
        // Eliminar todas las relaciones de amistad del usuario logueado
        relaciones_amistad.eliminarRelacionesUsuario(correo);
        // Eliminar las solicitudes enviadas y recibidas del usuario logueado
        lista_usuarios.eliminarSolicitudes(correo);
        // Eliminar al usuario logueado de la lista de usuarios
        lista_usuarios.eliminar(correo);
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
    std::string graficarPublicaciones()
    {
        std::string dotPath = "renders/publicaciones.dot";
        std::string imgPath = "renders/publicaciones.png";

        std::ofstream file(dotPath);

        if (file.is_open())
        {
            file << "digraph G {\n";
            file << "rankdir = TB;\n";
            file << "node [shape=record];\n";
            file << "label=\"Lista de publicaciones\" fontsize = 20 fontname = \"Arial\";";

            for (int i = 0; i < lista_publicaciones.size(); ++i)
            {
                Structs::Publicacion *p = lista_publicaciones.obtener(i);
                if (p)
                {
                    file << "node" << i << " [label=\"{ID: " << p->id << " | Autor: " << p->correo_autor << " | Fecha: " << p->fecha << " | Hora: " << p->hora << " | Contenido: " << p->contenido << "}\"];\n";
                }
            }

            for (int i = 0; i < lista_publicaciones.size(); ++i)
            {
                Structs::Publicacion *p = lista_publicaciones.obtener(i);
                if (p)
                {
                    if (i + 1 < lista_publicaciones.size())
                    {
                        file << "node" << i << " -> node" << i + 1 << ";\n";
                        file << "node" << i + 1 << " -> node" << i << ";\n";
                    }
                }
            }

            file << "}";
            file.close();

            std::string command = "dot -Tpng " + dotPath + " -o " + imgPath;
            system(command.c_str());
        }
        return imgPath;
    }

    // TODO: Metodos usuario / Publicaciones
    void ActualizarFeed()
    {
        actualizarArbolPost();
        actualizarListaFechas();

        QWidget *contentWidget = userPostFeed->widget();
        QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(contentWidget->layout());
        if (!layout)
        {
            layout = new QVBoxLayout(contentWidget);
            contentWidget->setLayout(layout);
        }
        while (QLayoutItem *item = layout->takeAt(0))
        {
            if (item->widget())
            {
                item->widget()->deleteLater();
            }
            delete item;
        }
        // obtenemos amigos e insertamos las publicaciones
        int orden = selectedOrder->currentIndex();
        int cantidad = countPost->value();
        ListaEnlazada::ListaEnlazada<Structs::Publicacion> posts_feed;

        // si la fecha es 01-01-0001, se obtienen todas las publicaciones
        std::string _f = convertirFecha(fecha);
        if (_f == "01-01-0001")
        {
            if (orden == 0)
            {
                posts_feed = posts.inorder(cantidad);
            }
            else if (orden == 1)
            {
                posts_feed = posts.preorder(cantidad);
            }
            else if (orden == 2)
            {
                posts_feed = posts.postorder(cantidad);
            }
        }
        else
        {
            posts_feed = posts.obtenerPublicaciones(fecha, orden, cantidad);
        }

        for (int i = 0; i < posts_feed.size(); ++i)
        {
            Structs::Publicacion *p = posts_feed.obtener(i);
            if (p)
            {
                int postid = p->id;
                WidgetPost *newPost = new WidgetPost(postid);
                layout->addWidget(newPost);
            }
        }
    }
    void actualizarListaFechas()
    {
        // ACTUALIZAR EL FEED CON LA LISTA DE PUBLICACIONES
        if (selectedDate->currentText().contains("Todos"))
        {
            fecha = convertirFechaTm("01-01-0001");
        }
        else
        {
            fecha = convertirFechaTm(selectedDate->currentText().toStdString());
        }
        ListaEnlazada::ListaEnlazada<std::tm> fechas = posts.obtenerFechas();
        selectedDate->clear();
        selectedDate->addItem("Todos");
        int index = -1;
        for (int i = 0; i < fechas.size(); ++i)
        {
            std::tm *f = fechas.obtener(i);
            if (f)
            {
                if (f->tm_year == fecha.tm_year && f->tm_mon == fecha.tm_mon && f->tm_mday == fecha.tm_mday)
                {
                    index = i;
                }
                QString fecha = QString::fromStdString(convertirFecha(*f));
                selectedDate->addItem(fecha);
            }
        }
        selectedDate->setCurrentIndex(index + 1);
    }
    void actualizarArbolPost()
    {
        posts.limpiar();
        ListaEnlazada::ListaEnlazada<Structs::Usuario> amigos = relaciones_amistad.obtenerAmigos(usuario_logeado->correo);
        for (int i = 0; i < lista_publicaciones.size(); i++)
        {
            Structs::Publicacion *publicacion = lista_publicaciones.obtener(i);
            std::tm _fecha = convertirFechaTm(publicacion->fecha);
            if (amigos.size() == 0)
            {
                if (usuario_logeado->correo == publicacion->correo_autor)
                {
                    posts.insertar(_fecha, *publicacion);
                }
            }
            else
            {
                for (int i = 0; i < amigos.size(); i++)
                {
                    Structs::Usuario *amigo = amigos.obtener(i);
                    if (amigo == nullptr)
                    {
                        continue;
                    }
                    if (usuario_logeado->correo == publicacion->correo_autor || amigo->correo == publicacion->correo_autor)
                    {
                        posts.insertar(_fecha, *publicacion);
                        break;
                    }
                }
            }
        }
    }
    int obtenerPostID()
    {
        int id = -1;
        int ultimoId = lista_publicaciones.size() - 1;
        Structs::Publicacion *p = lista_publicaciones.obtener(ultimoId);
        if (p)
        {
            id = p->id;
        }
        return id;
    }
    int obtenerCommentID(int postid)
    {
        try {
            int id = -1;
            Structs::Publicacion *p = lista_publicaciones.obtener(postid);
            if (p)
            {
                ListaEnlazada::ListaEnlazada<StructsComment::Comentario> comentarios = p->comentarios->obtenerComentarios();
                int ultimoId = comentarios.size() - 1;
                StructsComment::Comentario *ultimo = comentarios.obtener(ultimoId);
                if (ultimo) {
                    id = ultimo->id;
                }
            }
            return id;
        } catch (const std::exception &e) {
            return -1;
        }
    }
    void eliminarPublicacion(int id)
    {
        for (int i = 0; i < lista_publicaciones.size(); ++i)
        {
            Structs::Publicacion *p = lista_publicaciones.obtener(i);
            if (p)
            {
                if (p->id == id)
                {
                    lista_publicaciones.eliminarPosicion(i);
                    break;
                }
            }
        }
    }
    Structs::Publicacion *buscarPost(int id)
    {
        for (int i = 0; i < lista_publicaciones.size(); ++i)
        {
            Structs::Publicacion *p = lista_publicaciones.obtener(i);
            if (p)
            {
                if (p->id == id)
                {
                    return p;
                }
            }
        }
        return nullptr;
    }

    void modificarPublicacion(int id, std::string contenido, std::string pathImg)
    {
        for (int i = 0; i < lista_publicaciones.size(); ++i)
        {
            Structs::Publicacion *p = lista_publicaciones.obtener(i);
            if (p)
            {
                if (p->id == id)
                {
                    p->contenido = contenido;
                    p->imagen = pathImg;
                    break;
                }
            }
        }
        Func::agregarSeguridad();
    }
    void ComentarPublicacion(int id, StructsComment::Comentario comentario)
    {
        for (int i = 0; i < lista_publicaciones.size(); ++i)
        {
            Structs::Publicacion *p = lista_publicaciones.obtener(i);
            if (p)
            {
                if (p->id == id)
                {
                    p->comentarios->insertar(comentario);
                    break;
                }
            }
        }
        Func::agregarSeguridad();
    }
    bool existeComentario(int postid, int cid){
        Structs::Publicacion *p = buscarPost(postid);
        if (p) {
            ListaEnlazada::ListaEnlazada<StructsComment::Comentario> comentarios = p->comentarios->obtenerComentarios();
            for (int i = 0; i < comentarios.size(); ++i) {
                StructsComment::Comentario *c = comentarios.obtener(i);
                if (c) {
                    if (c->id==cid) {
                        return true;
                    }else{
                        return false;
                    }
                }
            }
        }
    }
    void agregarSeguridad()
    {
        ListaEnlazada::ListaEnlazada<Structs::Publicacion> posts;

        for (int i = 0; i < lista_publicaciones.size(); ++i) {
            Structs::Publicacion &p = *lista_publicaciones.obtener(i);
            Structs::Publicacion newPost(p.id, p.correo_autor,p.contenido, p.fecha, p.hora, p.imagen);
            ListaEnlazada::ListaEnlazada<StructsComment::Comentario> c = p.comentarios->obtenerComentarios();
            for (int j = 0; j < c.size(); ++j) {
                StructsComment::Comentario &cm =*c.obtener(j);
                StructsComment::Comentario newCm(cm.id, cm.fecha_hora,cm.usuario,cm.texto);
                newPost.comentarios->insertar(newCm);
            }
            posts.insertar(newPost);
        }
        if (posts.size() == 0) {
            cout << "no hay data";
            return;
        }
        Structs::Block nuevo;
        Merkle::Merkle root(posts);
        nuevo.data = posts;
        nuevo.rootHash = root.getRootHash();
        seguridad_blockchain.addBlock(nuevo);
        seguridad_blockchain.validateBlocks();
    }


    // TODO: Metodos Usuario / Solicitudes
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
    void ActualizarTablaSugeridos(QTableWidget *table)
    {
        // Limpiar la tabla antes de agregar nuevos elementos
        table->clearContents();

        // Obtener las listas de sugerencias y solicitudes enviadas/recibidas
        ListaEnlazada::ListaEnlazada<Structs::AmigoSugerido> listaSugeridos = relaciones_amistad.sugerirAmigos(usuario_logeado->correo);
        ListaEnlazada::ListaEnlazada<Structs::Usuario> enviadas = usuario_logeado->solicitudesEnviadas;
        ListaEnlazada::ListaEnlazada<Structs::Usuario> recibidas = usuario_logeado->solicitudesRecibidas;

        // Crear una nueva lista para los usuarios filtrados
        ListaEnlazada::ListaEnlazada<Structs::AmigoSugerido> listaFiltrada;

        // Filtrar los sugeridos eliminando aquellos que ya han sido enviados o recibidos
        for (int i = 0; i < listaSugeridos.size(); i++)
        {
            Structs::AmigoSugerido sugerido = *listaSugeridos.obtener(i);

            bool yaEnviado = false;
            bool yaRecibido = false;

            // Verificar si el usuario sugerido está en la lista de solicitudes enviadas
            for (int j = 0; j < enviadas.size(); j++)
            {
                if (enviadas.obtener(j)->correo == sugerido.usuario.correo)
                {
                    yaEnviado = true;
                    break;
                }
            }

            // Verificar si el usuario sugerido está en la lista de solicitudes recibidas
            for (int k = 0; k < recibidas.size(); k++)
            {
                if (recibidas.obtener(k)->correo == sugerido.usuario.correo)
                {
                    yaRecibido = true;
                    break;
                }
            }

            // Si no ha sido enviado ni recibido, agregarlo a la lista filtrada
            if (!yaEnviado && !yaRecibido)
            {
                listaFiltrada.insertar(sugerido);
            }
        }

        table->setRowCount(listaFiltrada.size()); // Ajustar el número de filas según el tamaño de la lista

        int row = 0;
        // Recorremos la lista para agregar los elementos a la tabla
        for (int i = 0; i < listaFiltrada.size(); ++i)
        {
            if (listaFiltrada.obtener(i))
            {
                QString correo_sugerido = QString::fromStdString(listaFiltrada.obtener(i)->usuario.correo);
                QString encomun = QString::number(listaFiltrada.obtener(i)->enComun);

                QTableWidgetItem *emailItem = new QTableWidgetItem(correo_sugerido);
                QTableWidgetItem *countItem = new QTableWidgetItem(encomun);

                // Agregamos los datos a la tabla
                table->setItem(row, 0, emailItem);
                table->setItem(row, 1, countItem);

                // Capturar el id por valor en las lambdas
                string correo = listaFiltrada.obtener(i)->usuario.correo;

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
                modifyButtonLayout->setContentsMargins(0, 0, 0, 0);
                sendButtonContainer->setLayout(modifyButtonLayout);
                sendButtonContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

                table->setCellWidget(row, 2, sendButtonContainer); // Botón en columna 2

                row++;
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
    void ActualizarTablas()
    {
        Func::ActualizarTablaUsuarios(userTablaUsuarios);
        Func::ActualizarTablaSugeridos(userTablaSugerencias);
        Func::ActualizarTablaEnviados(userTablaEnviadas);
        Func::ActualizarTablaRecibidos(userTablaRecibidas);
    }

    // TODO: Metodos usuario / reportes
    ListaEnlazada::ListaEnlazada<Structs::ReportePosts> obtenerReporteFechasPost()
    {
        ArbolABB misPost;
        for (int i = 0; i < lista_publicaciones.size(); i++)
        {
            Structs::Publicacion *publicacion = lista_publicaciones.obtener(i);
            std::tm _fecha = convertirFechaTm(publicacion->fecha);
            if (publicacion && publicacion->correo_autor == usuario_logeado->correo)
            {
                misPost.insertar(_fecha, *publicacion);
            }
        }
        ListaEnlazada::ListaEnlazada<std::tm> fechas = misPost.obtenerFechas();
        ListaEnlazada::ListaEnlazada<Structs::ReportePosts> reportes;
        for (int i = 0; i < fechas.size(); ++i)
        {
            std::string _fecha = convertirFecha(*fechas.obtener(i));
            ListaEnlazada::ListaEnlazada<Structs::Publicacion> posts = misPost.obtenerPublicaciones(*fechas.obtener(i));
            Structs::ReportePosts reporte(_fecha, posts);
            reportes.insertar(reporte);
        }
        return reportes;
    }

    ListaEnlazada::ListaEnlazada<Structs::ReportePosts> obtenerReporteFechasPostFriends()
    {
        ArbolABB misPost;
        ListaEnlazada::ListaEnlazada<Structs::Usuario> amigos = relaciones_amistad.obtenerAmigos(usuario_logeado->correo);
        for (int i = 0; i < lista_publicaciones.size(); i++)
        {
            Structs::Publicacion *publicacion = lista_publicaciones.obtener(i);
            std::tm _fecha = convertirFechaTm(publicacion->fecha);
            if (amigos.size() == 0)
            {
                if (usuario_logeado->correo == publicacion->correo_autor)
                {
                    misPost.insertar(_fecha, *publicacion);
                }
            }
            else
            {
                for (int i = 0; i < amigos.size(); i++)
                {
                    Structs::Usuario *amigo = amigos.obtener(i);
                    if (amigo == nullptr)
                    {
                        continue;
                    }
                    if (usuario_logeado->correo == publicacion->correo_autor || amigo->correo == publicacion->correo_autor)
                    {
                        misPost.insertar(_fecha, *publicacion);
                        break;
                    }
                }
            }
        }
        ListaEnlazada::ListaEnlazada<std::tm> fechas = misPost.obtenerFechas();
        ListaEnlazada::ListaEnlazada<Structs::ReportePosts> reportes;
        for (int i = 0; i < fechas.size(); ++i)
        {
            std::string _fecha = convertirFecha(*fechas.obtener(i));
            ListaEnlazada::ListaEnlazada<Structs::Publicacion> posts = misPost.obtenerPublicaciones(*fechas.obtener(i));
            Structs::ReportePosts reporte(_fecha, posts);
            reportes.insertar(reporte);
        }
        return reportes;
    }

    ListaEnlazada::ListaEnlazada<Structs::Publicacion> obtenerMisPosts()
    {
        ListaEnlazada::ListaEnlazada<Structs::Publicacion> reportes;
        for (int i = 0; i < lista_publicaciones.size(); i++)
        {
            Structs::Publicacion *publicacion = lista_publicaciones.obtener(i);
            if (publicacion && publicacion->correo_autor == usuario_logeado->correo)
            {
                reportes.insertar(*publicacion);
            }
        }
        return reportes;
    }

    void bubbleSort(ListaEnlazada::ListaEnlazada<Structs::ReportePosts> &lista)
    {
        for (int i = 0; i < lista.size(); i++)
        {
            for (int j = 0; j < lista.size() - 1; j++)
            {
                Structs::ReportePosts *r1 = lista.obtener(j);
                Structs::ReportePosts *r2 = lista.obtener(j + 1);
                if (r1->publicaciones.size() < r2->publicaciones.size())
                {
                    lista.intercambiar(j, j + 1);
                }
            }
        }
    }
    void bubbleSort(ListaEnlazada::ListaEnlazada<Structs::Publicacion> &lista)
    {
        for (int i = 0; i < lista.size(); i++)
        {
            for (int j = 0; j < lista.size() - 1; j++)
            {
                Structs::Publicacion *p1 = lista.obtener(j);
                Structs::Publicacion *p2 = lista.obtener(j + 1);
                if (p1->comentarios->size() < p2->comentarios->size())
                {
                    lista.intercambiar(j, j + 1);
                }
            }
        }
    }

    // TODO: Metodos usuario / perfil
    void EliminarMiCuenta()
    {
        // Eliminar las publicaciones del usuario
        eliminarPublicacionUsuario(usuario_logeado->correo);
        // Eliminar todas las relaciones de amistad del usuario logueado
        relaciones_amistad.eliminarRelacionesUsuario(usuario_logeado->correo);
        // Eliminar las solicitudes enviadas y recibidas del usuario logueado
        lista_usuarios.eliminarSolicitudes(usuario_logeado->correo);
        // Eliminar al usuario logueado de la lista de usuarios
        lista_usuarios.eliminar(usuario_logeado->correo);
        // Desloguear al usuario
        usuario_logeado = nullptr;
    }
    void ActualizarListaAmigos()
    {
        QWidget *contentWidget = userFriends->widget();
        QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(contentWidget->layout());
        if (!layout)
        {
            layout = new QVBoxLayout(contentWidget);
            contentWidget->setLayout(layout);
        }
        while (QLayoutItem *item = layout->takeAt(0))
        {
            if (item->widget())
            {
                item->widget()->deleteLater();
            }
            delete item;
        }
        ListaEnlazada::ListaEnlazada<Structs::Usuario> amigos = relaciones_amistad.obtenerAmigos(usuario_logeado->correo);
        for (int i = 0; i < amigos.size(); ++i)
        {
            Structs::Usuario *u = amigos.obtener(i);
            if (u)
            {
                WidgetFriend *newWidget = new WidgetFriend(*u);
                layout->addWidget(newWidget);
            }
        }
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
                if (u->correo == usuario_logeado->correo)
                {
                    lista.eliminar(i);
                    --i;
                    continue;
                }

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
                    --i;
                    continue;
                }

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
                    --i;
                    continue;
                }

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
                    --i;
                }
            }
        }

        return lista;
    }
    void eliminarPublicacionUsuario(string correo)
    {
        for (int i = 0; i < lista_publicaciones.size(); ++i)
        {
            Structs::Publicacion *p = lista_publicaciones.obtener(i);
            if (p)
            {
                if (p->correo_autor == correo)
                {
                    lista_publicaciones.eliminarPosicion(i);
                }
            }
        }
    }

    std::string convertirFecha(const std::string &fechaOriginal)
    {
        std::tm fecha = {};
        std::vector<std::string> formatos = {
            "%d-%m-%Y", // dd-mm-aaaa
            "%Y-%m-%d", // aaaa-mm-dd
            "%m/%d/%Y", // mm/dd/aaaa
            "%d/%m/%Y"  // dd/mm/aaaa
        };

        for (const auto &formato : formatos)
        {
            std::istringstream ss(fechaOriginal);
            ss >> std::get_time(&fecha, formato.c_str());
            if (!ss.fail())
            {
                std::ostringstream oss;
                oss << std::put_time(&fecha, "%d-%m-%Y");
                return oss.str();
            }
        }

        throw std::runtime_error("Error al parsear la fecha.");
    }

    std::string convertirFecha(const std::tm &fecha)
    {
        // Crear un stringstream para construir la fecha en el formato deseado
        std::ostringstream oss;

        // Convertir el std::tm a string en el formato "dd-mm-yyyy"
        oss << std::put_time(&fecha, "%d-%m-%Y");

        // Devolver la cadena formateada
        return oss.str();
    }

    std::tm convertirFechaTm(const std::string &fechaOriginal)
    {
        std::tm fecha = {};
        std::vector<std::string> formatos = {
            "%d-%m-%Y", // dd-mm-aaaa
            "%Y-%m-%d", // aaaa-mm-dd
            "%m/%d/%Y", // mm/dd/aaaa
            "%d/%m/%Y"  // dd/mm/aaaa
        };

        for (const auto &formato : formatos)
        {
            std::istringstream ss(fechaOriginal);
            ss >> std::get_time(&fecha, "%d-%m-%Y");
            if (!ss.fail())
            {
                return fecha;
            }
        }

        throw std::runtime_error("Error al parsear la fecha.");
    }

    string convertirFechayHora(const std::string &fechaOriginal)
    {
        std::tm fecha = {};
        std::vector<std::string> formatos = {
            "%d-%m-%Y %H:%M", // dd-mm-aaaa hh:mm
            "%Y-%m-%d %H:%M", // aaaa-mm-dd hh:mm
            "%m/%d/%Y %H:%M", // mm/dd/aaaa hh:mm
            "%d/%m/%Y %H:%M"  // dd/mm/aaaa hh:mm
        };

        for (const auto &formato : formatos)
        {
            std::istringstream ss(fechaOriginal);
            ss >> std::get_time(&fecha, formato.c_str());
            if (!ss.fail())
            {
                std::ostringstream oss;
                oss << std::put_time(&fecha, "%d-%m-%Y %H:%M");
                return oss.str();
            }
        }

        throw std::runtime_error("Error al parsear la fecha.");
    }

    std::tm convertirFechayHoraTm(const std::string &fechaOriginal)
    {
        std::tm fecha = {};
        std::vector<std::string> formatos = {
            "%d-%m-%Y %H:%M", // dd-mm-aaaa hh:mm
            "%Y-%m-%d %H:%M", // aaaa-mm-dd hh:mm
            "%m/%d/%Y %H:%M", // mm/dd/aaaa hh:mm
            "%d/%m/%Y %H:%M"  // dd/mm/aaaa hh:mm
        };

        for (const auto &formato : formatos)
        {
            std::istringstream ss(fechaOriginal);
            ss >> std::get_time(&fecha, formato.c_str());
            if (!ss.fail())
            {
                // Si parsea correctamente, retornamos el std::tm
                return fecha;
            }
        }

        throw std::runtime_error("Error al parsear la fecha.");
    }

    std::string convertirFechayHora(const std::tm &fecha)
    {
        // Crear un stringstream para construir la fecha en el formato deseado
        std::ostringstream oss;

        // Convertir el std::tm a string en el formato "dd-mm-yyyy hh:mm"
        oss << std::put_time(&fecha, "%d-%m-%Y %H:%M");

        // Devolver la cadena formateada
        return oss.str();
    }

    std::string generateTimestamp() {
        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        std::tm* localTime = std::localtime(&now_c);
        std::ostringstream oss;
        oss << std::put_time(localTime, "%d-%m-%y-::%H:%M:%S");
        return oss.str();
    }
}
