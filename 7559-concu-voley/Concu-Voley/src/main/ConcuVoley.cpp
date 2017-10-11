

#include <iostream>
#include <string>
#include <vector>

#include "../model/Comida.h"
#include "../model/Menu.h"
#include "../model/Pedido.h"
#include "../model/Plato.h"
#include "../utils/parser/Parser.h"
#include "../utils/serializer/LlamadoAMozoSerializer.h"
#include "MainProcess.h"



using namespace std;

int main() {

	Parser p = Parser();
	p.parsearDocumento("SetUp.xml");

//	int nivelDeLog = p.getNivelDeLog();
//	int cantPartidosJugador = p.getCantPartidosJugador();
//	int predioF = p.getPredioF();
//	int predioC = p.getPredioC();
//	int cantMJugadores = p.gentCantMJugadores();

	int nivelDeLog = 0;
	int cantPartidosJugador = 1;
	int predioF = 1;
	int predioC = 1;
	int cantMJugadores = 4;
	int cantNJugadores = 8;

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
	Logger::log(mainLogId, "nivelDeLog: " + Logger::intToString(nivelDeLog), INFO);

	//TODO
	int jugadoresPendientes = cantNJugadores;
	mainProcessReturnData mainProcessReturnData;
	do {
		MainProcess mainProcess(cantPartidosJugador, predioC, predioF, cantMJugadores, cantNJugadores);
		mainProcessReturnData = mainProcess.run();

		jugadoresPendientes -= mainProcessReturnData.cantJugadoresTerminados;
	} while(jugadoresPendientes > 0);

	if ( mainProcessReturnData.ganadores.size() > 1){
		Logger::log(mainLogId, "Ganadores del torneo: ", INFO);
		for( int i = 0; i < mainProcessReturnData.ganadores.size(); i ++) {
			Logger::log(mainLogId, "Ganador del torneo: " + mainProcessReturnData.ganadores[i], INFO);
		}
	}
	else{
		Logger::log(mainLogId, "Ganador del torneo: " + mainProcessReturnData.ganadores[0], INFO);
	}


	Logger::log(mainLogId, "Simulacion finalizada", INFO);


	return 0;
}
