#ifndef _ENCODING_MANAGER_H
#define _ENCODING_MANAGER_H

#include <fonts_manager.h>
#include <disp_manager.h>
typedef struct EncodingOpr {
	char *name;
	int iHeadLen;
	PT_FontOpr ptFontOprSupportedHead;
	int (*isSupport)(const unsigned char* pucBufHead);
	int (*GetCodeFrmBuf)(const unsigned char*pucBufStart, const unsigned char*pucBufEnd, unsigned int *pdwCode);
	struct EncodingOpr *ptNext;
}T_EncodingOpr, *PT_EncodingOpr;

int AddFontOprForEncoding(PT_EncodingOpr ptEncodingOpr, PT_FontOpr ptFontOpr);
#endif //_ENCODING_MANAGER_H