//
// Created by navent on 10/10/17.
//

#include <vector>
#include "Process.h"
#include "../model/ClaveJugador.h"
#include "../utils/ipc/shared-memory/MemoriaCompartida.h"
#include "../model/Pareja.h"
#include "../utils/ipc/pipe/Pipe.h"
#include "../utils/ipc/semaphore/Semaforo.h"

#ifndef RESTO_CUPIDO_H
#define RESTO_CUPIDO_H

const string cupidoProcessLogId = "Cupido";


class CupidoProcess : public Process {

private:

    Pipe *jugadores;


    vector<ClaveJugador> jugadoresSinPareja;

    vector<vector<bool>> matrizDeMatcheo;

    vector<vector<Semaforo>> *semCanchasLibres;

    vector<vector<MemoriaCompartida<bool>>> *shmCanchasLibres;

    vector<Semaforo> *semsTerminoDeJugar;


    Semaforo *semCupido;
    Semaforo *semCantCanchasLibres;
    Pipe *pipeResultados;
    Pipe *pipeFixture;


    int cantNJugadores;

    Pareja *parejaEnEspera;

    void inicializarPartido(Pareja *pareja1, Pareja *pareja2, vector<vector<Semaforo>> *semCanchasLibres,
                            vector<vector<MemoriaCompartida<bool>>> *shmCanchasLibres,
                            vector<Semaforo> *semTerminoDeJugar, Semaforo *semCantCanchasLibres, Pipe *pipeResultados, Pipe *pipeFixture);


public:

    CupidoProcess(Pipe *jugadores, vector<vector<Semaforo>> *semCanchasLibres,
                  vector<vector<MemoriaCompartida<bool>>> *shmCanchasLibres, int cantNJugadores, Semaforo *semCupido,
                  vector<Semaforo> *semsTerminoDeJugar, Semaforo *semCantCanchasLibres, Pipe *pipeResultados, Pipe *pipeFixture);

    void run();

    void limpiarRecursos();

    virtual ~CupidoProcess();

};


#endif //RESTO_CUPIDO_H
