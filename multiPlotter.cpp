#include "multiPlotter.h"
#include <QVarLengthArray>
#include <QHBoxLayout>
#include <qwt_text.h>
#include <cstdio>
#include "serial.h"
#include "frame.h"

MultiPlotter::MultiPlotter(QWidget *parent) :
	QWidget(parent),
	rollPlot(QwtText("Roll"),this),
	pitchPlot(QwtText("Pitch"),this),
	yawPlot(QwtText("Yaw"),this),

	axPlot(QwtText("Acelerómetro X"),this, -300, 300),
	ayPlot(QwtText("Acelerómetro Y"),this, -300, 300),
	azPlot(QwtText("Acelerómetro Z"),this, -300, 300),

	gxPlot(QwtText("Giroscopio X"),this, -10, 10),
	gyPlot(QwtText("Giroscopio Y"),this, -10, 10),
	gzPlot(QwtText("Giroscopio Z"),this, -10, 10),

	mxPlot(QwtText("Magnetómetro X"),this, -600, 600),
	myPlot(QwtText("Magnetómetro Y"),this, -600, 600),
	mzPlot(QwtText("Magnetómetro Z"),this, -600, 600),
	serial("/dev/ttyUSB0", 9600)
{
	//Layout
	layoutH = new QHBoxLayout(this);
	layoutV0 = new QVBoxLayout(this);
	layoutV1 = new QVBoxLayout(this);
	layoutH0 = new QHBoxLayout(this);
	layoutH1 = new QHBoxLayout(this);
	layoutH2 = new QHBoxLayout(this);
	layoutH3 = new QHBoxLayout(this);

	layoutH->addLayout(layoutV0);
	layoutH->addLayout(layoutV1);

	layoutV0->addLayout(layoutH0);
	layoutV0->addLayout(layoutH1);
	layoutV1->addLayout(layoutH2);
	layoutV1->addLayout(layoutH3);

	layoutH0->addWidget(&rollPlot);
	layoutH0->addWidget(&pitchPlot);
	layoutH0->addWidget(&yawPlot);

	layoutH1->addWidget(&axPlot);
	layoutH1->addWidget(&ayPlot);
	layoutH1->addWidget(&azPlot);

	layoutH2->addWidget(&gxPlot);
	layoutH2->addWidget(&gyPlot);
	layoutH2->addWidget(&gzPlot);

	layoutH3->addWidget(&mxPlot);
	layoutH3->addWidget(&myPlot);
	layoutH3->addWidget(&mzPlot);
	
	qRegisterMetaType<Frame>("Frame");
	connect(&serial, SIGNAL(newFrame(Frame)), this, SLOT(newFrame(Frame)));

	serial.start();
}

MultiPlotter::~MultiPlotter(){
	//TODO: Terminar la ejecución del hilo hijo ántes de salir
	//serial.exit();
}

void MultiPlotter::newFrame(Frame frame){
	//trataLaTrama(&frame);
	rollPlot.newData(frame.time, frame.roll);
	pitchPlot.newData(frame.time, frame.pitch);
	yawPlot.newData(frame.time, frame.yaw);

	axPlot.newData(frame.time, frame.ax);
	ayPlot.newData(frame.time, frame.ay);
	azPlot.newData(frame.time, frame.az);

	gxPlot.newData(frame.time, frame.gx);
	gyPlot.newData(frame.time, frame.gy);
	gzPlot.newData(frame.time, frame.gz);

	mxPlot.newData(frame.time, frame.mx);
	myPlot.newData(frame.time, frame.my);
	mzPlot.newData(frame.time, frame.mz);
}
