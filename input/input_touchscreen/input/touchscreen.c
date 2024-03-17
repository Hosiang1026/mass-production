
#include <input_manager.h>
#include <stdio.h>
#include <tslib.h>

static struct tsdev *g_ts;

static int TouchscreenGetInputEvent(PInputEvent ptInputEvent)
{
    int ret;
    struct ts_sample samp;

    ret = ts_read(g_ts, &samp, 1);

    if (ret != 1)
    {
        return -1;
    }
    
    ptInputEvent->iType        = INPUT_TYPE_TOUCH;
    ptInputEvent->iX           = samp.x;
    ptInputEvent->iY           = samp.y;
    ptInputEvent->tTime        = samp.tv;
    ptInputEvent->iPressure    = samp.pressure;

    return 0;
}

static 	int TouchscreenDeviceInit(void)
{
	g_ts = ts_setup(NULL, 0);
	if(!g_ts)
	{
		printf("ts_setup err\n");
		return -1;
	}

    return 0;
}

static int TouchscreenDeviceExit(void)
{
    ts_close(g_ts);
    return 0;
}

static InputDevice g_tTouchscreenDev ={
    .name = "touchscreen",
    .GetInputEvent = TouchscreenGetInputEvent,
	.DeviceInit = TouchscreenDeviceInit,
	.DeviceExit = TouchscreenDeviceExit,
};