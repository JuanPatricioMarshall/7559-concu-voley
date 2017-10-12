
#include "MainProcess.h"

#include <sys/wait.h>
#include <unistd.h>
#include <csignal>
#include <cstdlib>


#include "../processes/GerenteProcess.h"
#include "../utils/ipc/signal/SignalHandler.h"
#include "../processes/RecepcionistaProcess.h"
#include "../processes/CupidoProcess.h"
#include "../processes/AdminJugadoresProcess.h"

namespace std {

    MainProcess::MainProcess(int cantPartidosJugador, int predioC, int predioF, int cantMJugadores, int cantNJugadores,
                             int cantJugadoresMinimo)
            :
            semJugadoresPredio(SEM_JUGADORES_PREDIO_INIT_FILE, cantMJugadores, 0),
            semEsperarRecepcionista(SEM_ESPERAR_RECEPCIONISTA_INIT_FILE, 0, 0),
            semRanking(SEM_RANKING_INIT_FILE, 1, 0),
            semCupido(SEM_CUPIDO_INIT_FILE, 0, 0),
            semCantCanchasLibres(SEM_CANT_CANCHAS_LIBRES_FILE, predioC * predioF, 0),
            semNivelDeMarea(SEM_NIVEL_DE_MAREA_INIT_FILE, 1, 0) {

        this->cantPartidosJugador = cantPartidosJugador;
        this->predioC = predioC;
        this->predioF = predioF;
        this->cantMJugadores = cantMJugadores;
        this->cantNJugadores = cantNJugadores;
        this->cantJugadoresMinimo = cantJugadoresMinimo;

        inicializarIPCs();
    }


    void MainProcess::inicializarSemaforos() {


        for (int i = 0; i < cantNJugadores; i++) {
            semsTerminoDeJugar.push_back(Semaforo(SEMS_TERMINO_DE_JUGAR_INIT_FILE, 0, i));
            semJugadoresSinPareja.push_back(Semaforo(SEMS_JUGADOR_SIN_PAREJA, 1, i));
        }


        int cont = 0;
        for (int i = 0; i < predioF; i++) {

            vector<Semaforo> semFilaCanchaLibre;

            for (int j = 0; j < predioC; j++) {
                semFilaCanchaLibre.push_back(
                        Semaforo(SEMS_CANCHA_OCUPADA_INIT_FILE, 1, cont));
                cont++;

            }

            semCanchasLibres.push_back(semFilaCanchaLibre);

        }


    }

    void MainProcess::inicializarMemoriasCompartidas() {

        Logger::log(mainLogId, "Comenzando inicializacion de memorias compartidas ",
                    DEBUG);

        semNivelDeMarea.p();

        for (int i = 0; i < cantNJugadores; i++) {
            semJugadoresSinPareja.at(i).p();
            shmJugadoresSinPareja.push_back(MemoriaCompartida<bool>());

        }

        for (int i = 0; i < predioF; i++) {


            vector<MemoriaCompartida<bool>> shmCanchaLibreFila;

            for (int j = 0; j < predioC; j++) {
                semCanchasLibres.at(i).at(j).p();
                shmCanchaLibreFila.push_back(MemoriaCompartida<bool>());
            }
            shmCanchasLibres.push_back(shmCanchaLibreFila);
        }


    }

    void MainProcess::crearMemoriasCompartidas() {

        Logger::log(mainLogId, "Comenzando creacion de memorias compartidas ",
                    DEBUG);

        for (int i = 0; i < cantNJugadores; i++) {

            shmJugadoresSinPareja.at(i).crear(SHM_JUGADORES_SIN_PAREJA, i);

            shmJugadoresSinPareja.at(i).escribir(false);
            semJugadoresSinPareja.at(i).v();

        }

        int cont = 0;

        for (int i = 0; i < predioF; i++) {
            for (int j = 0; j < predioC; j++) {


                shmCanchasLibres.at(i).at(j).crear(SHM_CANCHAS_LIBRES, cont);
                shmCanchasLibres.at(i).at(j).escribir(true);
                semCanchasLibres.at(i).at(j).v();

                cont++;
            }
        }


        shmNivelDeMarea.crear(SHM_NIVEL_MAREA_INIT_FILE, 0);
        shmNivelDeMarea.escribir(0);
        semNivelDeMarea.v();

        Logger::log(mainLogId, "Fin de creacion de memorias compartidas ",
                    DEBUG);
    }


    void MainProcess::inicializarIPCs() {
        inicializarSemaforos();
        inicializarMemoriasCompartidas();

    }

    void MainProcess::finalizarProcesosPredio() {


        Logger::log(mainLogId, "Finalizando procesos", DEBUG);

        Logger::log(mainLogId,
                    "Finalizando recepcionista "
                    + Logger::intToString(idRecepcionista), DEBUG);
        kill(idRecepcionista, SIGINT);
        waitpid(idRecepcionista, NULL, 0);

        Logger::log(mainLogId,
                    "Finalizando Cupido "
                    + Logger::intToString(idCupido), DEBUG);
        kill(idCupido, SIGINT);
        waitpid(idCupido, NULL, 0);


    }


    void MainProcess::inicializarProcesosJugadores() {

        pid_t idAdminJugadores = fork();

        if (idAdminJugadores == 0) {

            AdminJugadoresProcess adminJugadoresProcess(cantNJugadores, cantPartidosJugador,
                                                        &semsTerminoDeJugar,
                                                        &semEsperarRecepcionista, &semJugadoresPredio, &pipeJugadores,
                                                        &shmJugadoresSinPareja, &semJugadoresSinPareja);

            int jugadoresTerminados = adminJugadoresProcess.run();

            Logger::log(adminJugadoresLogId,
                        "Exit con cantidad de jugtadores finalizados: " + Logger::intToString(jugadoresTerminados),
                        DEBUG);

            exit(jugadoresTerminados);


        } else {
            this->idAdminJugadores = idAdminJugadores;
        }
    }

    void MainProcess::inicializarSigintHandler() {
        SignalHandler::getInstance()->registrarHandler(SIGINT, &sigintHandler);
    }

    void MainProcess::inicializarSigusr1Handler() {
        SignalHandler::getInstance()->registrarHandler(SIGUSR1, &sigusr1Handler);
    }

    void MainProcess::handleCrecimientoOla() {
        Logger::log(mainLogId, "CRECE 1 NIVEL LA OLA", INFO);

        semNivelDeMarea.p();

        int nivelMarea = shmNivelDeMarea.leer();

        if (nivelMarea < predioC - 1) {
            nivelMarea++;
        }
        Logger::log(mainLogId, "Nivel actual de marea: " + Logger::intToString(nivelMarea), INFO);

        shmNivelDeMarea.escribir(nivelMarea);
        semNivelDeMarea.v();

        avisarAPartidos();

    }

    void MainProcess::avisarAPartidos() {


    }


//
//int MainProcess::handleCorteLuz(){
//	Logger::log(mainLogId, "CORTE DE LUZ", INFO);
//	acumularPerdidas();
//	int comensalesFinalizados = finalizarJugadores();
//	finalizarProcesosPredio();
//	eliminarIPCs();
//
//	Logger::log(mainLogId, "ESPERANDO QUE VUELVA LA LUZ", INFO);
//	if(TiemposEspera::tiempos){
//		sleep(TiemposEspera::TIEMPO_CORTE_LUZ);
//	}
//	return comensalesFinalizados;
//}

    void MainProcess::iniciarSimulacion() {

        inicializarRecepcionista();

        inicializarCupido();

//    inicializarProcesoFixture();
//   inicializarProcesoResultado();

        inicializarProcesosJugadores();

        crearMemoriasCompartidas();
        inicializarSigintHandler();
        inicializarSigusr1Handler();
    }

    void MainProcess::inicializarRecepcionista() {

        pid_t idRecepcionista = fork();

        if (idRecepcionista == 0) {

            RecepcionistaProcess recepcionista(&semEsperarRecepcionista, &semCupido, cantJugadoresMinimo);
            recepcionista.run();
            exit(0);
        } else {

            this->idRecepcionista = idRecepcionista;
        }

    }


    void MainProcess::inicializarCupido() {

        pid_t idCupido = fork();

        if (idCupido == 0) {

            CupidoProcess cupidoProcess(&pipeJugadores, &semCanchasLibres,
                                        &shmCanchasLibres, cantNJugadores, &semCupido, &semsTerminoDeJugar,
                                        &semCantCanchasLibres, &pipeResultados, &pipeFixture, cantJugadoresMinimo,
                                        &shmJugadoresSinPareja, &shmNivelDeMarea, &semNivelDeMarea, &semJugadoresSinPareja);
            cupidoProcess.run();
            exit(0);
        } else {

            this->idCupido = idCupido;
        }


    }


    mainProcessReturnData MainProcess::run() {
        iniciarSimulacion();
        Logger::log(mainLogId, "Simulacion iniciada", DEBUG);

        int jugadoresTerminados = 0;
        bool salir = false;

//    for (int i = 0; i < 10; i++) {
//        Logger::log(mainLogId,
//                    "Cantidad de Jugadores finalizados: "
//                    + Logger::intToString(jugadoresTerminados),
//                    DEBUG);
//    }

        while (!salir) {
            int response;
            waitpid(idAdminJugadores, &response, 0);
            bool marea = (sigintHandler.getGracefulQuit() == 1);

            bool terminoTorneoDePronto = (sigusr1Handler.getGracefulQuit() == 1);
            if (marea) {
//			comensalesFinalizados = handleCorteLuz();
            } else if (terminoTorneoDePronto) {
                sigusr1Handler.setGracefulQuit(0);
//			iniciarProcesoGerente();
                //Iniciar proceso que diga resultado final?
//            waitpid(idGerente, NULL, 0);
            } else {

                //Leyendo respuesta del AdminComensales
                if (WIFEXITED(response)) {
                    jugadoresTerminados = WEXITSTATUS(response);
                }
                Logger::log(mainLogId,
                            "Cantidad de jugadores finalizados: "
                            + Logger::intToString(jugadoresTerminados),
                            DEBUG);

                finalizarProcesosPredio();
                eliminarIPCs();

            }//TODO
            salir = true;
        }
        mainProcessReturnData returnData;
        // TODO
        returnData.cantJugadoresTerminados = jugadoresTerminados;

        return returnData;

    }

    void MainProcess::eliminarSemaforos() {


        semCupido.eliminar();
        semEsperarRecepcionista.eliminar();
        semJugadoresPredio.eliminar();
        semCantCanchasLibres.eliminar();
        semRanking.eliminar();

        for (int i = 0; i < predioF; i++) {
            for (int j = 0; j < predioC; j++) {
                semCanchasLibres.at(i).at(j).eliminar();

            }
        }

        for (int i = 0; i < cantNJugadores; i++) {
            semsTerminoDeJugar.at(i).eliminar();
            semJugadoresSinPareja.at(i).eliminar();
        }

    }

    void MainProcess::eliminarMemoriasCompartidas() {


        for (int i = 0; i < predioF; i++) {
            for (int j = 0; j < predioC; j++) {
                shmCanchasLibres.at(i).at(j).liberar();
            }
        }

        for (int i = 0; i < cantNJugadores; i++) {
            shmJugadoresSinPareja.at(i).liberar();
        }

    }

    void MainProcess::eliminarPipesFifos() {
    }

    void MainProcess::eliminarIPCs() {
        Logger::log(mainLogId, "Eliminando IPCs", DEBUG);
        eliminarPipesFifos();
        eliminarMemoriasCompartidas();
        eliminarSemaforos();
    }

    MainProcess::~MainProcess() {


    }
}
