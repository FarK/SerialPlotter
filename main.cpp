#include <QApplication>
#include <QWidget>
#include <QVector>
#include <cstdio>
#include <qwt_plot.h>
#include <cmath>
#include <qwt_plot_curve.h>

#define TAM 50

int main( int argc, char *argv[] )
{
	//double x[TAM], y[TAM];
	QVector<double> x(TAM), y(TAM);

	for(int i = 0 ; i < TAM ; ++i){
		x[i] = i;
		y[i] = sin(i);
	}

	QApplication a( argc, argv );

	QWidget p;

	QwtPlot myPlot(QwtText("Two Curves"), &p);
	QwtPlotCurve curve("Curve");
	//curve.setData(x, y, TAM);
	curve.setData(x.data(), y.data(), TAM);
	curve.attach(&myPlot);

	myPlot.replot();

	p.show();

	return a.exec();
}
