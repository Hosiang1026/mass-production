
#include <input_manager.h>
#include <tslib.h>

static 	struct tsdev *g_ts;

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

static 	int TouchscreenDeviceExit(void)
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

void TouchscreenRegister(void)
{
    TouchscreenRegister(&g_tNetinputDev);
}

#if 0

int main(int argc, char **argv)
{
    int ret;
    InputEvent event;
    g_tTouchscreenDev.DeviceInit();

    while (1)
    {
        ret = g_tTouchscreenDev.GetInputEvent(&event);
        if (ret)
        {
            printf("GetInputEvent err\n");
            return -1;
        }
        else
        {
            printf("Type      : %d\n", event.iType);
            printf("iX        : %d\n", event.iX);
            printf("iY        : %d\n", event.iY);
            printf("iPressure : %d\n", event.iPressure);
        }
    }
    return 0;
}

#endif