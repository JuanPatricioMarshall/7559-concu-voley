

#include <iostream>

#include "MainProcess.h"



using namespace std;

int main(int argc, char *argv[]) {

	int nivelDeLog = atoi(argv[1]);
	int cantPartidosJugador = atoi(argv[2]);
	int predioF = atoi(argv[3]);
	int predioC = atoi(argv[4]);
	int cantMJugadores = atoi(argv[5]);
	int cantNJugadores = atoi(argv[6]);

    cout << cantNJugadores <<" "<<getpid()<<" "<<getppid()<<endl;

    //TODO cambiar por 10
    int cantJugadoresMinimo = 10;

	LOG_MODE mode;
	if(nivelDeLog == 0) mode = DEBUG;
	else if(nivelDeLog == 1) mode = ERROR;
	else mode = INFO;

	Logger::setMode(mode);


	if (cantPartidosJugador <= 0 || predioF <= 0 || predioC <= 0 || cantMJugadores <= 0 ){
		Logger::log(mainLogId, "Datos invalidos en configuracion.", INFO);
		exit(0);
	}

	Logger::log(mainLogId, "Cantidad k de partidos maximos por jugador: " + Logger::intToString(cantPartidosJugador), INFO);
	Logger::log(mainLogId, "Cantidad de columnas del predio: " + Logger::intToString(predioC), INFO);
	Logger::log(mainLogId, "Cantidad de filas del predio: " + Logger::intToString(predioF), INFO);
	Logger::log(mainLogId, "Cantidad m de jugadores maxima del predio: " + Logger::intToString(cantMJugadores), INFO);
	Logger::log(mainLogId, "Cantidad n de jugadores a crear: " + Logger::intToString(cantNJugadores), INFO);
    Logger::log(mainLogId, "Cantidad minima de jugadores para empezar el torneo: " + Logger::intToString(cantJugadoresMinimo), INFO);

    Logger::log(mainLogId, "nivelDeLog: " + Logger::intToString(nivelDeLog), INFO);

	//TODO
	int jugadoresPendientes = cantNJugadores;
	mainProcessReturnData mainProcessReturnData;
	Logger::log(mainLogId, "IntentoCrearMainProcess", DEBUG);
	MainProcess mainProcess(cantPartidosJugador, predioC, predioF, cantMJugadores, cantNJugadores, cantJugadoresMinimo);
	Logger::log(mainLogId, "MainProcessCreadoCorrecto", DEBUG);
	mainProcessReturnData = mainProcess.run();

	Logger::log(mainLogId, "Simulacion finalizada", INFO);


	return 0;
}
