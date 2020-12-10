#include "location_module.h"

/*!
*   @brief stack space for gps thread
*/
static uint8_t location_stack_space[512]; 

/*!
*   @brief Function declarations
*/
static void location_module_thread(void *parameters); 
void start_location_module(void); 

/*!
*   @brief Thread function that handles all our location processing stuff
*/
static void location_module_thread(void *parameters){
    for(;;){
        os_thread_sleep_ms(100);
    }
}

/*!
*   @brief Call from main function, starts up location module
*/
void start_location_module(void){
    os_add_thread(&location_module_thread, NULL, sizeof(location_stack_space), location_stack_space); 
}