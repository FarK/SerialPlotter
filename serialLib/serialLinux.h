#ifndef SERIAL_LINUX
#define SERIAL_LINUX

#include <cstdio>   /* Standard input/output definitions. */
#include <unistd.h>  /* UNIX standard function definitions. */
#include <fcntl.h>   /* File control definitions. */
#include <sys/ioctl.h>
#include <sys/time.h>
#include <termios.h> /* POSIX terminal control definitions. */

#include <QThread>

#define NONE      0    
#define RTSCTS    1
#define HARD      1
#define XONXOFF   2
#define SOFT      2

#define TIMEOUT 100

#define ERROR_PORT_CONF -1
#define ERROR_OPEN_PORT -2
#define ERROR_CLOSE_PORT -3
#define BUFF_SIZE 16


typedef enum { P_8N1, P_8E1, P_7E1, P_701, P_7S1} Parity;
typedef struct termios Tconf;

class Serial : public QThread{
	Q_OBJECT

	public:
		int fd;
		char* port;
		unsigned int baudRate;
		Parity parity;
		char buff[BUFF_SIZE];

		Serial(char port[], int baudRate, Parity parity);

		void run();

	private:
		int openPort();
		int closePort();
		int configurePort();
		long readPort();
		long writePort(char data[], int dataSize);

	signals:
		void newData(char* data, long size);
};

#endif
