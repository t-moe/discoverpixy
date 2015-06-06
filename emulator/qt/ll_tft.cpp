#include "mainwindow.h"
#include <QDebug>

extern "C" {
#include "ll_tft.h"
}

MainWindow* mainwindow;

bool ll_tft_init() {
   qDebug() << "tft init done";
   mainwindow = new MainWindow();
   mainwindow->show();
   return true;

}

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
    return 2;
}

QFont get_font(uint8_t fontnum) {
    switch(fontnum) {
        case 0:
            return QFont("Courier New",8);
        case 1:
            return QFont("Courier New",14);
        default:
        return QFont();
    }
}


uint8_t ll_tft_font_height(uint8_t fontnum) {
    QFont f = get_font(fontnum);
    if(f == QFont()) return -1;
    QFontMetrics m(f);
    return m.height();
}

uint8_t ll_tft_font_width(uint8_t fontnum) {
    QFont f = get_font(fontnum);
    if(f == QFont()) return -1;
    QFontMetrics m(f);
    return m.averageCharWidth();
}

void ll_tft_draw_char(uint16_t x, uint16_t y, uint16_t color, uint16_t bgcolor, uint8_t font, char c) {
    QFont f = get_font(font);
    if(f == QFont()) return;
    mainwindow->draw_char(x,y,color,bgcolor,f,c);
}

