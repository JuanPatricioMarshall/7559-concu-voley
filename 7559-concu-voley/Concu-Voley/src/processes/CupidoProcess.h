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

#ifndef CONCU_CUPIDO_H
#define CONCU_CUPIDO_H

const string cupidoProcessLogId = "Cupido";


class CupidoProcess : public Process {

private:

    Pipe *jugadores;

    int filaPos;
    int colPos;
    vector<ClaveJugador> jugadoresSinPareja;
    vector<int >partidosFil;
    vector<int >partidosCol;
    vector<pid_t > partidos;
    vector<vector<bool>> matrizDeMatcheo;

    vector<int >partidosX;
    vector<int >partidosY;
    vector<vector<Semaforo>> *semCanchasLibres;

    vector<vector<MemoriaCompartida<int>>> *resultadosFinales;
    vector<vector<MemoriaCompartida<bool>>> *shmCanchasLibres;

    vector<Semaforo> *semsTerminoDeJugar;


    Semaforo *semCupido;
    Semaforo *semCantCanchasLibres;
    Pipe *pipeResultados;
    Pipe *pipeFixture;

    MemoriaCompartida<int> *shmNivelDeMarea;
    Semaforo *semNivelDeMarea;


    int cantNJugadores;
    int cantPartidosJugador;
    int cantJugadoresMinimosParaElTorneo;
    int predioC;
    int predioF;
    Pareja *parejaEnEspera;

    vector<MemoriaCompartida<bool>> *shmJugadoresSinPareja;
    vector<Semaforo> *semJugadoresSinPareja;
    Semaforo *semCantGenteEnElPredio;
    MemoriaCompartida<int> *shmCantGenteEnElPredio;


    void inicializarPartido(Pareja *pareja1, Pareja *pareja2,int filu,int colu);


    int leerTamanioClaveJugador();

    void inicializarMemoriasCompartidas();

    void liberarMemoriasCompartidas();


public:

    CupidoProcess(int predioF,int predioC,Pipe *jugadores, vector<vector<Semaforo>> *semCanchasLibres,
                  vector<vector<MemoriaCompartida<bool>>> *shmCanchasLibres,vector<vector<MemoriaCompartida<int>>> *resultadosFinales, int cantNJugadores,int cantPartidosJugador, Semaforo *semCupido,
                  vector<Semaforo> *semsTerminoDeJugar, Semaforo *semCantCanchasLibres, Pipe *pipeResultados,
                  Pipe *pipeFixture, int cantJugadoresMinimosParaElTorneo,
                  vector<MemoriaCompartida<bool>> *shmJugadoresSinPareja, MemoriaCompartida<int> *shmNivelDeMarea,
                  Semaforo *semNivelDeMarea, vector<Semaforo> *semJugadoresSinPareja, Semaforo *semCantGenteEnElPredio,
                  MemoriaCompartida<int> *shmCantGenteEnElPredio);

    void run();

    void limpiarRecursos();

    virtual ~CupidoProcess();


    void logTables(ClaveJugador claveJugador,int* pids,int* indices,int* datos);
    void buscarCancha(int *filu,int *colu);
    void logCanchas();
};


#endif //CONCU_CUPIDO_H
