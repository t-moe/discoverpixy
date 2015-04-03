#include "mainwindow.h"
#include <QDebug>
#include "ll_tft.h"

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
