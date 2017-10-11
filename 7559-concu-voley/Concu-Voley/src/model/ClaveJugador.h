//
// Created by navent on 10/10/17.
//

#ifndef RESTO_CLAVEJUGADOR_H
#define RESTO_CLAVEJUGADOR_H

#include <time.h>





class ClaveJugador {

private:

    pid_t pid;
    int indice;

public:

    ClaveJugador(pid_t pid, int indice);


    pid_t  getPid();
    int getIndice();

    virtual ~ClaveJugador();

};


#endif //RESTO_CLAVEJUGADOR_H
