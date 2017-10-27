
#include "MainProcess.h"

#include <sys/wait.h>


#include "../processes/RecepcionistaProcess.h"
#include "../processes/CupidoProcess.h"
#include "../processes/AdminJugadoresProcess.h"
#include "../processes/FixtureProcess.h"
#include "../utils/ipc/signal/SIGUSR1_Handler_Ranking.h"
#include "../utils/ipc/signal/SignalHandler.h"
#include "../utils/ipc/fifo/FifoLectura.h"

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

        for (int i = 0; i < cantNJugadores; i++) {
            vector<MemoriaCompartida<int>> filaPartidosResultado;
            for (int j = 0; j < cantPartidosJugador+20; j++) {
                filaPartidosResultado.push_back(MemoriaCompartida<int>());
            }
            resultadosFinales.push_back(filaPartidosResultado);
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
        int cont = 0;

        for (int i = 0; i < cantNJugadores; i++) {
            for (int j = 0; j < cantPartidosJugador+20; j++) {


                resultadosFinales.at(i).at(j).crear(SHM_RESULTADOS_FINALES, cont);
                resultadosFinales.at(i).at(j).escribir(0);

                cont++;
            }
        }
        Logger::log(mainLogId, "Comenzando creacion de memorias compartidas ",
                    DEBUG);

        for (int i = 0; i < cantNJugadores; i++) {

            shmJugadoresSinPareja.at(i).crear(SHM_JUGADORES_SIN_PAREJA, i);

            shmJugadoresSinPareja.at(i).escribir(false);
            semJugadoresSinPareja.at(i).v();

        }


        cont = 0;

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

        inicializarRanking();
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



    void MainProcess::iniciarSimulacion() {
        inicializarRecepcionista();

        inicializarCupido();
        inicializarProcesosJugadores();

        crearMemoriasCompartidas();
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



    void MainProcess::inicializarCupido() {

        pid_t idCupido = fork();

        if (idCupido == 0) {


            CupidoProcess cupidoProcess(predioF,predioC,&pipeJugadores, &semCanchasLibres,
                                        &shmCanchasLibres, &resultadosFinales,cantNJugadores,cantPartidosJugador, &semCupido, &semsTerminoDeJugar,
                                        &semCantCanchasLibres, &pipeResultados, &pipeFixture, cantJugadoresMinimo,
                                        &shmJugadoresSinPareja, &shmNivelDeMarea, &semNivelDeMarea,
                                        &semJugadoresSinPareja,&semCantGenteEnElPredio,&shmCantGenteEnElPredio);
            cupidoProcess.run();
            exit(0);
        } else {

            this->idCupido = idCupido;
            std::cout<<"Este es cupido "<<idCupido<<std::endl;
        }


    }

    void burbujeo( int puntos[], int ids[],int cantNJugadores){
        bool igual = true;
        while(igual){
            igual = false;
            for (int i = 0; i < cantNJugadores-1; i++) {
                if(puntos[i]<puntos[i+1]){
                    int aux = puntos[i];
                    puntos[i] = puntos[i+1];
                    puntos[i+1] = aux;
                    aux = ids[i];
                    ids[i] = ids[i+1];
                    ids[i+1] = aux;
                    igual=true;
                }
            }
        }
        for (int i = 0; i < cantNJugadores-1; i++) {
            Logger::log("Ranking",
                        "Jugador id:"+  Logger::intToString(ids[i])+ "con "+Logger::intToString(puntos[i])+" puntos.",
                        INFO);
        }
    }
    void MainProcess::inicializarRanking() {

        int pts[cantNJugadores];
        int ids[cantNJugadores];
        for (int i = 0; i < cantNJugadores; i++) {
            int puntos = 0;
            int hasta = resultadosFinales.at(i).at(0).leer();
            for (int j = 1; j <hasta+1; j++) {
                if(resultadosFinales.at(i).at(j).leer()<=2){
                    puntos+=3;
                }

            }
            pts[i]=puntos;
            ids[i]=i;
            //Logger::log("Ranking","Jugador id:"+  Logger::intToString(i)+ "con "+Logger::intToString(puntos)+" puntos.",INFO);
        }

        burbujeo(pts,ids,cantNJugadores);

    }

    mainProcessReturnData MainProcess::run() {
        Logger::log(mainLogId, "Running...", DEBUG);
        iniciarSimulacion();
        Logger::log(mainLogId, "Simulacion iniciada", DEBUG);

        int jugadoresTerminados = 0;

        int response;
        waitpid(idAdminJugadores, &response, 0);
        Logger::log(mainLogId, "Termino el AdminJugadores", INFO);



            if (WIFEXITED(response)) {
                jugadoresTerminados = WEXITSTATUS(response);
            }
            Logger::log(mainLogId,
                        "Cantidad de jugadores finalizados: "
                        + Logger::intToString(jugadoresTerminados),
                        INFO);

            finalizarProcesosPredio();
            eliminarIPCs();


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


        for (int i = 0; i < cantNJugadores; i++) {
            for (int j = 0; j < cantPartidosJugador+20; j++) {
                resultadosFinales.at(i).at(j).liberar();
            }
        }
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
