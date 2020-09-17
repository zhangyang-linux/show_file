#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <string.h>
#include "config.h"
#include "disp_manger.h"
static int g_fd;
static struct fb_var_screeninfo g_FBVar;
static struct fb_fix_screeninfo g_FBFix;
static unsigned char* g_ucFBMem;
static unsigned int g_dwFBMemSize;
static unsigned int g_dwLineWidthByte;
static unsigned int g_dwPixelWidthByte;


static int FBDeviceInit(void);
static int FBShowPixel(int iPenX, int iPenY, unsigned int dwColor);
static int FBCleanScreen(unsigned int dwBackColor);
static int FBDeviceExit(void);

static T_DispOpr g_tFBOpr = {
	.name = "fb",
	.DeviceInit  = FBShowPixel,
	.ShowPixel   = FBShowPixel,
	.CleanScreen = FBCleanScreen,
	.DeviceExit  = FBDeviceExit,	
};

static int FBDeviceInit(void)
{
	int fd;
	int iRet = 0;
	
	g_fd = open(FB_DEVICE_NAME, O_RDWR);
	if (g_fd < 0){
		DBG_PRINTF("can not open %s\n", FB_DEVICE_NAME);
	}
	
	iRet = ioctl(g_fd, FBIOGET_VSCREENINFO, &g_FBVar);
	if (iRet < 0){
		DBG_PRINTF("can not get fb's var\n");
		return -1;
	}

	iRet = ioctl(g_fd, FBIOGET_FSCREENINFO, &g_FBFix);
	if (iRet < 0){
		DBG_PRINTF("can not get fb's fix\n");
		return -1;
	}
	
	g_dwFBMemSize      = g_FBVar.xres * g_FBVar.yres * g_FBVar.bits_per_pixel / 8;
	g_dwLineWidthByte  = g_FBVar.xres * g_FBVar.bits_per_pixel / 8;
	g_dwPixelWidthByte = g_FBVar.bits_per_pixel / 8;
	
	g_ucFBMem = (unsigned char*)mmap(NULL, g_dwFBMemSize, PROT_READ | PROT_WRITE, MAP_SHARED, g_fd, 0);
	if (g_ucFBMem < 0){
		DBG_PRINTF("can not mmap\n");
		return -1;
	}

	g_tFBOpr.iXres = g_FBVar.xres;
	g_tFBOpr.iYres = g_FBVar.yres;
	g_tFBOpr.pucFBMem = g_ucFBMem;
	g_tFBOpr.iBpp  = g_FBVar.bits_per_pixel;
	return iRet;
}

static int FBShowPixel(int iPenX, int iPenY, unsigned int dwColor)
{
	int iRet= 0;
	unsigned char* pucFB;
	unsigned short* pw16bpp;
	unsigned int* pdw32bpp;
	unsigned int wColor16bpp;
	int red;
	int green;
	int blue;

	if ((iPenX >= g_FBVar.xres) || (iPenY >= g_FBVar.yres){
		DBG_PRINTF("out region!\n");
		return -1;
	}

	pucFB = g_ucFBMem + iPenY * g_dwLineWidthByte + iPenX * g_dwPixelWidthByte;
	pw16bpp = (unsigned short*)pucFB;
	pdw32bpp = (unsigned int*)pucFB;
	/* RRBBGG */
	switch (g_FBVar.bits_per_pixel){

		case '8':
			*pucFB = dwColor;
			break;
		/* RGB */
		/* 565 */
		case '16':
			red         = (dwColor >> (16 + 3)) & 0x1f;
			green       = (dwColor >> (8 + 2)) & 0x3f;
			blue        = (dwColor >> 3) & 0x1f;
			wColor16bpp = (red << 11) | (green << 5) | blue;
			*pw16bpp    = wColor16bpp:
			break;

		case '24':
			red   = (dwColor >> 16) & 0xff;
			green = (dwColor >> 8) & 0xff;
			blue  = dwColor & 0xff;
			*pucFB = red;
			*pucFB++ = green;
			*pucFB++ = blue;
			break;

		case '32':
			*pdw32bpp = dwColor;
			break;
		
		default:
			printf("can't support this %d bpp\n", g_FBVar.bits_per_pixel);
			return -1;
			break;
	}
	
	return iRet;

}

static int FBCleanScreen(unsigned int dwBackColor)
{
	int iRet= 0;
	unsigned char* pucFB;
	unsigned short* pw16bpp;
	unsigned int* pdw32bpp;
	unsigned int wColor16bpp;
	int red;
	int green;
	int blue;
	int i = 0;

	pucFB = g_ucFBMem + iPenY * g_dwLineWidthByte + iPenX * g_dwPixelWidthByte;
	pw16bpp = (unsigned short*)pucFB;
	pdw32bpp = (unsigned int*)pucFB;
	/* RRBBGG */
	switch (g_FBVar.bits_per_pixel){

		case '8':
			memset(pucFB, 0, g_dwFBMemSize);
			break;
		/* RGB */
		/* 565 */
		case '16':
			red         = (dwBackColor >> (16 + 3)) & 0x1f;
			green       = (dwBackColor >> (8 + 2)) & 0x3f;
			blue        = (dwBackColor >> 3) & 0x1f;
			wColor16bpp = (red << 11) | (green << 5) | blue;

			while (i < g_dwFBMemSize) {
				*pw16bpp++    = wColor16bpp:
				i += 2;
			}
			break;

		case '24':
			red   = (dwBackColor >> 16) & 0xff;
			green = (dwBackColor >> 8) & 0xff;
			blue  = dwBackColor & 0xff;
			while (i < g_dwFBMemSize) {
				*pucFB   = red;
			 	*pucFB++ = green;
				*pucFB++ = blue;
				i += 3;
			}
			break;

		case '32':
			while (i < g_dwFBMemSize) {
				*pdw32bpp++ = dwBackColor;
				i += 4;
			}
			break;

		default:
			printf("can't support this %d bpp\n", g_FBVar.bits_per_pixel);
			return -1;
			break;
	}
	
	return 0;

}

static int FBDeviceExit(PT_DispOpr ptDispOpr)
{
	munmap(ptDispOpr->pucFBMem, g_dwFBMemSize);
	return 0;
}

int FBInit(void)
{
	return RegisterDispOpr(&g_tFBOpr);
}

