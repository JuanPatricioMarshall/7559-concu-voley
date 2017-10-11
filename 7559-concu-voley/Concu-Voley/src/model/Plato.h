/*
 * Plato.h
 *
 *  Created on: Oct 15, 2016
 *      Author: gaston
 */

#ifndef MODEL_PLATO_H_
#define MODEL_PLATO_H_

#include <string>

using namespace std;

class Plato {

private:
	string nombre;
	double precio;

public:
	Plato(string nombre, double precio);

	const string& getNombre() const;
	void setNombre(const string& nombre);
	double getPrecio() const;
	void setPrecio(double precio);

	virtual ~Plato();

};

#endif /* MODEL_PLATO_H_ */
