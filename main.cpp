#include <QApplication>
#include <QWidget>
#include <QBoxLayout>
#include "plotter.h"
#include "multiPlotter.h"

int main( int argc, char *argv[] )
{
	QApplication app(argc, argv);
	//Plotter myPlot;

	//myPlot.show();
	
	MultiPlotter mp;

	mp.show();

	return app.exec();
}
