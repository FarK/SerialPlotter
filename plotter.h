#ifndef MY_PLOT
#define MY_PLOT

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QVector>
#include <QSlider>
#include <QScrollBar>
#include <QString>
#include <QLineEdit>
#include <qwt_plot_curve.h>
#include <qwt_plot.h>

class Plotter : public QWidget
{
	Q_OBJECT

	private:
	QVector<double> x, y;
	int t0, time, zoom;

	QwtPlot* plot;
	QwtPlotCurve* curve;

	QScrollBar* scroll;
	QSlider* slider;
	QLineEdit* sliderText;
	QHBoxLayout* layoutH;
	QWidget* layoutVWidget;
	QVBoxLayout* layoutV;
	int idTimmer;

	public:
	Plotter(QWidget *parent = NULL);
	void timerEvent(QTimerEvent *event);

	public slots:
	void setT0(int t0);
	void setZoom(int zoom);
	void setZoom();
};

#endif
