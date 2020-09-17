


#include <encoding_manager.h>


static int isUtf8Coding(const unsigned char*pucBufHead);
static int GetUtf8CodeFrmBuf(const unsigned char*pucBufStart, const unsigned char* pucBufEnd, unsigned int *pdwCode);
	

static T_EncodingOpr g_tUtf16leEncodingOpr = {
	.name          = "utf-8",
	.iHeadLen      = 3,
	.isSupport     = isUtf8Coding,
	.GetCodeFrmBuf = GetUtf8CodeFrmBuf,
}; 

static int isUtf8Coding(const unsigned char* pucBufHead)
{ 
	const unsigned char aStrUtf8[] = {0xEF, 0xBB, 0xBF, 0};
	if (pucBufHead == NULL){
		DBG_PRINTF("isUtf16leCoding parameter error\n!");
		return -1;
	}
	
	if (strncpy(pucBufHead, aStrUtf8, 2) == 0){
		return 1;
	}	
	return 0;		
}

static int GetPreOneByte(const unsigned char ucPreByte)
{
	int iCnt = 0;
	int i;
	for (i = 7; i >= 0; i--) {
		if (ucPreByte & (0x1 << i))
			iCnt++;
		break;
	}
	return iCnt;	
}
static int GetUtf8CodeFrmBuf(const unsigned char* pucBufStart, const unsigned char* pucBufEnd, unsigned int *pdwCode)
{	
	int iNum;
	int i;
	unsigned int iSum;
	unsigned int iTmpVal;
	const unsigned char* pucBuf =  pucBufStart;
	
	
	if (pucBufStart == NULL || pucBufEnd == NULL || pdwCode == NULL || pucBufStart > pucBufEnd){		
		DBG_PRINTF("GetAsciiCodeFrmBuf parameter error\n!");
		return -1;		
	}
	
	iNum = GetPreOneByte(pucBuf[0]);
	if (iNum == 0) {
		*pdwCode = *pucBuf;
		return 1;
	}
	if (pucBuf + iNum < pucBufEnd) {
		iTmpVal = pucBuf[0] << iNum;
		iTmpVal = iTmpVal >> iNum;
		iSum += iTmpVal;
		
		for (i = 1; i < iNum; i++) {
			iTmpVal = pucBuf[i] & 0x3f;
			iSum =	iSum << 6;
			iSum += iTmpVal;
		}
		*pdwCode = iSum;
		return 3;
	}

	if (pucBuf == pucBufEnd){
		*pdwCode = *pucBuf;
		return 1;
	}		
	return 0;
}


int Utf8EncodingInit(void)
{	
	
	AddFontOprForEncoding(&g_tASCIIEncodingOpr, GetFontOprForName("hzk"));
	AddFontOprForEncoding(&g_tASCIIEncodingOpr, GetFontOprForName("freetype"));
	return RegisterEncodingOpr(&g_tUtf16beEncodingOpr);
}




