#ifndef _ENCODING_MANAGER_H
#define _ENCODING_MANAGER_H

#include <fonts_manager.h>
#include <disp_manager.h>
typedef struct EncodingOpr {
	char *name;
	int iHeadLen;
	PT_FontOpr ptFontOprSupportedHead;
	int (*isSupport)(unsigned char *pucBufHead);
	int (*GetCodeFrmBuf)(unsigned char *pucBufStart, unsigned char *pucBufEnd, unsigned int *pdwCode);
	struct EncodingOpr *ptNext;
}T_EncodingOpr, *PT_EncodingOpr;

#endif //_ENCODING_MANAGER_H