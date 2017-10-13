

#ifndef PROCESSES_TIEMPOSESPERA_H_
#define PROCESSES_TIEMPOSESPERA_H_

using namespace std;

class TiemposEspera {

public:
	static const bool tiempos = true;

	static const int TIEMPO_RANDOM_ENTRE_JUGADORES = 2;

	static constexpr double PROBABILIDAD_GANAR_PAREJA_1 = 0.50;
	static constexpr double PROBABILIDAD_IRSE = 0.3;

	static constexpr double PROBABILIDAD_3_A_0 = 0.3;
	static constexpr double PROBABILIDAD_3_A_1 = 0.6;

	static const int TIEMPO_JUGAR = 8;
	static const int TIEMPO_AFUERA = 8;


};

#endif /* PROCESSES_TIEMPOSESPERA_H_ */
