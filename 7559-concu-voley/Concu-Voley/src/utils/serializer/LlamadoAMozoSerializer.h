/*
 * LlamadoAMozoSerializer.h
 *
 *  Created on: Oct 16, 2016
 *      Author: gaston
 */

#ifndef UTILS_SERIALIZER_LLAMADOAMOZOSERIALIZER_H_
#define UTILS_SERIALIZER_LLAMADOAMOZOSERIALIZER_H_

#include <string>

#include "../../model/Comida.h"
#include "../../model/Pedido.h"
#include "../../model/PedidoCuenta.h"

namespace std {

class LlamadoAMozoSerializer {
private:

	static string intToString(int i);
	static string doubleToString(double d);

public:

	static const char SEPARADOR = ',';

	static const int PEDIDO = 1;
	static const int COMIDA = 2;
	static const int PEDIDO_CUENTA = 3;

	LlamadoAMozoSerializer();

	static string serializar(Pedido pedido);
	static string serializar(Comida comida);
	static string serializar(PedidoCuenta pedidoCuenta);

	static int getTipoLlamado(string data);

	static Pedido deserializarPedido(string pedidoStr);
	static Comida deserializarComida(string comidaStr);
	static PedidoCuenta deserializarPedidoCuenta(string pedidoCuentaStr);

	virtual ~LlamadoAMozoSerializer();
};

} /* namespace std */

#endif /* UTILS_SERIALIZER_LLAMADOAMOZOSERIALIZER_H_ */
