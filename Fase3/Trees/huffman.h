#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <nlohmann/json.hpp>
#include <string>
#include <fstream>

namespace Huffman {

struct Node {
    char ch;
    int freq;
    Node *left, *right;

    Node(char ch, int freq) {
        this->ch = ch;
        this->freq = freq;
        left = right = nullptr;
    }
};

struct PriorityQueue {
    Node* data;
    PriorityQueue* next;

    PriorityQueue(Node* data) : data(data), next(nullptr) {}
};

class Huffman {
public:
    std::string compress(const std::string& text);
    std::string decompress(const std::string& compressedText);
    void createGraph(Node* root);
    Node* getRoot();
    void exportTree(const std::string& filename);
    bool importTree(const std::string& filename);

private:
    void insert(PriorityQueue*& head, Node* newNode);
    Node* extractMin(PriorityQueue*& head);
    void buildHuffmanCode(Node* root, std::string code, std::string codes[256]);
    std::string compress(const std::string& text, std::string codes[256]);
    void generateDotFile(Node* root, std::ofstream& dotFile, int& nullCount);
    Node* root;
    void serialize(Node* node, nlohmann::json& jsonNode);
    Node* deserialize(const nlohmann::json& jsonNode);
};

}

#endif // HUFFMAN_H
