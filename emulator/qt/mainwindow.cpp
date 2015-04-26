#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPainter>
#include <math.h>
#include <QMouseEvent>

extern "C" {
    #include "touch.h"
}

#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240
#define DISPLAY_X 10
#define DISPLAY_Y 10

QColor QColorFromRGB565(uint16_t color) {

    int R8 = (int) floor( (color>>(5+6)) * 255.0 / 31.0 + 0.5);
    int G8 = (int) floor( ((color>>5)&0x3F) * 255.0 / 63.0 + 0.5);
    int B8 = (int) floor( (color&0x1F) * 255.0 / 31.0 + 0.5);
    return QColor::fromRgb(R8,G8,B8);
}

QRgb QRgbFromRGB565(uint16_t color) {

    int R8 = (int) floor( (color>>(5+6)) * 255.0 / 31.0 + 0.5);
    int G8 = (int) floor( ((color>>5)&0x3F) * 255.0 / 63.0 + 0.5);
    int B8 = (int) floor( (color&0x1F) * 255.0 / 31.0 + 0.5);
    return qRgb(R8,G8,B8);
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    image(DISPLAY_WIDTH,DISPLAY_HEIGHT,QImage::Format_RGB16),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    image.fill(Qt::black);
}

void MainWindow::draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    //render_mutex.lock();
    QPainter painter(&(image));
    painter.setPen(QColorFromRGB565(color));
    painter.drawLine(x1,y1,x2,y2);
    //render_mutex.unlock();
    update();
}

void MainWindow::draw_pixel(uint16_t x, uint16_t y, uint16_t color)
{
    //render_mutex.lock();
    image.setPixel(x,y,QRgbFromRGB565(color));
    //render_mutex.unlock();
    update();
}

void MainWindow::clear(uint16_t color)
{
    //render_mutex.lock();
    image.fill(QColorFromRGB565(color));
    //render_mutex.unlock();
    update();
}

void MainWindow::draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    //render_mutex.lock();
    QPainter painter(&(image));
    painter.setPen(QColorFromRGB565(color));
    painter.drawRect(x1,y1,abs(x2-x1)+1,abs(y2-y1)+1);
    //render_mutex.unlock();
    update();
}

void MainWindow::fill_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    //render_mutex.lock();
    QPainter painter(&(image));
    painter.fillRect(x1,y1,abs(x2-x1)+1,abs(y2-y1)+1,QColorFromRGB565(color));
    //render_mutex.unlock();
    update();
}

void MainWindow::draw_bitmap_unscaled(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint16_t *dat)
{
    //Creating a new image and access it directly is faster than setPixel
    QImage img(width,height,QImage::Format_RGB32);

    for(int yi=0; yi<height; yi++) {
        uint32_t* line = (uint32_t*)img.scanLine(yi);
        for(int xi=0; xi<width; xi++) {
            *line++=QRgbFromRGB565(dat[yi*width+xi]);
        }
    }

    //render_mutex.lock();
    QPainter p(&image);
    p.drawImage(x,y,img);
    //render_mutex.unlock();
    update();
}

void MainWindow::paintEvent(QPaintEvent *)
{
  //render_mutex.lock();
  QPainter painter(this);

  painter.drawImage(DISPLAY_X,DISPLAY_Y,image);
  painter.setPen(QPen(Qt::green,2));
  painter.drawRect(DISPLAY_X-1,DISPLAY_Y-1,DISPLAY_WIDTH+2,DISPLAY_HEIGHT+2);
  //render_mutex.unlock();
}

void MainWindow::mousePressEvent(QMouseEvent *evt)
{
    //qDebug() << "down" << evt->pos();
    checkAndSendEvent(evt->pos(),true);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *evt)
{
    //qDebug() << "up" << evt->pos();
    checkAndSendEvent(evt->pos(),false);
}

void MainWindow::mouseMoveEvent(QMouseEvent *evt)
{
   //qDebug() << "move" << evt->pos();
    checkAndSendEvent(evt->pos(),true);

}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::checkAndSendEvent(QPoint pos, bool down)
{
    QPoint p = pos - QPoint(DISPLAY_X,DISPLAY_Y);
    if(p.x()<0 || p.y()<0 || p.x() >= DISPLAY_WIDTH || p.y() >= DISPLAY_HEIGHT) return;

    //qDebug() << down << p;

    touch_add_raw_event(p.x(),p.y(),down?TOUCH_DOWN:TOUCH_UP);
}

