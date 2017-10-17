

#include "AdminJugadoresProcess.h"

#include <sys/wait.h>

#include "../utils/ipc/signal/SignalHandler.h"
#include "JugadorProcess.h"

namespace std {

    AdminJugadoresProcess::AdminJugadoresProcess(int cantJugadores, int cantPartidosPorJugador,
                                                 vector<Semaforo> *semPartidoTerminado,
                                                 Semaforo *semEsperarRecepcionista, Semaforo *semJugadoresPredio,
                                                 Pipe *pipeJugadores,
                                                 vector<MemoriaCompartida<bool>> *shmJugadoresSinPareja,
                                                 vector<Semaforo> *semJugadoresSinPareja,
                                                 MemoriaCompartida<int> *shmCantGenteEnElPredio,
                                                 Semaforo *semCantGenteEnElPredio) {

        this->cantJugadores = cantJugadores;
        this->cantPartidosPorJugador = cantPartidosPorJugador;
        this->semPartidoTerminado = semPartidoTerminado;
        this->semJugadoresPredio = semJugadoresPredio;
        this->semEsperarRecepcionista = semEsperarRecepcionista;
        this->pipeJugadores = pipeJugadores;
        this->semJugadoresSinPareja = semJugadoresSinPareja;
        this->shmJugadoresSinPareja = shmJugadoresSinPareja;
        this->shmCantGenteEnElPredio = shmCantGenteEnElPredio;
        this->semCantGenteEnElPredio = semCantGenteEnElPredio;

        inicializarHandler();

    }

    void AdminJugadoresProcess::inicializarHandler() {
        SignalHandler::getInstance()->registrarHandler(SIGINT, &sigintHandler);
        SignalHandler::getInstance()->registrarHandler(SIGUSR1, &sigusr1Handler);
        SignalHandler::getInstance()->registrarHandler(SIGUSR2, &sigusr2Handler);

    }

    int AdminJugadoresProcess::run() {

        Logger::log(adminJugadoresLogId, "Comenzando inicializacion de jugadores", INFO);
        Logger::log(adminJugadoresLogId,
                    "Cantidad de jugadores a inicializar: " + Logger::intToString(cantJugadores), INFO);

        vector<pid_t> idsJugadores;

        int jugadoresCreados = 0;


        bool subidaMarea = false;
        bool bajadaMarea = false;
        while (jugadoresCreados < cantJugadores) {

            pid_t idJugador = fork();

            if (idJugador == 0) {

                JugadorProcess jugadorProcess(cantPartidosPorJugador,
                                              semPartidoTerminado, jugadoresCreados,
                                              semEsperarRecepcionista, semJugadoresPredio, pipeJugadores,
                                              shmJugadoresSinPareja,
                                              semJugadoresSinPareja, shmCantGenteEnElPredio, semCantGenteEnElPredio);

                jugadorProcess.run();
                exit(0);
            } else {
                Logger::log(adminJugadoresLogId,
                            "Se inicializo un nuevo jugador con id: " + Logger::intToString(idJugador) +
                            "con indice: " + Logger::intToString(jugadoresCreados), INFO);
                idsJugadores.push_back(idJugador);
                jugadoresCreados++;
            }

            if (TiemposEspera::tiempos) {
                sleep(RandomUtil::randomInt(TiemposEspera::TIEMPO_RANDOM_ENTRE_JUGADORES));
            }





        }
        Logger::log(adminJugadoresLogId,
                    "Todos los Jugadores fueron inicializados: " + Logger::intToString(jugadoresCreados), DEBUG);
        Logger::log(adminJugadoresLogId, "Esperando finalizacion de jugadores", DEBUG);


        int jugadoresTerminados = 0;

        while ((jugadoresTerminados < jugadoresCreados)) {
            pid_t id = wait(NULL);
            Logger::log(adminJugadoresLogId, "Termino el jugador " + Logger::intToString(id), DEBUG);
            jugadoresTerminados++;

        }


        Logger::log(adminJugadoresLogId,
                    "Devolviendo cantidad de jugadores inicializados: " + Logger::intToString(jugadoresCreados),
                    DEBUG);

        return jugadoresCreados;

    }

    AdminJugadoresProcess::~AdminJugadoresProcess() {
    }

} /* namespace std */
