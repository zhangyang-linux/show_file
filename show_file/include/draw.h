#ifndef _DRAW_H_
#define _DRAW_H_
typedef struct PageDesc {
	unsigned char* pucLcdFirstPostAtFile;
	unsigned char* pucLcdNextPagePostAtFile;
	struct PageDesc* ptPrePage;
	struct PageDesc* ptNextPage;
}T_PageDesc, *pT_PageDesc;

int OpenTextFile(char* pFileName);

int SetTextDetail(char* pcHzkFile, char* pcFreetypeFile, unsigned int dwFontSize);

int SelectAndInitDisplay(char* pcDiaplayName);

int ShowOnePage(unsigned char* pucTextFileMemCurPos);

int ShowNextPage();

int ShowPrePage();
#endif //_DRAW_H_