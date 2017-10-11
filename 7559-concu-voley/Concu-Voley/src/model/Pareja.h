//
// Created by navent on 10/10/17.
//

#ifndef RESTO_PAREJA_H
#define RESTO_PAREJA_H


#include "ClaveJugador.h"

class Pareja {

private:

    ClaveJugador* claveJugador1;

    ClaveJugador* claveJugador2;

public:

    Pareja(ClaveJugador* claveJugador1, ClaveJugador* claveJugador2);


    ClaveJugador* getClaveJugador1();

    ClaveJugador* getClaveJugador2();

    virtual ~Pareja();
};


#endif //RESTO_PAREJA_H
