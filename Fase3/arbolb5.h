#ifndef ARBOLB5_H
#define ARBOLB5_H

#include "ListaEnlazada.h"
#include "StructsComment.h"
#include <string>

// Arbol de orden 5
#define MAX 4
#define MIN 2

struct Comentario
{
    std::string fecha_hora;
    std::string usuario;
    std::string texto;

    // constructor por defecto
    Comentario() {}
    // constructor
    Comentario(std::string fecha_hora, std::string usuario, std::string texto)
    {
        this->fecha_hora = fecha_hora;
        this->usuario = usuario;
        this->texto = texto;
    }
    // destructor
    ~Comentario() {}
};
class ArbolB5
{
private:
    class Nodo
    {
    public:
        StructsComment::Comentario valor[MAX + 1];
        Nodo *link[MAX + 1];
        int numero = 0;
    };
    // Métodos privados
    int setValor(StructsComment::Comentario valor, StructsComment::Comentario *pval, Nodo *nodo, Nodo **hijo);
    void insertarNodo(StructsComment::Comentario valor, int pos, Nodo *nodo, Nodo *hijo);
    void dividirNodo(StructsComment::Comentario valor, StructsComment::Comentario *pval, int pos, Nodo *nodo, Nodo *hijo, Nodo **nuevo);
    Nodo *crearNodo(StructsComment::Comentario valor, Nodo *hijo);
    void transversalNodo(Nodo *nodo);
    void graficarNodo(Nodo *nodo, std::string &dot);
    int sizeNodo(Nodo *nodo);
    void obtenerComentariosNodo(Nodo *nodo, ListaEnlazada::ListaEnlazada<StructsComment::Comentario> &lista);

public:
    Nodo *raiz;
    // Constructor
    ArbolB5() : raiz(nullptr) {}
    // Destructor
    ~ArbolB5();
    // Métodos públicos
    void insertar(StructsComment::Comentario valor);
    std::string graficar();
    void transversal(Nodo *nodo);
    int size();
    ListaEnlazada::ListaEnlazada<StructsComment::Comentario> obtenerComentarios();
    std::string toJSON();
};

#endif // ARBOLB5_H
