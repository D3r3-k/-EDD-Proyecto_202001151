#include "huffman.h"
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>

namespace Huffman {

void Huffman::insert(PriorityQueue*& head, Node* newNode) {
    PriorityQueue* newElement = new PriorityQueue(newNode);
    if (!head || head->data->freq > newNode->freq) {
        newElement->next = head;
        head = newElement;
    } else {
        PriorityQueue* current = head;
        while (current->next && current->next->data->freq <= newNode->freq) {
            current = current->next;
        }
        newElement->next = current->next;
        current->next = newElement;
    }
}

Node* Huffman::extractMin(PriorityQueue*& head) {
    if (!head) return nullptr;
    PriorityQueue* temp = head;
    Node* minNode = head->data;
    head = head->next;
    delete temp;
    return minNode;
}

void Huffman::buildHuffmanCode(Node* root, std::string code, std::string codes[256]) {
    if (!root) return;

    if (!root->left && !root->right) {
        codes[(unsigned char)root->ch] = code;
    }

    buildHuffmanCode(root->left, code + "0", codes);
    buildHuffmanCode(root->right, code + "1", codes);
}

std::string Huffman::compress(const std::string& text) {
    int freq[256] = {0};
    for (char ch : text) {
        freq[(unsigned char)ch]++;
    }

    PriorityQueue* pq = nullptr;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            Node* newNode = new Node((char)i, freq[i]);
            insert(pq, newNode);
        }
    }

    while (pq && pq->next) {
        Node* left = extractMin(pq);
        Node* right = extractMin(pq);

        int sumFreq = left->freq + right->freq;
        root = new Node('\0', sumFreq); // Establecer la raíz
        root->left = left;
        root->right = right;

        insert(pq, root);
    }

    std::string codes[256];
    buildHuffmanCode(root, "", codes);

    std::string compressedText = "";
    for (char ch : text) {
        compressedText += codes[(unsigned char)ch];
    }

    return compressedText; // Retornamos el texto comprimido
}

std::string Huffman::decompress(const std::string& compressedText) {
    std::string decompressedText = "";
    Node* currentNode = root;
    for (char bit : compressedText) {
        if (bit == '0') {
            currentNode = currentNode->left;
        } else {
            currentNode = currentNode->right;
        }

        if (!currentNode->left && !currentNode->right) {
            decompressedText += currentNode->ch;
            currentNode = root;
        }
    }
    return decompressedText;
}

void Huffman::generateDotFile(Node* root, std::ofstream& dotFile, int& nullCount) {
    if (!root) return;

    if (!root->left && !root->right) {
        dotFile << "    \"" << root << "\" [label=\"" << root->ch << " (" << root->freq << ")\"];\n";
    } else {
        dotFile << "    \"" << root << "\" [label=\"Freq: " << root->freq << "\"];\n";
    }

    if (root->left) {
        dotFile << "    \"" << root << "\" -> \"" << root->left << "\" [label=\"0\"];\n";
        generateDotFile(root->left, dotFile, nullCount);
    } else {
        dotFile << "    null" << nullCount << " [shape=point];\n";
        dotFile << "    \"" << root << "\" -> null" << nullCount++ << " [label=\"0\"];\n";
    }

    if (root->right) {
        dotFile << "    \"" << root << "\" -> \"" << root->right << "\" [label=\"1\"];\n";
        generateDotFile(root->right, dotFile, nullCount);
    } else {
        dotFile << "    null" << nullCount << " [shape=point];\n";
        dotFile << "    \"" << root << "\" -> null" << nullCount++ << " [label=\"1\"];\n";
    }
}

void Huffman::createGraph(Node* root) {
    std::ofstream dotFile("huffman_tree.dot");
    dotFile << "digraph G {\n";
    dotFile << "    node [fontname=\"Arial\"];\n";

    int nullCount = 0;
    generateDotFile(root, dotFile, nullCount);

    dotFile << "}\n";
    dotFile.close();
    std::cout << "Archivo 'huffman_tree.dot' generado correctamente.\n";

    system("dot -Tpng huffman_tree.dot -o huffman_tree.png");
    std::cout << "Imagen PNG generada: huffman_tree.png\n";
}

Node* Huffman::getRoot() {
    return root; // Retorna la raíz del árbol
}

} // namespace Huffman
