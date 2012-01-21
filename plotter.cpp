#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSlider>
#include <QString>
#include <QLineEdit>
#include "plotter.h"
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_text.h>
#include <cmath>
#include <cstdio>

Plotter::Plotter(QwtText title, QWidget *parent) : QWidget(parent),
				  t0(0), dataCount(0), zoom(0)
{
	//Inicializamos
	plot = new QwtPlot(title, this);
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
	//idTimmer = startTimer(100);

	//Propieties
	slider->setRange(1,100);
	slider->setSingleStep(1);
	slider->setSliderPosition(100);
	sliderText->setMaxLength(3);

	scroll->setRange(0,0);
	scroll->setSingleStep(1);

	plot->setAxisScale(QwtPlot::yLeft, -PI, PI, 0);

	// Show a legend at the bottom

	// Show the axes
	//plot.setAxisTitle( QwtPlot::xBottom, "x" );
	//plot.setAxisTitle( QwtPlot::yLeft, "y" );

	curve->attach(plot);
	plot->replot();
}

void Plotter::newData(int time, float data){
	int dataInWindow = (int)(dataCount/100.0*zoom+0.5);
	x.append(time);
	y.append(data);
	curve->setRawData(&x.data()[t0], &y.data()[t0], dataInWindow + 1);
	//printf("Recibido %i: (%d,%f)\n", dataCount, time, data);
	//printf("Rango visible (%d%%): (%d,%d)\tRango total: (0, %d)\n", zoom, t0, dataCount - dataInWindow, dataCount);

	//Nos aseguramos de que la barra de scroll quede fija en el final al
	//actualizar su rango
	if(scroll->sliderPosition() == scroll->maximum()){
		scroll->setRange(0, dataCount - dataInWindow);
		scroll->setPageStep(dataInWindow);
		scroll->setSliderPosition(scroll->maximum());
	}
	else{
		scroll->setRange(0, dataCount - dataInWindow);
		scroll->setPageStep(dataInWindow);
	}

	curve->itemChanged();
	plot->replot();
	++dataCount;
}

void Plotter::setT0(int t){
	t0 = t;
}

void Plotter::setZoom(int z){
	zoom = z;
	sliderText->setText(QString::number(zoom));
}

void Plotter::setZoom(){
	zoom = sliderText->text().toInt();
	slider->setSliderPosition(zoom);
}
