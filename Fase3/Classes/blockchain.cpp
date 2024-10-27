#include "blockchain.h"
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include "nlohmann/json.hpp"
#include "../Trees/merkle.h"
#include "../globales.h"
#include "../funciones.h"
#include <QMessageBox>

Blockchain::Blockchain::Blockchain()
{
    chain = ListaEnlazada::ListaEnlazada<Structs::Block>();
}

std::string Blockchain::Blockchain::generateTimestamp()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm *localTime = std::localtime(&now_c);
    std::ostringstream oss;
    oss << std::put_time(localTime, "%d-%m-%y-::%H:%M:%S");
    return oss.str();
}

// TODO: metodos publicos
ListaEnlazada::ListaEnlazada<Structs::Block> Blockchain::Blockchain::getChain(){
    return chain;
}
void Blockchain::Blockchain::addBlock(Structs::Block newBlock)
{
    newBlock.index = chain.size();
    newBlock.timestamp = generateTimestamp();
    if (newBlock.index > 0)
    {
        Structs::Block *prevBlock = chain.obtener(newBlock.index - 1);
        if (prevBlock)
        {
            newBlock.prevHash = prevBlock->hashBlock;
        }
    }
    else
    {
        newBlock.prevHash = "0000";
    }
    newBlock.calculateNonceAndHash();
    chain.insertar(newBlock);
    newBlock.showBlock();
}

void Blockchain::Blockchain::exportBlocks()
{
    try {
        if (chain.size() == 0) {
            std::cerr << "Error: La cadena de bloques está vacía, no hay bloques para exportar." << std::endl;
            return;
        }

        int count = 0;
        for (int i = 0; i < chain.size(); i++)
        {
            Structs::Block *currentBlock = chain.obtener(i);
            if (currentBlock == nullptr) {
                std::cerr << "Error: Bloque nulo encontrado en la posición " << i << " de la cadena." << std::endl;
                continue;
            }

            // Crear el nombre del archivo para exportar el bloque
            std::string filename = "Blockchain/block_" + std::to_string(i) + ".json";
            std::ofstream file(filename);
            if (!file.is_open()) {
                std::cerr << "Error: No se pudo abrir el archivo para escribir: " << filename << std::endl;
                continue;
            }

            nlohmann::json jsonBlock;
            jsonBlock["index"] = currentBlock->index;
            jsonBlock["timestamp"] = currentBlock->timestamp;
            jsonBlock["nonce"] = currentBlock->nonce;
            jsonBlock["rootHash"] = currentBlock->rootHash;

            nlohmann::json jsonData;
            for (int j = 0; j < currentBlock->data.size(); j++)
            {
                Structs::Publicacion *currentData = currentBlock->data.obtener(j);
                if (currentData == nullptr) {
                    std::cerr << "Error: Publicación nula en el bloque " << i << ", posición " << j << std::endl;
                    continue;
                }

                nlohmann::json jsonCurrentData;
                jsonCurrentData["id"] = currentData->id;
                jsonCurrentData["correo"] = currentData->correo_autor;
                jsonCurrentData["contenido"] = currentData->contenido;
                jsonCurrentData["fecha"] = currentData->fecha;
                jsonCurrentData["hora"] = currentData->hora;
                jsonCurrentData["imagen"] = currentData->imagen;

                // Manejo de comentarios
                nlohmann::json jsonCurrentComments, jsonComment;
                ListaEnlazada::ListaEnlazada<StructsComment::Comentario> comments = currentData->comentarios->obtenerComentarios();
                if (comments.size() == 0) {
                    jsonCurrentComments = nlohmann::json::array();
                }else{
                    for (int k = 0; k < comments.size(); ++k) {
                        StructsComment::Comentario *comment = comments.obtener(k);
                        if (comment == nullptr) {
                            std::cerr << "Error: Comentario nulo en la publicación " << currentData->id << ", posición " << k << std::endl;
                            continue;
                        }

                        nlohmann::json jsonComment;
                        jsonComment["id"] = comment->id;
                        jsonComment["correo"] = comment->usuario;
                        jsonComment["comentario"] = comment->texto;
                        jsonComment["fechaHora"] = comment->fecha_hora;
                        jsonCurrentComments.push_back(jsonComment);
                    }
                }

                // Añadir comentarios a la publicación
                jsonCurrentData["comentarios"] = jsonCurrentComments;
                jsonData.push_back(jsonCurrentData);
            }

            // Añadir datos y hashes al bloque JSON
            jsonBlock["data"] = jsonData;
            jsonBlock["prevHash"] = currentBlock->prevHash;
            jsonBlock["hashBlock"] = currentBlock->hashBlock;

            // Guardar el bloque en el archivo JSON
            file << jsonBlock.dump(4);
            file.close();
            count++;
        }

        std::cout << "Bloques exportados en la carpeta [Blockchain], cantidad exportada: " << count << std::endl;

    } catch (const std::exception &e) {
        std::cerr << "Error en exportBlocks: " << e.what() << std::endl;
    }
}

void Blockchain::Blockchain::importBlocks()
{
    try {
        std::string path = "Blockchain/";
        std::string filename = "";
        std::ifstream file;

        for (const auto &entry : std::filesystem::directory_iterator(path))
        {
            filename = entry.path().string();
            if (filename.substr(filename.find_last_of(".") + 1) != "json") {
                std::cout << "No es un archivo JSON: " << filename << std::endl;
                continue;
            }
            file.open(filename);
            if (!file.is_open()) {
                std::cerr << "Error al abrir el archivo: " << filename << std::endl;
                continue;
            }

            nlohmann::json jsonBlock;
            try {
                file >> jsonBlock;
            } catch (const std::exception &e) {
                std::cerr << "Error al leer el archivo JSON: " << filename << " - " << e.what() << std::endl;
                file.close();
                continue;
            }

            Structs::Block bloque;
            bloque.index = jsonBlock["index"];
            bloque.timestamp = jsonBlock["timestamp"];
            bloque.nonce = jsonBlock["nonce"];
            bloque.rootHash = jsonBlock["rootHash"];
            bloque.prevHash = jsonBlock["prevHash"];
            bloque.hashBlock = jsonBlock["hashBlock"];
            nlohmann::json jsonData = jsonBlock["data"];

            for (int i = 0; i < jsonData.size(); ++i) {
                Structs::Publicacion post(
                    jsonData[i]["id"],
                    jsonData[i]["correo"],
                    jsonData[i]["contenido"],
                    jsonData[i]["fecha"],
                    jsonData[i]["hora"],
                    jsonData[i]["imagen"]
                    );
                Structs::Publicacion postBlock(
                    jsonData[i]["id"],
                    jsonData[i]["correo"],
                    jsonData[i]["contenido"],
                    jsonData[i]["fecha"],
                    jsonData[i]["hora"],
                    jsonData[i]["imagen"]
                    );

                // Verificar si la publicación ya existe en la lista de publicaciones
                Structs::Publicacion *existingPost = Func::buscarPost(post.id);
                if (existingPost == nullptr) {
                    // Si no existe, se inserta tanto en la lista global como en el bloque
                    nlohmann::json jsonComments = jsonData[i]["comentarios"];
                    for (int j = 0; j < jsonComments.size(); ++j) {
                        StructsComment::Comentario comment2(
                            jsonComments[j]["id"],
                            jsonComments[j]["fechaHora"],
                            jsonComments[j]["correo"],
                            jsonComments[j]["comentario"]
                            );
                        postBlock.comentarios->insertar(comment2);
                    }
                    lista_publicaciones.insertar(post);
                    bloque.data.insertar(postBlock);
                } else {
                    // actualizar contenido
                    existingPost->contenido = jsonData[i]["contenido"];
                    existingPost->imagen =jsonData[i]["imagen"];

                    // Si existe, verificar comentarios
                    nlohmann::json jsonComments = jsonData[i]["comentarios"];
                    for (int j = 0; j < jsonComments.size(); ++j) {
                        StructsComment::Comentario comment(
                            jsonComments[j]["id"],
                            jsonComments[j]["fechaHora"],
                            jsonComments[j]["correo"],
                            jsonComments[j]["comentario"]
                            );
                        // Verificar si el comentario ya existe
                        if (!Func::existeComentario(existingPost->id, comment.id)) {
                            // Si no existe, agregar el comentario
                            existingPost->comentarios->insertar(comment);
                        }
                        postBlock.comentarios->insertar(comment);
                    }
                    // Actualizar la publicación en el bloque
                    bloque.data.insertar(postBlock);
                }
            }

            // Insertar el bloque en la cadena
            chain.insertar(bloque);
            file.close();
        }
        validateBlocks();
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
}

void Blockchain::Blockchain::validateBlock(int index)
{
    Structs::Block *b = chain.obtener(index);
    if (b) {
        Merkle::Merkle temp(b->data);
        if (temp.getRootHash() == b->rootHash) {
            b->validateBlock(true);
        }else{
            b->validateBlock(false);
        }
    }
}

void Blockchain::Blockchain::validateBlocks()
{
    for (int i = 0; i < chain.size(); i++)
    {
        Structs::Block *currentBlock = chain.obtener(i);
        // validar el root hash
        Merkle::Merkle raiz(currentBlock->data);
        if (currentBlock->rootHash == raiz.getRootHash()) {
            currentBlock->validateBlock(true);
        }else{
            currentBlock->validateBlock(false);
            continue;
        }
        // Validar los hashes
        if (i > 0)
        {
            Structs::Block *prevBlock = chain.obtener(i - 1);
            if (prevBlock->hashBlock != currentBlock->prevHash)
            {
                std::cout << "Error: El Bloque" << i << " es invalido" << std::endl;
                currentBlock->validateBlock(false);
                continue;
            }
            else
            {
                currentBlock->validateBlock(true);
            }
        }
        else
        {
            if (currentBlock->prevHash != "0000")
            {
                std::cout << "Error: El Bloque" << i << " es invalido" << std::endl;
                currentBlock->validateBlock(false);
                continue;
            }
            else
            {
                currentBlock->validateBlock(true);
            }
        }
    }
}

std::string Blockchain::Blockchain::graficar(){
    {
        if (chain.size() == 0)
        {
            QMessageBox::warning(nullptr, "Error", "No hay bloques en la cadena.");
            return "";
        }
        std::string dotPath = "renders/blockchain.dot";
        std::string imagePath = "renders/blockchain.png";

        std::ofstream archivoDot(dotPath);
        if (!archivoDot.is_open())
        {
            QMessageBox::warning(nullptr, "Error", "No se pudo abrir el archivo .dot");
            return "";
        }

        archivoDot << "digraph G {" << std::endl;
        archivoDot << "label=\"Blockchain\"; fontsize=25; fontname=\"Arial\";"<< std::endl;
        std::string props = "fontname=\"Arial\";"
                            "fontsize=20;"
                            "style=rounded;";
        for (int i = 0; i < chain.size(); ++i) {
            int no = i+1;
            Structs::Block *b = chain.obtener(i);
            if(!b) break;
            // abro el bloque
            archivoDot << "subgraph cluster_block"+std::to_string(no)+" {"<< std::endl;
            archivoDot << "label=\"Bloque #"<<std::to_string(no)<<"\";"<< std::endl;
            archivoDot << props<< std::endl;
            archivoDot << "color=black;"<< std::endl;
            archivoDot << "bgcolor=lightgray;"<< std::endl;
            archivoDot << "edge [style=invis];"<< std::endl;
            // TimeStamp y Nonce
            std::string timestamp = "label=\"Timestamp: \n"+b->timestamp+"\";"
                                    "fontname=\"Arial\"; shape=box;"
                                    "style=\"rounded, filled\";"
                                    "fillcolor=lightblue;"
                                    "width=5;";
            std::string nonce = "label=\"Nonce: \n"+std::to_string(b->nonce)+"\";"
                                    "fontname=\"Arial\";"
                                    "shape=box;"
                                    "style=\"rounded, filled\";"
                                    "fillcolor=lightblue;"
                                    "width=5;";
            archivoDot << "timestamp"<<std::to_string(no)<<"["<<timestamp<<"]"<< std::endl;
            archivoDot << "nonce"<<std::to_string(no)<<"["<<nonce<<"]"<< std::endl;
            archivoDot << "timestamp"<<std::to_string(no)<<" -> nonce"<<std::to_string(no)<<" -> root"<<std::to_string(no)<< std::endl;
            // abro el header
            archivoDot << "subgraph cluster_header"+std::to_string(no)+" {"<< std::endl;
            archivoDot << "label=\"Header\";"<< std::endl;
            archivoDot << props<< std::endl;
            archivoDot << "color=green;"<< std::endl;
            archivoDot << "bgcolor=lightgreen;"<< std::endl;
            std::string root = "label=\"RootHash: \n"+b->rootHash+"\";"
                                "fontname=\"Arial\";"
                                "shape=box;"
                                "style=\"rounded, filled\";"
                                "fillcolor=lightblue;"
                                "width=7;";
            std::string hash = "label=\"Hash: \n"+b->hashBlock+"\";"
                                "fontname=\"Arial\";"
                                "shape=box;"
                                "style=\"rounded, filled\";"
                                "fillcolor=lightblue;"
                                "width=7;";
            std::string prev = "label=\"Prev Hash: \n"+b->prevHash+"\";"
                                "fontname=\"Arial\";"
                                "shape=box;"
                                "style=\"rounded, filled\";"
                                "fillcolor=lightblue;"
                                "width=7;";
            archivoDot << "root"<<std::to_string(no)<<"["<<root<<"]"<< std::endl;
            archivoDot << "hash"<<std::to_string(no)<<"["<<hash<<"]"<< std::endl;
            archivoDot << "prev"<<std::to_string(no)<<"["<<prev<<"]"<< std::endl;
            archivoDot << "root"<<std::to_string(no)<<" -> hash"<<std::to_string(no)<<" -> prev"<<std::to_string(no)<< std::endl;
            // cierro el header
            archivoDot << "}"<< std::endl;
            // transacciones
            std::string transac_con ="prev"+std::to_string(no);
            for (int j = 0; j < b->data.size(); ++j) {
                Structs::Publicacion *p = b->data.obtener(j);
                if(!p) break;
                std::string transac = "label=\"{{"+p->correo_autor+"|"+p->fecha+" "+p->hora+"}|{comentarios | "+std::to_string(p->comentarios->size())+"}|"+p->contenido+"}\";"
                                      "shape=record;"
                                      "fontname=\"Arial\";"
                                      "style=\"rounded, filled\";"
                                      "fillcolor=white;";
                archivoDot << "transac"<<std::to_string(no)<<"_"<<std::to_string(j)<<"["<<transac<<"]"<< std::endl;
                transac_con += "-> transac"+std::to_string(no)+"_"+std::to_string(j);

            }
            archivoDot << transac_con<< std::endl;
            // cierro el bloque
            archivoDot << "}"<< std::endl;
        }
        std::string conn = "";
        for (int i = 0; i < chain.size(); ++i) {
            if(i==0) continue;
            conn += "hash"+std::to_string(i)+" -> prev"+std::to_string(i+1)+"[constraint=false;];\n";
        }
        archivoDot << conn<< std::endl;
        // cierro el graph
        archivoDot << "}" << std::endl;
        archivoDot.close();

        std::string cmd = "dot -Tpng " + dotPath + " -o " + imagePath;
        system(cmd.c_str());

        return imagePath;
    }
}
