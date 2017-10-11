//
// Created by navent on 10/10/17.
//

#include "RecepcionistaProcess.h"


#include "CocineroProcess.h"

#include <unistd.h>
#include <iostream>
#include <string>

#include "../main/MainProcess.h"
#include "../model/Plato.h"
#include "../utils/serializer/LlamadoAMozoSerializer.h"

using namespace std;

RecepcionistaProcess::RecepcionistaProcess(Semaforo *semEsperarRecepcionista, Semaforo *semCupido) {

    this->semEsperarRecepcionista = semEsperarRecepcionista;

    this->semCupido = semCupido;

    inicializarHandler();

}


void RecepcionistaProcess::run() {
    Logger::log(recepcionistaLogId, "Iniciando Recepcionista ", DEBUG);

    for (int i = 0; i < 10; i++) {
        this->semEsperarRecepcionista->p();
        Logger::log(recepcionistaLogId, "Recepcionista recibio a 1 jugador ", INFO);

    }

    Logger::log(recepcionistaLogId, "Dejo pasar a 10 jugadores ", INFO);

    this->semCupido->v();

    Logger::log(recepcionistaLogId, "Empieza el torneo ", INFO);

}


RecepcionistaProcess::~RecepcionistaProcess() {
}

void RecepcionistaProcess::limpiarRecursos(){

}





