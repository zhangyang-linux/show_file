#ifndef _DISP_MANAGER_H_
#define _DISP_MANAGER_H_

typedef struct DispOpr {
	char *name;
	int iXres;
	int iYres;
	int iBpp;
	unsigned char* pucFBMem;
	int (*DeviceInit)(void);
	int (*ShowPixel)(int iPenX, int iPenY, unsigned int dwColor);
	int (*CleanScreen)(unsigned int dwBackColor);
	int (*DeviceExit)(struct DispOpr* ptDispOpr);
	struct DispOpr* ptNext;
}T_DispOpr, *PT_DispOpr;

PT_DispOpr GetDispOpr(char* pcName);

void ShowDisplayOpr();

int RegisterDispOpr(PT_DispOpr ptDispOpr);

int DisplayInit(void);

int FBInit(void);
#endif //_DISP_MANAGER_H_