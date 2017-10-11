//
// Created by navent on 10/10/17.
//

#include "Pareja.h"


Pareja::Pareja(ClaveJugador* claveJugador1, ClaveJugador* claveJugador2){

    this->claveJugador1 = claveJugador1;
    this->claveJugador2 = claveJugador2;
}


ClaveJugador* Pareja::getClaveJugador1(){
    return this->claveJugador1;
}

ClaveJugador* Pareja::getClaveJugador2(){
    return this->claveJugador2;
}

Pareja::~Pareja(){

}