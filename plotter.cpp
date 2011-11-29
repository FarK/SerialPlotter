#include <QWidget>
#include <QBoxLayout>
#include <QSlider>
#include <QString>
#include <QLineEdit>
#include "plotter.h"
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <cmath>
#include <cstdio>

MyPlot::MyPlot(QWidget *parent) : QWidget(parent),
				  plot(this),
				  t0(0), time(0), zoom(0),
				  slider(Qt::Horizontal,this),
				  sliderText(QString::number(100), this),
				  layout(this)
{
	//Layout
	layout.addWidget(&plot);
	layout.addWidget(&slider);
	layout.addWidget(&sliderText);

	//Signals
	connect(&slider, SIGNAL(valueChanged(int)),this, SLOT(setZoom(int)));
	connect(&sliderText, SIGNAL(editingFinished()),this, SLOT(setZoom()));

	//Timmer
	idTimmer = startTimer(25);

	//Propieties
	plot.setTitle( "This is an Example" );
	slider.setRange(1,100);
	slider.setSingleStep(1);
	slider.setSliderPosition(100);
	sliderText.setMaxLength(3);

	// Show a legend at the bottom

	// Show the axes
	//plot.setAxisTitle( QwtPlot::xBottom, "x" );
	//plot.setAxisTitle( QwtPlot::yLeft, "y" );

	curve.attach(&plot);
	plot.replot();
}

void MyPlot::timerEvent(QTimerEvent *event){
	t0 = time - (int)(time/100.0*zoom+0.5);
	x.insert(time,time);
	y.insert(time,sin((double)time/6.2832));
	curve.setRawData(&x.data()[t0], &y.data()[t0], time-t0+1);
	//printf("plot %i: (%f,%f)\n",time,x[time],y[time]);
	curve.itemChanged();
	plot.replot();
	++time;
}

void MyPlot::setZoom(int z){
	zoom = z;
	sliderText.setText(QString::number(zoom));
}

void MyPlot::setZoom(){
	zoom = sliderText.text().toInt();
	slider.setSliderPosition(zoom);
}
