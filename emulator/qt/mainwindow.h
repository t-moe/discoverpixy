#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMutex>
#include <stdint.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
    void draw_pixel(uint16_t x,uint16_t y,uint16_t color);
    void clear(uint16_t color);
    void draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
    void fill_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
    void draw_bitmap_unscaled(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint16_t *dat);
    void draw_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color);


protected:
     void paintEvent(QPaintEvent * evt);
     void mousePressEvent(QMouseEvent* evt);
     void mouseReleaseEvent(QMouseEvent* evt);
     void mouseMoveEvent(QMouseEvent* evt);
    ~MainWindow();

private:
    //QMutex render_mutex;
    QImage image;
    void checkAndSendEvent(QPoint pos, bool down);

    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
