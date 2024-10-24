#include "graforelacion.h"
#include <iostream>
#include <QString>
#include <QMessageBox>
#include <fstream>
#include <cstdlib> // Para system()

// TODO: Metodos privados
void GrafoRelacion::insertarNodo(Structs::Usuario &usuario)
{
    if (buscarNodo(usuario.correo) != nullptr)
    {
        return;
    }
    Nodo *nuevoNodo = new Nodo(&usuario);
    if (cabeza == nullptr)
    {
        cabeza = nuevoNodo;
        cola = nuevoNodo;
    }
    else
    {
        cola->siguiente = nuevoNodo;
        cola = nuevoNodo;
    }
}

GrafoRelacion::Nodo *GrafoRelacion::buscarNodo(const std::string &correo) const
{
    Nodo *temp = cabeza;

    while (temp != nullptr)
    {
        if (temp->usuario->correo == correo)
        {
            return temp;
        }
        temp = temp->siguiente;
    }
    return nullptr;
}

// TODO: Metodos publicos
void GrafoRelacion::agregarRelacion(Structs::Usuario &user1, Structs::Usuario &user2)
{
    insertarNodo(user1);
    insertarNodo(user2);

    Nodo *nodo1 = buscarNodo(user1.correo);
    Nodo *nodo2 = buscarNodo(user2.correo);

    if (nodo1 == nullptr || nodo2 == nullptr)
    {
        return;
    }

    bool existeRelacion1 = false;
    bool existeRelacion2 = false;

    for (int i = 0; i < nodo1->amigos.size(); ++i)
    {
        Structs::Usuario *amigo = nodo1->amigos.obtener(i);
        if (amigo->correo == user2.correo)
        {
            existeRelacion1 = true;
            break;
        }
    }

    for (int i = 0; i < nodo2->amigos.size(); ++i)
    {
        Structs::Usuario *amigo = nodo2->amigos.obtener(i);
        if (amigo->correo == user1.correo)
        {
            existeRelacion2 = true;
            break;
        }
    }

    if (!existeRelacion1)
    {
        nodo1->amigos.insertar(user2);
    }
    if (!existeRelacion2)
    {
        nodo2->amigos.insertar(user1);
    }
}

void GrafoRelacion::eliminarRelacion(const std::string &correo1, const std::string &correo2)
{
    Nodo *nodo1 = buscarNodo(correo1);
    Nodo *nodo2 = buscarNodo(correo2);

    if (nodo1 == nullptr || nodo2 == nullptr)
    {
        return;
    }

    for (int i = 0; i < nodo1->amigos.size(); ++i)
    {
        Structs::Usuario *amigo = nodo1->amigos.obtener(i);
        if (amigo->correo == correo2)
        {
            nodo1->amigos.eliminar(i);
            break;
        }
    }

    for (int i = 0; i < nodo2->amigos.size(); ++i)
    {
        Structs::Usuario *amigo = nodo2->amigos.obtener(i);
        if (amigo->correo == correo1)
        {
            nodo2->amigos.eliminar(i);
            break;
        }
    }
}

void GrafoRelacion::eliminarRelacionesUsuario(const std::string &correo)
{
    Nodo *nodo = buscarNodo(correo);

    if (nodo == nullptr)
    {
        return;
    }

    Nodo *temp = cabeza;
    while (temp)
    {
        Nodo *usuario = temp;
        for (int i = 0; i < usuario->amigos.size(); ++i)
        {
            Structs::Usuario *amigo = usuario->amigos.obtener(i);
            if (amigo->correo == correo)
            {
                usuario->amigos.eliminar(i);
                break;
            }
        }
        temp = temp->siguiente;
    }

    nodo->amigos.limpiar();

    Nodo *anterior = nullptr;
    temp = cabeza;
}

ListaEnlazada::ListaEnlazada<Structs::Usuario> GrafoRelacion::obtenerAmigos(const std::string &correo)
{
    Nodo *nodo = buscarNodo(correo);

    if (nodo == nullptr)
    {
        return ListaEnlazada::ListaEnlazada<Structs::Usuario>();
    }

    return nodo->amigos;
}

ListaEnlazada::ListaEnlazada<Structs::AmigoSugerido> GrafoRelacion::sugerirAmigos(const std::string &correo)
{
    ListaEnlazada::ListaEnlazada<Structs::AmigoSugerido> amigos_sugeridos;
    Nodo *nodo = buscarNodo(correo);
    if (nodo == nullptr)
    {
        return amigos_sugeridos;
    }
    ListaEnlazada::ListaEnlazada<Structs::Usuario> temp;
    ListaEnlazada::ListaEnlazada<Structs::Usuario> amigos = nodo->amigos;

    for (int i = 0; i < amigos.size(); i++)
    {
        Structs::Usuario *amigo = amigos.obtener(i);
        Nodo *nodo_amigo = buscarNodo(amigo->correo);

        if (nodo_amigo == nullptr)
        {
            continue;
        }
        ListaEnlazada::ListaEnlazada<Structs::Usuario> amigos_amigo = nodo_amigo->amigos;
        for (int i = 0; i < amigos_amigo.size(); ++i)
        {
            Structs::Usuario amigo_am = *amigos_amigo.obtener(i);
            if (amigo_am.correo == correo)
            {
                continue;
            }
            temp.insertar(amigo_am);
        }
    }
    for (int i = 0; i < temp.size(); i++)
    {
        Structs::Usuario *amigo_amigo = temp.obtener(i);

        if (!verificarRelacion(correo, amigo_amigo->correo))
        {
            bool existe = false;
            for (int j = 0; j < amigos_sugeridos.size(); j++)
            {
                Structs::AmigoSugerido *sugerido = amigos_sugeridos.obtener(j);
                if (sugerido->usuario->correo == amigo_amigo->correo)
                {
                    sugerido->agregarEnComun();
                    existe = true;
                    break;
                }
            }
            if (!existe)
            {
                Structs::AmigoSugerido nuevo(amigo_amigo, 1);
                amigos_sugeridos.insertar(nuevo);
            }
        }
    }

    return amigos_sugeridos;
}

bool GrafoRelacion::verificarRelacion(const std::string &correo1, const std::string &correo2)
{
    Nodo *nodo1 = buscarNodo(correo1);
    Nodo *nodo2 = buscarNodo(correo2);

    if (nodo1 == nullptr || nodo2 == nullptr)
    {
        return false;
    }

    for (int i = 0; i < nodo1->amigos.size(); ++i)
    {
        Structs::Usuario *amigo = nodo1->amigos.obtener(i);
        if (amigo->correo == correo2)
        {
            return true;
        }
    }

    return false;
}

bool GrafoRelacion::estaVacio() const
{
    return cabeza == nullptr;
}

std::string GrafoRelacion::graficarGrafo()
{
    // if (!cabeza)
    // {
    //     QMessageBox::warning(nullptr, "Error", "No hay relaciones registradas.");
    //     return "";
    // }
    std::string dotPath = "renders/grafo_relacion.dot";
    std::string imagePath = "renders/grafo_relacion.png";

    std::ofstream archivoDot(dotPath);
    if (!archivoDot.is_open())
    {
        QMessageBox::warning(nullptr, "Error", "No se pudo abrir el archivo .dot");
        return "";
    }

    archivoDot << "digraph G {" << std::endl;
    archivoDot << "node [shape=circle];" << std::endl;
    archivoDot << "rankdir=LR;" << std::endl;
    archivoDot << "edge [dir=none];" << std::endl;
    archivoDot << "label=\"Grafo Relaciones\" fontsize = 20 fontname = \"Arial\";";

    Nodo *temp = cabeza;
    while (temp)
    {
        Nodo *usuario = temp;
        archivoDot << "\"" << usuario->usuario->correo << "\" [label=\"" << usuario->usuario->nombres << "\"];" << std::endl;
        temp = temp->siguiente;
    }

    ListaEnlazada::ListaEnlazada<std::string> relaciones_procesadas;
    temp = cabeza;
    while (temp)
    {
        Nodo *usuario = temp;
        for (int i = 0; i < usuario->amigos.size(); ++i)
        {
            Structs::Usuario *amigo = usuario->amigos.obtener(i);

            std::string relacion = "\"" + usuario->usuario->correo + "\" -> \"" + amigo->correo + "\"";
            std::string relacion_inversa = "\"" + amigo->correo + "\" -> \"" + usuario->usuario->correo + "\"";

            bool existe_relacion = false;
            for (int j = 0; j < relaciones_procesadas.size(); ++j)
            {
                std::string *relacion_existente = relaciones_procesadas.obtener(j);
                if (*relacion_existente == relacion || *relacion_existente == relacion_inversa)
                {
                    existe_relacion = true;
                    break;
                }
            }

            if (existe_relacion)
            {
                continue;
            }

            archivoDot << "\"" << usuario->usuario->correo << "\" -> \"" << amigo->correo << "\";" << std::endl;

            relaciones_procesadas.insertar(relacion);
            relaciones_procesadas.insertar(relacion_inversa);
        }
        temp = temp->siguiente;
    }

    archivoDot << "}" << std::endl;
    archivoDot.close();

    std::string cmd = "dot -Tpng " + dotPath + " -o " + imagePath;
    system(cmd.c_str());

    return imagePath;
}

std::string GrafoRelacion::graficarGrafo(const std::string &correo)
{
    // if (!cabeza)
    // {
    //     QMessageBox::warning(nullptr, "Error", "No hay relaciones registradas.");
    //     return "";
    // }
    std::string dotPath = "grafo_relacion.dot";
    std::string imagePath = "grafo_relacion.png";

    std::ofstream archivoDot(dotPath);
    if (!archivoDot.is_open())
    {
        QMessageBox::warning(nullptr, "Error", "No se pudo abrir el archivo .dot");
        return "";
    }

    archivoDot << "digraph G {" << std::endl;
    archivoDot << "node [shape=circle];" << std::endl;
    archivoDot << "rankdir=LR;" << std::endl;
    archivoDot << "edge [dir=none];" << std::endl;
    archivoDot << "label=\"Grafo Relaciones\" fontsize = 20 fontname = \"Arial\";";

    Nodo *temp = cabeza;
    while (temp)
    {
        Nodo *usuario = temp;
        if (correo == usuario->usuario->correo)
        {
            archivoDot << usuario->usuario->correo << " [label=\"" << usuario->usuario->nombres << "\"; fillcolor=\"#41c1da\"; style=\"filled\"];" << std::endl;
        }
        else
        {
            archivoDot << usuario->usuario->correo << " [label=\"" << usuario->usuario->nombres << "\"];" << std::endl;
        }
        temp = temp->siguiente;
    }

    ListaEnlazada::ListaEnlazada<std::string> relaciones_procesadas;
    temp = cabeza;
    while (temp)
    {
        Nodo *usuario = temp;
        for (int i = 0; i < usuario->amigos.size(); ++i)
        {
            Structs::Usuario *amigo = usuario->amigos.obtener(i);

            std::string relacion = usuario->usuario->correo + " -> " + amigo->correo;
            std::string relacion_inversa = amigo->correo + " -> " + usuario->usuario->correo;

            bool existe_relacion = false;
            for (int j = 0; j < relaciones_procesadas.size(); ++j)
            {
                std::string *relacion_existente = relaciones_procesadas.obtener(j);
                if (*relacion_existente == relacion || *relacion_existente == relacion_inversa)
                {
                    existe_relacion = true;
                    break;
                }
            }

            if (existe_relacion)
            {
                continue;
            }

            archivoDot << usuario->usuario->correo << " -> " << amigo->correo << ";" << std::endl;

            relaciones_procesadas.insertar(relacion);
            relaciones_procesadas.insertar(relacion_inversa);
        }
        temp = temp->siguiente;
    }

    archivoDot << "}" << std::endl;
    archivoDot.close();

    std::string cmd = "dot -Tpng " + dotPath + " -o " + imagePath;
    system(cmd.c_str());

    return imagePath;
}

std::string GrafoRelacion::graficarGrafoSugeridos(const std::string &correo)
{
    // if (!cabeza)
    // {
    //     QMessageBox::warning(nullptr, "Error", "No hay relaciones registradas.");
    //     return "";
    // }
    std::string dotPath = "renders/grafo_relacion_sugeridos.dot";
    std::string imagePath = "renders/grafo_relacion_sugeridos.png";

    std::ofstream archivoDot(dotPath);
    if (!archivoDot.is_open())
    {
        QMessageBox::warning(nullptr, "Error", "No se pudo abrir el archivo .dot");
        return "";
    }

    archivoDot << "digraph G {" << std::endl;
    archivoDot << "node [shape=circle];" << std::endl;
    archivoDot << "rankdir=LR;" << std::endl;
    archivoDot << "edge [dir=none];" << std::endl;
    archivoDot << "label=\"Grafo Relacion de Sugeridos\" fontsize = 20 fontname = \"Arial\";" << std::endl;

    Nodo *usuario = buscarNodo(correo);
    ListaEnlazada::ListaEnlazada<Structs::AmigoSugerido> amigos_sugeridos = sugerirAmigos(correo);
    ListaEnlazada::ListaEnlazada<Structs::Usuario> amigos = usuario->amigos;
    Nodo *temp = cabeza;
    while (temp)
    {
        Nodo *amigo = temp;
        if (correo == amigo->usuario->correo)
        {
            archivoDot <<"\"" << amigo->usuario->correo << "\" [label=\"" << amigo->usuario->nombres << "\"; fillcolor=\"#41c1da\"; style=\"filled\"];" << std::endl;
        }
        else
        {
            bool esAmigo = false;
            bool esSugerido = false;
            for (int i = 0; i < amigos.size(); i++)
            {
                Structs::Usuario *amigo_usuario = amigos.obtener(i);
                if (amigo_usuario->correo == amigo->usuario->correo)
                {
                    esAmigo = true;
                    break;
                }
            }
            for (int i = 0; i < amigos_sugeridos.size(); i++)
            {
                Structs::AmigoSugerido *amigo_sugerido = amigos_sugeridos.obtener(i);
                if (amigo_sugerido->usuario->correo == amigo->usuario->correo)
                {
                    esSugerido = true;
                    break;
                }
            }
            if (esAmigo)
            {
                archivoDot << "\""<<amigo->usuario->correo << "\" [label=\"" << amigo->usuario->nombres << "\"; fillcolor=\"#f4d03f\"; style=\"filled\"];" << std::endl;
            }
            else if (esSugerido)
            {
                archivoDot << "\""<<amigo->usuario->correo << "\" [label=\"" << amigo->usuario->nombres << "\"; fillcolor=\"#7DDA58\"; style=\"filled\"];" << std::endl;
            }
            else
            {
                archivoDot << "\""<<amigo->usuario->correo << "\" [label=\"" << amigo->usuario->nombres << "\"];" << std::endl;
            }
        }
        temp = temp->siguiente;
    }

    ListaEnlazada::ListaEnlazada<std::string> relaciones_procesadas;
    temp = cabeza;
    while (temp)
    {
        Nodo *usuario = temp;
        for (int i = 0; i < usuario->amigos.size(); ++i)
        {
            Structs::Usuario *amigo = usuario->amigos.obtener(i);

            std::string relacion = "\"" + usuario->usuario->correo + "\" -> \"" + amigo->correo + "\"";
            std::string relacion_inversa = "\"" + amigo->correo + "\" -> \"" + usuario->usuario->correo + "\"";

            bool existe_relacion = false;
            for (int j = 0; j < relaciones_procesadas.size(); ++j)
            {
                std::string *relacion_existente = relaciones_procesadas.obtener(j);
                if (*relacion_existente == relacion || *relacion_existente == relacion_inversa)
                {
                    existe_relacion = true;
                    break;
                }
            }

            if (existe_relacion)
            {
                continue;
            }

            archivoDot << "\"" + usuario->usuario->correo << "\" -> \"" << amigo->correo << "\";" << std::endl;

            relaciones_procesadas.insertar(relacion);
            relaciones_procesadas.insertar(relacion_inversa);
        }
        temp = temp->siguiente;
    }

    archivoDot << "}" << std::endl;
    archivoDot.close();

    std::string cmd = "dot -Tpng " + dotPath + " -o " + imagePath;
    system(cmd.c_str());

    return imagePath;
}

std::string GrafoRelacion::tablaAdyacencia()
{

    // if (!cabeza)
    // {
    //     QMessageBox::warning(nullptr, "Error", "No hay relaciones registradas.");
    //     return "";
    // }
    std::string dotPath = "renders/tabla_adyacencia.dot";
    std::string imagePath = "renders/tabla_adyacencia.png";

    std::ofstream archivoDot(dotPath);
    if (!archivoDot.is_open())
    {
        QMessageBox::warning(nullptr, "Error", "No se pudo abrir el archivo .dot");
        return "";
    }

    archivoDot << "digraph G {" << std::endl;
    archivoDot << "rankdir=LR;" << std::endl;
    archivoDot << "node [shape=record];" << std::endl;
    archivoDot << "label=\"Tabla de Adyacencia\" fontsize = 20 fontname = \"Arial\";";

    Nodo *temp = cabeza;
    while (temp)
    {
        Nodo *usuario = temp;
        archivoDot << "\"list_" << usuario->usuario->correo << "\" [label=\"{" << usuario->usuario->nombres << " | }\"];" << std::endl;
        temp = temp->siguiente;
    }

    temp = cabeza;
    while (temp)
    {
        Nodo *usuario = temp;
        for (int i = 0; i < usuario->amigos.size(); ++i)
        {
            Structs::Usuario *amigo = usuario->amigos.obtener(i);
            archivoDot << "\"sublist_" << usuario->usuario->correo << "_" << amigo->correo << "\" [label=\"{" << amigo->nombres << " | }\"];" << std::endl;
        }
        temp = temp->siguiente;
    }

    std::string sublista_actual = "";
    std::string sublista_siguiente = "";
    temp = cabeza;
    while (temp)
    {
        Nodo *usuario = temp;
        sublista_actual = "\"list_" + usuario->usuario->correo + "\"";
        for (int i = 0; i < usuario->amigos.size(); ++i)
        {
            Structs::Usuario *amigo = usuario->amigos.obtener(i);
            sublista_siguiente = "\"sublist_" + usuario->usuario->correo + "_" + amigo->correo + "\"";
            archivoDot << sublista_actual << " -> " << sublista_siguiente << ";" << std::endl;
            sublista_actual = sublista_siguiente;
        }
        temp = temp->siguiente;
    }

    archivoDot << "{ rank=same; Usuarios";
    temp = cabeza;
    while (temp)
    {
        Nodo *usuario = temp;
        archivoDot << "\"list_" << usuario->usuario->correo << "\";";
        temp = temp->siguiente;
    }
    archivoDot << " }" << std::endl;

    archivoDot << "}" << std::endl;
    archivoDot.close();

    std::string cmd = "dot -Tpng " + dotPath + " -o " + imagePath;
    system(cmd.c_str());

    return imagePath;
}
