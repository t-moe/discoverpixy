#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPainter>
#include <math.h>
#include <QMouseEvent>

extern "C" {
    #include "touch.h"
    #include "tft.h"
}

#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240

//function to calculate QColor out of a RGB565 16bit color
QColor QColorFromRGB565(uint16_t color) {
    //interpolate colors
    int R8 = (int) floor( (color>>(5+6)) * 255.0 / 31.0 + 0.5);
    int G8 = (int) floor( ((color>>5)&0x3F) * 255.0 / 63.0 + 0.5);
    int B8 = (int) floor( (color&0x1F) * 255.0 / 31.0 + 0.5);
    return QColor::fromRgb(R8,G8,B8);
}

//function to calculate QRgb out of a RGB565 16bit color
QRgb QRgbFromRGB565(uint16_t color) {
    //interpolate colors
    int R8 = (int) floor( (color>>(5+6)) * 255.0 / 31.0 + 0.5);
    int G8 = (int) floor( ((color>>5)&0x3F) * 255.0 / 63.0 + 0.5);
    int B8 = (int) floor( (color&0x1F) * 255.0 / 31.0 + 0.5);
    return qRgb(R8,G8,B8);
}


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), image(DISPLAY_WIDTH,DISPLAY_HEIGHT, QImage::Format_RGB16), ui(new Ui::MainWindow){
    ui->setupUi(this);
    image.fill(Qt::black); //clear display buffer
    currentScale = 1; //start with scale factor 1
    ui->widgetDisplay->setMouseTracking(true); //enable mouse move events, even when mouse is not pressed
    ui->widgetDisplay->installEventFilter(this); //install event filter for "display" widget, so that we receive those events as well
}

void MainWindow::draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    QPainter painter(&(image));
    painter.setPen(QColorFromRGB565(color));
    painter.drawLine(x1,y1,x2,y2);
    update();
}

void MainWindow::draw_pixel(uint16_t x, uint16_t y, uint16_t color)
{
    image.setPixel(x,y,QRgbFromRGB565(color));
    update();
}

void MainWindow::clear(uint16_t color)
{
    image.fill(QColorFromRGB565(color));
    update();
}

void MainWindow::draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    QPainter painter(&(image));
    painter.setPen(QColorFromRGB565(color));
    painter.drawRect(qMin(x1,x2),qMin(y1,y2),abs((int)x2-(int)x1),abs((int)y2-(int)y1));
    update();
}

void MainWindow::fill_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    QPainter painter(&(image));
    painter.fillRect(qMin(x1,x2),qMin(y1,y2),abs((int)x2-(int)x1)+1,abs((int)y2-(int)y1)+1,QColorFromRGB565(color));
    update();
}

void MainWindow::draw_bitmap_unscaled(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint16_t *dat)
{
    //Creating a new image and access it directly is faster than setPixel
    QImage img(width,height,QImage::Format_RGB32); //create a new image

    for(int yi=0; yi<height; yi++) { //for each line
        uint32_t* line = (uint32_t*)img.scanLine(yi); //get the pointer to the imagedata of the current line
        for(int xi=0; xi<width; xi++) { //for each column
            *line++=QRgbFromRGB565(dat[yi*width+xi]); //set pixel
        }
    }

    QPainter p(&image);
    p.drawImage(x,y,img); //draw created image
    update();
}

void MainWindow::draw_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color)
{
    QPainter painter(&(image));
    painter.setPen(QColorFromRGB565(color));
    painter.drawEllipse(QPoint(x,y), r, r);
    update();
}

void MainWindow::draw_char(uint16_t x, uint16_t y, uint16_t color, uint16_t bgcolor, QFont font, char c)
{
    QPainter painter(&(image));
    painter.setFont(font);

    if(bgcolor!=TRANSPARENT) { //background color is not transparent
            painter.setBackgroundMode(Qt::OpaqueMode);
            painter.setBackground(QColorFromRGB565(bgcolor)); //set text background
    }

    painter.setPen(QColorFromRGB565(color)); //set fontcolor
    y+=QFontMetrics(font).ascent(); //use y pos as highest point of char, instead of baseline
    painter.drawText(QPoint(x,y), QString(QChar(c))); //draw char
    update();
}

void MainWindow::paintEvent(QPaintEvent *)
{
  //this method is called whenever the window needs to be redrawn (or after update() is called)
  QPainter painter(this);

  //Create a QRectF which represents the rectangle to draw the buffered image to
  QRectF imgRect (ui->widgetDisplay->geometry().topLeft(),QSizeF(DISPLAY_WIDTH*currentScale,DISPLAY_HEIGHT*currentScale));

  painter.drawImage(imgRect,image); //draw buffer
  painter.setPen(QPen(Qt::green,2)); //set border color
  painter.drawRect(imgRect.adjusted(-1,-1,1,1)); //draw border
}

void MainWindow::mousePressEvent(QMouseEvent *evt)
{
    //the mouse was pressed
    checkAndSendEvent(evt->pos(),true);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *evt)
{
    //the mouse was released
    checkAndSendEvent(evt->pos(),false);
}

void MainWindow::mouseMoveEvent(QMouseEvent *evt)
{
    //the mouse was released
    checkAndSendEvent(evt->pos(),true);
}


bool MainWindow::eventFilter(QObject *obj, QEvent *evt)
{
   if(obj==ui->widgetDisplay) { //we received a redirect event from the target rectangle
        switch(evt->type()) {
            case QEvent::MouseMove: //it's a mouse move event
            {
                QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(evt); //get mouse event
                QPoint p = (mouseEvent->pos()-QPoint(1,1))/currentScale; //calculate correct corrdinates (undo scale)
                if(p.x()<DISPLAY_WIDTH && p.y()<DISPLAY_HEIGHT) { //mouse position not out of bounds
                    //set mouse position text
                    ui->txtMousePos->setText(QString("Mouse Position: (%1,%2)").arg(p.x()).arg(p.y()));
                }
            }
            break;
            default: break;
        }
   }
     return false;
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::checkAndSendEvent(QPoint pos, bool down)
{
    QPoint p = pos - ui->widgetDisplay->geometry().topLeft(); //make coordinate relative to target area (0,0)
    p/=currentScale; //undo scaling
    if(p.x()<0 || p.y()<0 || p.x() >= DISPLAY_WIDTH || p.y() >= DISPLAY_HEIGHT) return; //abort if out of bounds

    touch_add_raw_event(p.x(),p.y(),down?TOUCH_DOWN:TOUCH_UP); //submit touch event to touch module (common)
}


void MainWindow::on_cboZoom_currentIndexChanged(int index)
{
    currentScale=index+1; //zoom factor 1 is the 0th entry, zoom factor 2 is the 1st entry, zoom factor 3 is the 2nd entry
    update(); //force redraw
}
