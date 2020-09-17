typedef struct PageDesc {
	unsigned char* pucLcdFirstPostAtFile;
	unsigned char* pucLcdNextPagePostAtFile;
	struct PageDesc* ptPrePage;
	struct PageDesc* ptNextPage;
}T_PageDesc, pT_PageDesc;

static int g_iFdTextFile;
static unsigned char* g_pucTextFileMem;
static unsigned char* g_pucTextFileMemEnd;
static PT_EncodingOpr g_ptEncodingOprForTextFile;
static unsigned char* g_pucLcdFirstPostAtFile;
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

void ShowNextPage()
{
	unsigned char* pucTextFileMemCurPos;
	if (g_ptCurPage) {
		pucTextFileMemCurPos = g_ptCurPage->pucLcdNextPagePostAtFile;
	}
	else {
		pucTextFileMemCurPos = 
	}
		
}


