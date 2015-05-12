#include "screen_filetest.h"
#include "button.h"
#include "tft.h"
#include "filesystem.h"
#include <stdlib.h>


static BUTTON_STRUCT b_back;

static void b_back_cb(void* button) {
        gui_screen_back();
}


static void image_test();

static void enter(void* screen) {
	tft_clear(HEX(0xBABECD));
	
	//Back button
	b_back.base.x1=10; //Start X of Button
        b_back.base.y1=200; //Start Y of Button
        b_back.base.x2=AUTO; //b_back.base.x1+160; //Auto Calculate X2 with String Width
        b_back.base.y2=AUTO; //Auto Calculate Y2 with String Height
        b_back.txtcolor=WHITE; //Set foreground color
        b_back.bgcolor=HEX(0xAE1010); //Set background color (Don't take 255 or 0 on at least one channel, to make shadows possible)
        b_back.font=0; //Select Font
        b_back.text="Back"; //Set Text (For formatted strings take sprintf)
        b_back.callback=b_back_cb; //Call b_back_cb as Callback
        gui_button_add(&b_back); //Register Button (and run the callback from now on)


/*
	//tft test
	tft_draw_pixel(0,0,BLACK);
	tft_draw_pixel(319,239,BLACK);
	tft_draw_pixel(10,210,BLUE);
        tft_draw_pixel(12,210,BLUE);
        tft_draw_rectangle(40,100,60,235,BLUE);
        tft_fill_rectangle(100,215,200,225,GREEN);
        tft_draw_line(10,50,310,225,RGB(0xFF,0,0xFF));
        tft_draw_circle(10,10,100, RED);
        tft_print_line(30, 130, RED, BLUE, 0, "Hallo");
*/

	tft_draw_line(10,30,310,30,BLACK);
	tft_print_line(10,18,BLUE,TRANSPARENT,0,"Name         D H RO Date   Time     Size");
	
	int y = 33;	
	
	DIRECTORY_STRUCT* dir = filesystem_dir_open(".");
	if(dir==NULL) return;

	for(int i=0; i<dir->num_files; i++) {
		FILE_STRUCT* file = &(dir->files[i]);
		tft_print_formatted(10,y,
			(file->fattrib&F_DIR)?GREEN:RED,
			TRANSPARENT,0,"%-13s%c %c %s %02u%02u%02u %02u:%02u:%02u %u",
			file->fname,
			(file->fattrib&F_DIR)?'D':' ',
			(file->fattrib&F_HID)?'H':' ',
			(file->fattrib&F_RDO)?"R ":"RW",
			file->fdate.day,
			file->fdate.month,
			(file->fdate.year+1980)%100,
			file->ftime.hour,
			file->ftime.min,
			file->ftime.sec*2,
			file->fsize);
		y+=14;
	}


	y+=14;

	FILE_HANDLE* file = filesystem_file_open("test.txt");
	if(file==NULL) {
		tft_print_line(10,y,BLUE,TRANSPARENT,0,"Could not open test.txt");
	} else {
		char buf [30];
		int size = (file->fsize>30)?29:file->fsize-1;
		FILE_STATUS st = filesystem_file_read(file,buf,size);
	
		if(st==F_OK) {
			buf[file->fpos]='\0';
			tft_print_formatted(10,y,BLUE,TRANSPARENT,0,"test.txt contains \"%s\"",buf);
			long num = strtol(&(buf[file->fpos-4]),NULL,0);
			num++;
			
			y+=14;
			
			if(filesystem_file_seek(file,file->fpos-4)!=F_OK) {
				tft_print_formatted(10,y,BLUE,TRANSPARENT,0,"Could not seek to %d",file->fpos-4);
			} else {
				sprintf(buf,"%04d",num);
				if(filesystem_file_write(file,buf,4) != F_OK) {
					tft_print_formatted(10,y,BLUE,TRANSPARENT,0,"Could not write new number %d",num);
				} else {
					tft_print_formatted(10,y,BLUE,TRANSPARENT,0,"New number written %d",num);
				}
			}
		} else {
			tft_print_line(10,y,BLUE,TRANSPARENT,0,"Could not read from test.txt");
		}

	}
	filesystem_file_close(file);

	image_test();	
}

static void leave(void* screen) {
	gui_button_remove(&b_back);
}

static void update(void* screen) {
}


static SCREEN_STRUCT screen = {
	enter,
	leave,
	update
};


SCREEN_STRUCT* get_screen_filetest() {
	return &screen;
}

static void image_test() {
	//Source: http://stackoverflow.com/a/17040962/2606757

	FILE_HANDLE* file = filesystem_file_open("cpu.bmp");
	if(file==NULL) {
		tft_print_line(10,180,BLUE,TRANSPARENT,0,"Could not open cpu.bmp");
		return;
	}


	unsigned char info[54];
	if(filesystem_file_read(file,info,54)!=F_OK) {
		tft_print_line(10,180,BLUE,TRANSPARENT,0,"Could not read header of cpu.bmp");
		filesystem_file_close(file);        
		return;
	}

	// extract image height and width from header
	int width = *(int*)&info[18];
	int height = *(int*)&info[22];

	filesystem_file_seek(file,*(int*)&info[10]);

	unsigned char data [width*4];
	tft_draw_rectangle(100-1,160-1,100-1+width,160-1+height,BLACK);

	for(int i = 0; i < height; i++)
	{
		filesystem_file_read(file,data,width*4);
		for(int j = 0; j < width*4; j += 4)
		{
			unsigned char a = data[j];
			unsigned char r = data[j+1]; 
			unsigned char g = data[j+2]; 
			unsigned char b = data[j+3]; 
			if(a!=0) {
				tft_draw_pixel(100+j/4,160+height-1-i,RGB(r,g,b));
			}
		}
	}

	filesystem_file_close(file);

}
