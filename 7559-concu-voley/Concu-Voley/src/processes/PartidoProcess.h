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

const string partidoProcessLogId = "Partido";


class PartidoProcess : public Process {

private:

    Pareja *pareja1;
    Pareja *pareja2;
    vector<vector<Semaforo>> *semCanchasLibres;
    vector<vector<MemoriaCompartida<bool>>> *shmCanchasLibres;
    vector<vector<MemoriaCompartida<int>>> *resultadosFinales;
    vector<Semaforo> *semTerminoDeJugar;
    Semaforo *semCantCanchasLibres;
    Pipe *pipeResultados;
    Pipe *pipeFixture;
    int cantidadJugadores;
    int cantPartidosJugador;
    MemoriaCompartida<int> *shmNivelDeMarea;
    Semaforo *semNivelDeMarea;


    Cancha *cancha;


    void liberarCancha();

    void avisarJugadores();

    void inicializarMemoriasCompartidas();

    void liberarMemoriasCompartidas();

public:


    PartidoProcess(int cantidadJugadores,int cantPartidosJugador, vector<vector<MemoriaCompartida<int>>> *resultadosFinales,int fila,int columna,Pareja *pareja1, Pareja *pareja2, vector<vector<Semaforo>> *semCanchasLibres,
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
