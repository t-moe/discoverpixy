/**
 * @defgroup app Application
 * The App Module contains the effective, platform independent application.
 */


/**
 * @addtogroup app
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

/**
 * @defgroup screens Screens
 * The Screens of the application. \sa Screen
 */


/*@}*/
