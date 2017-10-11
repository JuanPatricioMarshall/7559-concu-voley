/*
 * Comida.h
 *
 *  Created on: Oct 16, 2016
 *      Author: gaston
 */

#ifndef MODEL_COMIDA_H_
#define MODEL_COMIDA_H_

#include <vector>

#include "LlamadoAMozo.h"
#include "Plato.h"

using namespace std;

class Comida : public LlamadoAMozo{

private:
	int mesa;
	vector<Plato> platos;

public:
	Comida(int mesa);

	int getMesa();
	void setMesa(int mesa);
	vector<Plato> getPlatos();
	void agregarPlato(Plato plato);

	virtual ~Comida();

};

#endif /* MODEL_COMIDA_H_ */
