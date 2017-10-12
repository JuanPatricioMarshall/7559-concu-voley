//
// Created by navent on 10/10/17.
//

#include "PartidoProcess.h"
#include "../model/Cancha.h"

#include "TiemposEspera.h"
#include "../utils/serializer/ResultadoSerializer.h"
#include "../main/MainProcess.h"

PartidoProcess::PartidoProcess(Pareja *pareja1, Pareja *pareja2, vector<vector<Semaforo>> *semCanchasLibres,
                               vector<vector<MemoriaCompartida<bool>>> *shmCanchasLibres,
                               vector<Semaforo> *semTerminoDeJugar, Semaforo *semCantCanchasLibres,
                               Pipe *pipeResultados, Pipe *pipeFixture, MemoriaCompartida<int> *shmNivelDeMarea,
                               Semaforo *semNivelDeMarea) {


    this->pareja1 = pareja1;
    this->pareja2 = pareja2;
    this->semCanchasLibres = semCanchasLibres;
    this->shmCanchasLibres = shmCanchasLibres;
    this->semTerminoDeJugar = semTerminoDeJugar;
    this->semCantCanchasLibres = semCantCanchasLibres;
    this->pipeResultados = pipeResultados;
    this->pipeFixture = pipeFixture;
    this->semNivelDeMarea = semNivelDeMarea;
    this->shmNivelDeMarea = shmNivelDeMarea;

    this->cancha = new Cancha(-1, -1);

    inicializarHandler();
    inicializarMemoriasCompartidas();


}

void PartidoProcess::inicializarMemoriasCompartidas() {


    int cont = 0;
    for (unsigned int i = 0; i < shmCanchasLibres->size(); i++) {
        for (unsigned int j = 0; j < shmCanchasLibres->at(0).size(); j++) {
            shmCanchasLibres->at(i).at(j).crear(SHM_CANCHAS_LIBRES, cont);
            cont ++;
        }
    }

}


void PartidoProcess::run() {


    Logger::log(partidoProcessLogId, "Las parejas buscan cancha ", DEBUG);
    Logger::log(partidoProcessLogId, "Tamaño" + Logger::intToString(semCanchasLibres->size()), INFO);


    encontrarCancha();

    Logger::log(partidoProcessLogId, "Las parejas encontraron cancha ", DEBUG);

    Resultado resultado = simularPartido();


    Logger::log(partidoProcessLogId, "El partido termino " + Logger::intToString(resultado.getSetsPareja1()) + " a " +
                                     Logger::intToString(resultado.getSetsPareja2()), INFO);


    string resultadoStr = ResultadoSerializer::serializar(&resultado);

    this->pipeResultados->escribir(static_cast<const void *>(resultadoStr.c_str()), resultadoStr.size());
    this->pipeFixture->escribir(static_cast<const void *>(resultadoStr.c_str()), resultadoStr.size());


    //Este orden estara bien?
    liberarCancha();
    avisarJugadores();


    //TODO
    limpiarRecursos();
    //TODO
    finalizar();


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
    Logger::log(partidoProcessLogId, "Tamaño" + Logger::intToString(semCanchasLibres->size()), INFO);


    Logger::log(partidoProcessLogId, "Partido buscando cancha. ", INFO);




    for (unsigned int i = 0; i < semCanchasLibres->size(); i++) {

        for (unsigned int j = 0; j < semCanchasLibres[0].size(); j++) {

            Logger::log(partidoProcessLogId,
                        "Partido esperando semsCanchasLibre: " + Logger::intToString(i) + ", " + Logger::intToString(j),
                        DEBUG);

            semCanchasLibres->at(i).at(j).p();

            Logger::log(partidoProcessLogId,
                        "Partido obtuvo semsCanchasLibre: " + Logger::intToString(i) + ", " + Logger::intToString(j),
                        DEBUG);

//            Logger::log(comensalLogId, "shmMesasLibre size: " + Logger::intToString(shmMesasLibres->size()), DEBUG);


            //TODO VER CAPAZ HAY QUE HACER * A *
            canchaLibre = shmCanchasLibres->at(i).at(j).leer();

            if (canchaLibre) {
                canchaF = i;
                canchaC = j;

                Logger::log(partidoProcessLogId,
                            "Cancha libre encontrada. En fila: " + Logger::intToString(canchaF) + " y columna: " +
                            Logger::intToString(canchaC), INFO);

                Logger::log(partidoProcessLogId,
                            "Cancha coupada. En fila: " + Logger::intToString(canchaF) + " y columna: " +
                            Logger::intToString(canchaC), INFO);


                shmCanchasLibres->at(i).at(j).escribir(false);

                cancha->setColumna(canchaC);
                cancha->setFila(canchaF);


            }

            Logger::log(partidoProcessLogId,
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

void PartidoProcess::liberarCancha() {

    int fila = cancha->getFila();
    int columna = cancha->getColumna();

    this->semCanchasLibres->at(fila).at(columna).p();
    this->shmCanchasLibres->at(fila).at(columna).escribir(true);
    Logger::log(partidoProcessLogId, "Cancha Liberada. En fila: " + Logger::intToString(fila) + " y columna: " +
                                     Logger::intToString(columna), INFO);

    this->semCanchasLibres->at(fila).at(columna).v();

    this->semCantCanchasLibres->v();
    Logger::log(partidoProcessLogId, "Aumento la cantidad de canchas libres", INFO);


}

void PartidoProcess::avisarJugadores() {

    ClaveJugador *jugador1 = this->pareja1->getClaveJugador1();
    ClaveJugador *jugador2 = this->pareja1->getClaveJugador2();

    ClaveJugador *jugador3 = this->pareja2->getClaveJugador1();
    ClaveJugador *jugador4 = this->pareja2->getClaveJugador2();

    Logger::log(partidoProcessLogId, "Empiezo a avisarle a los jugadores que termino su partido", INFO);
    this->semTerminoDeJugar->at(jugador1->getIndice()).v();
    this->semTerminoDeJugar->at(jugador2->getIndice()).v();
    this->semTerminoDeJugar->at(jugador3->getIndice()).v();
    this->semTerminoDeJugar->at(jugador4->getIndice()).v();
    Logger::log(partidoProcessLogId, "Termino de avisarle a los jugadores", INFO);


}


void PartidoProcess::limpiarRecursos() {

    Logger::log(partidoProcessLogId, "Limpiando recursos", DEBUG);
    liberarMemoriasCompartidas();
    Logger::log(partidoProcessLogId, "Termino de limpiar recursos", DEBUG);



}

void PartidoProcess::liberarMemoriasCompartidas() {

    for (unsigned int i = 0; i < shmCanchasLibres->size(); i++) {
        for (unsigned int j = 0; j < shmCanchasLibres->at(0).size(); j++) {
            shmCanchasLibres->at(i).at(j).liberar();
        }
    }

}

void PartidoProcess::finalizar() {

}

PartidoProcess::~PartidoProcess() {

}