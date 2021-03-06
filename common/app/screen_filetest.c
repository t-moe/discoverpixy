/**************************************************************************************************************************************
* Project:       discoverpixy
* Website:       https://github.com/t-moe/discoverpixy
* Authors:       Aaron Schmocker, Timo Lang
* Institution:   BFH Bern University of Applied Sciences
* File:          common/app/screen_filetest.c
*
* Version History:
* Date			Autor Email			SHA		Changes
* 2015-05-10	timolang@gmail.com	e2bce8f	Added filesystem module, tests and implementation for it in emulator.
* 2015-05-10	timolang@gmail.com	790f602	Added bitmap decoding/drawing example
* 2015-05-10	timolang@gmail.com	21edc56	Added doxyfile (doxygen) for the common folder. Started with doxygen comments for app and tft module.
* 2015-05-15	timolang@gmail.com	9a16865	Added doxgen comments to filesyste, checkbox, numupdown and screen module. And some minor changes to the other modules.
* 2015-05-15	timolang@gmail.com	85f1aee	Changed filetest to use new bitmap draw method.
*
**************************************************************************************************************************************/

#include "screen_filetest.h"
#include "button.h"
#include "tft.h"
#include "filesystem.h"
#include <stdlib.h>


static BUTTON_STRUCT b_back;

static void b_back_cb(void* button)
{
    gui_screen_back();
}


static void image_test();

static void enter(void* screen)
{
    tft_clear(HEX(0xBABECD));

    //Back button
    b_back.base.x1 = 10; //Start X of Button
    b_back.base.y1 = 200; //Start Y of Button
    b_back.base.x2 = AUTO; //b_back.base.x1+160; //Auto Calculate X2 with String Width
    b_back.base.y2 = AUTO; //Auto Calculate Y2 with String Height
    b_back.txtcolor = WHITE; //Set foreground color
    b_back.bgcolor = HEX(0xAE1010); //Set background color (Don't take 255 or 0 on at least one channel, to make shadows possible)
    b_back.font = 0; //Select Font
    b_back.text = "Back"; //Set Text (For formatted strings take sprintf)
    b_back.callback = b_back_cb; //Call b_back_cb as Callback
    gui_button_add(&b_back); //Register Button (and run the callback from now on)

    tft_draw_line(10, 30, 310, 30, BLACK);
    tft_print_line(10, 18, BLUE, TRANSPARENT, 0, "Name         D H RO Date   Time     Size");

    int y = 33;

    DIRECTORY_STRUCT* dir = filesystem_dir_open(".");

    if (dir == NULL) {
        return;
    }

    for (int i = 0; i < dir->num_files; i++) {
        FILE_STRUCT* file = &(dir->files[i]);
        tft_print_formatted(10, y,
                            (file->fattrib & F_DIR) ? GREEN : RED,
                            TRANSPARENT, 0, "%-13s%c %c %s %02u%02u%02u %02u:%02u:%02u %u",
                            file->fname,
                            (file->fattrib & F_DIR) ? 'D' : ' ',
                            (file->fattrib & F_HID) ? 'H' : ' ',
                            (file->fattrib & F_RDO) ? "R " : "RW",
                            file->fdate.day,
                            file->fdate.month,
                            (file->fdate.year + 1980) % 100,
                            file->ftime.hour,
                            file->ftime.min,
                            file->ftime.sec * 2,
                            file->fsize);
        y += 14;
    }

    filesystem_dir_close(dir);

    y += 14;

    FILE_HANDLE* file = filesystem_file_open("test.txt");

    if (file == NULL) {
        tft_print_line(10, y, BLUE, TRANSPARENT, 0, "Could not open test.txt");
    } else {
        char buf [30];
        int size = (file->fsize > 30) ? 29 : file->fsize - 1;
        FILE_STATUS st = filesystem_file_read(file, buf, size);

        if (st == F_OK) {
            buf[file->fpos] = '\0';
            tft_print_formatted(10, y, BLUE, TRANSPARENT, 0, "test.txt contains \"%s\"", buf);
            long num = strtol(&(buf[file->fpos - 4]), NULL, 0);
            num++;

            y += 14;

            if (filesystem_file_seek(file, file->fpos - 4) != F_OK) {
                tft_print_formatted(10, y, BLUE, TRANSPARENT, 0, "Could not seek to %d", file->fpos - 4);
            } else {
                sprintf(buf, "%04d", num);

                if (filesystem_file_write(file, buf, 4) != F_OK) {
                    tft_print_formatted(10, y, BLUE, TRANSPARENT, 0, "Could not write new number %d", num);
                } else {
                    tft_print_formatted(10, y, BLUE, TRANSPARENT, 0, "New number written %d", num);
                }
            }
        } else {
            tft_print_line(10, y, BLUE, TRANSPARENT, 0, "Could not read from test.txt");
        }

    }

    filesystem_file_close(file);

    image_test();
}

static void leave(void* screen)
{
    gui_button_remove(&b_back);
}

static void update(void* screen)
{
}


static SCREEN_STRUCT screen = {
    enter,
    leave,
    update
};


SCREEN_STRUCT* get_screen_filetest()
{
    return &screen;
}

static void image_test()
{


    if (!tft_draw_bitmap_file_unscaled(250, 170, "cpu.bmp")) {
        tft_print_line(10, 180, BLUE, TRANSPARENT, 0, "Could not open cpu.bmp");
    }

    tft_draw_rectangle(250 - 1, 170 - 1, 250 - 1 + 64, 170 - 1 + 64, BLACK);
}
