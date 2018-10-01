//
// Created by navent on 10/10/17.
//

#include <sys/stat.h>
#include "PartidoProcess.h"

#include "TiemposEspera.h"
#include "../utils/serializer/ResultadoSerializer.h"
#include "../main/MainProcess.h"
#include "../utils/ipc/signal/SIGINT_Handler_Partidos.h"
#include "../utils/ipc/signal/SignalHandler.h"

PartidoProcess::PartidoProcess( int cantidadJugadores,int cantPartidosJugador,vector<vector<MemoriaCompartida<int>>> *resultadosFinales,int fila,int columna,Pareja *pareja1, Pareja *pareja2, vector<vector<Semaforo>> *semCanchasLibres,
                               vector<vector<MemoriaCompartida<bool>>> *shmCanchasLibres,
                               vector<Semaforo> *semTerminoDeJugar, Semaforo *semCantCanchasLibres,
                               Pipe *pipeResultados, Pipe *pipeFixture, MemoriaCompartida<int> *shmNivelDeMarea,
                               Semaforo *semNivelDeMarea) {
    this->cantidadJugadores=cantidadJugadores;
    this->resultadosFinales=resultadosFinales;
    this->cantPartidosJugador=cantPartidosJugador;
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


    this->cancha = new Cancha(fila,columna);

    inicializarMemoriasCompartidas();


}

void PartidoProcess::inicializarMemoriasCompartidas() {

    int cont = 0;
    for (unsigned int i = 0; i < cantidadJugadores; i++) {
        for (unsigned int j = 0; j < cantPartidosJugador+20; j++) {
            resultadosFinales->at(i).at(j).crear(SHM_RESULTADOS_FINALES, cont);
            cont ++;
        }
    }

    cont = 0;
    for (unsigned int i = 0; i < shmCanchasLibres->size(); i++) {
        for (unsigned int j = 0; j < shmCanchasLibres->at(0).size(); j++) {
            shmCanchasLibres->at(i).at(j).crear(SHM_CANCHAS_LIBRES, cont);
            cont ++;
        }
    }

}


void PartidoProcess::run() {

    int fu = this->cancha->getFila();
    int cu = this->cancha->getColumna();

    semCanchasLibres->at(fu).at(cu).p();


    shmCanchasLibres->at(fu).at(cu).escribir(false);

    semCanchasLibres->at(fu).at(cu).v();

    SIGINT_Handler_Partidos sigint_handler;
    SignalHandler :: getInstance()->registrarHandler ( SIGINT,&sigint_handler );

    sleep(TiemposEspera::TIEMPO_JUGAR);



    Resultado resultado = Resultado(this->pareja1, this->pareja2, true);
    Logger::log(partidoProcessLogId, "El partido termino " + Logger::intToString(resultado.getSetsPareja1()) + " a " +
                                     Logger::intToString(resultado.getSetsPareja2()), DEBUG);

    string resultadoStr = ResultadoSerializer::serializar(&resultado);

    Logger::log(partidoProcessLogId, "Voy a pushearle a fixture " + resultadoStr, DEBUG);

    this->pipeFixture->escribir(static_cast<const void *>(resultadoStr.c_str()), resultadoStr.size());


    if ( sigint_handler.getGracefulQuit() == 0 ){
        liberarCancha();
    }else{
        Logger::log(partidoProcessLogId, "Me mojo la shuta loco!", INFO);
    }


    avisarJugadores();


    int  ind = resultadosFinales->at(this->pareja1->getClaveJugador1()->getIndice()).at(0).leer();
    ind+=1;
    resultadosFinales->at(this->pareja1->getClaveJugador1()->getIndice()).at(0).escribir(ind);
    resultadosFinales->at(this->pareja1->getClaveJugador1()->getIndice()).at(ind).escribir(3);

    ind = resultadosFinales->at(this->pareja1->getClaveJugador2()->getIndice()).at(0).leer();
    ind+=1;
    resultadosFinales->at(this->pareja1->getClaveJugador2()->getIndice()).at(0).escribir(ind);
    resultadosFinales->at(this->pareja1->getClaveJugador2()->getIndice()).at(ind).escribir(3);

    ind = resultadosFinales->at(this->pareja2->getClaveJugador1()->getIndice()).at(0).leer();
    ind+=1;
    resultadosFinales->at(this->pareja2->getClaveJugador1()->getIndice()).at(0).escribir(ind);
    resultadosFinales->at(this->pareja2->getClaveJugador1()->getIndice()).at(ind).escribir(0);

    ind = resultadosFinales->at(this->pareja2->getClaveJugador2()->getIndice()).at(0).leer();
    ind+=1;
    resultadosFinales->at(this->pareja2->getClaveJugador2()->getIndice()).at(0).escribir(ind);
    resultadosFinales->at(this->pareja2->getClaveJugador2()->getIndice()).at(ind).escribir(0);



    limpiarRecursos();
    finalizar();


}

void PartidoProcess::liberarCancha() {

    int fila = cancha->getFila();
    int columna = cancha->getColumna();

    this->semCanchasLibres->at(fila).at(columna).p();
    this->shmCanchasLibres->at(fila).at(columna).escribir(true);
    Logger::log(partidoProcessLogId, "Cancha Liberada. En fila: " + Logger::intToString(fila) + " y columna: " +
                                     Logger::intToString(columna), DEBUG);

    this->semCanchasLibres->at(fila).at(columna).v();

    this->semCantCanchasLibres->v();
    Logger::log(partidoProcessLogId, "Aumento la cantidad de canchas libres", DEBUG);


}

void PartidoProcess::avisarJugadores() {

    ClaveJugador *jugador1 = this->pareja1->getClaveJugador1();
    ClaveJugador *jugador2 = this->pareja1->getClaveJugador2();

    ClaveJugador *jugador3 = this->pareja2->getClaveJugador1();
    ClaveJugador *jugador4 = this->pareja2->getClaveJugador2();

    Logger::log(partidoProcessLogId, "Empiezo a avisarle a los jugadores que termino su partido", DEBUG);
    this->semTerminoDeJugar->at(jugador1->getIndice()).v();
    this->semTerminoDeJugar->at(jugador2->getIndice()).v();
    this->semTerminoDeJugar->at(jugador3->getIndice()).v();
    this->semTerminoDeJugar->at(jugador4->getIndice()).v();
    Logger::log(partidoProcessLogId, "Termino de avisarle a los jugadores", DEBUG);


}


void PartidoProcess::limpiarRecursos() {
    Logger::log(partidoProcessLogId, "Limpiando recursos", DEBUG);
    liberarMemoriasCompartidas();
    Logger::log(partidoProcessLogId, "Termino de limpiar recursos", DEBUG);

}

void PartidoProcess::liberarMemoriasCompartidas() {

    for (unsigned int i = 0; i < cantidadJugadores; i++) {
        for (unsigned int j = 0; j < cantPartidosJugador+20; j++) {
            resultadosFinales->at(i).at(j).liberar();
        }
    }
    for (unsigned int i = 0; i < shmCanchasLibres->size(); i++) {
        for (unsigned int j = 0; j < shmCanchasLibres->at(0).size(); j++) {
            shmCanchasLibres->at(i).at(j).liberar();
        }
    }

}

void PartidoProcess::finalizar() {

    exit(0);
}

PartidoProcess::~PartidoProcess() {

}

