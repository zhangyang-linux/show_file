#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



/*./show_file [-s Size] [-f freetype_font_file] [-h HZK] <text_file> */
int main(int argc, char **argv)
{
	int iErro;
	unsigned long int dwFontSize;
	char acFreetypeFile[128] = {'\0'};
	char acHzkFile[128] = {'\0'};
	char acTextFile[128] = {'\0'};
	char acDisplay[128] = {'\0'};

	int bList = 0;
	
	strcpy(acDisplay, "fb");
	while (iErro = getopt(argc, argv, "ls:f:h:d:") != -1) {
		switch (iErro) {
			case 'l':
				bList = 1
				break;
		
			case 's':
				dwFontSize = unsigned long int strtoul(optarg, NULL, 0);
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
				printf("Usage: %s [-s Size] [-f freetype_font_file] [-h HZK] <text_file>\n", argv[0]);
				printf("Usage: %s -l\n", argv[0]);
				return -1;
				break;
		}
	}
	
	if (!bList && optarg >= argc){
		printf("Usage: %s [-s Size] [-f freetype_font_file] [-h HZK] <text_file>\n", argv[0]);
		printf("Usage: %s -l\n", argv[0]);
		return -1;
	}

	iErro = DisplayInit();
	if (iErro){
		printf("DisplayInit failed!\n");
		return -1;
	}

	iErro = FontInit();
	if (iErro){
		printf("FontInit failed!\n");
		return -1;
	}

	iErro = EncodingInit();
	if (iErro){
		printf("FontInit failed!\n");
		return -1;
	}	
	
}
