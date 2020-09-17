#include <encoding_manager.h>


static int isUtf16beCoding(const unsigned char*pucBufHead);
static int GetUtf16beCodeFrmBuf(const unsigned char*pucBufStart, const unsigned char* pucBufEnd, unsigned int *pdwCode);
	

static T_EncodingOpr g_tUtf16beEncodingOpr = {
	.name          = "utf-16be",
	.iHeadLen      = 2,
	.isSupport     = isUtf16beCoding,
	.GetCodeFrmBuf = GetUtf16beCodeFrmBuf,
}; 

static int isUtf16beCoding(const unsigned char* pucBufHead)
{ 
	const unsigned char aStrUtf16be[] = {0xFE, 0xFF, 0};

	if (pucBufHead == NULL){
		DBG_PRINTF("isUtf16beCoding parameter error\n!");
		return -1;
	}

	if (strncpy(pucBufHead, aStrUtf16be, 2) == 0){
		return 1;
	}	
	return 0;		
}
static int GetUtf16beCodeFrmBuf(const unsigned char* pucBufStart, const unsigned char* pucBufEnd, unsigned int *pdwCode)
{	
	const unsigned char* pucBuf =  pucBufStart;
	unsigned char c = *pucBuf;
	
	if (pucBufStart == NULL || pucBufEnd == NULL || pdwCode == NULL || pucBufStart > pucBufEnd){		
		DBG_PRINTF("GetAsciiCodeFrmBuf parameter error\n!");
		return -1;		
	}
	
	if (pucBuf + 1 < pucBufEnd){
		*pdwCode = (unsigned int)pucBuf[1] + ((unsigned int)pucBuf[0]) << 8;
		return 2;
	}
	
	if (pucBuf == pucBufEnd){
		*pdwCode = c;
		return 1;
	}		
	return 0;
}


int Utf16beEncodingInit(void)
{
	
	AddFontOprForEncoding(&g_tASCIIEncodingOpr, GetFontOprForName("hzk"));
	AddFontOprForEncoding(&g_tASCIIEncodingOpr, GetFontOprForName("freetype"));
	return RegisterEncodingOpr(&g_tUtf16beEncodingOpr);
}


