/*
 * MainProcess.h
 *
 *  Created on: Oct 15, 2016
 *      Author: gaston
 */

#ifndef PROCESSES_MAINPROCESS_H_
#define PROCESSES_MAINPROCESS_H_

#include <sched.h>
#include <string>
#include <vector>

#include "../model/Menu.h"
#include "../utils/ipc/pipe/Pipe.h"
#include "../utils/ipc/semaphore/Semaforo.h"
#include "../utils/ipc/shared-memory/MemoriaCompartida.h"
#include "../utils/ipc/signal/SIGINT_Handler.h"
#include "../utils/ipc/signal/SIGUSR1_Handler.h"
#include "../processes/TiemposEspera.h"


namespace std {

    const string mainLogId = "Main";


    const string SEM_COMENSALES_EN_PUERTA_INIT_FILE = "../ipc-init-files/sem_comensales_en_puerta.txt";
    const string SEM_RECEPCIONISTAS_LIBRES_INIT_FILE = "../ipc-init-files/sems_cupido.txt";
    const string SEM_MESAS_LIBRES_INIT_FILE = "../ipc-init-files/sem_mesas_libres.txt";
    const string SEM_PERSONAS_LIVING_INIT_FILE = "../ipc-init-files/sems_ranking.txt";
    const string SEM_CAJA_INIT_FILE = "../ipc-init-files/sems_termino_de_jugar.txt";
    const string SEM_LLAMADOS_MOZOS_INIT_FILE = "../ipc-init-files/sems_cancha_ocupada.txt";


    const string SEMS_CANCHA_OCUPADA_INIT_FILE = "/home/navent/Facultad/concu/concu/7559-concu-voley/Concu-Voley/ipc-init-files/sems_cancha_ocupada.txt";
    const string SEM_JUGADORES_PREDIO_INIT_FILE = "/home/navent/Facultad/concu/concu/7559-concu-voley/Concu-Voley/ipc-init-files/sems_jugadores_predio.txt";
    const string SEM_ESPERAR_RECEPCIONISTA_INIT_FILE = "/home/navent/Facultad/concu/concu/7559-concu-voley/Concu-Voley/ipc-init-files/sems_esperar_recepcionista.txt";
    const string SEMS_TERMINO_DE_JUGAR_INIT_FILE = "/home/navent/Facultad/concu/concu/7559-concu-voley/Concu-Voley/ipc-init-files/sems_termino_de_jugar.txt";
    const string SEM_RANKING_INIT_FILE = "/home/navent/Facultad/concu/concu/7559-concu-voley/Concu-Voley/ipc-init-files/sems_ranking.txt";
    const string SEM_CUPIDO_INIT_FILE = "/home/navent/Facultad/concu/concu/7559-concu-voley/Concu-Voley/ipc-init-files/sems_cupido.txt";

    const string SEM_CANT_CANCHAS_LIBRES_FILE = "/home/navent/Facultad/concu/concu/7559-concu-voley/Concu-Voley/ipc-init-files/sems_cant_canchas_libres.txt";


    const string SEMS_MESAS_LIBRES_INIT_FILE = "../ipc-init-files/sems_mesas_libres.txt";

    const string SHM_PERSONAS_LIVING = "../ipc-init-files/shm_personas_living.txt";
    const string SHM_CAJA = "../ipc-init-files/shm_caja.txt";
    const string SHM_FACTURAS = "../ipc-init-files/shm_facturas.txt";
    const string SHM_MESAS_LIBRES = "../ipc-init-files/shm_mesas_libres.txt";


    struct mainProcessReturnData {
        vector<string> ganadores;
        int cantJugadoresTerminados;
    };

    class MainProcess {

    private:

        int cantPartidosJugador;
        int predioC;
        int predioF;
        int cantMJugadores;
        int cantNJugadores;


        vector<pid_t> idsRecepcionistas;
        vector<pid_t> idsMozos;
        vector<pid_t> idsComensales;
        pid_t idAdminJugadores;
        pid_t idCocinero;

        //pid_t del que dice el resultado final si termino de una?
        pid_t idGerente;


        Semaforo semJugadoresPredio;
        Semaforo semEsperarRecepcionista;
        Semaforo semRanking;
        Semaforo semCupido;
        Semaforo semCantCanchasLibres;

//	Semaforo semMesasLibres;

        vector<Semaforo> semsTerminoDeJugar;

        vector<vector<Semaforo>> semCanchasLibres;

        vector<vector<MemoriaCompartida<bool>>> shmCanchasLibres;


        vector<MemoriaCompartida<bool>> shmMesasLibres;
        vector<MemoriaCompartida<double>> s;

        Pipe pipeJugadores;
        Pipe pipePedidosACocinar;
        Pipe pipeResultados;
        Pipe pipeFixture;

        SIGINT_Handler sigintHandler;
        SIGUSR1_Handler sigusr1Handler;

        void inicializarIPCs();

        void inicializarSemaforos();

        void inicializarMemoriasCompartidas();

        void crearMemoriasCompartidas();

        void inicializarPipesFifos();

        void inicializarSigintHandler();

        void inicializarSigusr1Handler();


        void inicializarRecepcionista();

        void inicializarCupido();


        void inicializarProcesosJugadores();


        void inicializarProcesosPredio();

        void iniciarProcesosMozo();

        void iniciarProcesoLobby();

        void iniciarProcesoCocinero();

        void iniciarProcesoGerente();


        void iniciarSimulacion();

        void inicializarProcesosComensales();

        void finalizarProcesosRestaurant();

        int finalizarComensales();

        void eliminarIPCs();

        void eliminarSemaforos();

        void eliminarMemoriasCompartidas();

        void eliminarPipesFifos();


    public:

        MainProcess(int cantPartidosJugador, int predioC, int predioF, int cantMJugadores, int cantNJugadores);
//	MainProcess(int cantRecepcionistas, int cantMozos, int cantMesas, int cantComensales, int perdidas, Menu menu);

        /**
         * Devuelve la cantidad de comensales que pasaron por el restaurant.
         */
        mainProcessReturnData run();

        virtual ~MainProcess();
    };

} /* namespace std */

#endif /* PROCESSES_MAINPROCESS_H_ */
