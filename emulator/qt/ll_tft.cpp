#include "mainwindow.h"

extern "C" {
#include "ll_tft.h"
}

MainWindow* mainwindow;

bool ll_tft_init() {
   mainwindow = new MainWindow(); //create the designed window
   mainwindow->show(); //open it (non blocking)
   return true;

}

//the following functions redirect the call to the mainwindow, to a function with the same signature

void ll_tft_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
    mainwindow->draw_line(x1,y1,x2,y2,color);
}

void ll_tft_clear(uint16_t color) {
    mainwindow->clear(color);
}

void ll_tft_draw_pixel(uint16_t x,uint16_t y,uint16_t color) {
    mainwindow->draw_pixel(x,y,color);
}

void ll_tft_draw_rectangle(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2, uint16_t color) {
    mainwindow->draw_rectangle(x1,y1,x2,y2,color);
}

void ll_tft_fill_rectangle(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2, uint16_t color) {
     mainwindow->fill_rectangle(x1,y1,x2,y2,color);
}

void ll_tft_draw_bitmap_unscaled(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint16_t* dat) {
    mainwindow->draw_bitmap_unscaled(x,y,width,height,dat);
}

void ll_tft_draw_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color) {
    mainwindow->draw_circle(x,y,r,color);
}

uint8_t ll_tft_num_fonts() {
    return 2; //we have two fonts (see below)
}

//Helper function to get the QFont to the corresponding font number
//Note: only return monospaced fonts!!!
QFont get_font(uint8_t fontnum) {
    switch(fontnum) {
        case 0:
            return QFont("Monospace",8);
        case 1:
            return QFont("DejaVu Sans Mono",14);
        default:
        return QFont();
    }
}


uint8_t ll_tft_font_height(uint8_t fontnum) {
    QFont f = get_font(fontnum);
    if(f == QFont()) return -1;
    QFontMetrics m(f); //use font metcris object to calculate height of font
    return m.height();
}

uint8_t ll_tft_font_width(uint8_t fontnum) {
    QFont f = get_font(fontnum);
    if(f == QFont()) return -1;
    QFontMetrics m(f); //use font metcris object to calculate width of font
    return m.averageCharWidth();
}

void ll_tft_draw_char(uint16_t x, uint16_t y, uint16_t color, uint16_t bgcolor, uint8_t font, char c) {
    QFont f = get_font(font);
    if(f == QFont()) return; //if the font is the default-font, we want to abort.
    mainwindow->draw_char(x,y,color,bgcolor,f,c);
}

