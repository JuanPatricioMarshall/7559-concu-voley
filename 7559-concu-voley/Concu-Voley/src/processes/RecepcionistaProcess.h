
//
// Created by navent on 10/10/17.
//

#ifndef RESTO_RECEPCIONISTAPROCESS_H
#define RESTO_RECEPCIONISTAPROCESS_H

#include "../main/MainProcess.h"
#include "Process.h"

const string recepcionistaLogId = "Recepcionista";



class RecepcionistaProcess : public Process {

private:

    Semaforo *semEsperarRecepcionista;
    Semaforo *semCupido;

public:

    RecepcionistaProcess(Semaforo *semEsperarRecepcionista, Semaforo *semCupido);

    void run();

    void limpiarRecursos();

    virtual ~RecepcionistaProcess();

};


#endif //RESTO_RECEPCIONISTAPROCESS_H
