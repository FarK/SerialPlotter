#ifndef FRAME_H
#define FRAME_H

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

		float ax;
		float ay;
		float az;

		float gx;
		float gy;
		float gz;

		float mx;
		float my;
		float mz;
	};
	unsigned char buff[16];
} Frame;

#endif
