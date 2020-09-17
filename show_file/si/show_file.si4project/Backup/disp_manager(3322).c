
static PT_DispOpr g_ptDispOprHead;

PT_DispOpr GetDispOpr(char* pcName)
{
	PT_DispOpr ptTmp = g_ptDispOprHead;
	while (ptTmp) {
		if (strcmp(ptTmp->name, pcName) == 0) {
			return ptTmp;
		}
		ptTmp = ptTmp->ptNext;
	}
	return NULL;
}



void ShowDisplayOpr()
{	
	int i = 0;
	PT_DispOpr ptTmp = g_ptDispOprHead;
	while (ptTmp) {
		DBG_PRINTF("%02d,%s\n", i++,ptTmp->name);
		ptTmp = ptTmp->ptNext;
	}
}
int RegisterDispOpr(PT_DispOpr ptDispOpr)
{
	int iRet = -1;
	PT_DispOpr ptTmp = g_ptDispOprHead;
	if (!g_ptDispOprHead) {
		g_ptDispOprHead = ptDispOpr;
		ptDispOpr->ptNext = NULL;
	    iRet = 0;
	}
	else {
		while (ptTmp->ptNext) {
		ptTmp = ptTmp->ptNext;
	}
	ptTmp->ptNext = ptDispOpr;
	ptDispOpr->ptNext = NULL;
	iRet = 0;
	}
	
	return iRet;
}


int DisplayInit(void)
{
	int iError = 0;
	iError = FBInit();
	if (iError){
		DBG_PRINTF("FBInit error!\n");
		return -1;
	}
	return iError;
}

