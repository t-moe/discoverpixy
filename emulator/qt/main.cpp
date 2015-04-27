#include <QApplication>
#include <QtConcurrent/QtConcurrent>

extern "C" {
    //C Functions from the common folder
    void app_init(); //Initializes the app
    void app_process(); //Processes one eventloop of the app
}

void app_loop() {
    while(!QApplication::closingDown()) {
        app_process();
    }
}

int main(int argc, char* argv[]) {
    QApplication app(argc,argv);
    app_init();

    QtConcurrent::run(&app_loop);
    return app.exec();
}



