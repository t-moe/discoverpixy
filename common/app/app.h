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
