//
// Created by navent on 09/10/17.
//

#include "JugadorProcess.h"


#include "../utils/random/RandomUtil.h"

#include "../utils/serializer/ClaveJugadorSerializer.h"

using namespace std;

JugadorProcess::JugadorProcess(int cantidadDePartidosPendientes,
                               vector<Semaforo> *semPartidoTerminado, int indice,
                               Semaforo *semEsperarRecepcionista, Semaforo *semJugadoresPredio, Pipe *pipeJugadores,
                               vector<MemoriaCompartida<bool>> *shmJugadoresSinPareja,
                               vector<Semaforo> *semJugadoresSinPareja, MemoriaCompartida<int> *shmCantGenteEnElPredio,
                               Semaforo *semCantGenteEnElPredio) {

    this->cantidadDePartidosPendientes = cantidadDePartidosPendientes;

    this->semPartidoTerminado = semPartidoTerminado;
    this->indice = indice;
    this->semEsperarRecepcionista = semEsperarRecepcionista;
    this->semJugadoresPredio = semJugadoresPredio;
    this->pipeJugadores = pipeJugadores;
    this->semJugadoresSinPareja = semJugadoresSinPareja;
    this->shmJugadoresSinPareja = shmJugadoresSinPareja;
    this->shmCantGenteEnElPredio = shmCantGenteEnElPredio;
    this->semCantGenteEnElPredio = semCantGenteEnElPredio;

    inicializarHandler();
    inicializarMemoriasCompartidas();
}

void JugadorProcess::inicializarMemoriasCompartidas() {

    for (unsigned int i = 0; i < shmJugadoresSinPareja->size(); i++) {
        semJugadoresSinPareja->at(i).p();
        shmJugadoresSinPareja->at(i).crear(SHM_JUGADORES_SIN_PAREJA, i);
        semJugadoresSinPareja->at(i).v();
    }

    semCantGenteEnElPredio->p();
    shmCantGenteEnElPredio->crear(SHM_GENTE_EN_EL_PREDIO, 0);
    semCantGenteEnElPredio->v();
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

    this->semCantGenteEnElPredio->p();
    int cant = this->shmCantGenteEnElPredio->leer();
    this->shmCantGenteEnElPredio->escribir(cant++);
    this->semCantGenteEnElPredio->p();


    Logger::log(jugadorLogId, "Jugador " + Logger::intToString(indice) + " le avisa a la recepcionista", DEBUG);
    this->semEsperarRecepcionista->v();
    Logger::log(jugadorLogId, "Jugador " + Logger::intToString(indice) + " le aviso a la recepcionista", DEBUG);


}

void JugadorProcess::jugar() {


    ClaveJugador claveJugador(getpid(), indice);

    string claveJugadorStr = ClaveJugadorSerializer::serializar(&claveJugador);


    Logger::log(jugadorLogId, "Jugador escribiendo en pipeJugadores: " + claveJugadorStr, DEBUG);

    this->pipeJugadores->escribir(static_cast<const void *>(claveJugadorStr.c_str()), claveJugadorStr.size());

    Logger::log(jugadorLogId, "Jugador escribio en pipeJugadores: " + claveJugadorStr, DEBUG);


    Logger::log(jugadorLogId, "Jugador esperando que termine el partido: ", DEBUG);

    this->semPartidoTerminado->at(indice).p();

    Logger::log(jugadorLogId, "Jugador ve que termino su partido/o que no habia podido conseguir pareja: ", DEBUG);


    this->semJugadoresSinPareja->at(indice).p();

    Logger::log(jugadorLogId, "Me fijo si habia encontrado pareja ", DEBUG);


    bool consiguioPareja = this->shmJugadoresSinPareja->at(indice).leer();

    this->semJugadoresSinPareja->at(indice).v();


    if (consiguioPareja) {
        Logger::log(jugadorLogId, "No habia encontrado pareja " + consiguioPareja, DEBUG);
        this->semJugadoresPredio->v();
        Logger::log(jugadorLogId, "Jugador se va del predio", DEBUG);
        this->semCantGenteEnElPredio->p();
        int cant = this->shmCantGenteEnElPredio->leer();
        this->shmCantGenteEnElPredio->escribir(cant--);
        this->semCantGenteEnElPredio->p();
        terminar();
        exit(0);
    }

    Logger::log(jugadorLogId, "Habia encontrado pareja: " + consiguioPareja, DEBUG);


    Logger::log(jugadorLogId, "Jugador " + Logger::intToString(indice) + " termino de jugar", DEBUG);

    this->cantidadDePartidosPendientes -= 1;

    Logger::log(jugadorLogId,
                "Tengo " + Logger::intToString(cantidadDePartidosPendientes) + " cantidad de partidos pendientes",
                DEBUG);


    decidirQueHacer();


}

void JugadorProcess::decidirQueHacer() {

    Logger::log(jugadorLogId, "Jugador " + Logger::intToString(indice) + " decide que hacer", DEBUG);


    if (cantidadDePartidosPendientes == 0) {
        Logger::log(jugadorLogId,
                    "Jugador " + Logger::intToString(indice) + " se va porque no tiene partidos pendientes", DEBUG);

        this->semJugadoresPredio->v();
        this->semCantGenteEnElPredio->p();
        int cant = this->shmCantGenteEnElPredio->leer();
        this->shmCantGenteEnElPredio->escribir(cant--);
        this->semCantGenteEnElPredio->p();
        terminar();
        exit(0);

    } else {
        if (RandomUtil::randomCeroUno() < TiemposEspera::PROBABILIDAD_IRSE) {
            Logger::log(jugadorLogId, "Jugador " + Logger::intToString(indice) + " decide volver a jugar", DEBUG);

            jugar();
        } else {
            Logger::log(jugadorLogId, "Jugador " + Logger::intToString(indice) + " decide irse", DEBUG);

            this->semJugadoresPredio->v();

            this->semCantGenteEnElPredio->p();
            int cant = this->shmCantGenteEnElPredio->leer();
            this->shmCantGenteEnElPredio->escribir(cant--);
            this->semCantGenteEnElPredio->p();

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

    shmCantGenteEnElPredio->liberar();

}


void JugadorProcess::limpiarRecursos() {
    Logger::log(jugadorLogId, "Limpiando recursos", DEBUG);
    liberarMemoriasCompartidas();
}


JugadorProcess::~JugadorProcess() {

}

/* namespace std */

