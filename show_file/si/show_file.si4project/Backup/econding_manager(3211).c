#include "encoding_manager.h"
#include "config.h"


static PT_DispOpr g_ptEncodingOprHead;

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
	int iRet = -1;
	if (!ptEncodingOpr || !ptFontOpr){
		printf("AddFontOprForEncoding parameter error\n");
		return -1
	}
	PT_FontOpr ptFontOprCopy = (PT_FontOpr)malloc(sizeof(T_FontOpr));
	if (!ptFontOprCopy){
		return -1;
	}

	memcpy(ptFontOprCopy, ptFontOpr, sizeof(T_FontOpr));
	
	return iRet;
	
}



int EncodingInit(void)
{
	int iError;
	iError = AsciiEncodingInit();
	if (iError){
		printf("AsciiEncodingInit Failed\n");
		return -1;
	}

	iError = Utf8EncodingInit();
	if (iError){
		printf("Utf8EncodingInitEncodingInit Failed\n");
		return -1;
	}

	iError = Utf16beEncodingInit();
	if (iError){
		printf("Utf16beEncodingInit Failed\n");
		return -1;
	}

	iError = Utf16leEncodingInit();
	if (iError){
		printf("Utf16leEncodingInit Failed\n");
		return -1;
	}
}

