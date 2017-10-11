//
// Created by navent on 10/10/17.
//

#include "CupidoProcess.h"
#include "../utils/serializer/ClaveJugadorSerializer.h"
#include "PartidoProcess.h"

CupidoProcess::CupidoProcess(Pipe *jugadores, vector<vector<Semaforo>> *semCanchasLibres,
                             vector<vector<MemoriaCompartida<bool>>> *shmCanchasLibres, int cantNJugadores,
                             Semaforo *semCupido, vector<Semaforo> *semsTerminoDeJugar, Semaforo *semCantCanchasLibres,
                             Pipe *pipeResultados, Pipe *pipeFixture) {

    this->jugadores = jugadores;
    this->cantNJugadores = cantNJugadores;
    this->semCupido = semCupido;
    this->pipeFixture = pipeFixture;
    this->pipeResultados = pipeResultados;

    this->parejaEnEspera = new Pareja(new ClaveJugador(-1, -1), new ClaveJugador(-1, -1));

    this->semCanchasLibres = semCanchasLibres;

    this->shmCanchasLibres = shmCanchasLibres;

    this->semsTerminoDeJugar = semsTerminoDeJugar;

    for (int i = 0; i < 10; i++) {
        this->jugadoresSinPareja.push_back(ClaveJugador(-1, -1));
    }

    for (int i = 0; i < cantNJugadores; i++) {

        vector<bool> filaDeMatcheo;
        for (int j = 0; j < cantNJugadores; j++) {
            filaDeMatcheo.push_back(false);
        }

        this->matrizDeMatcheo.push_back(filaDeMatcheo);
    }

}

void CupidoProcess::run() {

    Logger::log(cupidoProcessLogId, "Iniciando Cupido ", DEBUG);


    bool isAlive = true;
    Logger::log(cupidoProcessLogId, "Cupido empieza a leer del pipe", DEBUG);


    while (isAlive) {
        char claveJugadorStr;

        Logger::log(cupidoProcessLogId, "Cupido espera leer del pipe de jugadores", DEBUG);

        jugadores->leer(static_cast<void *>(&claveJugadorStr), sizeof(char));

        Logger::log(cupidoProcessLogId, "Cupido leyo del pipe de jugadores: " + claveJugadorStr, DEBUG);


        ClaveJugador claveJugador = ClaveJugadorSerializer::deserializarClaveJugador((string) &claveJugadorStr);
        for (int i = 0; i < jugadoresSinPareja.size(); i++) {
            if (jugadoresSinPareja[i].getPid() < 0) {
                jugadoresSinPareja[i] = claveJugador;
                break;
            } else {
                if (!matrizDeMatcheo[jugadoresSinPareja[i].getIndice()][claveJugador.getIndice()]) {


                    //Encontre una pareja posible

                    //Pongo que jugaron entre ellos
                    //TODO VER QUE ACA SE PUEDA VOLVER A FALSE CON MAREA COMO ROLLBACK PORQUE TODAVIA NO JUGARON
                    matrizDeMatcheo[jugadoresSinPareja[i].getIndice()][claveJugador.getIndice()] = true;
                    //Piso en la lista de jugadores sin pareja por una vacia
                    jugadoresSinPareja[i] = ClaveJugador(-1, -1);

                    //creo pareja nueva

                    Pareja *nuevaPareja = new Pareja(&claveJugador, &jugadoresSinPareja[i]);

                    //Tengo pareja en espera? --> Lanzo el partido con las 2 parejas
                    if (!(parejaEnEspera->getClaveJugador1()->getIndice() < 0)) {

                        inicializarPartido(parejaEnEspera, nuevaPareja, semCanchasLibres, shmCanchasLibres,
                                           semsTerminoDeJugar, semCantCanchasLibres, pipeResultados, pipeFixture);
                    } else {

                        parejaEnEspera = nuevaPareja;
                    }

                } else {
                    continue;
                }
            }
        }
    }

}

void CupidoProcess::limpiarRecursos() {

}

CupidoProcess::~CupidoProcess() {


}

void CupidoProcess::inicializarPartido(Pareja *pareja1, Pareja *pareja2, vector<vector<Semaforo>> *semCanchasLibres,
                                       vector<vector<MemoriaCompartida<bool>>> *shmCanchasLibres,
                                       vector<Semaforo> *semTerminoDeJugar, Semaforo *semCantCanchasLibres,
                                       Pipe *pipeResultados, Pipe *pipeFixture) {

    pid_t idPartido = fork();

    if (idPartido == 0) {

        PartidoProcess partidoProcess(pareja1, pareja2, semCanchasLibres, shmCanchasLibres, semTerminoDeJugar, semCantCanchasLibres, pipeResultados, pipeFixture);


        partidoProcess.run();


        exit(0);
    }

}