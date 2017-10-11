
#ifndef MODEL_PARTIDO_H_
#define MODEL_PARTIDO_H_

#include <string>

using namespace std;

class Partido {

private:
	vector<string> jugadores;
	int setsLocal;
	int setsVisitante;

public:
	Plato(string nombre, double precio);

	const string& getNombre() const;
	void setNombre(const string& nombre);
	double getPrecio() const;
	void setPrecio(double precio);

	virtual ~Plato();

};

#endif /* MODEL_PARTIDO_H */
