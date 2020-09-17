#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <config.h>
#include <unistd.h>


static int g_FdHZK;
static int g_dwHZKFontSize;
static unsigned char* g_pucHZKFontMem;
static unsigned char* g_pucHZKFontMemEnd;


static int HZKFontInit(char* pFileName, int dwFontSize);
static int HZKGetFontBitMap(int dwCode, PT_FontBitMap ptFontBitMap);
static int HZKFontExit(PT_FontOpr ptFontOpr);

static T_FontOpr g_tHZKFontOpr = {
	.name = "hzk"
	.FontInit = HZKFontInit,
	.GetFontBitMap = HZKGetFontBitMap,	
	.FontExit =  HZKFontExit,
};
	
static int HZKFontInit(const char *pFileName, int dwFontSize)
{
	struct stat tStat;

	if ((dwFontSize != 16) || (pFileName == NULL)){
		DBG_PRINTF("HZK can't support %d font size\n", dwFontSize);
		return -1
	}
	g_FdHZK =  open(pFileName, O_RDONLY);	
	if (g_FdHZK < 0){
		DBG_PRINTF("open %s error!\n", pFileName);
		return -1
	}

	if (fstat(g_FdHZK, &tStat)){
		DBG_PRINTF("fstat %s error!\n", pFileName);
		return -1
	}
	g_dwHZKFontSize = tStat.st_size;
	g_pucHZKFontMem = (unsigned char*)mmap(NULL, g_dwHZKFontSize, PROT_READ, MAP_SHARED, g_FdHZK, 0);
	g_pucHZKFontMemEnd = g_pucHZKFontMem + g_dwHZKFontSize;
	return 0;	
}

static int HZKGetFontBitMap(unsigned int dwCode, PT_FontBitMap ptFontBitMap)
{
	int iXPen = ptFontBitMap->iCurOriginX;	
	int iYPen = ptFontBitMap->iCurOriginY;
	int iArea;
	int iWhere;
	if ((dwCode & 0xffff0000) || (ptFontBitMap == NULL)){
		DBG_PRINTF("HZK code error!\n");
		return -1;
	}
	iArea  = (int)(dwCode >> 8) - 0xA1;
	iWhere = (int)(dwCode & 0xff) - 0xA1;

	ptFontBitMap->iXLeft = iXPen;	
	ptFontBitMap->iYTop = iYPen - 16;		
	ptFontBitMap->iXMax = iXPen + 16;		
	ptFontBitMap->iYMax = iYPen;	
	ptFontBitMap->iBpp = 1;	
	ptFontBitMap->iPitch = 2;
	ptFontBitMap->pucFontBitMapBuffer = g_pucHZKFontMem + (iArea * 94 + iWhere) * 32;
	ptFontBitMap->iNextOriginX = iXPen + 16;	
	ptFontBitMap->iNextOriginY = iYPen;
	return 0;
}

static int HZKFontExit(PT_FontOpr ptFontOpr)
{	
	if (ptFontOpr->pucFontMem){
		munmap(ptFontOpr->pucFontMem);
	}
	return 0;
}

int HZKInit(void)
{
	RegisterFontOpr(&g_tHZKFontOpr);
}

