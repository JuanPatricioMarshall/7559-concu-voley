
#ifndef PROCESSES_MAINPROCESS_H_
#define PROCESSES_MAINPROCESS_H_

#include <sched.h>
#include <string>
#include <vector>


#include "../utils/ipc/pipe/Pipe.h"
#include "../utils/ipc/semaphore/Semaforo.h"
#include "../utils/ipc/shared-memory/MemoriaCompartida.h"
#include "../utils/ipc/signal/SIGINT_Handler.h"
#include "../utils/ipc/signal/SIGUSR1_Handler.h"
#include "../processes/TiemposEspera.h"
#include "../utils/ipc/signal/SIGUSR2_Handler.h"


namespace std {

    const string mainLogId = "Main";


    const string SEMS_CANCHA_OCUPADA_INIT_FILE = "/home/matiascarballo/Escritorio/tp/7559-concu-voley/7559-concu-voley/Concu-Voley/ipc-init-files/sems_cancha_ocupada.txt";
    const string SEM_JUGADORES_PREDIO_INIT_FILE = "/home/matiascarballo/Escritorio/tp/7559-concu-voley/7559-concu-voley/Concu-Voley/ipc-init-files/sems_jugadores_predio.txt";
    const string SEM_ESPERAR_RECEPCIONISTA_INIT_FILE = "/home/matiascarballo/Escritorio/tp/7559-concu-voley/7559-concu-voley/Concu-Voley/ipc-init-files/sems_esperar_recepcionista.txt";
    const string SEMS_TERMINO_DE_JUGAR_INIT_FILE = "/home/matiascarballo/Escritorio/tp/7559-concu-voley/7559-concu-voley/Concu-Voley/ipc-init-files/sems_termino_de_jugar.txt";
    const string SEM_RANKING_INIT_FILE = "/home/matiascarballo/Escritorio/tp/7559-concu-voley/7559-concu-voley/Concu-Voley/ipc-init-files/sems_ranking.txt";
    const string SEM_CUPIDO_INIT_FILE = "/home/matiascarballo/Escritorio/tp/7559-concu-voley/7559-concu-voley/Concu-Voley/ipc-init-files/sems_cupido.txt";

    const string SEM_CANT_CANCHAS_LIBRES_FILE = "/home/matiascarballo/Escritorio/tp/7559-concu-voley/7559-concu-voley/Concu-Voley/ipc-init-files/sems_cant_canchas_libres.txt";
    const string SHM_CANCHAS_LIBRES = "/home/matiascarballo/Escritorio/tp/7559-concu-voley/7559-concu-voley/Concu-Voley/ipc-init-files/shm_canchas_libres.txt";

    const string SHM_NIVEL_MAREA_INIT_FILE = "/home/matiascarballo/Escritorio/tp/7559-concu-voley/7559-concu-voley/Concu-Voley/ipc-init-files/shm_nivel_marea.txt";
    const string SEM_NIVEL_DE_MAREA_INIT_FILE = "/home/matiascarballo/Escritorio/tp/7559-concu-voley/7559-concu-voley/Concu-Voley/ipc-init-files/sem_nivel_marea.txt";

    const string SHM_LISTA_PID_FILE = "/home/matiascarballo/Escritorio/tp/7559-concu-voley/7559-concu-voley/Concu-Voley/ipc-init-files/shm_lista_pid.txt";
    const string SEM_LISTA_PID_FILE = "/home/matiascarballo/Escritorio/tp/7559-concu-voley/7559-concu-voley/Concu-Voley/ipc-init-files/sem_lista_pid.txt";

    const string SEM_GENTE_EN_EL_PREDIO = "/home/matiascarballo/Escritorio/tp/7559-concu-voley/7559-concu-voley/Concu-Voley/ipc-init-files/sem_gente_en_el_predio.txt";
    const string SHM_GENTE_EN_EL_PREDIO = "/home/matiascarballo/Escritorio/tp/7559-concu-voley/7559-concu-voley/Concu-Voley/ipc-init-files/shm_gente_en_el_predio.txt";


    const string SEMS_JUGADOR_SIN_PAREJA = "/home/matiascarballo/Escritorio/tp/7559-concu-voley/7559-concu-voley/Concu-Voley/ipc-init-files/sems_jugador_sin_pareja.txt";

    const string SHM_JUGADORES_SIN_PAREJA = "/home/matiascarballo/Escritorio/tp/7559-concu-voley/7559-concu-voley/Concu-Voley/ipc-init-files/shm_jugadores_sin_pareja.txt";


    struct mainProcessReturnData {
        vector<string> ganadores;
        int cantJugadoresTerminados;
    };

    class MainProcess {

    public:

        int cantPartidosJugador;
        int predioC;
        int predioF;
        int cantMJugadores;
        int cantNJugadores;
        int cantJugadoresMinimo;


        pid_t idAdminJugadores;
        pid_t idRecepcionista;
        pid_t idCupido;
        pid_t idFixture;


        Semaforo semJugadoresPredio;
        Semaforo semEsperarRecepcionista;
        Semaforo semRanking;
        Semaforo semCupido;
        Semaforo semCantCanchasLibres;
        Semaforo semNivelDeMarea;
        Semaforo semCantGenteEnElPredio;

        vector<Semaforo> semsTerminoDeJugar;
        vector<vector<Semaforo>> semCanchasLibres;
        vector<Semaforo> semJugadoresSinPareja;


        vector<vector<MemoriaCompartida<bool>>> shmCanchasLibres;
        vector<MemoriaCompartida<bool>> shmJugadoresSinPareja;
        MemoriaCompartida<int> shmNivelDeMarea;
        MemoriaCompartida<int> shmCantGenteEnElPredio;

        vector<MemoriaCompartida<pid_t>> shmIdProcesos;
        vector<Semaforo> semIdProcesos;

        Pipe pipeJugadores;
        Pipe pipeResultados;
        Pipe pipeFixture;

        SIGINT_Handler sigintHandler;
        SIGUSR2_Handler sigusr2Handler;

        void reloco(){







            semJugadoresSinPareja.at(0).p();
            shmJugadoresSinPareja.push_back(MemoriaCompartida<bool>());


            vector<Semaforo> semIdProcesosX;
            semIdProcesosX.push_back(Semaforo(SEM_LISTA_PID_FILE, 1, 0));

            vector<MemoriaCompartida<pid_t>> shmIdProcesosX;
            shmIdProcesosX.crear(SHM_NIVEL_MAREA_INIT_FILE, 0);
            int idCupido = shmIdProcesosX.at(0).leer();




            Logger::log(mainLogId, "CRECE 1 NIVEL LA OLA", INFO);
            Semaforo semNivelDeMareaX(SEM_NIVEL_DE_MAREA_INIT_FILE, 1, 0);
            MemoriaCompartida<int>  shmNivelDeMareaX;
            shmNivelDeMareaX.crear(SHM_NIVEL_MAREA_INIT_FILE, 0);
            semNivelDeMareaX.p();
            Logger::log(mainLogId, "Semaforeado", INFO);
            int nivelMarea = shmNivelDeMareaX.leer();

            if (nivelMarea < predioC - 1) {
                nivelMarea++;
            }
            Logger::log(mainLogId, "Nivel actual de marea: " + Logger::intToString(nivelMarea), INFO);

            shmNivelDeMareaX.escribir(nivelMarea);
            semNivelDeMareaX.v();
            Logger::log(mainLogId, "Id Cupido?: " + Logger::intToString(idCupido), INFO);

        }
         void yarrgh()
        {
            signal( SIGUSR1, terminate );
        }
        static void terminate(int param){
            MainProcess myClass(0,0,0,0,0,0);
            myClass.reloco();

        }

        void inicializarIPCs();

        void handleTerminar();

        void inicializarSemaforos();

        void inicializarMemoriasCompartidas();

        void crearMemoriasCompartidas();

        void inicializarProcesoFixture();

        void inicializarSigintHandler();

        void inicializarSigusrHandler();


        void inicializarRecepcionista();

        void inicializarCupido();


        void inicializarProcesosJugadores();


        void iniciarSimulacion();


        void finalizarProcesosPredio();

        void finalizarJugadores();

        void eliminarIPCs();

        void eliminarSemaforos();

        void eliminarMemoriasCompartidas();

        void eliminarPipesFifos();



        void handleBajaOla();

        void avisarAPartidos(int sig);




        MainProcess(int cantPartidosJugador, int predioC, int predioF, int cantMJugadores, int cantNJugadores,
                    int cantJugadoresMinimo);

        mainProcessReturnData run();

        virtual ~MainProcess();
    };

} /* namespace std */

#endif /* PROCESSES_MAINPROCESS_H_ */
