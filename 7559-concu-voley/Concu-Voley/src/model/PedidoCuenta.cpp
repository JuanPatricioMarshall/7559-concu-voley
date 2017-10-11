/*
 * PedidoCuenta.cpp
 *
 *  Created on: Oct 16, 2016
 *      Author: gaston
 */

#include "PedidoCuenta.h"

namespace std {

PedidoCuenta::PedidoCuenta(int mesa) {
	this->mesa = mesa;
}

int PedidoCuenta::getMesa() {
	return mesa;
}

void PedidoCuenta::setMesa(int mesa) {
	this->mesa = mesa;
}

PedidoCuenta::~PedidoCuenta() {
	// TODO Auto-generated destructor stub
}

} /* namespace std */
