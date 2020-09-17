#ifndef _ENCODING_MANAGER_H
#define _ENCODING_MANAGER_H

#include <font_manager.h>
#include <disp_manager.h>
typedef struct EncodingOpr {
	char *name;
	int iHeadLen;
	PT_FontOpr ptFontOprSupportedHead;
	int (*isSupport)(const char* pucBufHead);
	int (*GetCodeFrmBuf)(const unsigned char*pucBufStart, const unsigned char*pucBufEnd, unsigned int *pdwCode);
	struct EncodingOpr *ptNext;
}T_EncodingOpr, *PT_EncodingOpr;

PT_EncodingOpr SelectEncodingOprForTextFile(const unsigned char* pucFileMem);

void ShowEncodingOpr();

int RegisterEncodingOpr(PT_EncodingOpr ptEncodingOpr);

int DelFontOprFrmCodeEnding(PT_EncodingOpr ptEncodingOpr, PT_FontOpr ptFontOpr);

int EncodingInit(void);

int AsciiEncodingInit(void);

int Utf16beEncodingInit(void);

int Utf16leEncodingInit(void);

int Utf8EncodingInit(void);

int AddFontOprForEncoding(PT_EncodingOpr ptEncodingOpr, PT_FontOpr ptFontOpr);
#endif //_ENCODING_MANAGER_H