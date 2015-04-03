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
protected:
     void paintEvent(QPaintEvent * evt);

    ~MainWindow();

private:
    QMutex render_mutex;
    QPixmap pixmap;

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
