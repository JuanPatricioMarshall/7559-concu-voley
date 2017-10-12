//
// Created by navent on 09/10/17.
//

#include "JugadorProcess.h"


/*
 * GrupoComensalesProcess.cpp
 *
 *  Created on: Oct 15, 2016
 *      Author: gaston
 */

#include "GrupoComensalesProcess.h"

#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>


#include "../utils/random/RandomUtil.h"

#include "../utils/serializer/ClaveJugadorSerializer.h"
#include <unistd.h>

using namespace std;

JugadorProcess::JugadorProcess(int cantidadDePartidosPendientes,
                               vector<Semaforo> *semPartidoTerminado, int indice,
                               Semaforo *semEsperarRecepcionista, Semaforo *semJugadoresPredio, Pipe *pipeJugadores,
                               vector<MemoriaCompartida<bool>> *shmJugadoresSinPareja,
                               vector<Semaforo> *semJugadoresSinPareja) {

    this->cantidadDePartidosPendientes = cantidadDePartidosPendientes;

    this->semPartidoTerminado = semPartidoTerminado;
    this->indice = indice;
    this->semEsperarRecepcionista = semEsperarRecepcionista;
    this->semJugadoresPredio = semJugadoresPredio;
    this->pipeJugadores = pipeJugadores;
    this->semJugadoresSinPareja = semJugadoresSinPareja;
    this->shmJugadoresSinPareja = shmJugadoresSinPareja;

    inicializarHandler();
    inicializarMemoriasCompartidas();
}

void JugadorProcess::inicializarMemoriasCompartidas() {

    for (unsigned int i = 0; i < shmJugadoresSinPareja->size(); i++){
        semJugadoresSinPareja->at(i).p();
        shmJugadoresSinPareja->at(i).crear(SHM_JUGADORES_SIN_PAREJA, i);
        semJugadoresSinPareja->at(i).v();
    }

}


void JugadorProcess::run() {
    Logger::log(jugadorLogId, "Iniciando jugador", DEBUG);

    llegar();
    jugar();

}

void JugadorProcess::llegar() {

    Logger::log(jugadorLogId, "Jugador " + Logger::intToString(indice) + " tratando de entrar al predio", DEBUG);
    this->semJugadoresPredio->p();
    Logger::log(jugadorLogId, "Jugador " + Logger::intToString(indice) + " logro entrar al predio", DEBUG);

    Logger::log(jugadorLogId, "Jugador " + Logger::intToString(indice) + " le avisa a la recepcionista", DEBUG);
    this->semEsperarRecepcionista->v();
    Logger::log(jugadorLogId, "Jugador " + Logger::intToString(indice) + " le aviso a la recepcionista", DEBUG);


    ClaveJugador claveJugador(getpid(), indice);

    string claveJugadorStr = ClaveJugadorSerializer::serializar(&claveJugador);


    Logger::log(jugadorLogId, "Jugador escribiendo en pipeJugadores: " + claveJugadorStr, DEBUG);

    this->pipeJugadores->escribir(static_cast<const void *>(claveJugadorStr.c_str()), claveJugadorStr.size());

    Logger::log(jugadorLogId, "Jugador escribio en pipeJugadores: " + claveJugadorStr, DEBUG);


}

void JugadorProcess::jugar() {


    Logger::log(jugadorLogId, "Jugador esperando que termine el partido: ", DEBUG);

    this->semPartidoTerminado->at(indice).p();

    Logger::log(jugadorLogId, "Jugador ve que termino su partido/o que no habia podido conseguir pareja: ", DEBUG);


    this->semJugadoresSinPareja->at(indice).p();

    Logger::log(jugadorLogId, "Me fijo si habia encontrado pareja: ", DEBUG);


    bool consiguioPareja = this->shmJugadoresSinPareja->at(indice).leer();

    if(consiguioPareja) {
        Logger::log(jugadorLogId, "No encontre pareja: " + consiguioPareja, DEBUG);
    }
    Logger::log(jugadorLogId, "Encontre pareja: " + consiguioPareja, DEBUG);


    this->semJugadoresSinPareja->at(indice).v();


    if (consiguioPareja) {

        this->semJugadoresPredio->p();
        Logger::log(jugadorLogId, "Jugador se va del predio", DEBUG);
        terminar();

    }


    Logger::log(jugadorLogId, "Jugador " + Logger::intToString(indice) + " termino de jugar", DEBUG);

    this->cantidadDePartidosPendientes -= 1;

    Logger::log(jugadorLogId, "Tengo " + Logger::intToString(cantidadDePartidosPendientes) + " cantidad de partidos pendientes", DEBUG);


    decidirQueHacer();


}

void JugadorProcess::decidirQueHacer() {

    Logger::log(jugadorLogId, "Jugador " + Logger::intToString(indice) + " decide que hacer", DEBUG);


    if (cantidadDePartidosPendientes == 0) {
        Logger::log(jugadorLogId,
                    "Jugador " + Logger::intToString(indice) + " se va porque no tiene partidos pendientes", DEBUG);

        terminar();
        this->semJugadoresPredio->v();

    } else {
        if (RandomUtil::randomCeroUno() < TiemposEspera::PROBABILIDAD_IRSE) {
            Logger::log(jugadorLogId, "Jugador " + Logger::intToString(indice) + " decide volver a jugar", DEBUG);

            jugar();
        } else {
            Logger::log(jugadorLogId, "Jugador " + Logger::intToString(indice) + " decide irse", DEBUG);

            this->semJugadoresPredio->v();

            sleep(TiemposEspera::TIEMPO_AFUERA);
            Logger::log(jugadorLogId, "Jugador " + Logger::intToString(indice) + " vuelve a entrar", DEBUG);


            reentrar();
        }
    }

}

void JugadorProcess::reentrar() {

    llegar();
    jugar();

}


void JugadorProcess::terminar() {


    limpiarRecursos();

}

void JugadorProcess::liberarMemoriasCompartidas() {


    for (unsigned int i = 0; i < shmJugadoresSinPareja->size(); i++) {
        shmJugadoresSinPareja->at(i).liberar();
    }

}


void JugadorProcess::limpiarRecursos() {
    Logger::log(jugadorLogId, "Limpiando recursos", DEBUG);
    liberarMemoriasCompartidas();
}


//    void GrupoComensalesProcess::liberarMemoriasCompartidas(){
//        this->shmPersonasLiving->liberar();
//
//        for (unsigned int i = 0; i < shmMesasLibres->size(); i++){
//            shmMesasLibres->at(i).liberar();
//        }
//
//    }

JugadorProcess::~JugadorProcess() {
//        liberarMemoriasCompartidas();
}

/* namespace std */

