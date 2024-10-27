#include "merkle.h"
#include "../funciones.h"
#include <fstream>
#include <cstdlib>
#include <QMessageBox>

Merkle::Merkle::Merkle() {}

Merkle::Merkle::Merkle(ListaEnlazada::ListaEnlazada<Structs::Publicacion> &data)
{
    try
    {
        data_temp = data;
        int numLeaves = data.size();
        Nodo *leaves[numLeaves];
        for (int i = 0; i < numLeaves; ++i)
        {
            Structs::Publicacion *p = data.obtener(i);
            if (p)
            {
                std::string hashNode = Func::generarHash(p->toJSON());
                leaves[i] = new Nodo(hashNode);
                leaves[i]->indice = i;
            }
        }
        root = buildMerkleTree(leaves, 0, numLeaves - 1);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}
Merkle::Merkle::~Merkle()
{
    delete root;
}
std::string Merkle::Merkle::getRootHash() const
{
    try
    {
        if (root)
        {
            return root->hash;
        }
        else
        {
            return "";
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

Merkle::Merkle::Nodo *Merkle::Merkle::buildMerkleTree(Nodo **leaves, int start, int end)
{
    if (start == end)
    {
        return leaves[start];
    }
    int mid = (start + end) / 2;
    Nodo *leftSubTree = buildMerkleTree(leaves, start, mid);
    Nodo *rightSubTree = buildMerkleTree(leaves, mid + 1, end);
    return combineNodes(leftSubTree, rightSubTree);
}
Merkle::Merkle::Nodo *Merkle::Merkle::combineNodes(Nodo *left, Nodo *right)
{
    string combinedHash = Func::generarHash(left->hash + right->hash);
    Nodo *parent = new Nodo(combinedHash);
    parent->left = left;
    parent->right = right;
    return parent;
}

void Merkle::Merkle::generateDotFile(Nodo *node, ofstream &outFile, int &nodeCounter) const
{
    if (!node)
        return;

    int currentNode = nodeCounter++;
    if (currentNode == 0)
    {
        outFile << "hash" << currentNode << " [label=\"{Top Hash|" << node->hash << "}\"; shape=record;width=6.5;];\n";
    }
    else
    {
        std::string hashblock = node->hash;
        for (int i = 20; i < hashblock.size(); i += 20)
        {
            hashblock.insert(i, "\\n");
        }
        outFile << "hash" << currentNode << " [label=\"{Hash|" << hashblock << "}\"];\n";
    }
    // conectar
    if (node->left)
    {
        int leftNode = nodeCounter;
        outFile << "hash" << leftNode << " -> hash" << currentNode << ";\n";
        generateDotFile(node->left, outFile, nodeCounter);
    }
    if (node->right)
    {
        int rightNode = nodeCounter;
        outFile << "hash" << rightNode << " -> hash" << currentNode << ";\n";
        generateDotFile(node->right, outFile, nodeCounter);
    }
}

void Merkle::Merkle::generateDatablockConnections(Nodo *node, ofstream &outFile, int& nodeCounter)
{
    if (!node) return;
    int currentNode = nodeCounter++;
    if (node->left) {
        generateDatablockConnections(node->left, outFile, nodeCounter);
    }
    if (node->right) {
        generateDatablockConnections(node->right, outFile, nodeCounter);
    }
    if (!node->left && !node->right) {
        for (int i = 0; i < data_temp.size(); i++) {
            Structs::Publicacion *temp = data_temp.obtener(i);
            if (temp) {
                std::string hashNode = Func::generarHash(temp->toJSON());
                if (hashNode == node->hash) {
                    outFile << "datablock"<<i<<" -> hash"<<currentNode<<";\n"<<endl;
                    break;
                }
            }
        }
    }
}


std::string Merkle::Merkle::graficar()
{

    std::string dotPath = "renders/merkle_tree.dot";
    std::string imagePath = "renders/merkle_tree.png";

    std::ofstream archivoDot(dotPath);
    if (!archivoDot.is_open())
    {
        QMessageBox::warning(nullptr, "Error", "No se pudo abrir el archivo .dot");
        return "";
    }

    archivoDot << "digraph G {" << std::endl;
    archivoDot << "label=\"Arbol de Merkle\"; fontsize=25; fontname=\"Arial\";"<< std::endl;
    archivoDot << "\trankdir=BT;" << std::endl;
    archivoDot << "\tnode[shape=record];" << std::endl;
    // abro cluster_datablocks
    archivoDot << "\tsubgraph cluster_datablocks {" << std::endl;
    archivoDot << "\t\tstyle=dashed;" << std::endl;
    for (int i = 0; i < data_temp.size(); ++i)
    {
        Structs::Publicacion *temp = data_temp.obtener(i);
        if (temp)
        {
            std::string datap = "label=\"{{" + temp->correo_autor + "|" + temp->fecha + " " + temp->hora + "}|{comentarios | " + std::to_string(temp->comentarios->size()) + "}|" + temp->contenido + "|" + temp->imagen + "}\";";
            archivoDot << "\t\tdatablock" << std::to_string(i) << " [" << datap << "];" << std::endl;
        }
    }
    archivoDot << "\t\tdatablockt" << " [label=\"Data Blocks\" shape=plaintext];" << std::endl;
    // cierro cluster_datablocks
    archivoDot << "\t}" << std::endl;
    // genero los nodos
    int nodeCounter = 0;
    generateDotFile(root, archivoDot, nodeCounter);
    // conectar los datablocks con los ultimo nodos del arbol
    int coun = 0;
    generateDatablockConnections(root, archivoDot, coun);

    // cierro el graph
    archivoDot << "}" << std::endl;
    archivoDot.close();
    std::string cmd = "dot -Tpng " + dotPath + " -o " + imagePath;
    system(cmd.c_str());
    return imagePath;
}
