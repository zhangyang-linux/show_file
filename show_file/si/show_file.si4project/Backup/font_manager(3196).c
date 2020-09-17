
static T_FontOpr g_ptFontOprHead;

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
		printf("ASCIIInit error!\n");
		return -1;
	}
	iError = HZKInit();
	if (iError){
		printf("HZKInit error!\n");
		return -1;
   }
	iError = FreeTypeInit();
	if (iError){
		printf("FreeTypeInit error!\n");
		return -1;
 	}
	return iError;
}

