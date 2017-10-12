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


void Pareja::setClaveJugador1(ClaveJugador* clave){

    this->claveJugador1 = new ClaveJugador(clave->getPid(), clave->getIndice());

}

void Pareja::setClaveJugador2(ClaveJugador* clave){
    this->claveJugador2 = new ClaveJugador(clave->getPid(), clave->getIndice());

}


Pareja::~Pareja(){

}