#ifndef _FONT_MANAGER_H_
#define _FONT_MANAGER_H_

typedef struct FontBitMap {
	int iXLeft;
	int iYTop;
	int iXMax;
	int iYMax;
	int iBpp;
	unsigned char* pucFontBitMapBuffer;
	int iCurOriginX;
	int iCurOriginY;
	int iNextOriginX;
	int iNextOriginY;
}T_FontBitMap, *PT_FontBitMap;
typedef struct FontOpr {
	char* name;
	int (*FontInit)(const char *pFileName, int dwFontSize);
	int (*GetFontBitMap)(unsigned int dwCode, PT_FontBitMap ptFontBitMap);
	struct FontOpr* ptNext;
}T_FontOpr, *PT_FontOpr;


#endif //_FONT_MANAGER_H_