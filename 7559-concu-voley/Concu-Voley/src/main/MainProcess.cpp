
#include "MainProcess.h"

#include <sys/wait.h>


#include "../utils/ipc/signal/SignalHandler.h"
#include "../processes/RecepcionistaProcess.h"
#include "../processes/CupidoProcess.h"
#include "../processes/AdminJugadoresProcess.h"
#include "../processes/FixtureProcess.h"

namespace std {

    MainProcess::MainProcess(int cantPartidosJugador, int predioC, int predioF, int cantMJugadores, int cantNJugadores,
                             int cantJugadoresMinimo)
            :
            semJugadoresPredio(SEM_JUGADORES_PREDIO_INIT_FILE, cantMJugadores, 0),
            semEsperarRecepcionista(SEM_ESPERAR_RECEPCIONISTA_INIT_FILE, 0, 0),
            semRanking(SEM_RANKING_INIT_FILE, 1, 0),
            semCupido(SEM_CUPIDO_INIT_FILE, 0, 0),
            semCantCanchasLibres(SEM_CANT_CANCHAS_LIBRES_FILE, predioC * predioF, 0),
            semNivelDeMarea(SEM_NIVEL_DE_MAREA_INIT_FILE, 1, 0),
            semCantGenteEnElPredio(SEM_GENTE_EN_EL_PREDIO, 1, 0) {

        this->cantPartidosJugador = cantPartidosJugador;
        this->predioC = predioC;
        this->predioF = predioF;
        this->cantMJugadores = cantMJugadores;
        this->cantNJugadores = cantNJugadores;
        this->cantJugadoresMinimo = cantJugadoresMinimo;

        inicializarIPCs();
    }


    void MainProcess::inicializarSemaforos() {

        Logger::log(mainLogId, "Creo vector de semaforos termino de jugar y sin pareja de largo: ", DEBUG);
        for (int i = 0; i < cantNJugadores; i++) {
            semsTerminoDeJugar.push_back(Semaforo(SEMS_TERMINO_DE_JUGAR_INIT_FILE, 0, i));
            //probar si combiene N,M o 10
            semJugadoresSinPareja.push_back(Semaforo(SEMS_JUGADOR_SIN_PAREJA, 1, i));
        }


        int cont = 0;
        Logger::log(mainLogId, "Matriz de canchas libres", DEBUG);
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

        semCantGenteEnElPredio.p();

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

        shmCantGenteEnElPredio.crear(SHM_GENTE_EN_EL_PREDIO, 0);
        shmCantGenteEnElPredio.escribir(0);
        semCantGenteEnElPredio.v();


        Logger::log(mainLogId, "Fin de creacion de memorias compartidas ",
                    DEBUG);
    }


    void MainProcess::inicializarIPCs() {
        Logger::log(mainLogId, "InicializoSem", DEBUG);
        inicializarSemaforos();
        Logger::log(mainLogId, "InicializoSHM", DEBUG);
        inicializarMemoriasCompartidas();
    }

    void MainProcess::finalizarProcesosPredio() {


        Logger::log(mainLogId, "Finalizando procesos", INFO);

        Logger::log(mainLogId,
                    "Finalizando recepcionista "
                    + Logger::intToString(idRecepcionista), INFO);
        kill(idRecepcionista, SIGINT);
        waitpid(idRecepcionista, NULL, 0);

        Logger::log(mainLogId,
                    "Finalizando Cupido "
                    + Logger::intToString(idCupido), INFO);
        kill(idCupido, SIGINT);
        waitpid(idCupido, NULL, 0);


        Logger::log(mainLogId,
                    "Finalizando Fixture "
                    + Logger::intToString(idFixture), INFO);
        kill(idFixture, SIGINT);
        waitpid(idFixture, NULL, 0);


    }


    void MainProcess::inicializarProcesosJugadores() {

        pid_t idAdminJugadores = fork();

        if (idAdminJugadores == 0) {

            AdminJugadoresProcess adminJugadoresProcess(cantNJugadores, cantPartidosJugador,
                                                        &semsTerminoDeJugar,
                                                        &semEsperarRecepcionista, &semJugadoresPredio, &pipeJugadores,
                                                        &shmJugadoresSinPareja, &semJugadoresSinPareja,
                                                        &shmCantGenteEnElPredio, &semCantGenteEnElPredio);

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

    void MainProcess::inicializarSigusrHandler() {
        SignalHandler::getInstance()->registrarHandler(SIGUSR1, &sigusr1Handler);
        SignalHandler::getInstance()->registrarHandler(SIGUSR2, &sigusr2Handler);


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

        avisarAPartidos(SIGUSR1);

    }

    void MainProcess::handleBajaOla() {
        Logger::log(mainLogId, "BAJA 1 NIVEL LA OLA", INFO);

        semNivelDeMarea.p();

        int nivelMarea = shmNivelDeMarea.leer();

        if (nivelMarea != 0) {
            nivelMarea--;
        }
        Logger::log(mainLogId, "Nivel actual de marea: " + Logger::intToString(nivelMarea), INFO);

        shmNivelDeMarea.escribir(nivelMarea);
        semNivelDeMarea.v();

        avisarAPartidos(SIGUSR2);

    }

    void MainProcess::avisarAPartidos(int sig) {

        kill(idCupido, sig);

    }


    void MainProcess::iniciarSimulacion() {

        inicializarRecepcionista();
        Logger::log(mainLogId, "inicializarRecepcionista iniciada", DEBUG);

        inicializarCupido();
        Logger::log(mainLogId, "inicializarCupido iniciada", DEBUG);
        inicializarProcesoFixture();
        Logger::log(mainLogId, "inicializarProcesoFixture iniciada", DEBUG);
//   inicializarProcesoResultado();

        inicializarProcesosJugadores();
        Logger::log(mainLogId, "inicializarProcesosJugadores iniciada", DEBUG);
        crearMemoriasCompartidas();
        Logger::log(mainLogId, "crearMemoriasCompartidas iniciada", DEBUG);
        inicializarSigintHandler();
        Logger::log(mainLogId, "inicializarSigintHandler iniciada", DEBUG);
        inicializarSigusrHandler();
        Logger::log(mainLogId, "inicializarSigusrHandler iniciada", DEBUG);
    }

    void MainProcess::inicializarRecepcionista() {

        pid_t idRecepcionista = fork();

        if (idRecepcionista == 0) {

            RecepcionistaProcess recepcionista(&semEsperarRecepcionista, &semCupido, cantJugadoresMinimo);
            recepcionista.run();
            Logger::log(mainLogId, "Recepcionista Termino de Trabajar", DEBUG);
            exit(0);
        } else {

            this->idRecepcionista = idRecepcionista;
        }

    }

    void MainProcess::inicializarProcesoFixture() {

        pid_t idFixture = fork();

        if (idFixture == 0) {

            FixtureProcess fixtureProcess(&pipeFixture);
            fixtureProcess.run();
            exit(0);
        } else {

            this->idFixture = idFixture;
        }

    }


    void MainProcess::inicializarCupido() {

        pid_t idCupido = fork();

        if (idCupido == 0) {

            CupidoProcess cupidoProcess(&pipeJugadores, &semCanchasLibres,
                                        &shmCanchasLibres, cantNJugadores, &semCupido, &semsTerminoDeJugar,
                                        &semCantCanchasLibres, &pipeResultados, &pipeFixture, cantJugadoresMinimo,
                                        &shmJugadoresSinPareja, &shmNivelDeMarea, &semNivelDeMarea,
                                        &semJugadoresSinPareja,&semCantGenteEnElPredio,&shmCantGenteEnElPredio);
            cupidoProcess.run();
            exit(0);
        } else {

            this->idCupido = idCupido;
        }


    }


    mainProcessReturnData MainProcess::run() {
        Logger::log(mainLogId, "Running...", DEBUG);
        iniciarSimulacion();
        Logger::log(mainLogId, "Simulacion iniciada", DEBUG);

        int jugadoresTerminados = 0;

        int response;
        waitpid(idAdminJugadores, &response, 0);
        Logger::log(mainLogId, "Termino el AdminJugadores", INFO);
        bool terminoTorneoDePronto = (sigintHandler.getGracefulQuit() == 1);
       /*
        bool subidaMarea = (sigusr1Handler.getGracefulQuit() == 1);
        bool bajadaMarea = (sigusr2Handler.getGracefulQuit() == 1);

        if (subidaMarea){

            handleBajaOla();

        }

        if (bajadaMarea){

            handleCrecimientoOla();
        }
       */
        if (terminoTorneoDePronto) {
            Logger::log(mainLogId, "Termino de pronto ", INFO);
            handleTerminar();
        } else {

            if (WIFEXITED(response)) {
                jugadoresTerminados = WEXITSTATUS(response);
            }
            Logger::log(mainLogId,
                        "Cantidad de jugadores finalizados: "
                        + Logger::intToString(jugadoresTerminados),
                        INFO);

            finalizarProcesosPredio();
            eliminarIPCs();

        }

        mainProcessReturnData returnData;

        returnData.cantJugadoresTerminados = jugadoresTerminados;

        return returnData;

    }

    void MainProcess::handleTerminar(){

        Logger::log(mainLogId, "Termino el torneo", INFO);
        finalizarJugadores();
        finalizarProcesosPredio();
        eliminarIPCs();

    }

    void MainProcess::finalizarJugadores() {


        kill(idAdminJugadores, SIGINT);
        waitpid(idAdminJugadores, 0, 0);
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
        Logger::log(mainLogId, "Eliminando IPCs", INFO);
        eliminarPipesFifos();
        eliminarMemoriasCompartidas();
        eliminarSemaforos();
    }

    MainProcess::~MainProcess() {


    }
}
