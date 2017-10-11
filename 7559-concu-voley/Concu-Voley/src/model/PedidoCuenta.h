/*
 * PedidoCuenta.h
 *
 *  Created on: Oct 16, 2016
 *      Author: gaston
 */

#ifndef MODEL_PEDIDOCUENTA_H_
#define MODEL_PEDIDOCUENTA_H_

namespace std {

class PedidoCuenta {

private:
	int mesa;

public:
	PedidoCuenta(int mesa);

	int getMesa();
	void setMesa(int mesa);

	virtual ~PedidoCuenta();
};

} /* namespace std */

#endif /* MODEL_PEDIDOCUENTA_H_ */
