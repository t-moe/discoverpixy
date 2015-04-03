#include <QApplication>
#include <QtConcurrent/QtConcurrent>


extern "C" {
    void qt_init(int argc, char* argv[]); //Will be called at the top of the main() function
    int qt_execute(); //Will be called after calling qt_execute
    void app_init(); //Initializes the app
    void app_process(); //Processes one eventloop of the app
}

QApplication* app_ptr=NULL;

void qt_init(int argc, char* argv[]) {
    app_ptr = new QApplication(argc,argv);
    app_init();
}

void app_loop() {
    while(QApplication::activeWindow()!=NULL) {
        app_process();
        QApplication::processEvents();
    }
}

int qt_execute() {
    QtConcurrent::run(&app_loop);
    int ret= app_ptr->exec();
    delete app_ptr;
    return ret;
}
