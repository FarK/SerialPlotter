#ifndef MULTI_PLOTTER_H
#define MULTI_PLOTTER_H

#include <QWidget>
#include <QVarLengthArray>
#include "plotter.h"
#include "serial.h"

class MultiPlotter : public QWidget{
	Q_OBJECT

	private:
		char* port;
		QVarLengthArray<Plotter*,5> plotters;
		Serial serial;

		//Temporal
		void frameToHex(Frame* t){
			for(int i = 0 ; i < sizeof(Frame) ; ++i){
				if(i%4 == 0 && i != 0)	printf("  ");
				printf("[");
				printf("%X",t->buff[i]);
				printf("]");
			}
			printf("\n");
		}
		void trataLaTrama(Frame* t){
			frameToHex(t);
			printf("Time: %i\n",t->time);
			printf("Roll: %f\n",t->roll);
			printf("Pitch: %f\n",t->pitch);
			printf("Yaw: %f\n",t->yaw);
		}

	public:
		MultiPlotter(int numPlotters);
		~MultiPlotter();

	public slots:
		void newFrame(Frame frame);
};

#endif
