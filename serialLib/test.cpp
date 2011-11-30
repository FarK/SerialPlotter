#include "serialLinux.h"

int main(){
	char port[] = "/dev/ttyUSB0";
	Serial s(port, 9600, P_8N1);

	if(s.openPort() == ERROR_OPEN_PORT)
		s.configurePort();
		while(1){
			if(s.readPort() > 0)
				printf("%s",s.buff);
			else{
				printf("uff\n");
				return 1;
			}
		}

	return 0;
}
