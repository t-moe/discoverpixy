/**************************************************************************************************************************************
* Project:       discoverpixy
* Website:       https://github.com/t-moe/discoverpixy
* Authors:       Aaron Schmocker, Timo Lang
* Institution:   BFH Bern University of Applied Sciences
* File:          common/app/app.h
*
* Version History:
* Date			Autor Email			SHA		Changes
* 2015-04-03	timolang@gmail.com	51089aa	Refactored Project Structure for use with emulator
* 2015-05-10	timolang@gmail.com	21edc56	Added doxyfile (doxygen) for the common folder. Started with doxygen comments for app and tft module.
* 2015-05-11	timolang@gmail.com	08d9fe0	More work on doxygen module structure
* 2015-06-08	timolang@gmail.com	73db8b5	Added doxygen mainpage comment in app.h
*
**************************************************************************************************************************************/

#ifndef APP_H
#define APP_H

/*!
   \mainpage discoverpixy Doxygen Documentation
   Welcome to the code-documentation for all common (and plattformindependent) code. \n
   A good point to start is probably the <a href="modules.html">Modules</a> page.
 */

/**
 * @defgroup app Application
 * The App Module contains the effective, platform independent application.
 */
/*@{*/

/**
 * Starts/Initializes the app
 * This function should be called at the top of the main function of your platform
 */
void app_init();

/**
 * Executes one cycle of the app
 * Call this function repeatedly from a loop inside the main function
 */
void app_process();


/*@}*/

#endif /* APP_H */
