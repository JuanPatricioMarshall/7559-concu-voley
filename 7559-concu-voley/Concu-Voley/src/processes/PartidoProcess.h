//
// Created by navent on 10/10/17.
//

#ifndef RESTO_PARTIDOPROCESS_H
#define RESTO_PARTIDOPROCESS_H


#include "../model/Pareja.h"
#include "../utils/ipc/semaphore/Semaforo.h"
#include "Process.h"
#include <vector>
#include "Process.h"
#include "../model/ClaveJugador.h"
#include "../utils/ipc/shared-memory/MemoriaCompartida.h"
#include "../model/Pareja.h"
#include "../utils/ipc/pipe/Pipe.h"
#include "../utils/ipc/semaphore/Semaforo.h"
#include "../model/Cancha.h"
#include "../model/Resultado.h"

const string partidoLogId = "Partido";


class PartidoProcess : public Process {

private:

    Pareja *pareja1;
    Pareja *pareja2;
    vector<vector<Semaforo>> *semCanchasLibres;
    vector<vector<MemoriaCompartida<bool>>> *shmCanchasLibres;
    vector<Semaforo> *semTerminoDeJugar;
    Semaforo *semCantCanchasLibres;
    Pipe *pipeResultados;
    Pipe *pipeFixture;


    Cancha *cancha;


    void encontrarCancha();
    void liberarCancha();
    void avisarJugadores();

    Resultado simularPartido();

    void inicializarMemoriasCompartidas();


public:


    PartidoProcess(Pareja *pareja1, Pareja *pareja2, vector<vector<Semaforo>> *semCanchasLibres,
                   vector<vector<MemoriaCompartida<bool>>> *shmCanchasLibres, vector<Semaforo> *semTerminoDeJugar,
                   Semaforo *semCantCanchasLibres, Pipe *pipeResultados, Pipe *pipeFixture);

    void run();

    void limpiarRecursos();

    void finalizar();

    ~PartidoProcess();

};


#endif //RESTO_PARTIDOPROCESS_H
