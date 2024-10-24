#ifndef STRUCTSCOMMENT_H
#define STRUCTSCOMMENT_H

#include <string>

namespace StructsComment
{
struct Comentario
{
    std::string fecha_hora;
    std::string usuario;
    std::string texto;

    // constructor por defecto
    Comentario() {
        fecha_hora = "";
        usuario = "";
        texto = "";
    };

    // constructor con parámetros, usando lista de inicialización
    Comentario(const std::string& fecha_hora, const std::string& usuario, const std::string& texto)
        : fecha_hora(fecha_hora), usuario(usuario), texto(texto) {}

    // destructor por defecto
    ~Comentario() = default;
};

}

#endif // STRUCTSCOMMENT_H
