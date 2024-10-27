#ifndef STRUCTS_H
#define STRUCTS_H

#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

#include "arbolb5.h"
#include "hash_functions.h"

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
        void modificar(string nombres, string apellidos, string fecha, string contrasena, string rol)
        {
            this->nombres = nombres;
            this->apellidos = apellidos;
            this->fechaNacimiento = fecha;
            this->contrasena = contrasena;
            this->rol = rol;
        }

        // Enviar Solicitud
        void enviarSolicitud(const Usuario &u)
        {
            this->solicitudesEnviadas.insertar(u);
        }
        void cancelarSolicitud(const string correo)
        {
            for (int i = 0; i < this->solicitudesEnviadas.size(); ++i)
            {
                Usuario *temp = this->solicitudesEnviadas.obtener(i);
                if (temp)
                {
                    if (temp->correo == correo)
                    {
                        this->solicitudesEnviadas.eliminar(i);
                        break;
                    }
                }
            }
        }

        // Recibir Solicitud
        void recibirSolicitud(const Usuario &u)
        {
            this->solicitudesRecibidas.insertar(u);
        }
        void aceptarSolicitudRec(const string correo)
        {
            for (int i = 0; i < this->solicitudesRecibidas.size(); ++i)
            {
                Usuario *temp = this->solicitudesRecibidas.obtener(i);
                if (temp)
                {
                    if (temp->correo == correo)
                    {
                        this->solicitudesRecibidas.eliminar(i);
                        break;
                    }
                }
            }
        }
        void aceptarSolicitudEnv(const string correo)
        {
            for (int i = 0; i < this->solicitudesEnviadas.size(); ++i)
            {
                Usuario *temp = this->solicitudesEnviadas.obtener(i);
                if (temp)
                {
                    if (temp->correo == correo)
                    {
                        this->solicitudesEnviadas.eliminar(i);
                        break;
                    }
                }
            }
        }
        void rechazarSolicitud(const string correo)
        {
            for (int i = 0; i < this->solicitudesRecibidas.size(); ++i)
            {
                Usuario *temp = this->solicitudesRecibidas.obtener(i);
                if (temp)
                {
                    if (temp->correo == correo)
                    {
                        this->solicitudesRecibidas.eliminar(i);
                        break;
                    }
                }
            }
        }

        bool verificarSolicitudRecibida(const string correo)
        {
            for (int i = 0; i < this->solicitudesRecibidas.size(); ++i)
            {
                Usuario *temp = this->solicitudesRecibidas.obtener(i);
                if (temp)
                {
                    if (temp->correo == correo)
                    {
                        return true;
                    }
                }
            }
            return false;
        }

        bool verificarSolicitudEnviada(const string correo)
        {
            for (int i = 0; i < this->solicitudesEnviadas.size(); ++i)
            {
                Usuario *temp = this->solicitudesEnviadas.obtener(i);
                if (temp)
                {
                    if (temp->correo == correo)
                    {
                        return true;
                    }
                }
            }
            return false;
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
        string imagen;
        ArbolB5 *comentarios = new ArbolB5();
        // Constructor
        Publicacion(int id, string correo_autor, const string &contenido, const string &fecha, const string &hora, const string imagen = "default.jpg")
            : id(id), correo_autor(correo_autor), contenido(contenido), fecha(fecha), hora(hora), imagen(imagen) {}

        // Destructor
        ~Publicacion() {}

        std::string toJSON() const {
            using json = nlohmann::json;
            json j;
            j["id"] = id;
            j["correo_autor"] = correo_autor;
            j["contenido"] = contenido;
            j["fecha"] = fecha;
            j["hora"] = hora;
            j["imagen"] = imagen;

            // Crear un array JSON para los comentarios
            json comentariosArray = json::array();
            ListaEnlazada::ListaEnlazada<StructsComment::Comentario> arr_comentarios = comentarios->obtenerComentarios();
            for (int i = 0; i < arr_comentarios.size(); ++i) {
                StructsComment::Comentario* c = arr_comentarios.obtener(i);
                if (c) {
                    json comentarioObj;
                    comentarioObj["usuario"] = c->usuario;
                    comentarioObj["fecha_hora"] = c->fecha_hora;
                    comentarioObj["texto"] = c->texto;
                    comentariosArray.push_back(comentarioObj);
                }
            }

            // Asignar el array de comentarios al JSON principal
            j["comentarios"] = comentariosArray;

            return j.dump();
        }

    };

    struct ReportePosts
    {
        string fecha;
        ListaEnlazada::ListaEnlazada<Structs::Publicacion> publicaciones;

        // Constructor
        ReportePosts(string fecha, ListaEnlazada::ListaEnlazada<Structs::Publicacion> publicaciones) : fecha(fecha), publicaciones(publicaciones) {}

        // Destructor
        ~ReportePosts() {}
    };

    struct AmigoSugerido
    {
        Structs::Usuario usuario;
        int enComun;

        AmigoSugerido(Usuario usuario, int enComun = 0) : usuario(usuario), enComun(enComun) {}

        ~AmigoSugerido() {}

        void mostrar()
        {
            cout << "Nombre: " << usuario.nombres << " " << usuario.apellidos << endl;
            cout << "Correo: " << usuario.correo << endl;
            cout << "Amigos en común: " << enComun << endl;
        }

        Structs::Usuario getUsuario()
        {
            return this->usuario;
        }


        // agregar amigos en común
        void agregarEnComun()
        {
            this->enComun += 1;
        }
    };

    struct Block {
        int index;
        string timestamp;
        ListaEnlazada::ListaEnlazada<Publicacion> data;
        string rootHash;
        string prevHash = "0000";
        bool validate;
        int nonce = 0;
        string hashBlock;

        Block(){}

        void calculateNonceAndHash() {
            do {
                nonce++;
                hashBlock = sha256(std::to_string(index) + timestamp + std::to_string(nonce) + rootHash + prevHash);
            } while (hashBlock.compare(0, 4, "0000") != 0);
        }

        void validateBlock(bool val){
            validate = val;
        }

        std::string getJsonData(){
            nlohmann::json jsonData;
            for (int j = 0; j < data.size(); j++)
            {
                Structs::Publicacion *currentData = data.obtener(j);
                if (currentData == nullptr) {
                    std::cerr << "Error: Publicación nula " << j << std::endl;
                    continue;
                }

                nlohmann::json jsonCurrentData;
                jsonCurrentData["id"] = currentData->id;
                jsonCurrentData["correo"] = currentData->correo_autor;
                jsonCurrentData["contenido"] = currentData->contenido;
                jsonCurrentData["fecha"] = currentData->fecha;
                jsonCurrentData["hora"] = currentData->hora;
                jsonCurrentData["imagen"] = currentData->imagen;

                // Manejo de comentarios
                nlohmann::json jsonCurrentComments, jsonComment;
                ListaEnlazada::ListaEnlazada<StructsComment::Comentario> comments = currentData->comentarios->obtenerComentarios();
                if (comments.size() == 0) {
                    jsonCurrentComments = nlohmann::json::array();
                }else{
                    for (int k = 0; k < comments.size(); ++k) {
                        StructsComment::Comentario *comment = comments.obtener(k);
                        if (comment == nullptr) {
                            std::cerr << "Error: Comentario nulo en la publicación " << currentData->id << ", posición " << k << std::endl;
                            continue;
                        }

                        nlohmann::json jsonComment;
                        jsonComment["id"] = comment->id;
                        jsonComment["correo"] = comment->usuario;
                        jsonComment["comentario"] = comment->texto;
                        jsonComment["fechaHora"] = comment->fecha_hora;
                        jsonCurrentComments.push_back(jsonComment);
                    }
                }

                // Añadir comentarios a la publicación
                jsonCurrentData["comentarios"] = jsonCurrentComments;
                jsonData.push_back(jsonCurrentData);
            }
            return jsonData.dump(4);
        }


        void showBlock() const {
            cout << "| ==================================================================================== |"<< endl;
            cout << "| INDEX:           " << std::to_string(index) << endl;
            cout << "| TIMESTAMP:       " << timestamp << endl;
            cout << "| NONCE:           " << std::to_string(nonce)<< endl;
            cout << "| ROOT HASH:       " << rootHash << endl;
            cout << "| PREV HASH:       " << prevHash<< endl;
            cout << "| HASH:            " << hashBlock << endl;
            cout << "| VALIDATE:        " << validate << endl;
            cout << "| DATA:            " << data.size() << endl;
        }
    };


    }
#endif // STRUCTS_H
