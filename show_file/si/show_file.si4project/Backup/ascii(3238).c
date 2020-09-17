#include <encoding_manager.h>

static T_EncodingOpr g_tASCIIEncodingOpr = {

}; 

int AsciiEncodingInit(void)
{
	return RegisterEncodingOpr();
}

