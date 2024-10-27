#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include "../ListaEnlazada.h"
#include "../Structs.h"

namespace Blockchain {
class Blockchain
{
private:
    ListaEnlazada::ListaEnlazada<Structs::Block> chain;
    // Extras
    std::string generateTimestamp();
public:
    Blockchain();
    void addBlock(Structs::Block newBlock);
    void exportBlocks();
    void importBlocks();
    void validateBlock(int index);
    void validateBlocks();
    string graficar();
    ListaEnlazada::ListaEnlazada<Structs::Block> getChain();
};

}

#endif // BLOCKCHAIN_H
