#ifndef MULTI_PLOTTER_H
#define MULTI_PLOTTER_H

#include <QWidget>
#include <QVarLengthArray>

class MultiPlotter : QWidget{
	private:
		QVarLengthArray<Plotter,5> plotterArray;

	public:
		MultiPlotter(int numPlotters);
};

#endif
