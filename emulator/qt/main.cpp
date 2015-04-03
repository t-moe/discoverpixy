#include <QApplication>
#include <QtConcurrent/QtConcurrent>


extern "C" {
    void qt_init(int argc, char* argv[]);
    int qt_execute();
    void app_init();
    void app_process();
}

QApplication* app_ptr;

void qt_init(int argc, char* argv[]) {
    static QApplication app(argc,argv);
    app_ptr = &app;
    app_init();
}

void app_loop() {
    while(1) {
        app_process();
        QApplication::processEvents();
    }
}

int qt_execute() {

    QtConcurrent::run(&app_loop);
    return app_ptr->exec();
}
