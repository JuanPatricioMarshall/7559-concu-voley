//
// Created by navent on 10/10/17.
//

#include "ClaveJugador.h"


ClaveJugador::ClaveJugador(pid_t pid, int indice){

    this->pid = pid;
    this->indice = indice;

}

pid_t  ClaveJugador::getPid(){

    return pid;
}
int ClaveJugador::getIndice(){

    return indice;
}

ClaveJugador::~ClaveJugador(){

}