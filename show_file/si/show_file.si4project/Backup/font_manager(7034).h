#ifndef _FONT_MANAGER_H_
#define _FONT_MANAGER_H_

typedef struct FontBitMap {
	int iXLeft;
	int iYTop;
	int iXMax;
	int iYMax;
	int iBpp;
	int iPitch;
	unsigned char* pucFontBitMapBuffer;
	int iCurOriginX;
	int iCurOriginY;
	int iNextOriginX;
	int iNextOriginY;
}T_FontBitMap, *PT_FontBitMap;
typedef struct FontOpr {
	char* name;
	unsigned char* pucFontMem;
	int (*FontInit)(const char *pFileName, int dwFontSize);
	int (*GetFontBitMap)(unsigned int dwCode, PT_FontBitMap ptFontBitMap);
	int (*FontExit)(PT_FontOpr ptFontOpr);
	struct FontOpr* ptNext;
}T_FontOpr, *PT_FontOpr;

int RegisterFontOpr(PT_FontOpr ptFontOpr);
int FontInit(void);
#endif //_FONT_MANAGER_H_