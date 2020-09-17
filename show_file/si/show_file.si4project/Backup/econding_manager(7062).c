#include "encoding_manager.h"
#include "config.h"

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

