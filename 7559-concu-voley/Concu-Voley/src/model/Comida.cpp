/*
 * Comida.cpp
 *
 *  Created on: Oct 16, 2016
 *      Author: gaston
 */

#include "Comida.h"

Comida::Comida(int mesa) {
	this->mesa = mesa;
}

int Comida::getMesa() {
	return mesa;
}

void Comida::setMesa(int mesa) {
	this->mesa = mesa;
}

vector<Plato> Comida::getPlatos(){
	return platos;
}

void Comida::agregarPlato(Plato plato){
	platos.push_back(plato);
}


Comida::~Comida() {
	// TODO Auto-generated destructor stub
}

