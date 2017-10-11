
#include "MainProcess.h"

#include <sys/wait.h>
#include <unistd.h>
#include <csignal>
#include <cstdlib>

#include "../processes/AdminComensalesProcess.h"
#include "../processes/CocineroProcess.h"
#include "../processes/MozoProcess.h"
#include "../processes/GerenteProcess.h"
#include "../utils/ipc/signal/SignalHandler.h"
#include "../processes/RecepcionistaProcess.h"
#include "../processes/CupidoProcess.h"
#include "../processes/AdminJugadoresProcess.h"

namespace std {

    MainProcess::MainProcess(int cantPartidosJugador, int predioC, int predioF, int cantMJugadores, int cantNJugadores)
            :
            semJugadoresPredio(SEM_JUGADORES_PREDIO_INIT_FILE, cantMJugadores, 0),
            semEsperarRecepcionista(SEM_ESPERAR_RECEPCIONISTA_INIT_FILE, 0, 0),
            semRanking(SEM_RANKING_INIT_FILE, 1, 0),
            semCupido(SEM_CUPIDO_INIT_FILE, 0, 0),
            semCantCanchasLibres(SEM_CANT_CANCHAS_LIBRES_FILE, predioC * predioF, 0) {

        this->cantPartidosJugador = cantPartidosJugador;
        this->predioC = predioC;
        this->predioF = predioF;
        this->cantMJugadores = cantMJugadores;
        this->cantNJugadores = cantNJugadores;

        inicializarIPCs();
    }

//void MainProcess::iniciarProcesoCocinero() {
//	Logger::log(mainLogId, "Iniciando cocinero", DEBUG);
//	pid_t idCocinero = fork();
//	if (idCocinero == 0) {
//		CocineroProcess cocinero(&pipePedidosACocinar, &semsFacturas,
//				&shmFacturas, &pipeLlamadosAMozos);
//		cocinero.run();
//		exit(0);
//	} else {
//		this->idCocinero = idCocinero;
//	}
//}
//void MainProcess::iniciarProcesoGerente() {
//	Logger::log(mainLogId, "Iniciando gerente", DEBUG);
//	pid_t idGerente = fork();
//	if (idGerente == 0) {
//		GerenteProcess gerente(&semCajaB, &shmCaja, &semPersonasLivingB,
//				&shmPersonasLiving, perdidas);
//		gerente.run();
//		exit(0);
//	} else {
//		this->idGerente = idGerente;
//	}
//}

//void MainProcess::iniciarProcesosMozo() {
//	for (int i = 0; i < cantMozos; i++) {
//		Logger::log(mainLogId, "Iniciando mozo " + Logger::intToString(i),
//				DEBUG);
//
//		pid_t idMozo = fork();
//
//		if (idMozo == 0) {
//			MozoProcess mozo(&pipeLlamadosAMozos, &pipePedidosACocinar,
//					&semLlamadosAMozos, &semsLlegoComida, &semsFacturas,
//					&shmFacturas, &semCajaB, &shmCaja, &semsMesaPago);
//			mozo.run();
//			exit(0);
//		} else {
//			idsMozos.push_back(idMozo);
//		}
//
//	}
//}

    /*  void MainProcess::iniciarProcesoLobby() {
        Logger::log(mainLogId, "Iniciando Lobby ",
                    DEBUG);
        pid_t idLobby = fork();

        if (idLobby == 0) {
            LobbyProcess lobbyProcess(&semSalaDeEspera);
            lobbyProcess.run();
            exit(0);
        }
    }*/
}


void MainProcess::inicializarSemaforos() {


    for (int i = 0; i < cantNJugadores; i++) {
        semsTerminoDeJugar.push_back(Semaforo(SEMS_TERMINO_DE_JUGAR_INIT_FILE, 0, 0));
    }


    for (int i = 0; i < predioF; i++) {

        vector<Semaforo> semFilaCanchaLibre;

        for (int j = 0; j < predioC; j++) {
            semFilaCanchaLibre.push_back(
                    Semaforo(SEMS_CANCHA_OCUPADA_INIT_FILE, 1, j));

        }

        semCanchasLibres.push_back(semFilaCanchaLibre);

    }


}

void MainProcess::inicializarMemoriasCompartidas() {

    // Bloqueo para primero crear los procesos, y despues asignarles los valores iniciales sin que ningun proceso hijo acceda antes.
//	semPersonasLivingB.p();
//	semCajaB.p();

    /*
    for (int i = 0; i < predioF; i++) {

        vector<MemoriaCompartida> shmFilaCanchaLibre;

        for (int j = 0; j < predioC; j++) {
            shmFilaCanchaLibre.push_back(
                    MemoriaCompartida(SEMS_CANCHA_OCUPADA_INIT_FILE, 1, j));

        }

        shmCanchasLibres.push_back(shmFilaCanchaLibre);

    }

    for(int i = 0; i < cantMesas; i++){
        semsFacturas.at(i).p();
        shmFacturas.push_back(MemoriaCompartida<double>());

        semsMesasLibres.at(i).p();
        shmMesasLibres.push_back(MemoriaCompartida<bool>());
    }*/

}

void MainProcess::crearMemoriasCompartidas() {

//	Logger::log(mainLogId, "Comenzando inicializacion de memorias compartidas ",
//			DEBUG);
//	shmPersonasLiving.crear(SHM_PERSONAS_LIVING, 0);
//	shmPersonasLiving.escribir(0);
//	semPersonasLivingB.v();
//
//	shmCaja.crear(SHM_CAJA, 0);
//	shmCaja.escribir(0);
//	semCajaB.v();
//
//	for (int i = 0; i < cantMesas; i++) {
//		shmFacturas.at(i).crear(SHM_FACTURAS, i);
//		shmFacturas.at(i).escribir(0);
//		semsFacturas.at(i).v();
//
//		shmMesasLibres.at(i).crear(SHM_MESAS_LIBRES, i);
//		shmMesasLibres.at(i).escribir(true);
//		semsMesasLibres.at(i).v();
//
//	}
//
//	Logger::log(mainLogId, "Fin de inicializacion de memorias compartidas ",
//			DEBUG);
}

void MainProcess::inicializarPipesFifos() {
//
//    this->pipeJugadores =

}

void MainProcess::inicializarIPCs() {
    inicializarSemaforos();
    inicializarMemoriasCompartidas();
    inicializarPipesFifos();

}

void MainProcess::finalizarProcesosRestaurant() {
//	Logger::log(mainLogId, "Finalizando procesos del restaurant", DEBUG);
//
//	for (unsigned int i = 0; i < idsRecepcionistas.size(); i++) {
//		Logger::log(mainLogId,
//				"Finalizando recepcionista "
//						+ Logger::intToString(idsRecepcionistas[i]), DEBUG);
//		kill(idsRecepcionistas[i], SIGINT);
//		waitpid(idsRecepcionistas[i], NULL, 0);
//	}
//
//	for (unsigned int i = 0; i < idsMozos.size(); i++) {
//		Logger::log(mainLogId,
//				"Finalizando mozo " + Logger::intToString(idsMozos[i]), DEBUG);
//		kill(idsMozos[i], SIGINT);
//		waitpid(idsMozos[i], NULL, 0);
//	}
//
//	Logger::log(mainLogId,
//			"Finalizando cocinero" + Logger::intToString(idCocinero), DEBUG);
//	kill(idCocinero, SIGINT);
//	waitpid(idCocinero, NULL, 0);

}

int MainProcess::finalizarComensales() {
//	Logger::log(mainLogId, "Finalizando procesos de comensales", DEBUG);
//	kill(idAdminComensales, SIGINT);
//	int cantComensalesFinalizados;
//	int response;
//	waitpid(idAdminComensales, &response, 0);
//	if (WIFEXITED(response)) {
//		cantComensalesFinalizados = WEXITSTATUS(response);
//	}
//
//	Logger::log(mainLogId,
//			"Cantidad de comensales finalizados: "
//					+ Logger::intToString(cantComensalesFinalizados), DEBUG);
//	return cantComensalesFinalizados;
}

void MainProcess::inicializarProcesosJugadores() {

    pid_t idAdminJugadores = fork();

    if (idAdminJugadores == 0) {

        AdminJugadoresProcess adminJugadoresProcess(cantNJugadores, cantPartidosJugador,
                                                    &semsTerminoDeJugar,
                                                    &semEsperarRecepcionista, &semJugadoresPredio, &pipeJugadores);

        int jugadoresTerminados = adminJugadoresProcess.run();

        Logger::log(adminJugadoresLogId,
                    "Exit con cantidad de jugtadores finalizados: " + Logger::intToString(jugadoresTerminados), DEBUG);

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

//void MainProcess::acumularPerdidas(){
//
//	Logger::log(mainLogId, "Acumulando perdidas por corte de luz", INFO);
//	for (int mesa = 0; mesa < cantMesas; mesa++){
//		semsFacturas.at(mesa).p();
//		Logger::log(mainLogId, "Leyendo factura de mesa: " + Logger::intToString(mesa), DEBUG);
//		double factura = shmFacturas.at(mesa).leer();
//		Logger::log(mainLogId, "Factura de mesa leida: $" + Logger::doubleToString(factura), DEBUG);
//		perdidas += factura;
//
//		semsFacturas.at(mesa).v();
//	}
//	Logger::log(mainLogId, "Perdidas acumuladas: $" + Logger::doubleToString(perdidas), INFO);
//}

//
//int MainProcess::handleCorteLuz(){
//	Logger::log(mainLogId, "CORTE DE LUZ", INFO);
//	acumularPerdidas();
//	int comensalesFinalizados = finalizarComensales();
//	finalizarProcesosRestaurant();
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

        RecepcionistaProcess recepcionista(&semEsperarRecepcionista, &semCupido);
        recepcionista.run();
        exit(0);
    }

}


void MainProcess::inicializarCupido() {

    pid_t idCupido = fork();

    if (idCupido == 0) {

        CupidoProcess cupidoProcess(&pipeJugadores, &semCanchasLibres,
                                    &shmCanchasLibres, cantNJugadores, &semCupido, &semsTerminoDeJugar,
                                    &semCantCanchasLibres, &pipeResultados, &pipeFixture);
        cupidoProcess.run();
        exit(0);
    }

}


mainProcessReturnData MainProcess::run() {
    iniciarSimulacion();
    Logger::log(mainLogId, "Simulacion iniciada", DEBUG);

    int jugadoresTerminados = 0;
    bool salir = false;

    for (int i = 0; i < 10; i++) {
        Logger::log(mainLogId,
                    "Cantidad de Jugadores finalizados: "
                    + Logger::intToString(jugadoresTerminados),
                    DEBUG);
    }

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
            waitpid(idGerente, NULL, 0);
        } else {

            //Leyendo respuesta del AdminComensales
            if (WIFEXITED(response)) {
                jugadoresTerminados = WEXITSTATUS(response);
            }
            Logger::log(mainLogId,
                        "Cantidad de jugadores finalizados: "
                        + Logger::intToString(jugadoresTerminados),
                        DEBUG);

            finalizarProcesosRestaurant();
            eliminarIPCs();

        }
//        salir = !llegoGerente;
        //TODO
        salir = false;
    }
    mainProcessReturnData returnData;
    // TODO
    returnData.cantJugadoresTerminados = 0;
    returnData.ganadores = *new vector<string>();
    returnData.ganadores.push_back("Fazzito");
//	returnData.cantComensalesFinalizados = comensalesFinalizados;
//	returnData.perdidas = perdidas;

    return returnData;

}

void MainProcess::eliminarSemaforos() {


//	semRecepcionistasLibres.eliminar();



//    semSalaDeEspera.eliminar();
//
//    for (int i = 0; i < predioF; i++) {
//        for (int j = 0; j < predioC; j++) {
//            semsCanchaLibre.at(i).at(j).eliminar();
//            semsListoParaJugar.at(i).at(j).eliminar();
//            semsPartidoTerminado.at(i).at(j).eliminar();
//
//        }
//    }

//
//	for (int i = 0; i < cantMesas; i++) {
//		semsLlegoComida.at(i).eliminar();
//
//		semsMesaPago.at(i).eliminar();
//
//		semsFacturas.at(i).eliminar();
//
//		semsMesasLibres.at(i).eliminar();
//
//	}
}

void MainProcess::eliminarMemoriasCompartidas() {

//	shmPersonasLiving.liberar();
//
//	shmCaja.liberar();
//
//	for (int i = 0; i < cantMesas; i++) {
//
//		shmFacturas.at(i).liberar();
//
//		shmMesasLibres.at(i).liberar();
//
//	}
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
