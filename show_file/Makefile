CROSSCOMPILE := arm-linux-gnueabihf-

CFLAGS 	:= -Wall -O2 -c
CFLAGS  += -I$(PWD)/include

LDFLAGS := -lm -lfreetype

CC 	:= $(CROSSCOMPILE)gcc
LD 	:= $(CROSSCOMPILE)ld

OBJS := main.o \
			display/disp_manager.o        \
			display/fb.o                  \
			encoding/ascii.o              \
			encoding/utf-16be.o           \
			encoding/encoding_manager.o   \
			encoding/utf-8.o              \
			encoding/utf-16le.o           \
			draw/draw.o                   \
			font/ascii.o                 \
			font/hzk.o                   \
			font/freetype.o              \
			font/font_manager.o

all: $(OBJS)
	$(CC) $(LDFLAGS) -o show_file $^

clean:
	rm -f show_file
	rm -f $(OBJS)

%.o:%.c
	$(CC) $(CFLAGS) -o $@ $<

