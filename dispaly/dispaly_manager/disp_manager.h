#ifndef _DISP_MANAGER_H
#define _DISP_MANAGER_H

typedef struct Region{
	int iLeftUpX;
	int iLefUpY;
	int iWidth;
	int iHeigh;
}Region, *PRegion;


typedef struct DispOpr{
	char *name;
	int DeviceInit(void);
	int DeviceExit(void);
	struct DispOpr *ptNext;
	int GetBuffer(PDispBuff ptDispBuff);
	int FlushRegion(PRegion ptRegion, PDispBuff ptDispBuff);
}DispOpr *pDispOpr;


void DisplayInit(void);
void RegisterDisplay(pDispOpr ptDispOpr);
int SelectDefaultDisplay(char *name);
int PutPixel(int x, int y, unsigned int dwColor);
int FbFlushDisplayRegion(PRegion ptRegion, PDispBuff ptDispBuff);


#endif






