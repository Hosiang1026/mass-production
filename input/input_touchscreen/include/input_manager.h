#ifndef _INPUT_MANAGER_H
#define _INPUT_MANAGER_H

#define INPUT_TYPE_TOUCH 1
#define INPUT_TYPE_NET 2


typedef struct InputEvent{
	int iType;
	int iX;
	int iY;
	int iPressure;
	char str[1024];
	struct timeval tTime;
}InputEvent, *PInputEvent;

typedef struct InputDevice{
	char *name;
	int (*GetInputEvent)(PInputEvent ptInputEvent);
	int (*DeviceInit)(void);
	int (*DeviceExit)(void);
	struct InputDevice *ptNext;
}InputDevice, *PInputDevice;

#endif






