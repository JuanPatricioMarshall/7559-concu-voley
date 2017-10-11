/*
 * TiemposEspera.h
 *
 *  Created on: Oct 20, 2016
 *      Author: gaston
 */

#ifndef PROCESSES_TIEMPOSESPERA_H_
#define PROCESSES_TIEMPOSESPERA_H_

using namespace std;

class TiemposEspera {

public:
	static const bool tiempos = true;

	static const int TIEMPO_RANDOM_ENTRE_JUGADORES = 15;
	static const int TIEMPO_ATENDER = 5;
	static const int TIEMPO_TOMAR_PEDIDO = 2;
	static const int TIEMPO_COMER = 10;
	static const int TIEMPO_ELECCION_COMIDA = 2;
	static constexpr double PROBABILIDAD_DEJAR_DE_COMER = 0.50;

	static constexpr double PROBABILIDAD_GANAR_PAREJA_1 = 0.50;
	static constexpr double PROBABILIDAD_IRSE = 0.50;

	static constexpr double PROBABILIDAD_3_A_0 = 0.3;
	static constexpr double PROBABILIDAD_3_A_1 = 0.6;

	static const int TIEMPO_JUGAR = 15;
	static const int TIEMPO_AFUERA = 10;

	static const int TIEMPO_COCINA = 5;
	static const int TIEMPO_CORTE_LUZ = 20;

};

#endif /* PROCESSES_TIEMPOSESPERA_H_ */
