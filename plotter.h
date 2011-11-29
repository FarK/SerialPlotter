#ifndef MY_PLOT
#define MY_PLOT

#include <QWidget>
#include <QVBoxLayout>
#include <QVector>
#include <QSlider>
#include <QString>
#include <QLineEdit>
#include <qwt_plot_curve.h>
#include <qwt_plot.h>

class MyPlot : public QWidget
{
	Q_OBJECT

	private:
	QwtPlot plot;
	QwtPlotCurve curve;
	QVector<double> x, y;
	int t0, time, zoom;
	QSlider slider;
	QLineEdit sliderText;
	QVBoxLayout layout;
	int idTimmer;

	public:
	MyPlot(QWidget *parent = NULL);
	void timerEvent(QTimerEvent *event);

	public slots:
	void setZoom(int zoom);
	void setZoom();
};

#endif
