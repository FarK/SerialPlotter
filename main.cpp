#include <QApplication>
#include <QWidget>
#include <QBoxLayout>
#include "plotter.h"

int main( int argc, char *argv[] )
{
	QApplication app(argc, argv);
	MyPlot myPlot;

	myPlot.show();

	return app.exec();
}
