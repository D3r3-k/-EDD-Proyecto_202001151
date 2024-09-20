#include "arbolb5.h"
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>

using namespace std;

// Métodos públicos
void ArbolB5::insertar(StructsComment::Comentario val)
{
    StructsComment::Comentario i;
    Nodo *child = nullptr;
    if (setValor(val, &i, raiz, &child))
        raiz = crearNodo(i, child);
}

int ArbolB5::size()
{
    return sizeNodo(raiz);
}

std::string ArbolB5::graficar()
{
    try
    {
        string dot = "digraph G {\n";
        dot += "node [shape=record];\n";
        dot += "rankdir=TB;\n";
        graficarNodo(raiz, dot);
        dot += "}\n";
        // Crear un archivo .dot y convertirlo a .png
        std::string path = "arbolb5.dot";
        ofstream file(path);
        file << dot;
        file.close();
        system("dot -Tpng arbolb5.dot -o arbolb5.png");
        return path;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

void ArbolB5::transversal(Nodo *nodo)
{
    transversalNodo(nodo);
}

ListaEnlazada::ListaEnlazada<StructsComment::Comentario> ArbolB5::obtenerComentarios()
{

    ListaEnlazada::ListaEnlazada<StructsComment::Comentario> lista;
    obtenerComentariosNodo(raiz, lista);
    return lista;
}

// Métodos privados
int ArbolB5::setValor(StructsComment::Comentario val, StructsComment::Comentario *pval, Nodo *node, Nodo **child)
{
    int pos;

    // Si no hay nodo, crea un nuevo valor y no hay hijos
    if (!node)
    {
        *pval = val;
        *child = nullptr;
        return 1;
    }

    // Determina la posición para insertar el nuevo valor en el nodo
    if (val.fecha_hora < node->valor[1].fecha_hora)
    {
        pos = 0;
    }
    else
    {
        for (pos = node->numero;
             (val.fecha_hora < node->valor[pos].fecha_hora && pos > 1); pos--);

        // Revisa si el valor ya existe, no permitimos duplicados
        if (val.fecha_hora == node->valor[pos].fecha_hora)
        {
            cout << "No se admiten duplicados" << endl;
            return 0;
        }
    }

    // Llama recursivamente para ubicar la posición adecuada en los subárboles
    if (setValor(val, pval, node->link[pos], child))
    {
        // Si hay espacio en el nodo, inserta el valor
        if (node->numero < MAX)
        {
            insertarNodo(*pval, pos, node, *child);
        }
        // Si el nodo está lleno, divídelo
        else
        {
            dividirNodo(*pval, pval, pos, node, *child, child);
            return 1;
        }
    }

    return 0;
}


ArbolB5::Nodo *ArbolB5::crearNodo(StructsComment::Comentario val, Nodo *child)
{
    Nodo *newNode = new Nodo();
    newNode->valor[1] = val;
    newNode->numero = 1;
    newNode->link[0] = raiz;
    newNode->link[1] = child;
    return newNode;
}

void ArbolB5::insertarNodo(StructsComment::Comentario val, int pos, Nodo *node, Nodo *child)
{
    int j = node->numero;

    // Desplaza los valores y enlaces hacia la derecha para hacer espacio
    while (j > pos)
    {
        node->valor[j + 1] = node->valor[j];
        node->link[j + 1] = node->link[j];
        j--;
    }

    // Inserta el nuevo valor y enlace en su posición correcta
    node->valor[j + 1] = val;
    node->link[j + 1] = child;
    node->numero++;
}


void ArbolB5::dividirNodo(StructsComment::Comentario val, StructsComment::Comentario *pval, int pos, Nodo *node, Nodo *child, Nodo **newNode)
{
    int median, j;

    // Determina el valor medio para dividir el nodo
    if (pos > MIN)
        median = MIN + 1;
    else
        median = MIN;

    *newNode = new Nodo();
    j = median + 1;

    // Mueve los valores y enlaces al nuevo nodo
    while (j <= MAX)
    {
        (*newNode)->valor[j - median] = node->valor[j];
        (*newNode)->link[j - median] = node->link[j];
        j++;
    }

    // Ajusta el número de valores en los nodos originales y nuevos
    node->numero = median;
    (*newNode)->numero = MAX - median;

    // Inserta el valor en el nodo adecuado
    if (pos <= MIN)
    {
        insertarNodo(val, pos, node, child);
    }
    else
    {
        insertarNodo(val, pos - median, *newNode, child);
    }

    *pval = node->valor[node->numero];
    (*newNode)->link[0] = node->link[node->numero];
    node->numero--;
}


void ArbolB5::graficarNodo(Nodo *nodo, string &dot)
{
    if (nodo)
    {
        string nodeId = "node" + to_string(reinterpret_cast<uintptr_t>(nodo)); // Identificador único
        dot += nodeId + "[label=\"<f0> ";
        for (int i = 1; i <= nodo->numero; i++)
        {
            dot += "| {" + nodo->valor[i].usuario + " | " + nodo->valor[i].texto + " | " + nodo->valor[i].fecha_hora + "} |<f" + to_string(i) + "> ";
        }
        dot += "\"];\n";

        // Graficar los nodos hijos recursivamente
        for (int i = 0; i <= nodo->numero; i++)
        {
            if (nodo->link[i])
            {
                string childId = "node" + to_string(reinterpret_cast<uintptr_t>(nodo->link[i]));
                dot += nodeId + ":f" + to_string(i) + " -> " + childId + ";\n";
                graficarNodo(nodo->link[i], dot);
            }
        }
    }
}


void ArbolB5::transversalNodo(Nodo *myNode)
{
    int i;
    if (myNode)
    {
        cout << "[";
        for (i = 0; i < myNode->numero; i++)
        {
            // traversal(myNode->link[i]);
            cout << myNode->valor[i + 1].fecha_hora << ",";
        }
        for (i = 0; i <= myNode->numero; i++)
        {
            transversalNodo(myNode->link[i]);
        }
        cout << "]";
    }
}

int ArbolB5::sizeNodo(Nodo *nodo)
{
    if (!nodo)
    {
        return 0;
    }
    int size = 0;
    for (int i = 0; i <= nodo->numero; i++)
    {
        size += sizeNodo(nodo->link[i]);
    }
    return size + nodo->numero;
}

void ArbolB5::obtenerComentariosNodo(Nodo *nodo, ListaEnlazada::ListaEnlazada<StructsComment::Comentario> &lista)
{
    if (nodo)
    {
        for (int i = 0; i < nodo->numero; i++)
        {
            lista.insertar(nodo->valor[i + 1]);
        }
        for (int i = 0; i <= nodo->numero; i++)
        {
            obtenerComentariosNodo(nodo->link[i], lista);
        }
    }
}

ArbolB5::~ArbolB5()
{
    // Liberar memoria (implementación básica)
}
