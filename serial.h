#ifndef SERIAL_H
#define SERIAL_H

#include <iostream>
#include <QThread>
#include <boost/asio.hpp>
#include "frame.h"

//Carácteres especiales de la trama (inicio de trama y carácter de escape)
const unsigned char STX = 0x63;
const unsigned char DLE = 0x64;	

//Serial es la clase que gestionará la comunicación serie
class Serial : public QThread{
	Q_OBJECT

	private:
		//Estructuras con todo lo que va a usar la máquina de estados
		typedef enum {WAIT_STX, RECIEVING_FRAME}State; 
		struct StateMachine{
			State state;			//Estado
			unsigned int buffCounter;	//Caracteres por leer del buffer
			unsigned int frameCounter;	//Caracteres por recibir de la trama
			unsigned char* pBuffer;		//Puntero al buffer
			unsigned char* pFrame;		//Puntero a la trama
		}stMach;

		//Clases necesarias para la comunicación serie
		boost::asio::io_service io;
		boost::asio::serial_port port;

		//Trama recibida y buffer de lectura
		Frame frame;
		unsigned char readBuff[2*sizeof(Frame) + 1];	//Con este tamaño se asegura que se reciba una trama entera
		void run();

	public:
		Serial(const std::string& portname, unsigned int baudRate);

		void frameToHex(Frame* f);
		void trataLaTrama(Frame* f);

	signals:
		void newFrame(Frame frame);
};

#endif
