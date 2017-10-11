/*
 * Menu.h
 *
 *  Created on: Oct 16, 2016
 *      Author: gaston
 */

#ifndef MODEL_MENU_H_
#define MODEL_MENU_H_

#include <vector>

#include "Plato.h"

namespace std {

class Menu {
private:

	vector<Plato> platos;

public:
	Menu();

	Plato getPlatoRandom();

	void agregarPlato(Plato plato);

	virtual ~Menu();
};

} /* namespace std */

#endif /* MODEL_MENU_H_ */
