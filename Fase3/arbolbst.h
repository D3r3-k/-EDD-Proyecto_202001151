#ifndef ARBOLBST_H
#define ARBOLBST_H

#include "Structs.h"
#include <string>

using namespace std;

class ArbolBST
{
private:
    class Node
    {
    public:
        Structs::ReportePosts val;
        Node *left;
        Node *right;
        Node(Structs::ReportePosts val_):val(val_), left(nullptr), right(nullptr) {}
    };
    // Metodos privados
    void add(Structs::ReportePosts val, Node* tmp);
    void preorder(Node* tmp);
    void inorder(Node* tmp);
    void postorder(Node* tmp);
    void levelorder(Node* tmp);
    void buscarPorFecha(string fecha, Node* tmp);
    // metodo para graficar el arbol con graphviz
    void graficar(Node* tmp, string fecha, string &dot);
public:
    Node *root;
    // Constructor
    ArbolBST() : root(nullptr) {}
    // Destructor
    ~ArbolBST();
    // Metodos publicos
    void add(Structs::ReportePosts val);
    void preorder();
    void inorder();
    void postorder();
    void buscarPorFecha(string fecha);
    // metodo para graficar el arbol con graphviz
    string graficar(string fecha);
};

#endif // ARBOLBST_H
