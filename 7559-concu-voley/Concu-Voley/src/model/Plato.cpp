/*
 * Plato.cpp
 *
 *  Created on: Oct 15, 2016
 *      Author: gaston
 */

#include "Plato.h"

Plato::Plato(string nombre, double precio) {
	this->nombre = nombre;
	this->precio = precio;
}

const string& Plato::getNombre() const {
	return nombre;
}

void Plato::setNombre(const string& nombre) {
	this->nombre = nombre;
}

double Plato::getPrecio() const {
	return precio;
}

void Plato::setPrecio(double precio) {
	this->precio = precio;
}

Plato::~Plato() {
	// TODO Auto-generated destructor stub
}

