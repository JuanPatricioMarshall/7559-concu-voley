
#include "Process.h"


namespace std {

Process::Process() {
}



void Process::finalizar(){
	Logger::log(processLogId, "Finalizando proceso", DEBUG);
	exit(0);
}

Process::~Process() {
	// TODO Auto-generated destructor stub
}

} /* namespace std */
