#include "multiPlotter.h"
#include <QVarLengthArray>
#include <QHBoxLayout>
#include <qwt_text.h>
#include <cstdio>
#include "serial.h"

MultiPlotter::MultiPlotter(QWidget *parent) :
	QWidget(parent),
	rollPlot(QwtText("Roll"),this),
	pitchPlot(QwtText("Pitch"),this),
	yawPlot(QwtText("Yaw"),this),
	serial("/dev/ttyUSB0", 9600)
{
	//Layout
	layoutH = new QHBoxLayout(this);
	layoutH->addWidget(&rollPlot);
	layoutH->addWidget(&pitchPlot);
	layoutH->addWidget(&yawPlot);
	
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
}
