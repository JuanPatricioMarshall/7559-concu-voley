//
// Created by navent on 10/10/17.
//

#include "PartidoProcess.h"
#include "../model/Cancha.h"

#include "TiemposEspera.h"
#include "../utils/serializer/ResultadoSerializer.h"

PartidoProcess::PartidoProcess(Pareja *pareja1, Pareja *pareja2, vector<vector<Semaforo>> *semCanchasLibres,
                               vector<vector<MemoriaCompartida<bool>>> *shmCanchasLibres,
                               vector<Semaforo> *semTerminoDeJugar, Semaforo *semCantCanchasLibres,
                               Pipe *pipeResultados, Pipe *pipeFixture) {


    this->pareja1 = pareja1;
    this->pareja2 = pareja2;
    this->semCanchasLibres = semCanchasLibres;
    this->shmCanchasLibres = shmCanchasLibres;
    this->semTerminoDeJugar = semTerminoDeJugar;
    this->semCantCanchasLibres = semCantCanchasLibres;
    this->pipeResultados = pipeResultados;
    this->pipeFixture = pipeFixture;

    this->cancha = new Cancha(-1, -1);

    inicializarHandler();
    inicializarMemoriasCompartidas();


}


void PartidoProcess::run() {

    encontrarCancha();
    Resultado resultado = simularPartido();


    string resultadoStr = ResultadoSerializer::serializar(&resultado);

    this->pipeResultados->escribir(static_cast<const void*>(resultadoStr.c_str()), resultadoStr.size());
    this->pipeFixture->escribir(static_cast<const void*>(resultadoStr.c_str()), resultadoStr.size());


    //Este orden estara bien?
    liberarCancha();
    avisarJugadores();


    //TODO
    limpiarRecursos();
    //TODO
    finalizar();


}

void PartidoProcess::inicializarMemoriasCompartidas(){

}

Resultado PartidoProcess::simularPartido() {

    sleep(TiemposEspera::TIEMPO_JUGAR);

    return Resultado(this->pareja1, this->pareja2, true);

}

void PartidoProcess::encontrarCancha() {

    //TODO
    //ACORDARSE DE CUANDO TERMINA DE HACE V
    semCantCanchasLibres->p();

    int canchaC = -1;
    int canchaF = -1;

    bool canchaLibre = false;

    Logger::log(partidoLogId, "Partido buscando cancha. ", INFO);

    for (unsigned int i = 0; i < semCanchasLibres->size(); i++) {

        for (unsigned int j = 0; j < semCanchasLibres[0].size(); j++) {

            Logger::log(partidoLogId,
                        "Partido esperando semsCanchasLibre: " + Logger::intToString(i) + ", " + Logger::intToString(j),
                        DEBUG);

            semCanchasLibres->at(i).at(j).p();

            Logger::log(partidoLogId,
                        "Partido obtuvo semsCanchasLibre: " + Logger::intToString(i) + ", " + Logger::intToString(j),
                        DEBUG);

//            Logger::log(comensalLogId, "shmMesasLibre size: " + Logger::intToString(shmMesasLibres->size()), DEBUG);


            //TODO VER CAPAZ HAY QUE HACER * A *
            canchaLibre = shmCanchasLibres->at(i).at(j).leer();

            if (canchaLibre) {
                canchaF = i;
                canchaC = j;

                Logger::log(partidoLogId,
                            "Cancha libre encontrada. En fila: " + Logger::intToString(canchaF) + " y columna: " +
                            Logger::intToString(canchaC), INFO);

                Logger::log(partidoLogId, "Cancha coupada. En fila: " + Logger::intToString(canchaF) + " y columna: " +
                                          Logger::intToString(canchaC), INFO);


                shmCanchasLibres->at(i).at(j).escribir(false);

                cancha->setColumna(canchaC);
                cancha->setFila(canchaF);


            }

            Logger::log(partidoLogId,
                        "Partido liberando semaforo en En fila: " + Logger::intToString(i) + " y columna: " +
                        Logger::intToString(j), DEBUG);

            semCanchasLibres->at(i).at(j).v();

            if (canchaLibre) {
                break;
            }

        }

        if (canchaLibre) {
            break;
        }

    }


}

void PartidoProcess::liberarCancha(){

    int fila = cancha->getFila();
    int columna = cancha->getColumna();

    this->semCanchasLibres->at(fila).at(columna).p();
    this->shmCanchasLibres->at(fila).at(columna).escribir(true);
    Logger::log(partidoLogId, "Cancha Liberada. En fila: " + Logger::intToString(fila) + " y columna: " +
                              Logger::intToString(columna), INFO);

    this->semCanchasLibres->at(fila).at(columna).v();

    this->semCantCanchasLibres->v();
    Logger::log(partidoLogId, "Aumento la cantidad de canchas libres", INFO);


}

void PartidoProcess::avisarJugadores(){

    ClaveJugador* jugador1 = this->pareja1->getClaveJugador1();
    ClaveJugador* jugador2 = this->pareja1->getClaveJugador2();

    ClaveJugador* jugador3 = this->pareja2->getClaveJugador1();
    ClaveJugador* jugador4 = this->pareja2->getClaveJugador2();

    //Acceso entre distintos partidos al vector de semaforos? Lapsus mental
    this->semTerminoDeJugar->at(jugador1->getIndice()).v();
    this->semTerminoDeJugar->at(jugador2->getIndice()).v();
    this->semTerminoDeJugar->at(jugador3->getIndice()).v();
    this->semTerminoDeJugar->at(jugador4->getIndice()).v();

}


void PartidoProcess::limpiarRecursos() {

}

void PartidoProcess::finalizar() {

}

PartidoProcess::~PartidoProcess() {

}