#include "arbolbst.h"
#include <queue>
#include <iostream>
#include <fstream>
#include <QMessageBox>

// Metodos privados
void ArbolBST::add(Structs::ReportePosts val, Node *tmp)
{
    if (val.fecha < tmp->val.fecha)
    {
        if (tmp->left != nullptr)
            add(val, tmp->left);
        else
            tmp->left = new Node(val);
    }
    else
    {
        if (tmp->right != nullptr)
            add(val, tmp->right);
        else
            tmp->right = new Node(val);
    }
}

void ArbolBST::preorder(Node *tmp)
{
    if (tmp != nullptr)
    {
        cout << tmp->val.fecha << " ";
        preorder(tmp->left);
        preorder(tmp->right);
    }
}

void ArbolBST::inorder(Node *tmp)
{
    if (tmp != nullptr)
    {
        inorder(tmp->left);
        cout << tmp->val.fecha << " ";
        inorder(tmp->right);
    }
}

void ArbolBST::postorder(Node *tmp)
{
    if (tmp != nullptr)
    {
        postorder(tmp->left);
        postorder(tmp->right);
        cout << tmp->val.fecha << " ";
    }
}

void ArbolBST::levelorder(Node *tmp)
{
    if (tmp == nullptr)
        return;

    queue<Node *> q;
    q.push(tmp);

    while (!q.empty())
    {
        Node *node = q.front();
        cout << node->val.fecha << " ";
        q.pop();

        if (node->left != 0)
            q.push(node->left);
        if (node->right != 0)
            q.push(node->right);
    }
}

void ArbolBST::buscarPorFecha(string fecha, Node *tmp)
{
    if (tmp != nullptr)
    {
        if (tmp->val.fecha == fecha)
        {
            cout << "Fecha: " << tmp->val.fecha << endl;
        }
        else if (fecha < tmp->val.fecha)
            buscarPorFecha(fecha, tmp->left);
        else
            buscarPorFecha(fecha, tmp->right);
    }
}

void ArbolBST::graficar(Node *tmp, string fecha, string &dot)
{
    if (tmp != nullptr)
    {
        // Generar un identificador único para el nodo basado en la dirección de memoria
        std::string nodeId = "node" + std::to_string(reinterpret_cast<uintptr_t>(tmp));

        // Si la fecha coincide, el nodo se pinta de verde
        if (tmp->val.fecha == fecha)
        {
            dot += nodeId + " [label=\"Fecha: " + tmp->val.fecha + "\", style=filled, fillcolor=green];\n";
            for (int i = 0; i < tmp->val.publicaciones.size(); ++i)
            {
                Structs::Publicacion *p = tmp->val.publicaciones.obtener(i);
                if (p)
                {
                    string id = to_string(p->id);
                    dot += "post" + id + " [label=\"{<f0> Publicacion: " + id + " | <f1> Fecha y Hora: " + p->fecha + " " + p->hora + " | <f2> Correo Autor: " + p->correo_autor + " | <f3> Contenido: " + p->contenido + "}\", shape=record];\n";
                }
            }
            // Enlazar el nodo actual con la primera publicacion y luego la primera con la segunda y asi sucesivamente
            for (int i = 0; i < tmp->val.publicaciones.size(); ++i)
            {
                Structs::Publicacion *p = tmp->val.publicaciones.obtener(i);
                if (p)
                {
                    string id = to_string(p->id);
                    if (i == 0)
                    {
                        dot += nodeId + "-> post" + id + ";\n";
                    }
                    else if (i > 0)
                    {
                        Structs::Publicacion *pAnterior = tmp->val.publicaciones.obtener(i - 1);
                        if (pAnterior)
                        {
                            string idAnterior = to_string(pAnterior->id);
                            dot += "post" + idAnterior + " -> post" + id + ";\n";
                        }
                    }
                }
            }
        }
        else
        {
            dot += nodeId + " [label=\"Fecha: " + tmp->val.fecha + "\"];\n";
        }

        // Enlazar el nodo actual con su hijo izquierdo
        if (tmp->left != nullptr)
        {
            std::string leftNodeId = "node" + std::to_string(reinterpret_cast<uintptr_t>(tmp->left));
            dot += nodeId + " -> " + leftNodeId + ";\n";
            graficar(tmp->left, fecha, dot);
        }

        // Enlazar el nodo actual con su hijo derecho
        if (tmp->right != nullptr)
        {
            std::string rightNodeId = "node" + std::to_string(reinterpret_cast<uintptr_t>(tmp->right));
            dot += nodeId + "-> " + rightNodeId + ";\n";
            graficar(tmp->right, fecha, dot);
        }
    }
}

// Metodos publicos
void ArbolBST::add(Structs::ReportePosts val)
{
    if (root != nullptr)
        add(val, root);
    else
        root = new Node(val);
}

void ArbolBST::preorder()
{
    preorder(root);
}

void ArbolBST::inorder()
{
    inorder(root);
}

void ArbolBST::postorder()
{
    postorder(root);
}

void ArbolBST::buscarPorFecha(string fecha)
{
    buscarPorFecha(fecha, root);
}
string ArbolBST::graficar(std::string fecha)
{
    try
    {
        std::string dot = "digraph G {\n";
        graficar(root, fecha, dot);
        dot += "}\n";

        std::ofstream file("arbolbst.dot");
        if (file.is_open())
        {
            file << dot;
            file.close();
        }
        else
        {
            throw std::runtime_error("No se pudo abrir el archivo arbolbst.dot");
        }

        // Generar la imagen con el comando dot
        std::string command = "dot -Tpng arbolbst.dot -o arbolbst.png";
        if (system(command.c_str()) == -1)
        {
            throw std::runtime_error("No se pudo generar la imagen arbolbst.png");
        }

        return "arbolbst.png";
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}
