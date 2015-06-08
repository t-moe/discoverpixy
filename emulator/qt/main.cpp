/**************************************************************************************************************************************
* Project:       discoverpixy
* Website:       https://github.com/t-moe/discoverpixy
* Authors:       Aaron Schmocker, Timo Lang
* Institution:   BFH Bern University of Applied Sciences
* File:          emulator/qt/main.cpp
*
* Version History:
* Date			Autor Email			SHA		Changes
* 2015-04-03	timolang@gmail.com	51089aa	Refactored Project Structure for use with emulator
* 2015-04-03	timolang@gmail.com	1f2af9f	Added more tft functions to common and emulator. Fixed eventloop.
* 2015-04-03	timolang@gmail.com	cab8609	Integrated pixy into emulator. Pixy is no longer in the common/libs folder but in emulator/libs and discovery/libs
* 2015-04-07	timolang@gmail.com	383fc86	Simplified Emulator (main)
* 2015-04-25	timolang@gmail.com	3d1e4b2	Simplified code a bit. Emulator does not work stable when replugging pixy.
* 2015-06-07	timolang@gmail.com	4b5768c	Improved Comments in whole emulator. Finalized emulator section in docu.
*
**************************************************************************************************************************************/

#include <QApplication>
#include <QtConcurrent/QtConcurrent>

extern "C" {
    //Imported C Functions from the common folder
    void app_init(); //Initializes the app
    void app_process(); //Processes one eventloop of the app
}

void app_loop()
{
    while (!QApplication::closingDown()) { //as long as the application is not terminating
        app_process(); //let the application process it's events
    }
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv); //Process qt-specific commandline arguments and create event loop
    app_init(); //Let the application initialize it self

    QtConcurrent::run(&app_loop); //Start a thread that executes app_loop
    return app.exec(); //Run the event loop until the last window is closed.
}
