#ifndef LISTASOLICITUDES_H
#define LISTASOLICITUDES_H

#include <iostream>
#include <string>

using namespace std;

namespace ListaSolicitudes
{
    struct Solicitud
    {
        string emisor;
        string receptor;
        string estado;
    };

}

#endif // LISTASOLICITUDES_H