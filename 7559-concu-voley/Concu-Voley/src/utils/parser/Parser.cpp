/*

 * Parser.cpp
 *
 *  Created on: Oct 16, 2016
 *      Author: juampa_94
*/

#include "Parser.h"


#include <iostream>
#include <string>

using namespace std;

#include "Parser.h"

Parser::Parser()
{

}

Parser::~Parser()
{

}


double string_to_double( const string& s )
{
	std::istringstream i(s);
	double x;
	i >> x;
	return x;
}

int string_to_int(const string& s){
	int result;
	std::stringstream(s) >> result;
	return result;
}

void Parser::extraerCantPartidosJugador(const pugi::xml_document* doc)
{
	pugi::xml_node setUpNode = doc->child("SetUp");

	std::string cantPartidosJugador = setUpNode.child("cantPartidosJugador").first_child().value();

	this->cantPartidosJugador = string_to_int(cantPartidosJugador);
}

void Parser::extraerPredioF(const pugi::xml_document* doc)
{
	pugi::xml_node setUpNode = doc->child("SetUp");

	std::string predioF = setUpNode.child("predioF").first_child().value();

	this->predioF = string_to_int(predioF);

}
void Parser::extraerPredioC(const pugi::xml_document* doc){

	pugi::xml_node setUpNode = doc->child("SetUp");
	std::string predioC = setUpNode.child("predioC").first_child().value();


	this->predioC = string_to_int(predioC);

}
void Parser::extraerCantMJugadores(const pugi::xml_document* doc){

	pugi::xml_node setUpNode = doc->child("SetUp");
	std::string cantMJugadores = setUpNode.child("cantMJugadores").first_child().value();

	this->cantMJugadores = string_to_int(cantMJugadores);

}
void Parser::extraerNivelDeLog(const pugi::xml_document* doc){

	pugi::xml_node setUpNode = doc->child("SetUp");
	std::string nivelDeLog = setUpNode.child("nivelDeLog").first_child().value();
	this->nivelDeLog = string_to_int(nivelDeLog);
}


void Parser::parsearDocumento(const std::string& nombreArchivoXML)
{
	pugi::xml_document doc;


	pugi::xml_parse_result result = doc.load_file(nombreArchivoXML.c_str());


	pugi::xml_node root = doc.root().first_child();

	extraerCantPartidosJugador(&doc);
	extraerPredioF(&doc);
	extraerPredioC(&doc);
	extraerCantMJugadores(&doc);
	extraerNivelDeLog(&doc);
}

int Parser::getCantPartidosJugador(){
	return this->cantPartidosJugador;
}
int Parser::getPredioF(){
	return this->predioF;
}
int Parser::getPredioC(){
	return this->predioC;
}
int Parser::gentCantMJugadores(){
	return this->cantMJugadores;
}

int Parser::getNivelDeLog(){
	return this->nivelDeLog;
}

