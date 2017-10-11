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

#include "../model/Comida.h"
#include "../model/Pedido.h"
#include "../model/Plato.h"
#include "../utils/random/RandomUtil.h"
#include "../utils/serializer/LlamadoAMozoSerializer.h"
#include "../utils/serializer/ClaveJugadorSerializer.h"
#include <unistd.h>

using namespace std;

JugadorProcess::JugadorProcess(int cantidadDePartidosPendientes,
                               vector<Semaforo> *semPartidoTerminado, int indice,
                               Semaforo *semEsperarRecepcionista, Semaforo *semJugadoresPredio, Pipe *pipeJugadores) {

    this->cantidadDePartidosPendientes = cantidadDePartidosPendientes;

    this->semPartidoTerminado = semPartidoTerminado;
    this->indice = indice;
    this->semEsperarRecepcionista = semEsperarRecepcionista;
    this->semJugadoresPredio = semJugadoresPredio;
    this->pipeJugadores = pipeJugadores;

    inicializarHandler();
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

    this->pipeJugadores->escribir(static_cast<const void*>(claveJugadorStr.c_str()), claveJugadorStr.size());

    Logger::log(jugadorLogId, "Jugador escribio en pipeJugadores: " + claveJugadorStr, DEBUG);


}

void JugadorProcess::jugar() {

    this->semPartidoTerminado->at(indice).p();

    Logger::log(jugadorLogId, "Jugador " + Logger::intToString(indice) + " termino de jugar", DEBUG);

    this->cantidadDePartidosPendientes -= 1;

    decidirQueHacer();


}

void JugadorProcess::decidirQueHacer() {

    Logger::log(jugadorLogId, "Jugador " + Logger::intToString(indice) + " decide que hacer", DEBUG);


    if(cantidadDePartidosPendientes == 0){
        Logger::log(jugadorLogId, "Jugador " + Logger::intToString(indice) + " se va porque no tiene partidos pendientes", DEBUG);

        terminar();
        this->semJugadoresPredio->v();

    }
    else{
        if(RandomUtil::randomCeroUno() < TiemposEspera::PROBABILIDAD_IRSE){
            Logger::log(jugadorLogId, "Jugador " + Logger::intToString(indice) + " decide volver a jugar", DEBUG);

            jugar();
        }
        else{
            Logger::log(jugadorLogId, "Jugador " + Logger::intToString(indice) + " decide irse", DEBUG);

            this->semJugadoresPredio->v();

            sleep(TiemposEspera::TIEMPO_AFUERA);
            Logger::log(jugadorLogId, "Jugador " + Logger::intToString(indice) + " vuelve a entrar", DEBUG);


            reentrar();
        }
    }

}

void JugadorProcess::reentrar(){

    llegar();
    jugar();

}


void JugadorProcess::terminar(){


    limpiarRecursos();

}


void JugadorProcess::limpiarRecursos() {
    Logger::log(jugadorLogId, "Limpiando recursos", DEBUG);
//        liberarMemoriasCompartidas();
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

