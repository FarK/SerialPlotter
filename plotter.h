#ifndef MY_PLOT
#define MY_PLOT

#include <qwt_plot.h>
#include <qwt_text.h>

class MyPlot : public QwtPlot
{
	private:
	double* x, *y;

	public:
	MyPlot(QWidget *parent = NULL);
};

#endif
