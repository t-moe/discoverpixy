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


