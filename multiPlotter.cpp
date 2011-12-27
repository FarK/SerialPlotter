#include "multiPlotter.h"
#include <QVarLengthArray>
#include <cstdio>
#include "serial.h"

MultiPlotter::MultiPlotter(int numPlot) :
	plotters(numPlot),
	serial("/dev/ttyUSB0", 9600)
{
	connect(&serial, SIGNAL(newFrame(Frame)), this, SLOT(newFrame(Frame)));

	//Creamos las gráficas
	for(int i = 0 ; i < numPlot ; ++i){
		plotters.append(new Plotter(this));
	}

	serial.start();
}

MultiPlotter::~MultiPlotter(){
	//TODO: Terminar la ejecución del hilo hijo ántes de salir
	//serial.exit();
}

void MultiPlotter::newFrame(Frame frame){
	trataLaTrama(&frame);
}
