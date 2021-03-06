#include <encoding_manager.h>


static int isAsciiCoding(const unsigned char*pucBufHead);
static int GetAsciiCodeFrmBuf(const unsigned char*pucBufStart, const unsigned char* pucBufEnd, unsigned int *pdwCode);
	

static T_EncodingOpr g_tASCIIEncodingOpr = {
	.name          = "ascii",
	.iHeadLen      = 0,
	.isSupport     = isAsciiCoding,
	.GetCodeFrmBuf = GetAsciiCodeFrmBuf,
}; 

static int isAsciiCoding(const unsigned char* pucBufHead)
{ 
	const unsigned char aStrUtf8[]    = {0xEF, 0xBB, 0xBF, 0};
	const unsigned char aStrUtf16le[] = {0xFF, 0xFE, 0};
	const unsigned char aStrUtf16be[] = {0xFE, 0xFF, 0};

	if (pucBufHead == NULL){
		printf("isAsciiCoding parameter error\n!");
		return -1;
	}

	if (strncpy(pucBufHead, aStrUtf8, 3) == 0){
		return 0;
	}

	if (strncpy(pucBufHead, aStrUtf16le, 2) == 0){
		return 0;
	}	

	if (strncpy(pucBufHead, aStrUtf16be, 2) == 0){
		return 0;
	}

	return 1;		
}
static int GetAsciiCodeFrmBuf(const unsigned char* pucBufStart, const unsigned char* pucBufEnd, unsigned int *pdwCode)
{	
	const unsigned char* pucBuf =  pucBufStart;
	const unsigned char c = *pucBuf;
	if (pucBufStart == NULL || pucBufEnd == NULL || pdwCode == NULL || pucBufStart > pucBufEnd){		
		printf("GetAsciiCodeFrmBuf parameter error\n!");
		return -1;		
	}
	
	if ((pucBuf < pucBufEnd) && (c < (unsigned char)0x80)){
		*pdwCode = pucBuf[0];
		return 1;
	}

	if ((pucBuf + 1 < pucBufEnd) && (c > (unsigned char)0x80)){
		*pdwCode = (unsigned int)pucBuf[0] + ((unsigned int)pucBuf[1]) << 8;
		return 2;
	}

	if (pucBuf == pucBufEnd){
		*pdwCode = c;
		return 1;
	}		
	return 0;
}


int AsciiEncodingInit(void)
{	
	AddFontOprForEncoding();
	return RegisterEncodingOpr(&g_tASCIIEncodingOpr);
}

