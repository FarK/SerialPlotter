#include "multiPlotter.h"
#include <QVarLengthArray>
#include <cstdio>

MultiPlotter::MultiPlotter(int numPlot) :
	plotters(numPlot),
	serialManager("/dev/ttyUSB0", 9600, P_8N1)
{
	connect(&serialManager, SIGNAL(newData(char*, long)), this, SLOT(newData(char*,long)));

	for(int i = 0 ; i < numPlot ; ++i){
		plotters.append(new Plotter(this));
	}

	serialManager.run();
}

void MultiPlotter::newData(char data[], long size){
	for(int i = 0 ; i < size ; ++i)
		printf("%c", data[i]);
}
