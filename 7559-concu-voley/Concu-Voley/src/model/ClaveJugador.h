//
// Created by navent on 10/10/17.
//

#ifndef CONCU_CLAVEJUGADOR_H
#define CONCU_CLAVEJUGADOR_H

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


#endif //CONCU_CLAVEJUGADOR_H
