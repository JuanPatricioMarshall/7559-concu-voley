//
// Created by navent on 10/10/17.
//

#include "CupidoProcess.h"
#include "../utils/serializer/ClaveJugadorSerializer.h"
#include "PartidoProcess.h"

CupidoProcess::CupidoProcess(Pipe *jugadores, vector<vector<Semaforo>> *semCanchasLibres,
                             vector<vector<MemoriaCompartida<bool>>> *shmCanchasLibres, int cantNJugadores,
                             Semaforo *semCupido, vector<Semaforo> *semsTerminoDeJugar, Semaforo *semCantCanchasLibres,
                             Pipe *pipeResultados, Pipe *pipeFixture, int cantJugadoresMinimosParaElTorneo) {

    this->jugadores = jugadores;
    this->cantNJugadores = cantNJugadores;
    this->cantJugadoresMinimosParaElTorneo = cantJugadoresMinimosParaElTorneo;
    this->semCupido = semCupido;
    this->pipeFixture = pipeFixture;
    this->pipeResultados = pipeResultados;

    this->parejaEnEspera = new Pareja(new ClaveJugador(-1, -1), new ClaveJugador(-1, -1));

    this->semCanchasLibres = semCanchasLibres;

    this->shmCanchasLibres = shmCanchasLibres;

    this->semsTerminoDeJugar = semsTerminoDeJugar;

    for (int i = 0; i < cantJugadoresMinimosParaElTorneo; i++) {
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


    this->semCupido->p();
    Logger::log(cupidoProcessLogId, "Ya entraron " + Logger::intToString(cantJugadoresMinimosParaElTorneo) + " personas, asi que empiezo el torneo", DEBUG);



    bool isAlive = true;
    Logger::log(cupidoProcessLogId, "Cupido empieza a leer del pipe", DEBUG);


    while (isAlive) {
        char buffer[sizeof(ClaveJugador)];
        string claveJugadorStr;


        Logger::log(cupidoProcessLogId, "Cupido espera leer del pipe de jugadores", DEBUG);

        jugadores->leer(static_cast<void *>(&buffer), sizeof(ClaveJugador));

        claveJugadorStr = buffer;


        Logger::log(cupidoProcessLogId, "Cupido leyo del pipe de jugadores: " + claveJugadorStr, DEBUG);


        ClaveJugador claveJugador = ClaveJugadorSerializer::deserializarClaveJugador(claveJugadorStr);

        Logger::log(cupidoProcessLogId,
                    "Cupido buscando pareja para " + Logger::intToString(claveJugador.getPid()) + " indice: " +
                    Logger::intToString(claveJugador.getIndice()), DEBUG);


        for (int i = 0; i < jugadoresSinPareja.size(); i++) {
            if (jugadoresSinPareja[i].getPid() < 0) {
                Logger::log(cupidoProcessLogId, "Cupido encontro una mesa vacia en posicion " + Logger::intToString(i) + " para el jugador " +
                                                Logger::intToString(claveJugador.getPid()) + " indice: " +
                                                Logger::intToString(claveJugador.getIndice()), DEBUG);

                jugadoresSinPareja[i] = claveJugador;

                Logger::log(cupidoProcessLogId, "Cupido sienta en una mesa vacia al jugador " +
                                                Logger::intToString(claveJugador.getPid()) + " indice: " +
                                                Logger::intToString(claveJugador.getIndice()), DEBUG);
                break;
            } else {
                if (!matrizDeMatcheo[jugadoresSinPareja[i].getIndice()][claveJugador.getIndice()]) {


                    Logger::log(cupidoProcessLogId,
                                "No jugaron entre ellos los jugadores: " + Logger::intToString(claveJugador.getPid()) +
                                " indice: " +
                                Logger::intToString(claveJugador.getIndice()) + " y el " +
                                Logger::intToString(jugadoresSinPareja[i].getPid()) + " indice: " +
                                Logger::intToString(jugadoresSinPareja[i].getIndice()), DEBUG);


                    //Encontre una pareja posible

                    //Pongo que jugaron entre ellos
                    //TODO VER QUE ACA SE PUEDA VOLVER A FALSE CON MAREA COMO ROLLBACK PORQUE TODAVIA NO JUGARON
                    matrizDeMatcheo[jugadoresSinPareja[i].getIndice()][claveJugador.getIndice()] = true;

                    Logger::log(cupidoProcessLogId, "Marco que jugaron entre ellos los jugadores " +
                                                    Logger::intToString(jugadoresSinPareja[i].getIndice()) + " y " +
                                                    Logger::intToString(claveJugador.getIndice()), DEBUG);



                    //creo pareja nueva

                    ClaveJugador claveJugadorSinPareja(jugadoresSinPareja[i].getPid(), jugadoresSinPareja[i].getIndice());

                    Pareja *nuevaPareja = new Pareja(&claveJugador, &claveJugadorSinPareja);


                    //Piso en la lista de jugadores sin pareja por una vacia
                    Logger::log(cupidoProcessLogId, "Vacio la mesa: " + Logger::intToString(i), DEBUG);
                    jugadoresSinPareja[i] = ClaveJugador(-1, -1);


                    Logger::log(cupidoProcessLogId, "Armo la pareja ", DEBUG);


                    //Tengo pareja en espera? --> Lanzo el partido con las 2 parejas
                    if (!(parejaEnEspera->getClaveJugador1()->getIndice() < 0)) {

                        Logger::log(cupidoProcessLogId, "Ya habia una pareja esperando ", DEBUG);


                        inicializarPartido(parejaEnEspera, nuevaPareja, semCanchasLibres, shmCanchasLibres,
                                           semsTerminoDeJugar, semCantCanchasLibres, pipeResultados, pipeFixture);

                        parejaEnEspera = new Pareja(new ClaveJugador(-1, -1), new ClaveJugador(-1, -1));

                    } else {
                        Logger::log(cupidoProcessLogId, "No habia una pareja esperando ", DEBUG);

                        parejaEnEspera = nuevaPareja;

                        Logger::log(cupidoProcessLogId, "Guarde la nueva pareja a esperar ", DEBUG);

                    }
                    break;

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

    Logger::log(cupidoProcessLogId,
                "Inicio un partido entre " + Logger::intToString(pareja1->getClaveJugador1()->getIndice()) + ", " +
                Logger::intToString(pareja1->getClaveJugador2()->getIndice()) + " y " +
                Logger::intToString(pareja2->getClaveJugador1()->getIndice()) + ", " +
                Logger::intToString(pareja2->getClaveJugador2()->getIndice()), DEBUG);


    pid_t idPartido = fork();

    if (idPartido == 0) {

        PartidoProcess partidoProcess(pareja1, pareja2, semCanchasLibres, shmCanchasLibres, semTerminoDeJugar,
                                      semCantCanchasLibres, pipeResultados, pipeFixture);


        partidoProcess.run();


        exit(0);
    }

}