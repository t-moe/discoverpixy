/**************************************************************************************************************************************
* Project:       discoverpixy
* Website:       https://github.com/t-moe/discoverpixy
* Authors:       Aaron Schmocker, Timo Lang
* Institution:   BFH Bern University of Applied Sciences
* File:          emulator/qt/mainwindow.h
*
* Version History:
* Date			Autor Email			SHA		Changes
* 2015-04-03	timolang@gmail.com	51089aa	Refactored Project Structure for use with emulator
* 2015-04-03	timolang@gmail.com	1f2af9f	Added more tft functions to common and emulator. Fixed eventloop.
* 2015-04-03	timolang@gmail.com	1aa9194	Fixed Drawing of rects in Emulator. Got frames from pixy to emulator. Slooooow.
* 2015-04-27	timolang@gmail.com	259d446	Added touch support to emulator. Implemented basic touch function.
* 2015-04-27	aaron@duckpond.ch	aed90ef	Drawcircle added (emulator)
* 2015-04-27	timolang@gmail.com	e249fb2	Added font support
* 2015-05-09	timolang@gmail.com	c652b6b	Improved Emulator Gui
* 2015-05-25	timolang@gmail.com	911760e	Added "Mouse Position"-Label to Emulator.
* 2015-06-07	timolang@gmail.com	4b5768c	Improved Comments in whole emulator. Finalized emulator section in docu.
*
**************************************************************************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMutex>
#include <stdint.h>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    void draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
    void draw_pixel(uint16_t x, uint16_t y, uint16_t color);
    void clear(uint16_t color);
    void draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
    void fill_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
    void draw_bitmap_unscaled(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint16_t* dat);
    void draw_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color);
    void draw_char(uint16_t x, uint16_t y, uint16_t color, uint16_t bgcolor, QFont font, char c);

protected:
    bool eventFilter(QObject* obj , QEvent* env);
    void paintEvent(QPaintEvent* evt);
    void mousePressEvent(QMouseEvent* evt);
    void mouseReleaseEvent(QMouseEvent* evt);
    void mouseMoveEvent(QMouseEvent* evt);
    ~MainWindow();

private slots:
    void on_cboZoom_currentIndexChanged(int index); //slot that is called when the zoomlevel changed

private:
    QImage image; //Display buffer
    int currentScale; //current scale factor
    void checkAndSendEvent(QPoint pos, bool down);

    Ui::MainWindow* ui;

};
#endif // MAINWINDOW_H
