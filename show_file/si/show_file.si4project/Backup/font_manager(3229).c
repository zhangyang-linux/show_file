
static T_FontOpr g_ptFontOprHead;

void ShowFontOpr()
{	
	int i = 0;
	T_FontOpr ptTmp = g_ptFontOprHead;
	while (ptTmp) {
		DBG_PRINTF("%02d,%s\n", i++,ptTmp->name);
		ptTmp = ptTmp->ptNext;
	}
}


int RegisterFontOpr(PT_FontOpr ptFontOpr)
{
	int iRet = -1;
	PT_FontOpr ptTmp = g_ptFontOprHead;
	if (!g_ptFontOprHead) {
		g_ptFontOprHead = ptFontOpr;
		ptFontOpr->ptNext = NULL;
	    iRet = 0;
	}
	else {
		while (ptTmp->ptNext) {
		ptTmp = ptTmp->ptNext;
	}
	ptTmp->ptNext = ptFontOpr;
	ptFontOpr->ptNext = NULL;
	iRet = 0;
	}
	
	return iRet;
}

PT_FontOpr GetFontOprForName(unsigned char* pucName)
{
	PT_FontOpr ptTmp = g_ptFontOprHead;
	while (ptTmp) {
		if (strcmp(ptTmp->name, pucName) == 0) {
			return ptTmp;
		}
	ptTmp = ptTmp->ptNext;
	}
	return NULL;
}

int FontInit(void)
{
	int iError = 0;
	iError = ASCIIInit();
	if (iError){
		DBG_PRINTF("ASCIIInit error!\n");
		return -1;
	}
	iError = HZKInit();
	if (iError){
		DBG_PRINTF("HZKInit error!\n");
		return -1;
   }
	iError = FreeTypeInit();
	if (iError){
		DBG_PRINTF("FreeTypeInit error!\n");
		return -1;
 	}
	return iError;
}

