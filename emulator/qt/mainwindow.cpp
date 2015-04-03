#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPainter>
#include <math.h>

QColor colorFromRGB565(uint16_t color) {
    int R8 = (int) floor( (color>>(5+6)) * 255.0 / 31.0 + 0.5);
    int G8 = (int) floor( ((color>>5)&0x2F) * 255.0 / 63.0 + 0.5);
    int B8 = (int) floor( (color&0x1F) * 255.0 / 31.0 + 0.5);
    return QColor::fromRgb(R8,G8,B8);

}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    pixmap(320,240),
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

void MainWindow::paintEvent(QPaintEvent *)
{
  render_mutex.lock();
  QPainter painter(this);

  painter.drawPixmap(100,100,pixmap);
  painter.setPen(Qt::green);
  painter.drawRect(100-1,100-1,320+2,240+2);
  render_mutex.unlock();
}


MainWindow::~MainWindow()
{
    delete ui;
}
