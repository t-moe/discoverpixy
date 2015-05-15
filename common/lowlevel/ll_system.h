#include <stdbool.h>
#include <stdint.h>


/**
 * @defgroup lowlevel LowLevel
 * The Low-Level platform abstraction layer
 */
/*@{*/

/**
 * @defgroup ll_system System (LowLevel)
 * Low level functions of the \ref system Module
 */

/*@}*/

/**
 * @addtogroup ll_system
 */
/*@{*/


bool ll_system_init();
void ll_system_delay(uint32_t msec);
void ll_system_process();
void ll_system_toggle_led();

/*@}*/
