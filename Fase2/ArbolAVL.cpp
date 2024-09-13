// arbolavl.cpp
#include "arbolavl.h"
#include "globales.h"
#include <fstream>
#include <cstdlib> // Para system()
#include <QMessageBox>

// Constructor
ArbolAVL::ArbolAVL() : raiz(nullptr) {}

// Destructor
ArbolAVL::~ArbolAVL()
{
    destruirArbolRecursivo(raiz);
}

// TODO: METODOS PUBLICOS
// Método público para insertar un usuario
bool ArbolAVL::insertar(const Structs::Usuario &usuario)
{
    Nodo *nodo = buscarNodo(raiz, usuario.correo);
    if (nodo != nullptr)
    {
        return false;
    }
    raiz = insertarNodo(raiz, usuario);
    return true;
}

// Método para eliminar un usuario del árbol AVL
bool ArbolAVL::eliminar(const std::string &correo)
{
    Nodo *nodo = buscarNodo(raiz, correo);
    if (nodo != nullptr)
    {
        raiz = eliminarNodo(raiz, correo);
        return true;
    }
    return false;
}

// Método para modificar un usuario en el árbol AVL
bool ArbolAVL::modificar(const std::string &correo, std::string nombres, std::string apellidos, std::string fecha, std::string contrasena)
{
    Nodo *nodo = buscarNodo(raiz, correo);
    if (nodo != nullptr)
    {
        nodo->usuario.modificar(nombres,apellidos,fecha,contrasena);
        return true;
    }
    return false;
}
// Método para enviar una solicitud de amistad
bool ArbolAVL::enviarSolicitud(const std::string &correoEmisor, const std::string &correoReceptor)
{
    Nodo *nodoEmisor = buscarNodo(raiz, correoEmisor);
    Nodo *nodoReceptor = buscarNodo(raiz, correoReceptor);
    if (nodoEmisor != nullptr && nodoReceptor != nullptr)
    {
        nodoEmisor->usuario.enviarSolicitud(nodoReceptor->usuario);
        nodoReceptor->usuario.recibirSolicitud(nodoEmisor->usuario);
        return true;
    }
    return false;
}

// Método para cancelar una solicitud de amistad
bool ArbolAVL::cancelarSolicitud(const std::string &correoEmisor, const std::string &correoReceptor)
{
    Nodo *nodoEmisor = buscarNodo(raiz, correoEmisor);
    Nodo *nodoReceptor = buscarNodo(raiz, correoReceptor);
    if (nodoEmisor != nullptr && nodoReceptor != nullptr)
    {
        nodoEmisor->usuario.cancelarSolicitud(nodoReceptor->usuario.correo);
        nodoReceptor->usuario.rechazarSolicitud(nodoEmisor->usuario.correo);
        return true;
    }
    return false;
}
// Método para rechazar una solicitud de amistad
bool ArbolAVL::rechazarSolicitud(const std::string &correoEmisor, const std::string &correoReceptor)
{
    Nodo *nodoEmisor = buscarNodo(raiz, correoEmisor);
    Nodo *nodoReceptor = buscarNodo(raiz, correoReceptor);
    if (nodoEmisor != nullptr && nodoReceptor != nullptr)
    {
        nodoEmisor->usuario.rechazarSolicitud(nodoReceptor->usuario.correo);
        nodoReceptor->usuario.cancelarSolicitud(nodoEmisor->usuario.correo);
        return true;
    }
    return false;
}
// Método para aceptar una solicitud de amistad
bool ArbolAVL::aceptarSolicitud(const std::string &correoEmisor, const std::string &correoReceptor)
{
    Nodo *nodoEmisor = buscarNodo(raiz, correoEmisor);
    Nodo *nodoReceptor = buscarNodo(raiz, correoReceptor);
    if (nodoEmisor != nullptr && nodoReceptor != nullptr)
    {
        nodoEmisor->usuario.aceptarSolicitudRec(nodoReceptor->usuario.correo);
        nodoReceptor->usuario.aceptarSolicitudEnv(nodoEmisor->usuario.correo);
        relaciones_amistad.agregarRelacion(&nodoEmisor->usuario, &nodoReceptor->usuario);
        return true;
    }
    return false;
}


// Método para buscar un usuario en el árbol AVL
Structs::Usuario *ArbolAVL::buscar(const std::string &correo)
{
    Nodo *resultado = buscarNodo(raiz, correo);
    if (resultado != nullptr)
    {
        return &resultado->usuario; // Retorna el puntero al usuario encontrado
    }
    return nullptr; // Si no se encuentra el usuario, retorna nullptr
}

// Método para obtener una lista enlazada de usuarios en orden in-order
ListaEnlazada::ListaEnlazada<Structs::Usuario> ArbolAVL::InOrder()
{
    ListaEnlazada::ListaEnlazada<Structs::Usuario> listaUsuarios;
    inOrdenRecursivo(raiz, listaUsuarios);
    return listaUsuarios;
}

// Método para obtener una lista enlazada de usuarios en pre-order
ListaEnlazada::ListaEnlazada<Structs::Usuario> ArbolAVL::PreOrder()
{
    ListaEnlazada::ListaEnlazada<Structs::Usuario> listaUsuarios;
    preOrdenRecursivo(raiz, listaUsuarios);
    return listaUsuarios;
}

// Método para obtener una lista enlazada de usuarios en post-order
ListaEnlazada::ListaEnlazada<Structs::Usuario> ArbolAVL::PostOrder()
{
    ListaEnlazada::ListaEnlazada<Structs::Usuario> listaUsuarios;
    postOrdenRecursivo(raiz, listaUsuarios);
    return listaUsuarios;
}

// Método para graficar el árbol en formato Graphviz
string ArbolAVL::graficar()
{
    std::string dotPath = "arbolAVL.dot";
    std::string imagePath = "arbolAVL.png";

    // Crear el archivo .dot
    std::ofstream archivoDot(dotPath);

    if (!archivoDot.is_open())
    {
        QMessageBox::information(nullptr, "Error", "No se pudo abrir el archivo para escribir.");
        return "";
    }

    archivoDot << "digraph ArbolAVL {" << std::endl;
    // Llamar al método privado para construir la representación en DOT
    graficarNodo(raiz, archivoDot);
    archivoDot << "}" << std::endl;
    archivoDot.close();

    // Convertir el archivo .dot a imagen (ejemplo en PNG)
    std::string command = "dot -Tpng " + dotPath + " -o " + imagePath;
    int result = system(command.c_str());

    if (result != 0)
    {
        QMessageBox::information(nullptr, "Error", "Error al ejecutar el comando dot.");
        return "";
    }

    return imagePath; // Devuelve la ruta de la imagen generada
}

// Método público para obtener el último ID del árbol
int ArbolAVL::obtenerId()
{
    Structs::Usuario usuarioConMayorID = obtenerUsuarioConMayorID();
    return usuarioConMayorID.id;
}


// ============================================================================================================= //
// TODO: METODOS PRIVADOS
// ============================================================================================================= //
// Método privado para destruir el árbol de forma recursiva
void ArbolAVL::destruirArbolRecursivo(Nodo *nodo)
{
    if (nodo != nullptr)
    {
        destruirArbolRecursivo(nodo->izquierda);
        destruirArbolRecursivo(nodo->derecha);
        delete nodo;
    }
}

// Obtener la altura de un nodo
int ArbolAVL::obtenerAltura(Nodo *nodo)
{
    if (nodo == nullptr)
    {
        return 0;
    }
    return nodo->altura;
}

// Obtener el factor de balance de un nodo
int ArbolAVL::obtenerBalance(Nodo *nodo)
{
    if (nodo == nullptr)
    {
        return 0;
    }
    return obtenerAltura(nodo->izquierda) - obtenerAltura(nodo->derecha);
}

// Método privado para insertar un nodo en el árbol AVL
ArbolAVL::Nodo *ArbolAVL::insertarNodo(Nodo *nodo, const Structs::Usuario &usuario)
{
    // Paso 1: Inserción estándar en un árbol binario de búsqueda
    if (nodo == nullptr)
    {
        return new Nodo(usuario);
    }

    // Comparación para determinar si se inserta a la izquierda o derecha
    if (usuario.correo < nodo->usuario.correo)
    {
        nodo->izquierda = insertarNodo(nodo->izquierda, usuario);
    }
    else if (usuario.correo > nodo->usuario.correo)
    {
        nodo->derecha = insertarNodo(nodo->derecha, usuario);
    }
    else
    {
        return nodo;
    }

    // Paso 2: Actualizar la altura del nodo ancestro
    nodo->altura = 1 + std::max(obtenerAltura(nodo->izquierda), obtenerAltura(nodo->derecha));

    // Paso 3: Obtener el factor de balance del nodo ancestro
    int balance = obtenerBalance(nodo);

    // Paso 4: Realizar rotaciones si el nodo está desbalanceado

    // Caso 1: Rotación derecha (Left-Left Case)
    if (balance > 1 && usuario.correo < nodo->izquierda->usuario.correo)
    {
        return rotacionDerecha(nodo);
    }

    // Caso 2: Rotación izquierda (Right-Right Case)
    if (balance < -1 && usuario.correo > nodo->derecha->usuario.correo)
    {
        return rotacionIzquierda(nodo);
    }

    // Caso 3: Rotación izquierda-derecha (Left-Right Case)
    if (balance > 1 && usuario.correo > nodo->izquierda->usuario.correo)
    {
        nodo->izquierda = rotacionIzquierda(nodo->izquierda);
        return rotacionDerecha(nodo);
    }

    // Caso 4: Rotación derecha-izquierda (Right-Left Case)
    if (balance < -1 && usuario.correo < nodo->derecha->usuario.correo)
    {
        nodo->derecha = rotacionDerecha(nodo->derecha);
        return rotacionIzquierda(nodo);
    }

    // Retornar el puntero al nodo (sin cambios si no hubo desbalance)
    return nodo;
}

// Función privada auxiliar para eliminar un nodo del árbol
ArbolAVL::Nodo *ArbolAVL::eliminarNodo(Nodo *nodo, const std::string &correo)
{
    if (nodo == nullptr)
    {
        return nodo; // Caso base: nodo vacío
    }

    // Buscar el nodo a eliminar según el correo
    if (correo < nodo->usuario.correo)
    {
        nodo->izquierda = eliminarNodo(nodo->izquierda, correo); // Recorrer hacia la izquierda
    }
    else if (correo > nodo->usuario.correo)
    {
        nodo->derecha = eliminarNodo(nodo->derecha, correo); // Recorrer hacia la derecha
    }
    else
    {
        // Nodo encontrado, procedemos a eliminar
        if (nodo->izquierda == nullptr)
        {
            Nodo *temp = nodo->derecha;
            delete nodo;
            return temp; // El nodo tiene solo un hijo derecho o ningún hijo
        }
        else if (nodo->derecha == nullptr)
        {
            Nodo *temp = nodo->izquierda;
            delete nodo;
            return temp; // El nodo tiene solo un hijo izquierdo
        }

        // El nodo tiene dos hijos, buscar el valor mínimo del subárbol derecho
        Nodo *temp = nodoConValorMinimo(nodo->derecha);
        nodo->usuario = temp->usuario;                                     // Copiar el contenido del sucesor
        nodo->derecha = eliminarNodo(nodo->derecha, temp->usuario.correo); // Eliminar el sucesor
    }

    // Actualizar la altura del nodo
    nodo->altura = 1 + std::max(obtenerAltura(nodo->izquierda), obtenerAltura(nodo->derecha));

    // Balancear el nodo después de la eliminación
    return balancearNodo(nodo);
}

// Función privada auxiliar para encontrar el nodo con el valor mínimo en un subárbol
ArbolAVL::Nodo *ArbolAVL::nodoConValorMinimo(Nodo *nodo)
{
    Nodo *actual = nodo;
    while (actual->izquierda != nullptr)
    {
        actual = actual->izquierda; // Encontrar el nodo más a la izquierda (mínimo)
    }
    return actual;
}

// Función privada auxiliar para buscar un nodo en el árbol
ArbolAVL::Nodo *ArbolAVL::buscarNodo(Nodo *nodo, const std::string &correo)
{
    if (nodo == nullptr || nodo->usuario.correo == correo)
    {
        return nodo; // Retorna el nodo si es nullptr o si el correo coincide
    }

    // Buscar en el subárbol izquierdo o derecho según la comparación del correo
    if (correo < nodo->usuario.correo)
    {
        return buscarNodo(nodo->izquierda, correo); // Buscar en el subárbol izquierdo
    }
    else
    {
        return buscarNodo(nodo->derecha, correo); // Buscar en el subárbol derecho
    }
}

// Método para realizar una rotación hacia la derecha
ArbolAVL::Nodo *ArbolAVL::rotacionDerecha(Nodo *nodo)
{
    Nodo *nuevoRaiz = nodo->izquierda; // El nodo que se convertirá en la nueva raíz
    Nodo *temp = nuevoRaiz->derecha;   // El subárbol derecho del nuevo nodo raíz

    // Realizar la rotación
    nuevoRaiz->derecha = nodo;
    nodo->izquierda = temp;

    // Actualizar alturas
    nodo->altura = 1 + std::max(obtenerAltura(nodo->izquierda), obtenerAltura(nodo->derecha));
    nuevoRaiz->altura = 1 + std::max(obtenerAltura(nuevoRaiz->izquierda), obtenerAltura(nuevoRaiz->derecha));

    return nuevoRaiz; // Nuevo nodo raíz
}

// Método para realizar una rotación hacia la izquierda
ArbolAVL::Nodo *ArbolAVL::rotacionIzquierda(Nodo *nodo)
{
    Nodo *nuevoRaiz = nodo->derecha;   // El nodo que se convertirá en la nueva raíz
    Nodo *temp = nuevoRaiz->izquierda; // El subárbol izquierdo del nuevo nodo raíz

    // Realizar la rotación
    nuevoRaiz->izquierda = nodo;
    nodo->derecha = temp;

    // Actualizar alturas
    nodo->altura = 1 + std::max(obtenerAltura(nodo->izquierda), obtenerAltura(nodo->derecha));
    nuevoRaiz->altura = 1 + std::max(obtenerAltura(nuevoRaiz->izquierda), obtenerAltura(nuevoRaiz->derecha));

    return nuevoRaiz; // Nuevo nodo raíz
}

// Método para balancear un nodo
ArbolAVL::Nodo *ArbolAVL::balancearNodo(Nodo *nodo)
{
    int balance = obtenerBalance(nodo); // Obtener el factor de balance del nodo

    // Caso 1: Desbalanceo a la izquierda
    if (balance > 1)
    {
        // Caso 1.1: Subárbol izquierdo desbalanceado a la derecha
        if (obtenerBalance(nodo->izquierda) < 0)
        {
            nodo->izquierda = rotacionIzquierda(nodo->izquierda); // Rotación izquierda en el subárbol izquierdo
        }
        return rotacionDerecha(nodo); // Rotación derecha en el nodo actual
    }

    // Caso 2: Desbalanceo a la derecha
    if (balance < -1)
    {
        // Caso 2.1: Subárbol derecho desbalanceado a la izquierda
        if (obtenerBalance(nodo->derecha) > 0)
        {
            nodo->derecha = rotacionDerecha(nodo->derecha); // Rotación derecha en el subárbol derecho
        }
        return rotacionIzquierda(nodo); // Rotación izquierda en el nodo actual
    }

    return nodo; // Nodo balanceado
}

// Método para recorrer el árbol in-order
void ArbolAVL::inOrdenRecursivo(Nodo *nodo, ListaEnlazada::ListaEnlazada<Structs::Usuario> &lista)
{
    if (nodo == nullptr)
    {
        return;
    }
    inOrdenRecursivo(nodo->izquierda, lista);
    lista.insertar(nodo->usuario);
    inOrdenRecursivo(nodo->derecha, lista);
}

// Función recursiva privada que realiza el recorrido pre-order
void ArbolAVL::preOrdenRecursivo(Nodo *nodo, ListaEnlazada::ListaEnlazada<Structs::Usuario> &lista)
{
    if (nodo == nullptr)
    {
        return;
    }
    lista.insertar(nodo->usuario);
    preOrdenRecursivo(nodo->izquierda, lista);
    preOrdenRecursivo(nodo->derecha, lista);
}

// Función recursiva privada que realiza el recorrido post-order
void ArbolAVL::postOrdenRecursivo(Nodo *nodo, ListaEnlazada::ListaEnlazada<Structs::Usuario> &lista)
{
    if (nodo == nullptr)
    {
        return;
    }
    postOrdenRecursivo(nodo->izquierda, lista);
    postOrdenRecursivo(nodo->derecha, lista);
    lista.insertar(nodo->usuario);
}

// Método privado para recorrer el árbol y escribir en formato DOT
void ArbolAVL::graficarNodo(Nodo *nodo, std::ofstream &archivoDot)
{
    if (nodo == nullptr)
    {
        return;
    }

    // Escribir el nodo actual
    archivoDot << "\"" << nodo->usuario.correo << "\" [label=\"" << nodo->usuario.correo << "\"];" << std::endl;

    // Recursivamente escribir los hijos
    if (nodo->izquierda != nullptr)
    {
        archivoDot << "\"" << nodo->usuario.correo << "\" -> \"" << nodo->izquierda->usuario.correo << "\";" << std::endl;
        graficarNodo(nodo->izquierda, archivoDot);
    }

    if (nodo->derecha != nullptr)
    {
        archivoDot << "\"" << nodo->usuario.correo << "\" -> \"" << nodo->derecha->usuario.correo << "\";" << std::endl;
        graficarNodo(nodo->derecha, archivoDot);
    }
}
// Método para obtener el usuario con el mayor ID
Structs::Usuario ArbolAVL::obtenerUsuarioConMayorID()
{
    ListaEnlazada::ListaEnlazada<Structs::Usuario> listaUsuarios = InOrder();
    Structs::Usuario *usuarioConMayorID;
    int mayorID = std::numeric_limits<int>::min();
    for (int i = 0; i < listaUsuarios.size(); ++i) {
        Structs::Usuario *temp = listaUsuarios.obtener(i);
        if (temp->id > mayorID)
        {
            mayorID = temp->id;
            usuarioConMayorID = temp;
        }
    }
    return *usuarioConMayorID;
}
