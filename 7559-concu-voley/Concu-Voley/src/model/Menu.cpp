/*
 * Menu.cpp
 *
 *  Created on: Oct 16, 2016
 *      Author: gaston
 */

#include "Menu.h"

#include "../utils/random/RandomUtil.h"

namespace std {

Menu::Menu() {
	// TODO Auto-generated constructor stub
}

Plato Menu::getPlatoRandom(){
	return platos.at(RandomUtil::randomInt(platos.size()));
}

void Menu::agregarPlato(Plato plato){
	platos.push_back(plato);
}

Menu::~Menu() {
	// TODO Auto-generated destructor stub
}

} /* namespace std */
