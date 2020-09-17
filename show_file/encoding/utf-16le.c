#include <encoding_manager.h>
#include <config.h>
#include <string.h>


static int isUtf16leCoding(const unsigned char* pucBufHead);
static int GetUtf16leCodeFrmBuf(const unsigned char* pucBufStart, const unsigned char* pucBufEnd, unsigned int *pdwCode);
	

static T_EncodingOpr g_tUtf16leEncodingOpr = {
	.name          = "utf-16le",
	.iHeadLen      = 2,
	.isSupport     = isUtf16leCoding,
	.GetCodeFrmBuf = GetUtf16leCodeFrmBuf,
}; 

static int isUtf16leCoding(const unsigned char* pucBufHead)
{ 
	const unsigned char aStrUtf16le[] = {0xFF, 0xFE, 0};

	if (pucBufHead == NULL){
		DBG_PRINTF("isUtf16leCoding parameter error\n!");
		return -1;
	}
		
	if (strncmp((const char*)pucBufHead, (const char*)aStrUtf16le, 2) == 0){
		return 1;
	}		
	return 0;		
}
static int GetUtf16leCodeFrmBuf(const unsigned char* pucBufStart, const unsigned char* pucBufEnd, unsigned int *pdwCode)
{	
	const unsigned char* pucBuf =  pucBufStart;
	const unsigned char c = *pucBuf;
	
	if (pucBufStart == NULL || pucBufEnd == NULL || pdwCode == NULL || pucBufStart > pucBufEnd){		
		DBG_PRINTF("GetAsciiCodeFrmBuf parameter error\n!");
		return -1;		
	}
	
	if (pucBuf + 1 < pucBufEnd){
		*pdwCode = ((unsigned int)pucBuf[0]) + ((unsigned int)pucBuf[1] << 8);
		return 2;
	}
	
	if (pucBuf == pucBufEnd){
		*pdwCode = c;
		return 1;
	}		
	return 0;

}


int Utf16leEncodingInit(void)
{
	
	AddFontOprForEncoding(&g_tUtf16leEncodingOpr, GetFontOprForName("hzk"));
	AddFontOprForEncoding(&g_tUtf16leEncodingOpr, GetFontOprForName("freetype"));
	return RegisterEncodingOpr(&g_tUtf16leEncodingOpr);
}



