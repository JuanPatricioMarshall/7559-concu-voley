//
// Created by navent on 10/10/17.
//

#include "RecepcionistaProcess.h"


using namespace std;

RecepcionistaProcess::RecepcionistaProcess(Semaforo *semEsperarRecepcionista, Semaforo *semCupido, int cantJugadoresMinimo) {

    this->semEsperarRecepcionista = semEsperarRecepcionista;

    this->semCupido = semCupido;

    this->cantJugadoresMinimo = cantJugadoresMinimo;

    inicializarHandler();

}


void RecepcionistaProcess::run() {
    Logger::log(recepcionistaLogId, "Iniciando Recepcionista ", DEBUG);

    for (int i = 0; i < cantJugadoresMinimo; i++) {
        this->semEsperarRecepcionista->p();
        Logger::log(recepcionistaLogId, "Recepcionista recibio a 1 jugador ", INFO);

    }

    Logger::log(recepcionistaLogId, "Dejo pasar a "+ Logger::intToString(cantJugadoresMinimo)  +" jugadores ", INFO);

    this->semCupido->v();

    Logger::log(recepcionistaLogId, "Empieza el torneo ", INFO);

}


RecepcionistaProcess::~RecepcionistaProcess() {
}

void RecepcionistaProcess::limpiarRecursos(){

}





