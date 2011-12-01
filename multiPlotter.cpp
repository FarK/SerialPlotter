#include "multiPlotter.h"
#include <QVarLengthArray>
#include <cstdio>

typedef struct
{
	float roll;
	float pitch;
	float yaw;
}euler_angles;

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
//	for(int i = 0 ; i < size ; ++i)
//		printf("%c", data[i]);
	long time = *(long*)&data[0];
	float roll= *(float*)&data[4];
	float pitch= *(float*)&data[8];
	float yaw= *(float*)&data[12];

	printf("%f\t%f\t%f\t%f\n",time, roll, pitch, yaw);
}
