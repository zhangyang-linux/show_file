#include <encoding_manager.h>
#include <config.h>


static PT_EncodingOpr g_ptEncodingOprHead;

PT_EncodingOpr SelectEncodingOprForTextFile(const unsigned char* pucFileMem)
{
	PT_EncodingOpr tmp = g_ptEncodingOprHead;
	while (tmp) {
	if ((tmp->isSupport) && (tmp->isSupport(pucFileMem))
		return tmp;
	tmp = tmp->ptNext;
	}
	return NULL;
}

void ShowEncodingOpr()
{	
	int i = 0;
	PT_EncodingOpr ptTmp = g_ptEncodingOprHead;
	while (ptTmp) {
		DBG_PRINTF("%02d,%s\n", i++,ptTmp->name);
		ptTmp = ptTmp->ptNext;
	}
}

int RegisterEncodingOpr(PT_EncodingOpr ptEncodingOpr)
{
	int iRet = -1;
	PT_EncodingOpr ptTmp = g_ptEncodingOprHead;
	if (!g_ptEncodingOprHead) {
		g_ptEncodingOprHead = ptEncodingOpr;
		ptEncodingOpr->ptNext = NULL;
	    iRet = 0;
	}
	else {
		while (ptTmp->ptNext) {
		ptTmp = ptTmp->ptNext;
		}
	ptTmp->ptNext = ptEncodingOpr;
	ptEncodingOpr->ptNext = NULL;
	iRet = 0;
	}
	return iRet;
}


int AddFontOprForEncoding(PT_EncodingOpr ptEncodingOpr, PT_FontOpr ptFontOpr)
{	
	PT_FontOpr ptFontOprCopy;
	if (!ptEncodingOpr || !ptFontOpr){
		DBG_PRINTF("AddFontOprForEncoding parameter error\n");
		return -1
	}
	ptFontOprCopy = malloc(sizeof(T_FontOpr));
	if (!ptFontOprCopy){
		return -1;
	}

	memcpy(ptFontOprCopy, ptFontOpr, sizeof(T_FontOpr));
	ptFontOprCopy->ptNext = ptEncodingOpr->ptFontOprSupportedHead;
	ptEncodingOpr->ptFontOprSupportedHead = ptFontOprCopy;
	return 0;	
}

int DelFontOprFrmCodeEnding(PT_EncodingOpr ptEncodingOpr, PT_FontOpr ptFontOpr)
{
	PT_FontOpr ptPre;
	PT_FontOpr ptTmp = ptEncodingOpr->ptFontOprSupportedHead;
	if (!ptEncodingOpr || !ptFontOpr) {
		return -1;
	}
	if (strcmp(ptTmp->name, ptFontOpr->name) == 0) {
		ptEncodingOpr->ptFontOprSupportedHead = ptTmp->ptNext;
		free(ptTmp);
		return 0;
	}
	ptPre = ptEncodingOpr->ptFontOprSupportedHead;;
	ptTmp = ptPre->ptNext;
	while (ptTmp) {
		if (strcmp(ptTmp->name, ptFontOpr->name) == 0) {
			ptPre->ptNext = ptTmp->ptNext;
			free(ptTmp);
			return 0;
		}
		ptPre = ptTmp;
		ptTmp = ptTmp->ptNext;
	}
	return -1;
}


int EncodingInit(void)
{
	int iError;
	iError = AsciiEncodingInit();
	if (iError){
		DBG_PRINTF("AsciiEncodingInit Failed\n");
		return -1;
	}

	iError = Utf8EncodingInit();
	if (iError){
		DBG_PRINTF("Utf8EncodingInitEncodingInit Failed\n");
		return -1;
	}

	iError = Utf16beEncodingInit();
	if (iError){
		DBG_PRINTF("Utf16beEncodingInit Failed\n");
		return -1;
	}

	iError = Utf16leEncodingInit();
	if (iError){
		DBG_PRINTF("Utf16leEncodingInit Failed\n");
		return -1;
	}
	return iError;
}

