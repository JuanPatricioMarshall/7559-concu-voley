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
#include "../utils/ipc/signal/SIGINT_Handler.h"
#include "../utils/ipc/signal/SIGUSR1_Handler.h"
#include "../utils/ipc/signal/SIGUSR2_Handler.h"

#ifndef RESTO_CUPIDO_H
#define RESTO_CUPIDO_H

const string cupidoProcessLogId = "Cupido";


class CupidoProcess : public Process {

private:

    Pipe *jugadores;


    vector<ClaveJugador> jugadoresSinPareja;

    vector<vector<bool>> matrizDeMatcheo;


    vector<pid_t > partidos;

    vector<vector<Semaforo>> *semCanchasLibres;

    vector<vector<MemoriaCompartida<bool>>> *shmCanchasLibres;

    vector<Semaforo> *semsTerminoDeJugar;


    Semaforo *semCupido;
    Semaforo *semCantCanchasLibres;
    Pipe *pipeResultados;
    Pipe *pipeFixture;

    MemoriaCompartida<int> *shmNivelDeMarea;
    Semaforo *semNivelDeMarea;


    int cantNJugadores;

    int cantJugadoresMinimosParaElTorneo;

    Pareja *parejaEnEspera;

    vector<MemoriaCompartida<bool>> *shmJugadoresSinPareja;
    vector<Semaforo> *semJugadoresSinPareja;
    Semaforo *semCantGenteEnElPredio;
    MemoriaCompartida<int> *shmCantGenteEnElPredio;


    SIGINT_Handler sigintHandler;
    SIGUSR1_Handler sigusr1Handler;
    SIGUSR2_Handler sigusr2Handler;



    void inicializarPartido(Pareja *pareja1, Pareja *pareja2);


    int leerTamanioClaveJugador();

    void inicializarMemoriasCompartidas();

    void liberarMemoriasCompartidas();

    void inicializarHandler();


public:

    CupidoProcess(Pipe *jugadores, vector<vector<Semaforo>> *semCanchasLibres,
                  vector<vector<MemoriaCompartida<bool>>> *shmCanchasLibres, int cantNJugadores, Semaforo *semCupido,
                  vector<Semaforo> *semsTerminoDeJugar, Semaforo *semCantCanchasLibres, Pipe *pipeResultados,
                  Pipe *pipeFixture, int cantJugadoresMinimosParaElTorneo,
                  vector<MemoriaCompartida<bool>> *shmJugadoresSinPareja, MemoriaCompartida<int> *shmNivelDeMarea,
                  Semaforo *semNivelDeMarea, vector<Semaforo> *semJugadoresSinPareja, Semaforo *semCantGenteEnElPredio,
                  MemoriaCompartida<int> *shmCantGenteEnElPredio);

    void run();

    void limpiarRecursos();

    virtual ~CupidoProcess();

    void handleSubida();
    void logTables(ClaveJugador claveJugador,int* pids,int* indices,int* datos);

    void handleBajada();
};


#endif //RESTO_CUPIDO_H
