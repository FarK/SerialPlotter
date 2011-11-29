#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSlider>
#include <QString>
#include <QLineEdit>
#include "plotter.h"
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <cmath>
#include <cstdio>

MyPlot::MyPlot(QWidget *parent) : QWidget(parent),
				  t0(0), time(0), zoom(0)
{
	//Inicializamos
	plot = new QwtPlot(this);
	curve = new QwtPlotCurve;
	scroll = new QScrollBar(Qt::Horizontal,this);
	slider = new QSlider(Qt::Vertical,this);
	sliderText = new QLineEdit(QString::number(100), this);
	layoutVWidget = new QWidget(this);

	layoutH = new QHBoxLayout(this);
	layoutV = new QVBoxLayout(layoutVWidget);


	//Layout
	layoutV->addWidget(plot);
	layoutV->addWidget(scroll);
	layoutV->addWidget(sliderText);

	layoutVWidget->setLayout(layoutV);

	layoutH->addWidget(layoutVWidget);
	layoutH->addWidget(slider);

	//Signals
	connect(scroll, SIGNAL(valueChanged(int)),this, SLOT(setT0(int)));
	connect(slider, SIGNAL(valueChanged(int)),this, SLOT(setZoom(int)));
	connect(sliderText, SIGNAL(editingFinished()),this, SLOT(setZoom()));

	//Timmer
	idTimmer = startTimer(100);

	//Propieties
	plot->setTitle( "This is an Example" );
	slider->setRange(1,100);
	slider->setSingleStep(1);
	slider->setSliderPosition(100);
	sliderText->setMaxLength(3);

	scroll->setRange(0,0);
	scroll->setSingleStep(1);

	// Show a legend at the bottom

	// Show the axes
	//plot.setAxisTitle( QwtPlot::xBottom, "x" );
	//plot.setAxisTitle( QwtPlot::yLeft, "y" );

	curve->attach(plot);
	plot->replot();
}

void MyPlot::timerEvent(QTimerEvent *event){
	int timeWindow = (int)(time/100.0*zoom+0.5);
	x.insert(time,time);
	y.insert(time,sin((double)time/6.2832));
	curve->setRawData(&x.data()[t0], &y.data()[t0], timeWindow + 1);
	//printf("plot %i: (%f,%f)\n",time,x[time],y[time]);

	//Actualizamos el Scroll solo sí no está al máximo
	if(scroll->sliderPosition() != scroll->maximum()){
		scroll->setRange(0, time - timeWindow);
		scroll->setPageStep(timeWindow);
	}

	curve->itemChanged();
	plot->replot();
	++time;
}

void MyPlot::setT0(int t){
	t0 = t;
}

void MyPlot::setZoom(int z){
	zoom = z;
	sliderText->setText(QString::number(zoom));
	
	//Actualizamos el Scroll solo si no se actualiza en el timer
	if(scroll->sliderPosition() == scroll->maximum()){
		int timeWindow = (int)(time/100.0*zoom+0.5);
		scroll->setRange(0, timeWindow);
		scroll->setPageStep((int)(time/100.0*zoom+0.5));
	}
}

void MyPlot::setZoom(){
	zoom = sliderText->text().toInt();
	slider->setSliderPosition(zoom);
}
