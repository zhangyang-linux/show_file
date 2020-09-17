#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <config.h>
#include <unistd.h>
#include <ft2build.h>
#include FT_FREETYPE_H


static int g_FdFreeType;
static int g_dwFreeTypeFontSize;
static unsigned char* g_pucFreeTypeFontMem;
static unsigned char* g_pucFreeTypeFontMemEnd;
static FT_Library  g_tLibrary;
static FT_Face 	g_tFace;
static FT_GlyphSlot g_tglyph;

static int FreeTypeFontInit(char* pFileName, int dwFontSize);
static int FreeTypeGetFontBitMap(int dwCode, PT_FontBitMap ptFontBitMap);

static T_FontOpr g_tFreeTypeFontOpr = {
	.name = "freetype"
	.FontInit = FreeTypeFontInit,
	.GetFontBitMap = FreeTypeGetFontBitMap,	
};
	
static int FreeTypeFontInit(char* pFileName, int dwFontSize)
{	
	int iError;
	if (pFileName == NULL){
		printf("Fontfile error!\n");
		return -1;
	}
	iError = FT_Init_FreeType( &g_tLibrary ); 
	if (iError){
		printf("library initialization error!\n");
		return -1;
	}

	iError = FT_New_Face( g_tLibrary, pFileName, 0, &g_tFace);
	if (iError){
		printf("creating face error!\n");
		return -1;
	}
	g_tglyph = g_tFace->glyph;
  	iError = FT_Set_Pixel_Sizes(g_tFace, dwFontSize, dwFontSize);  
	if (iError){
		printf("Set_Pixel_Sizes error!\n");
		return -1;
	}
	return 0;	
}

static int FreeTypeGetFontBitMap(unsigned int dwCode, PT_FontBitMap ptFontBitMap)
{
	int iError;
	int iXPen = ptFontBitMap->iCurOriginX;	
	int iYPen = ptFontBitMap->iCurOriginY;

	iError = FT_Load_Char( g_tFace, dwCode, FT_LOAD_RENDER | FT_LOAD_MONOCHROME );
	if (iError){
		printf("FT_Load_Char for code: 0x%x error!\n", dwCode);
		return -1;
	}
	
	ptFontBitMap->iXLeft = iXPen + g_tglyph->bitmap_left;	
	ptFontBitMap->iYTop = iYPen - g_tglyph->bitmap_top;		
	ptFontBitMap->iXMax = ptFontBitMap->iXLeft + g_tglyph->bitmap->width;		
	ptFontBitMap->iYMax = ptFontBitMap->iYTop + g_tglyph->bitmap->rows;	
	ptFontBitMap->iBpp = 1;	
	ptFontBitMap->pucFontBitMapBuffer = g_tglyph->bitmap->buffer;
	ptFontBitMap->iNextOriginX = iXPen + g_tglyph->advance.x / 64;	
	ptFontBitMap->iNextOriginY = iYPen;
	return 0;
}
static int HZKFontExit(PT_FontOpr ptFontOpr)
{
	FT_Done_Face	(g_tFace);
	FT_Done_FreeType(g_tLibrary);
	return 0;
}



int FreeTypeInit(void)
{
	RegisterFontOpr(&g_tFreeTypeFontOpr);
}


