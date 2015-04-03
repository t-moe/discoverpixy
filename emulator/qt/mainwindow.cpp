#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPainter>
#include <math.h>


#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240
#define DISPLAY_X 100
#define DISPLAY_Y 100

QColor colorFromRGB565(uint16_t color) {
    int R8 = (int) floor( (color>>(5+6)) * 255.0 / 31.0 + 0.5);
    int G8 = (int) floor( ((color>>5)&0x2F) * 255.0 / 63.0 + 0.5);
    int B8 = (int) floor( (color&0x1F) * 255.0 / 31.0 + 0.5);
    return QColor::fromRgb(R8,G8,B8);
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    pixmap(DISPLAY_WIDTH,DISPLAY_HEIGHT),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

void MainWindow::draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{

    render_mutex.lock();
    QPainter painter(&(pixmap));
    painter.setPen(colorFromRGB565(color));
    painter.drawLine(x1,y1,x2,y2);
    render_mutex.unlock();
}

void MainWindow::draw_pixel(uint16_t x, uint16_t y, uint16_t color)
{
    render_mutex.lock();
    QPainter painter(&(pixmap));
    painter.setPen(colorFromRGB565(color));
    painter.drawPoint(x,y);
    render_mutex.unlock();
}

void MainWindow::clear(uint16_t color)
{
    render_mutex.lock();
    pixmap.fill(colorFromRGB565(color));
    render_mutex.unlock();
}

void MainWindow::draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    render_mutex.lock();
    QPainter painter(&(pixmap));
    painter.setPen(colorFromRGB565(color));
    painter.drawRect(x1,y1,x2,y2);
    render_mutex.unlock();
}

void MainWindow::fill_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    render_mutex.lock();
    QPainter painter(&(pixmap));
    painter.fillRect(x1,y1,x2,y2,colorFromRGB565(color));
    render_mutex.unlock();
}

void MainWindow::draw_bitmap_unscaled(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *dat)
{
    render_mutex.lock();
    QPainter painter(&(pixmap));

    for(int yi=0; yi<height; yi++) {
        for(int xi=0; xi<width; xi++) {
            painter.setPen(colorFromRGB565(dat[yi*width+xi]));
            painter.drawPoint(x+xi,y+yi);
        }
    }
    render_mutex.unlock();
}

void MainWindow::paintEvent(QPaintEvent *)
{
  render_mutex.lock();
  QPainter painter(this);

  painter.drawPixmap(DISPLAY_X,DISPLAY_Y,pixmap);
  painter.setPen(QPen(Qt::green,2));
  painter.drawRect(DISPLAY_X-1,DISPLAY_Y-1,DISPLAY_WIDTH+2,DISPLAY_HEIGHT+2);
  render_mutex.unlock();
}


MainWindow::~MainWindow()
{
    delete ui;
}

