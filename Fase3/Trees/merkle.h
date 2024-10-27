#ifndef MERKLE_H
#define MERKLE_H
#include "../Structs.h"

namespace Merkle {
class Merkle
{
private:
    struct Nodo {
        int indice;
        std::string hash;
        Nodo *left, *right;
        Nodo(const std::string data) : hash(data), left(nullptr), right(nullptr) {}

    };
    Nodo *root;
    Nodo* combineNodes(Nodo* left, Nodo *right);
    Nodo* buildMerkleTree(Nodo** leaves,int start, int end);
    void generateDotFile(Nodo* node, ofstream &outFile,int& nodeCounter) const ;
    void generateDatablockConnections(Nodo* node, ofstream &outFile, int& nodeCounter);
    ListaEnlazada::ListaEnlazada<Structs::Publicacion> data_temp;
public:
    Merkle();
    Merkle(ListaEnlazada::ListaEnlazada<Structs::Publicacion> &data);
    ~Merkle();
    string getRootHash() const;
    std::string graficar();
};
}

#endif // MERKLE_H
