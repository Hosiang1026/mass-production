#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>

static PInputDevice g_InputDevs = NULL;

void RegisterInputDevice(PInputDevice ptInputDev)
{
    ptInputDev->ptNext = g_InputDevs;
    g_InputDevs = ptInputDev;
}


/* 注册各个输入设备 */
void InputInit(void)
{
    /* regiseter touchscreen */
    extern void TouchscreenRegister(void);
    TouchscreenRegister(void);
    /* regiseter netinput */
    extern void NetInputRegister(void);
    NetInputRegister(void);
}

static void *input_recv_thread_func(void *data)
{
    int ret;
    InputEvent tEvent;
    PInputDevice tInputDev = (PInputDevice)data;
    while (1)
    {
        /* 读数据 */
        ret = tInputDev->GetInputEvent(&tEvent);

        if (!ret)
        {
            /* 保存数据 */

            /* 唤醒等待数据的线程 */

            pthread_mutex_lock(&g_tMutex);
            pthread_cond_wait(&g_tConVar, &g_tMutex);

            pthread_mutex_unlock(&g_tMutex);
        }
    }
    
   return NULL; 
}

/** 
 * 初始化所有设备
 */
void InputDeviceInit(void)
{
    int ret;
    pthread_t tid;
    
    /* for each inputdevice, init, pthread_create */
    PInputDevice ptTmp = g_InputDevs;
    while (ptTmp)
    {
        /* init device */
        ret = ptTmp->DeviceInit();

        /* pthread create */
        if (!ret)
        {
            pthread_create(&tid, NULL, input_recv_thread_func, ptTmp);
        }
        
        ptTmp = ptTmp->ptNext;
    }
    
}

/**
 * 获得输入数据 
 */
int GetInputEvent(PT_InputEvent ptInputEvent)
{
    /* 没有数据则休眠 */

    /* 返回数据 */
}