
//
// Created by navent on 10/10/17.
//

#ifndef CONCU_RECEPCIONISTAPROCESS_H
#define CONCU_RECEPCIONISTAPROCESS_H

#include "../main/MainProcess.h"
#include "Process.h"

const string recepcionistaLogId = "Recepcionista";



class RecepcionistaProcess : public Process {

private:

    Semaforo *semEsperarRecepcionista;
    Semaforo *semCupido;
    int cantJugadoresMinimo;

public:

    RecepcionistaProcess(Semaforo *semEsperarRecepcionista, Semaforo *semCupido, int cantJugadoresMinimo);

    void run();

    void limpiarRecursos();

    virtual ~RecepcionistaProcess();

};


#endif //CONCU_RECEPCIONISTAPROCESS_H
