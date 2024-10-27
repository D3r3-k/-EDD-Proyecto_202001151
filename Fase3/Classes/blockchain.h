#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include "../ListaEnlazada.h"
#include "../Structs.h"
#include "nlohmann/json.hpp"

namespace Blockchain {
class Blockchain
{
private:
    ListaEnlazada::ListaEnlazada<Structs::Block> chain;
    void parseBlockToJSON(Structs::Block block);
    void parseJSONToBlock(const nlohmann::json& jsonNode);
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
};

}

#endif // BLOCKCHAIN_H
