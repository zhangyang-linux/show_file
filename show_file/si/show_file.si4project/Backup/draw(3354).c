#include "config.h"
#include "draw.h"
#include "encoding_manager.h"
#include "font_manager.h"



static int g_iFdTextFile;
static pT_PageDesc g_ptPage;
static unsigned int g_dwFontSize;
static unsigned char* g_pucTextFileMem;
static unsigned char* g_pucTextFileMemEnd;
static PT_EncodingOpr g_ptEncodingOprForTextFile;
static unsigned char* g_pucLcdFirstPostAtFile;
static unsigned char* g_pucLcdNextPostAtFile;

static PT_DispOpr g_ptDispOpr;
static pT_PageDesc g_ptCurPage;



int OpenTextFile(char* pFileName)
{
	int iRet;
	struct stat tStat;
	g_iFdTextFile = open(pFileName, O_RDONLY);
	if (0 > g_iFdTextFile)
	{
		DBG_PRINTF("can't open text file %s\n", pcFileName);
		return -1;
	}
	if (fstat(pFileName, &tStat)){
		DBG_PRINTF("can't get fstat\n");
		return -1;
	}
	g_pucTextFileMem = (unsigned char*)mmap(NULL , tStat.st_size, PROT_READ, MAP_SHARED, g_iFdTextFile, 0);
	if (g_pucTextFileMem == (unsigned char *)-1){
		DBG_PRINTF("can't mmap for text file\n");
		return -1;
	}

	g_pucTextFileMemEnd = g_pucTextFileMem + tStat.st_size;
	g_ptEncodingOprForTextFile = SelectEncodingOprForTextFile(g_pucTextFileMem);
	if (!g_ptEncodingOprForTextFile){
		return -1;
	}
	if (g_ptEncodingOprForTextFile) {
		g_pucLcdFirstPostAtFile = g_pucTextFileMem + g_ptEncodingOprForTextFile->iHeadLen;
	}
}


int SetTextDetail(char* pcHzkFile, char* pcFreetypeFile, unsigned int dwFontSize)
{
	int iError;
	int iRet = -1;
	PT_FontOpr ptTmp;
	g_dwFontSize = dwFontSize;
	PT_FontOpr pt_FontOpr =	g_ptEncodingOprForTextFile->ptFontOprSupportedHead;
	while (pt_FontOpr) {
		if (strcmp(pt_FontOpr->name, "ascii") == 0) {
			iError = pt_FontOpr->FontInit(NULL, dwFontSize);
		}
		if (strcmp(pt_FontOpr->name, "hzk") == 0) {
			iError = pt_FontOpr->FontInit(pcHzkFile, dwFontSize);
		}
		if (strcmp(pt_FontOpr->name, "freetype") == 0) {
			iError = pt_FontOpr->FontInit(pcFreetypeFile, dwFontSize);
		}
		ptTmp = pt_FontOpr->ptNext;
		if (iError == 0) {
			iRet = 0;
		}
		else {
			DelFontOprFrmCodeEnding(g_ptEncodingOprForTextFile, pt_FontOpr);
		}
		pt_FontOpr = ptTmp;
	}
	return iRet;
}


int SelectAndInitDisplay(char* pcDiaplayName)
{
	int iRet = -1;
	g_ptDispOpr = GetDispOpr(pcDiaplayName);
	if (g_ptDispOpr && g_ptDispOpr->DeviceInit) {
		iRet = g_ptDispOpr->DeviceInit();
	}
	return iRet;
}

static void RecordPage(pT_PageDesc ptPage)
{
	pT_PageDesc ptTmp;
	if (!g_ptPage) {
		g_ptPage = ptPage;
		ptPage->ptNextPage = NULL;
		ptPage->ptPrePage = NULL;
	}
	else {
		ptTmp = g_ptPage;
		while (ptTmp->ptNextPage) {
			ptTmp = ptTmp->ptNextPage;
		}
		ptTmp->ptNextPage = ptPage;
		ptPage->ptPrePage = ptTmp;
		ptPage->ptNextPage = NULL;
	}
}


static int IncLcdY(int iY)
{
	if (iY + g_dwFontSize < g_ptDispOpr->iYres) {
		return (iY + g_dwFontSize);
	}
	return 0;
}

static int RelocateFontPos(PT_FontBitMap ptFontBitMap)
{
	int iLcdY;
	int iDeltaX;
	int iDeltaY;
	if (ptFontBitMap->iYMax > g_ptDispOpr->iYres) {
		return -1;
	}
	if (ptFontBitMap->iXMax > g_ptDispOpr->iXres) {
		iLcdY = IncLcdY(ptFontBitMap->iCurOriginY);
		if (0 == iLcdY)
		{
			/* ÂúÒ³ÁË */
			return -1;
		}
		else {
			iDeltaX = 0 - ptFontBitMap->iCurOriginX;
			iDeltaY = iLcdY - ptFontBitMap->iCurOriginY;

			ptFontBitMap->iCurOriginX += iDeltaX;
			ptFontBitMap->iCurOriginY += iDeltaY;
			ptFontBitMap->iNextOriginX += iDeltaX;
			ptFontBitMap->iNextOriginY += iDeltaY;
			ptFontBitMap->iXLeft += iDeltaX;
			ptFontBitMap->iYTop += iDeltaY;
			ptFontBitMap->iXMax += iDeltaX;
			ptFontBitMap->iYMax += iDeltaY;
			return 0;
		}
	}
	return 0;
}

static int ShowOneFont(PT_FontBitMap PtFontBitMap)
{
	int x;
	int y;
	int bit;
	int i = 0;
	unsigned char ucByet;
	if (PtFontBitMap->iBpp == 1) {
		for (y = PtFontBitMap->iYTop; y < PtFontBitMap->iYMax; y++) {
			i = (y - PtFontBitMap->iYTop) * PtFontBitMap->iPitch;
			for (x = PtFontBitMap->iXLeft, bit = 7; x < PtFontBitMap->iXMax; x++) {
				if (bit = 7) {
					ucByet = PtFontBitMap->pucFontBitMapBuffer[i++];
				}
				if (ucByet & (0x1 << bit)) {
					g_ptDispOpr->ShowPixel(x, y 0xffffffff);
				}
				else {
					//g_ptDispOpr->ShowPixel(x, y 0x0);
				}
				bit--;
				if (bit < 0) {
					bit = 7;
				}
			}
	   }
	}
	else if (PtFontBitMap->iBpp == 8) {
			for (y = PtFontBitMap->iYTop; y < PtFontBitMap->iYMax; y++){
				for (x = PtFontBitMap->iXLeft; x < PtFontBitMap->iXMax; x++) {
					if (PtFontBitMap->pucFontBitMapBuffer[i++]) {
						g_ptDispOpr->ShowPixel(x, y 0xffffffff);
					}
				}
		}
	}
	else {
		DBG_PRINTF("ShowOneFont error, can't support %d bpp\n", PtFontBitMap->iBpp);
		return -1;
	}
		
	return 0;
}


int ShowOnePage(unsigned char* pucTextFileMemCurPos)
{
	unsigned char* pucBufStart = pucTextFileMemCurPos;
	unsigned int dwCode;
	int iLen;
	int iError;
	int bHasGetCode = 0;
	int bHasClearLcd; = 0;
	T_FontBitMap tFontBitMap;
	PT_FontOpr ptFontOpr;
	tFontBitMap.iCurOriginX = 0;
	tFontBitMap.iCurOriginY = g_dwFontSize;
	while (1) {
		iLen = g_ptEncodingOprForTextFile->GetCodeFrmBuf(pucBufStart, g_pucTextFileMemEnd, &dwCode);
		if (iLen == 0) {
			if (!bHasGetCode) [
				return -1;
			}
			else {
				return 0;
			}
		}
		bHasGetCode = 1;
		pucBufStart += iLen;
		if (dwCode == '\n') {
			g_pucLcdNextPostAtFile = pucBufStart;
			tFontBitMap.iCurOriginX = 0;
			tFontBitMap.iCurOriginY = IncLcdY(tFontBitMap.iCurOriginY);
			if (tFontBitMap.iCurOriginY == 0) {
				return 0;
			}
			continue;
		}
		if (dwCode == '\t') {
			dwCode = ' ';
		}
		if (dwCode == '\r') {
			continue;
		}
		
		ptFontOpr = g_ptEncodingOprForTextFile->ptFontOprSupportedHead;

		while (ptFontOpr) {
			iError = ptFontOpr->GetFontBitMap(dwCode, &tFontBitMap);
			if (iError == 0) {
				if (RelocateFontPos(&tFontBitMap)) {
					return 0;
				}
				if (!bHasClearLcd) {
					g_ptDispOpr->CleanScreen();
					bHasClearLcd = 1;
				}
				if (ShowOneFont(tFontBitMap)) {
					return -1;
				}
				tFontBitMap.iCurOriginX = tFontBitMap.iNextOriginX;
				tFontBitMap.iCurOriginY = tFontBitMap.iNextOriginY;
				g_pucLcdNextPostAtFile = pucBufStart;
				break;
			}
			ptFontOpr = ptFontOpr->ptNext;
		}
	}
	return 0;
}


void ShowNextPage()
{
	int iError;
	pT_PageDesc ptPage;
	unsigned char* pucTextFileMemCurPos;
	if (g_ptCurPage) {
		pucTextFileMemCurPos = g_ptCurPage->pucLcdNextPagePostAtFile;
	}
	else {
		pucTextFileMemCurPos = g_pucLcdFirstPostAtFile£»
	}
	iError = ShowOnePage(pucTextFileMemCurPos);
	if (iError == 0) {
		if (g_ptCurPage && g_ptCurPage->ptNextPage) {
			g_ptCurPage = g_ptCurPage->ptNextPage;
			return 0;
		}
		ptPage = (pT_PageDesc)malloc(sizeof(T_PageDesc));
		if (ptPage) {
			ptPage->pucLcdFirstPostAtFile = pucTextFileMemCurPos;
			ptPage->pucLcdNextPagePostAtFile = g_pucLcdNextPostAtFile;
			ptPage->ptPrePage = NULL;
			ptPage->ptNextPage = NULL;
			g_ptCurPage = ptPage;
			RecordPage(ptPage);
			return 0;
		}
		else {
			return -1;
		}
	}
	
	return iError;
}

int ShowPrePage()
{	
	int iError = -1;
	pT_PageDesc ptPrePage;
	if (!g_ptCurPage || !(g_ptCurPage->ptPrePage)) {
		return -1;
	}
	
	ptPrePage = g_ptCurPage->ptPrePage;
	iError = ShowOnePage(ptPrePage->pucLcdFirstPostAtFile);
	if (iError == 0) {
		g_ptCurPage = ptPrePage;
	}
	return iError;	
}


