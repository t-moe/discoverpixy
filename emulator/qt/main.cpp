#include <QApplication>
#include <QtConcurrent/QtConcurrent>

extern "C" {
    //Imported C Functions from the common folder
    void app_init(); //Initializes the app
    void app_process(); //Processes one eventloop of the app
}

void app_loop() {
    while(!QApplication::closingDown()) { //as long as the application is not terminating
        app_process(); //let the application process it's events
    }
}

int main(int argc, char* argv[]) {
    QApplication app(argc,argv); //Process qt-specific commandline arguments and create event loop
    app_init(); //Let the application initialize it self

    QtConcurrent::run(&app_loop); //Start a thread that executes app_loop
    return app.exec(); //Run the event loop until the last window is closed.
}



