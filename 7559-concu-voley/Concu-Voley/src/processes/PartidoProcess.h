//
// Created by navent on 10/10/17.
//

#ifndef CONCU_PARTIDOPROCESS_H
#define CONCU_PARTIDOPROCESS_H


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
#include "../utils/ipc/signal/SIGINT_Handler.h"
#include "../utils/ipc/signal/SIGUSR2_Handler.h"
#include "../utils/ipc/signal/SIGUSR1_Handler_Partidos.h"

const string partidoProcessLogId = "Partido";


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

    MemoriaCompartida<int> *shmNivelDeMarea;
    Semaforo *semNivelDeMarea;


    Cancha *cancha;

    SIGINT_Handler sigintHandler;
    SIGUSR1_Handler_Partidos *sigusr1Handler;


    void encontrarCancha();

    void liberarCancha();

    void avisarJugadores();

    Resultado simularPartido();

    void inicializarMemoriasCompartidas();

    void liberarMemoriasCompartidas();

public:


    PartidoProcess(Pareja *pareja1, Pareja *pareja2, vector<vector<Semaforo>> *semCanchasLibres,
                   vector<vector<MemoriaCompartida<bool>>> *shmCanchasLibres, vector<Semaforo> *semTerminoDeJugar,
                   Semaforo *semCantCanchasLibres, Pipe *pipeResultados, Pipe *pipeFixture,
                   MemoriaCompartida<int> *shmNivelDeMarea, Semaforo *semNivelDeMarea);

    void run();

    void limpiarRecursos();

    void finalizar();

    ~PartidoProcess();

    void inicializarHandler();

    void handleSubida();

    void handleBajada();
};


#endif //CONCU_PARTIDOPROCESS_H
