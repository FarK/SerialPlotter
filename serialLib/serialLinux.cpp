#include "serialLinux.h"
#include <stdio.h>   /* Standard input/output definitions. */
#include <string.h>  /* String function definitions. */
#include <unistd.h>  /* UNIX standard function definitions. */
#include <fcntl.h>   /* File control definitions. */
#include <sys/ioctl.h>
#include <sys/time.h>
#include <termios.h> /* POSIX terminal control definitions. */

Serial::Serial(char p[], int br, Parity prty) : 
	port(p), baudRate(br), parity(prty)
{
}

void Serial::run(){
	int readBytes = 0;

	if(openPort() != ERROR_OPEN_PORT){
		if(configurePort() != ERROR_PORT_CONF)
			while(1){
				emit(newData(buff,readPort()));
				//printf("envidad señal: %s\n",buff);
			}
		else
			printf("no configura\n");
	}
	else
		printf("no conecta\n");

}

int Serial::openPort(){
	fd = open(port, O_RDWR | O_NOCTTY );//| O_NDELAY);

	if (fd < 0){
		fprintf(stderr,"Can't open port %s\n", port);
		return ERROR_OPEN_PORT;
	}
		
	tcflush(fd, TCIOFLUSH);
	return 0;
}

int Serial::closePort()
{
	if (close(fd)!=0){
		fprintf(stderr,"Can't close port: %i\n",fd);
		return ERROR_CLOSE_PORT;
	}
	else
		return 0;
}

int Serial::configurePort(){
	Tconf newtio;
	bzero(&newtio, sizeof(newtio));
	//nuevos parámetros del puerto.
	//tcflush(fd, TCIOFLUSH);

	//CLOCAL  : conexion local, sin control de modem.
	//CREAD   : activa recepcion de caracteres.
	newtio.c_cflag = CLOCAL | CREAD ;
	cfsetospeed(&newtio,baudRate);

	switch(parity){
		case P_8N1:
			newtio.c_cflag &= ~PARENB;
			newtio.c_cflag &= ~CSTOPB;
			newtio.c_cflag &= ~CSIZE;
			newtio.c_cflag |= CS8;
		break;
		//ETC
	}

	// IGNPAR  : ignora los bytes con error de paridad.
	// ICRNL   : mapea CR a NL (en otro caso una entrada CR del otro ordenador
	// no terminaría la entrada) en otro caso hace un dispositivo en bruto
	// (sin otro proceso de entrada).
	newtio.c_iflag = 0;
	//newtio.c_iflag = IGNPAR;
	//newtio.c_iflag |= ICRNL;

	//Salida en bruto.
	newtio.c_oflag = 0;

	//ICANON  : activa entrada canónica(Modo texto).
	//desactiva todas las funcionalidades del eco, y no envía señales al
	//programa llamador.
	newtio.c_lflag = ICANON;
	//newtio.c_lflag = 0;

	// inicializa todos los caracteres de control.
	// Los valores por defecto se pueden encontrar en /usr/include/termios.h,
	// y vienen dadas en los comentarios, pero no los necesitamos aquí.

	newtio.c_cc[VTIME]    = 0;     /* temporizador entre caracter, no usado */
	newtio.c_cc[VMIN]     = 1;     /* bloquea lectura hasta llegada de un caracter */

	if(tcsetattr(fd,TCSANOW,&newtio) != 0){
		fprintf(stderr,"Can't configure port %s\n",port);
		perror("");
		return ERROR_PORT_CONF;
	}
}

long Serial::readPort(){
	Tconf newtio;
	struct timeval inic,fin,temp;
	float tiempo,t;
	int bytes;
	int ESTADO;
	int TEMPO;

	if(tcgetattr(fd,&newtio)!=0) return -1;
	else
	{
		ESTADO=newtio.c_cc[VMIN];
		TEMPO=newtio.c_cc[VTIME];
		if( (ESTADO==0)&&(TEMPO==0) )
		{
			return read(fd,buff,BUFF_SIZE);
		}
		else
		{
			if(TEMPO==0)
			{
				do{ioctl(fd, FIONREAD, &bytes);}
				while(bytes<BUFF_SIZE);
				return read(fd,buff,BUFF_SIZE);
			}
			else
			{
				gettimeofday(&inic,NULL);
				tiempo=newtio.c_cc[VTIME];
				do
				{
					gettimeofday(&fin,NULL);
					temp.tv_sec=fin.tv_sec-inic.tv_sec;
					temp.tv_usec=fin.tv_usec-inic.tv_usec ;
					t=((temp.tv_usec/1000.0)+temp.tv_sec*1000.0)/100.0;
				}while((t<tiempo*BUFF_SIZE) && (ioctl(fd, FIONREAD, &bytes) < BUFF_SIZE) );

				if(ioctl(fd, FIONREAD, &bytes) != 0)	return read(fd,buff,BUFF_SIZE);
				else			return 0;
			}
		}
	}
}

long Serial::writePort(char data[], int dataSize){
	return  write(fd, data, dataSize);
}
