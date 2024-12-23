#include "Trees/huffman.h"
#include "funciones.h"
#include "windowlogin.h"

#include <QApplication>

#include "globales.h"
#include "Structs.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    Huffman::Huffman huff;
    bool impTree = huff.importTree(".backups/_frecuencias.json");
    if (impTree) {
        std::string compressStr = Func::leerArchivo(".backups/_backup.edd");
        std::string descompress = huff.decompress(compressStr);
        //cout << descompress << endl;
        Func::cargarEstructuras(descompress);
    }
    Structs::Usuario *admin = lista_usuarios.buscar("admin@gmail.com");
    if (!admin) {
        std::string pass = "EDD2S2024";
        std::string hashpass = Func::generarHash(pass);
        Structs::Usuario admin(0, "Administrador", "Lab EDD", "2000-09-16", "admin@gmail.com", hashpass,"admin");
        lista_usuarios.insertar(admin);
        cout << "Si se creo el usuario admin "<< endl;
    }else{
        cout << "No se creo el usuario admin "<< endl;
    }

    seguridad_blockchain.importBlocks();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
