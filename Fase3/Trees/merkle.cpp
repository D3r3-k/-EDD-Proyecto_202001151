#include "merkle.h"
#include "../funciones.h"
#include <fstream>
#include <cstdlib>



Merkle::Merkle::Merkle() {}

Merkle::Merkle::Merkle(ListaEnlazada::ListaEnlazada<Structs::Publicacion> &data) {
    try {
        int numLeaves = data.size();
        Nodo* leaves[numLeaves];
        for (int i = 0; i < numLeaves; ++i) {
            Structs::Publicacion *p = data.obtener(i);
            if (p) {
                std::string hashNode = Func::generarHash(p->toJSON());
                leaves[i] = new Nodo(hashNode);
            }
        }
        root = buildMerkleTree(leaves, 0, numLeaves-1);
    }catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

}
Merkle::Merkle::~Merkle() {
    delete root;
}
std::string Merkle::Merkle::getRootHash() const {
    try {
        if (root) {
            return root->hash;
        }else{
            return "";
        }
    }catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

Merkle::Merkle::Nodo* Merkle::Merkle::buildMerkleTree(Nodo** leaves,int start, int end){
    if (start == end) {
        return leaves[start];
    }
    int mid = (start + end) / 2;
    Nodo* leftSubTree = buildMerkleTree(leaves, start, mid);
    Nodo* rightSubTree = buildMerkleTree(leaves, mid + 1, end);
    return combineNodes(leftSubTree, rightSubTree);
}
Merkle::Merkle::Nodo* Merkle::Merkle::combineNodes(Nodo* left, Nodo* right) {
    string combinedHash = Func::generarHash(left->hash + right->hash);
    Nodo* parent = new Nodo(combinedHash);
    parent->left = left;
    parent->right = right;
    return parent;
}

void Merkle::Merkle::generateDotFile(Nodo* node, ofstream &outFile, int& nodeCounter) const {
    if (!node) return;

    int currentNode = nodeCounter++;
    outFile << "node" << currentNode << " [label=\"" << node->hash << "\"];\n";

    if (node->left) {
        int leftNode = nodeCounter;
        outFile << "node" << currentNode << " -- node" << leftNode << ";\n";
        generateDotFile(node->left, outFile, nodeCounter);
    }

    if (node->right) {
        int rightNode = nodeCounter;
        outFile << "node" << currentNode << " -- node" << rightNode << ";\n";
        generateDotFile(node->right, outFile, nodeCounter);
    }
}

void Merkle::Merkle::graphMerkleTree() const {
    try {
        ofstream outFile("renders/merkle_tree.dot");
        outFile << "graph Merkle {\n";

        int nodeCounter = 0;
        generateDotFile(root, outFile, nodeCounter);

        outFile << "}\n";
        outFile.close();

        // Ejecutar el comando para generar la imagen con Graphviz
        system("dot -Tpng renders/merkle_tree.dot -o renders/merkle_tree.png");
        cout << "Árbol de Merkle graficado en renders/merkle_tree.png\n";
    }catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}
