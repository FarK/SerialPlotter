#ifndef MULTI_PLOTTER_H
#define MULTI_PLOTTER_H

#include <QWidget>
#include <QVarLengthArray>
#include "plotter.h"
#include "serialLib/serialLinux.h"

class MultiPlotter : public QWidget{
	Q_OBJECT

	private:
		char* port;
		QVarLengthArray<Plotter*,5> plotters;
		Serial serialManager;

	public:
		MultiPlotter(int numPlotters);

	public slots:
		void newData(char* data, long size);
};

#endif
