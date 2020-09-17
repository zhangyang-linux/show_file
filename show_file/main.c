#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <config.h>
#include <draw.h>
#include <encoding_manager.h>
#include <font_manager.h>
#include <disp_manager.h>



/*./show_file [-s Size] [-f freetype_font_file] [-h HZK] <text_file> */
int main(int argc, char **argv)
{
	int iError;
	char c;
	unsigned long int dwFontSize = 16;
	char acFreetypeFile[128] = {'\0'};
	char acHzkFile[128] = {'\0'};
	char acTextFile[128] = {'\0'};
	char acDisplay[128] = {'\0'};

	int bList = 0;
	
	strcpy(acDisplay, "fb");
	while ((iError = getopt(argc, argv, "ls:f:h:d:")) != -1) {
		switch (iError) {
			case 'l':
				bList = 1;
				break;
		
			case 's':
				dwFontSize = (unsigned long int)strtoul(optarg, NULL, 0);
				break;

			case 'f':
				strncpy(acFreetypeFile, optarg, 128);
				break;

			case 'h':
				strncpy(acHzkFile, optarg, 128);
				break;

			case 'd':
				strncpy(acDisplay, optarg, 128);
				break;

			default :
				DBG_PRINTF("Usage: %s [-s Size] [-f freetype_font_file] [-h HZK] <text_file>\n", argv[0]);
				DBG_PRINTF("Usage: %s -l\n", argv[0]);
				return -1;
				break;
		}
	}
	
	if (!bList && optind >= argc){
		DBG_PRINTF("Usage: %s [-s Size] [-f freetype_font_file] [-h HZK] <text_file>\n", argv[0]);
		DBG_PRINTF("Usage: %s -l\n", argv[0]);
		return -1;
	}

	iError = DisplayInit();
	if (iError){
		DBG_PRINTF("DisplayInit failed!\n");
		return -1;
	}

	iError = FontInit();
	if (iError){
		DBG_PRINTF("FontInit failed!\n");
		return -1;
	}

	iError = EncodingInit();
	if (iError){
		DBG_PRINTF("FontInit failed!\n");
		return -1;
	}	

	if (bList) {
		DBG_PRINTF("supported display:\n");
		ShowDisplayOpr();
		DBG_PRINTF("supported font:\n");
		ShowFontOpr();
		DBG_PRINTF("supported ecoding:\n");
		ShowEncodingOpr();	
	}

	strncpy(acTextFile, argv[optind], 128);
	acTextFile[127] = '\0';
	iError = OpenTextFile(acTextFile);
	if (iError) {
		printf("OpenTextFile error!\n");
		return -1;
	}

	iError = SetTextDetail(acHzkFile, acFreetypeFile, dwFontSize);
	if (iError) {
		printf("OpenTextFile error!\n");
		return -1;
	}
	
	DBG_PRINTF("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);

	iError = SelectAndInitDisplay(acDisplay);
	if (iError)
	{
		printf("SelectAndInitDisplay error!\n");
		return -1;
	}

	ShowNextPage();

	while (1) {
		printf("Enter 'n' to show next page, 'u' to show previous page, 'q' to exit: ");
		do {
			c = getchar();
		} while ((c != 'n') && (c != 'b') && (c != 'q'));
		
		if (c == 'n') {
			ShowNextPage();
		}
		else if (c == 'b') {
			ShowPrePage();
		}
		else if (c == 'q') {
			return 0;
		}
	}
	return 0;
}
