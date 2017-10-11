/*
 * Pedido.h
 *
 *  Created on: Oct 15, 2016
 *      Author: gaston
 */

#ifndef MODEL_PEDIDO_H_
#define MODEL_PEDIDO_H_

#include <vector>

#include "LlamadoAMozo.h"
#include "Plato.h"

using namespace std;

class Pedido: public LlamadoAMozo {

private:
	int mesa;
	vector<Plato> platos;

public:
	Pedido(int mesa);
	
	int getMesa();
	void setMesa(int mesa);
	vector<Plato> getPlatos();
	void agregarPlato(Plato plato);

	virtual ~Pedido();

};

#endif /* MODEL_PEDIDO_H_ */
