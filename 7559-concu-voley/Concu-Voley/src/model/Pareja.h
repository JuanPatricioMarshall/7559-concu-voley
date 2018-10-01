//
// Created by navent on 10/10/17.
//

#ifndef CONCU_PAREJA_H
#define CONCU_PAREJA_H


#include "ClaveJugador.h"

class Pareja {

private:

    ClaveJugador* claveJugador1;

    ClaveJugador* claveJugador2;

public:

    Pareja(ClaveJugador* claveJugador1, ClaveJugador* claveJugador2);


    ClaveJugador* getClaveJugador1();

    ClaveJugador* getClaveJugador2();

    void setClaveJugador1(ClaveJugador* clave);

    void setClaveJugador2(ClaveJugador* clave);

    virtual ~Pareja();
};


#endif //CONCU_PAREJA_H
