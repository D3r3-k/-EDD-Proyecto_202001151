#include "matrizrelacion.h"
#include <QDebug>
#include <QString>
#include <iomanip>

// Destructor
MatrizRelacion::~MatrizRelacion() {
    Nodo *aux = cabeza;
    while (aux != nullptr)
    {
        Nodo *aux2 = aux;
        aux = aux->abajo;
        while (aux2 != nullptr)
        {
            Nodo *aux3 = aux2;
            aux2 = aux2->siguiente;
            delete aux3;
        }
    }
}

void MatrizRelacion::agregarRelacion(Structs::Usuario *user1, Structs::Usuario *user2)
{
    if (!cabeza)
    {
        cabeza = new Nodo(nullptr, nullptr);
    }
    Nodo*fila1 = buscarFila(user1);
    Nodo *columna1 = buscarColumna(user2);

    Nodo *fila2 = buscarFila(user2);
    Nodo *columna2 = buscarColumna(user1);

    if (!fila1)
    {
        fila1 = insertarFilaCabecera(user1);
    }
    if (!columna1)
    {
        columna1 = insertarColumnaCabecera(user2);
    }

    if (!fila2)
    {
        fila2 = insertarFilaCabecera(user2);
    }
    if (!columna2)
    {
        columna2 = insertarColumnaCabecera(user1);
    }

    if (!NodoRelacionExiste(fila1, user2))
    {
        Nodo *nuevo1 = new Nodo(user1, user2);
        insertarEnFila(nuevo1, fila1);
        insertarEnColumna(nuevo1, columna1);
    }

    if (!NodoRelacionExiste(fila2, user1))
    {
        Nodo *nuevo2 = new Nodo(user2, user1);
        insertarEnFila(nuevo2, fila2);
        insertarEnColumna(nuevo2, columna2);
    }
}

MatrizRelacion::Nodo* MatrizRelacion::buscarFila(Structs::Usuario *u1)
{
    Nodo*current = cabeza->abajo;
    while (current)
    {
        if (current->usuario1 == u1)
        {
            return current;
        }
        current = current->abajo;
    }
    return nullptr;
}

MatrizRelacion::Nodo*MatrizRelacion::buscarColumna(Structs::Usuario *u2)
{
    Nodo *current = cabeza->siguiente;
    while (current)
    {
        if (current->usuario2 == u2)
        {
            return current;
        }
        current = current->siguiente;
    }
    return nullptr;
}


bool MatrizRelacion::NodoRelacionExiste(Nodo*fila, Structs::Usuario *u2)
{
    Nodo*current = fila->siguiente;
    while (current)
    {
        if (current->usuario2 == u2)
        {
            return true;
        }
        current = current->siguiente;
    }
    return false;
}

MatrizRelacion::Nodo *MatrizRelacion::insertarFilaCabecera(Structs::Usuario *user)
{
    Nodo*newRowHeader = new Nodo(user, nullptr);

    if (!cabeza->abajo)
    {
        cabeza->abajo = newRowHeader;
    }
    else
    {
        Nodo*current = cabeza;
        while (current->abajo && current->abajo->usuario1->id < user->id)
        {
            current = current->abajo;
        }
        newRowHeader->abajo = current->abajo;
        if (current->abajo)
        {
            current->abajo->arriba = newRowHeader;
        }
        current->abajo = newRowHeader;
        newRowHeader->arriba = current;
    }

    return newRowHeader;
}

MatrizRelacion::Nodo *MatrizRelacion::insertarColumnaCabecera(Structs::Usuario *user)
{
    Nodo *newColumnHeader = new Nodo(nullptr, user);

    if (!cabeza->siguiente)
    {
        cabeza->siguiente = newColumnHeader;
    }
    else
    {
        Nodo *current = cabeza;
        while (current->siguiente && current->siguiente->usuario2->id < user->id)
        {
            current = current->siguiente;
        }
        newColumnHeader->siguiente = current->siguiente;
        if (current->siguiente)
        {
            current->siguiente->anterior = newColumnHeader;
        }
        current->siguiente = newColumnHeader;
        newColumnHeader->anterior = current;
    }

    return newColumnHeader;
}

void MatrizRelacion::insertarEnFila(Nodo*nuevo, Nodo*filaCabecera)
{
    Nodo*current = filaCabecera;
    while (current->siguiente)
    {
        if (nuevo->usuario2->id < current->siguiente->usuario2->id)
        {
            break;
        }
        current = current->siguiente;
    }
    nuevo->siguiente = current->siguiente;
    if (current->siguiente)
    {
        current->siguiente->anterior = nuevo;
    }
    current->siguiente = nuevo;
    nuevo->anterior = current;
}

void MatrizRelacion::insertarEnColumna(Nodo*nuevo, Nodo*columnaCabecera)
{
    Nodo*current = columnaCabecera;
    while (current->abajo)
    {
        if (nuevo->usuario1->id < current->abajo->usuario1->id)
        {
            break;
        }
        current = current->abajo;
    }
    nuevo->abajo = current->abajo;
    if (current->abajo)
    {
        current->abajo->arriba = nuevo;
    }
    current->abajo = nuevo;
    nuevo->arriba = current;
}


// Función auxiliar para imprimir con formato
QString formatWithWidth(const std::string& str, int width) {
    std::stringstream ss;
    ss << std::setw(width) << str;
    return QString::fromStdString(ss.str());
}

// Función para imprimir la matriz con formato
void MatrizRelacion::imprimir(){
    // Verificar si la matriz tiene elementos
    if (!cabeza) {
        qDebug() << "La matriz de relaciones está vacía.";
        return;
    }

    qDebug() << "Imprimiendo la matriz de relaciones:";

    // Imprimir encabezado de columnas
    QString header = formatWithWidth(" ", 10); // Espacio para encabezado de fila
    Nodo *columna = cabeza->siguiente;
    while (columna) {
        header += formatWithWidth(QString::fromStdString(columna->usuario2->nombres).toStdString(), 10);
        columna = columna->siguiente;
    }
    qDebug() << header;

    // Imprimir filas
    Nodo *fila = cabeza->abajo;
    while (fila) {
        QString row = formatWithWidth(QString::fromStdString(fila->usuario1->nombres).toStdString(), 10);

        Nodo *current = fila->siguiente;
        while (current) {
            // Aquí podrías colocar un carácter específico si hay una relación o un espacio si no
            row += formatWithWidth("\tX\t", 10); // Usamos "X" como ejemplo para indicar una relación
            current = current->siguiente;
        }
        qDebug() << row;
        fila = fila->abajo;
    }
}

void MatrizRelacion::eliminarRelacionesUsuario(string correo_usuario)
{
    if (!cabeza)
    {
        return;
    }
    // borrando todas las relaciones que tengan relacion con el correo del usuario tanto en filas como en columnas
    Nodo *fila = cabeza->abajo;
    while (fila)
    {
        Nodo *temp = fila;
        fila = fila->abajo;
        if (temp->usuario1->correo == correo_usuario)
        {
            if (temp->arriba)
            {
                temp->arriba->abajo = temp->abajo;
            }
            if (temp->abajo)
            {
                temp->abajo->arriba = temp->arriba;
            }
            if (temp == cabeza->abajo)
            {
                cabeza->abajo = temp->abajo;
            }
            delete temp;
        }
    }

    Nodo *columna = cabeza->siguiente;
    while (columna)
    {
        Nodo *temp = columna;
        columna = columna->siguiente;
        if (temp->usuario2->correo == correo_usuario)
        {
            if (temp->anterior)
            {
                temp->anterior->siguiente = temp->siguiente;
            }
            if (temp->siguiente)
            {
                temp->siguiente->anterior = temp->anterior;
            }
            if (temp == cabeza->siguiente)
            {
                cabeza->siguiente = temp->siguiente;
            }
            delete temp;
        }
    }
}

bool MatrizRelacion::verificarRelacion(string correo1, string correo2)
{
    if (!cabeza)
    {
        return false;
    }
    Nodo *fila = cabeza->abajo;
    while (fila)
    {
        Nodo *current = fila->siguiente;
        while (current)
        {
            if (current->usuario1->correo == correo1 && current->usuario2->correo == correo2)
            {
                return true;
            }
            current = current->siguiente;
        }
        fila = fila->abajo;
    }
    return false;
}

// Función para obtener la lista de amigos de un usuario
ListaEnlazada::ListaEnlazada<Structs::Usuario> MatrizRelacion::obtenerAmigos(string correo)
{
    ListaEnlazada::ListaEnlazada<Structs::Usuario> amigos;
    if (!cabeza)
    {
        return amigos;
    }
    Nodo *fila = cabeza->abajo;
    while (fila)
    {
        if (fila->usuario1->correo == correo)
        {
            Nodo *current = fila->siguiente;
            while (current)
            {
                amigos.insertar(*current->usuario2);
                current = current->siguiente;
            }
            break;
        }
        fila = fila->abajo;
    }
    return amigos;
}