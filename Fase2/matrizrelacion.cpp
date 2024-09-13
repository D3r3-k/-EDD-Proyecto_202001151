#include "matrizrelacion.h"
#include <QDebug>
#include <QString>
#include <QMessageBox>
#include <iomanip>
#include <fstream>
#include <cstdlib> // Para system()

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

void MatrizRelacion::graficar()
{
    std::string dotPath = "matriz_relacion.dot";
    std::string imagePath = "matriz_relacion.png";

    // Crear el archivo .dot
    std::ofstream archivoDot(dotPath);

    if (!archivoDot.is_open())
    {
        QMessageBox::information(nullptr, "Error", "No se pudo abrir el archivo para escribir.");
        return;
    }

    // Escribir el encabezado del archivo
    archivoDot << "digraph G {" << std::endl;
    archivoDot << "node [shape=box];" << std::endl;
    archivoDot << "rankdir=TB;" << std::endl;
    archivoDot << "label=\"Matriz de Relaciones de Usuarios\" fontsize=20 fontname=\"Arial\";" << std::endl;

    // Definir encabezados de filas
    archivoDot << "// Definir encabezados de filas" << std::endl;
    Nodo *fila = cabeza->abajo;
    while (fila)
    {
        archivoDot << "fila" << fila->usuario1->id << " [label=\"" << fila->usuario1->nombres << "\", group=1];" << std::endl;
        fila = fila->abajo;
    }

    // Definir encabezados de columnas
    archivoDot << "// Definir encabezados de columnas" << std::endl;
    Nodo *columna = cabeza->siguiente;
    while (columna)
    {
        archivoDot << "col" << columna->usuario2->id << " [label=\"" << columna->usuario2->nombres << "\", group=" << columna->usuario2->id + 1 << "];" << std::endl;
        columna = columna->siguiente;
    }

    // Mantener los encabezados de las columnas en el mismo rango horizontal
    archivoDot << "{ rank=same;";
    columna = cabeza->siguiente;
    while (columna)
    {
        archivoDot << " col" << columna->usuario2->id << ";";
        columna = columna->siguiente;
    }
    archivoDot << " }" << std::endl;

    // Definir nodos en las coordenadas especificadas
    archivoDot << "// Definir nodos en las coordenadas especificadas" << std::endl;
    fila = cabeza->abajo;
    while (fila)
    {
        Nodo *current = fila->siguiente;
        while (current)
        {
            archivoDot << "nodo" << current->usuario1->id << "_" << current->usuario2->id
                       << " [label=\"\", color=green, style=filled, group=" << current->usuario2->id + 1 << "];" << std::endl;
            current = current->siguiente;
        }
        fila = fila->abajo;
    }

    // Conectar encabezados de filas con nodos y los nodos entre ellos horizontalmente
    archivoDot << "// Conectar encabezados de filas con nodos y los nodos entre ellos horizontalmente" << std::endl;
    fila = cabeza->abajo;
    while (fila)
    {
        Nodo *current = fila->siguiente;
        Nodo *prev = nullptr;
        if (current)
        {
            // Conectar el encabezado de la fila con el primer nodo de la fila
            archivoDot << "fila" << fila->usuario1->id << " -> nodo" << current->usuario1->id << "_" << current->usuario2->id
                       << " [dir=both, minlen=2];" << std::endl;

            // Conectar los nodos horizontalmente
            prev = current;
            current = current->siguiente;
            while (current)
            {
                archivoDot << "nodo" << prev->usuario1->id << "_" << prev->usuario2->id
                           << " -> nodo" << current->usuario1->id << "_" << current->usuario2->id
                           << " [dir=both, minlen=2];" << std::endl;
                prev = current;
                current = current->siguiente;
            }
        }
        fila = fila->abajo;
    }

    // Conectar encabezados de columnas con nodos y los nodos entre ellos verticalmente
    archivoDot << "// Conectar encabezados de columnas con nodos y los nodos entre ellos verticalmente" << std::endl;
    columna = cabeza->siguiente;
    while (columna)
    {
        Nodo *current = columna->abajo;
        Nodo *prev = nullptr;
        if (current)
        {
            // Conectar el encabezado de la columna con el primer nodo de la columna
            archivoDot << "col" << columna->usuario2->id << " -> nodo" << current->usuario1->id << "_" << current->usuario2->id
                       << " [dir=both, minlen=2];" << std::endl;

            // Conectar los nodos verticalmente
            prev = current;
            current = current->abajo;
            while (current)
            {
                archivoDot << "nodo" << prev->usuario1->id << "_" << prev->usuario2->id
                           << " -> nodo" << current->usuario1->id << "_" << current->usuario2->id
                           << " [dir=both, minlen=2];" << std::endl;
                prev = current;
                current = current->abajo;
            }
        }
        columna = columna->siguiente;
    }

    // Mantener los encabezados de filas alineados verticalmente
    archivoDot << "// Mantener los encabezados de filas alineados verticalmente" << std::endl;
    fila = cabeza->abajo;
    while (fila && fila->abajo)
    {
        archivoDot << "fila" << fila->usuario1->id << " -> fila" << fila->abajo->usuario1->id
                   << " [style=invis];" << std::endl;
        fila = fila->abajo;
    }

    // Mantener los nodos de la misma fila en el mismo rango horizontal
    archivoDot << "// Mantener los nodos de la misma fila en el mismo rango horizontal" << std::endl;
    fila = cabeza->abajo;
    while (fila)
    {
        Nodo *current = fila->siguiente;
        while (current)
        {
            archivoDot << "{ rank=same; fila" << fila->usuario1->id << "; nodo" << current->usuario1->id
                       << "_" << current->usuario2->id << " }" << std::endl;
            current = current->siguiente;
        }
        fila = fila->abajo;
    }

    // Conectar las columnas entre sí de forma invisible para mantener el orden
    archivoDot << "// Conectar las columnas entre sí de forma invisible para mantener el orden" << std::endl;
    columna = cabeza->siguiente;
    while (columna && columna->siguiente)
    {
        archivoDot << "col" << columna->usuario2->id << " -> col" << columna->siguiente->usuario2->id
                   << " [style=invis];" << std::endl;
        columna = columna->siguiente;
    }

    // Cerrar el archivo
    archivoDot << "}" << std::endl;
    archivoDot.close();

    // Generar la imagen con el comando dot
    std::string cmd = "dot -Tpng " + dotPath + " -o " + imagePath;
    system(cmd.c_str());
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
