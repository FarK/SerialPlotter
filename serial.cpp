#include "serial.h"
#include "frame.h"
#include <boost/asio.hpp>
#include <QObject>

using namespace boost::asio;

Serial::Serial(const std::string& portname, unsigned int baudRate) :
	port(io, portname)
{
	//Configuramos los parámetros de la conexión
	port.set_option(serial_port_base::baud_rate(baudRate));

	//Inicializamos la máquina de estados
	stMach.state = WAIT_STX;
	stMach.buffCounter = 2*sizeof(Frame)+1;
	stMach.frameCounter = sizeof(Frame);
	stMach.pBuffer = readBuff;
	stMach.pFrame = frame.buff;
}

void Serial::run(){
	while(1){
		stMach.buffCounter = read(port, buffer(readBuff, 2*sizeof(Frame)+1));
		stMach.pBuffer = readBuff;

		while(stMach.buffCounter > 0){
			//printf("\nBUFF_COUNTER = %i\n", stMach.buffCounter);
			switch(stMach.state){
				//Esperamos el caracter de inicio de la trama
				case WAIT_STX:
					//printf("WAIT_STX [%X]\n", *stMach.pBuffer);
					switch(*stMach.pBuffer++){
						case STX:
							stMach.pFrame = frame.buff;
							stMach.state = RECIEVING_FRAME;
							break;

						//Si el caracter recibido es DLE, nos saltamos el siguiente
						case DLE:
							++stMach.pBuffer;		
							--stMach.buffCounter;
							break;
					}
					break;

				//Recibimos y trataos la trama
				case RECIEVING_FRAME:
					//printf("RECIEVING_FRAME %d - [%X]\n", stMach.frameCounter, *stMach.pBuffer);
					switch(*stMach.pBuffer){
						//Si recibimos STX algo ha ido mal, volvemos a leer la trama de nuevo
						case STX:
							++stMach.pBuffer;
							--stMach.buffCounter;
							stMach.pFrame = frame.buff;
							stMach.frameCounter = sizeof(Frame);
							stMach.state = WAIT_STX;
							break;

						//Si recibimos DLE, guardamos el siguiente caracter sin procesarlo
						case DLE:
							++stMach.pBuffer;
							//Si DLE es el último caracter del buffer hay que leer de nuevo
							if(!--stMach.buffCounter){
								stMach.buffCounter = read(port, buffer(readBuff, 2*sizeof(Frame)+1));
								stMach.pBuffer = readBuff;
							}
						default:
							//printf("ADD CHAR: [%X]\n",*stMach.pBuffer);
							//Guardamos el caracter en la trama
							*stMach.pFrame++ = *stMach.pBuffer++;

							//Comprobamos si se ha terminado de recibir la trama
							if(!--stMach.frameCounter){
								emit newFrame(frame);

								stMach.pFrame = frame.buff;
								stMach.frameCounter = sizeof(Frame);
								stMach.state = WAIT_STX;
							}
					}
					break;
			}
			--stMach.buffCounter;
		}
	}
}

void Serial::trataLaTrama(Frame* t){
	frameToHex(t);
	//printf("Time: %i\nRoll: %f\nPitch: %f\nYaw: %f\n",t->time, t->roll, t->pitch, t->yaw);
	printf("Time: %i\n",t->time);
	printf("Roll: %f\n",t->roll);
	printf("Pitch: %f\n",t->pitch);
	printf("Yaw: %f\n",t->yaw);
}

void Serial::frameToHex(Frame* t){
	for(int i = 0 ; i < sizeof(Frame) ; ++i){
		if(i%4 == 0 && i != 0)	printf("  ");
		printf("[");
		printf("%X",t->buff[i]);
		printf("]");
	}
	printf("\n");
}
