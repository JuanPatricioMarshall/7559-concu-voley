

#include "RandomUtil.h"

#include <cstdlib>
#include <iostream>

namespace std {

bool RandomUtil::seeded = false;

RandomUtil::RandomUtil() {
	// TODO Auto-generated constructor stub

}

double RandomUtil::randomCeroUno(){
	if (!seeded){
		seed();
	}

	return ((double) rand() / (RAND_MAX));
}

int RandomUtil::randomInt(int max){
	if (!seeded){
		seed();
	}

	return (rand() % max);
}

void RandomUtil::seed() {
	srand (time(NULL));

	seeded = true;
}


RandomUtil::~RandomUtil() {
	// TODO Auto-generated destructor stub
}

} /* namespace std */

