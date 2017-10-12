/*
 * Parser.h
 *
 *  Created on: Mar 23, 2016
 *      Author: gonzalo
 */

#ifndef UTILS_PARSER_PARSER_H_
#define UTILS_PARSER_PARSER_H_

#include <string>
#include <algorithm>
#include <sstream>
#include <iostream>
#include "pugixml/pugixml.hpp"


class Parser
{
private:
	int cantPartidosJugador;
	int predioF;
	int predioC;
	int cantMJugadores;
	int nivelDeLog;

	void extraerCantPartidosJugador(const pugi::xml_document* doc);
	void extraerPredioF(const pugi::xml_document* doc);
	void extraerPredioC(const pugi::xml_document* doc);
	void extraerCantMJugadores(const pugi::xml_document* doc);
	void extraerNivelDeLog(const pugi::xml_document* doc);

public:
	Parser();
	~Parser();
	void parsearDocumento(const std::string& nombreArchivoXML);
	int getCantPartidosJugador();
	int getPredioF();
	int getPredioC();
	int gentCantMJugadores();
	int getNivelDeLog();

};


#endif /* UTILS_PARSER_PARSER_H_ */
