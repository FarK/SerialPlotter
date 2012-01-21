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
		Plotter rollPlot;
		Plotter pitchPlot;
		Plotter yawPlot;
		Serial serial;

		QHBoxLayout* layoutH;

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
		MultiPlotter(QWidget *parent = NULL);
		~MultiPlotter();

	public slots:
		void newFrame(Frame frame);
};

#endif
