/*
 * Pedido.cpp
 *
 *  Created on: Oct 15, 2016
 *      Author: gaston
 */

#include "Pedido.h"

Pedido::Pedido(int mesa) {
	this->mesa = mesa;
}

int Pedido::getMesa() {
	return mesa;
}

void Pedido::setMesa(int mesa) {
	this->mesa = mesa;
}

vector<Plato> Pedido::getPlatos(){
	return platos;
}

void Pedido::agregarPlato(Plato plato){
	platos.push_back(plato);
}

Pedido::~Pedido() {
	// TODO Auto-generated destructor stub
}

