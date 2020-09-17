
static PT_DispOpr g_ptDispOprHead;
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
		printf("FBInit error!\n");
		return -1;
	}
	return iError;
}

