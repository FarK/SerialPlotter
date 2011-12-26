#ifndef SERIAL_H
#define SERIAL_H

#include <iostream>
#include <boost/asio.hpp>

//Carácteres especiales de la trama (inicio de trama y carácter de escape)
const unsigned char STX = 0x63;
const unsigned char DLE = 0x64;	

//Esta es la trama que se recibirá
//La union con un array permite tratarla
//fácilmente como un buffer sin necesidad
//de hacer casting
typedef union{
	struct{
		int time;
		float roll;
		float pitch;
		float yaw;
	};
	unsigned char buff[16];
} Frame;

//Estructuras con todo lo que va a usar la máquina de estados
typedef enum {WAIT_STX, RECIEVING_FRAME}State; 

typedef struct{
	State state;			//Estado
	unsigned int buffCounter;	//Caracteres por leer del buffer
	unsigned int frameCounter;	//Caracteres por recibir de la trama
	unsigned char* pBuffer;		//Puntero al buffer
	unsigned char* pFrame;		//Puntero a la trama
}StateMachine;

//Serial es la clase que gestionará la comunicación serie
class Serial{
	private:
		boost::asio::io_service io;
		boost::asio::serial_port port;

		Frame frame;
		unsigned char readBuff[2*sizeof(frame) + 1];	//Con este tamaño se asegura que se reciba una trama entera
		StateMachine stMach;

	public:
		Serial(const std::string& portname, unsigned int baudRate);
		void start();

		void frameToHex(Frame* f);
		void trataLaTrama(Frame* f);
};

#endif
