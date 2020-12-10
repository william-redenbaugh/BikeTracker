#include "gps_module.h"
#include "OS/OSSignalKernel.h"

/*!
*   @brief For Whatever Reason UART2 isn't properly defined, so I needed to create an instance of it here. 
*/
HardwareSerial Serial2(USART2);

/*!
*   @brief Signal that spinlocks threads until the gps module has been initiated. 
*/
static OSSignal wait_gps_module; 

/*!
*   @brief stack space for gps thread
*/
static uint8_t gps_stack_space[512]; 

/*!
*   @brief Function declarations
*/
static void gps_thread(void *parameters); 
void start_gps_module(void);
void spinlock_gps(void); 
void sleep_gps(void); 
void wake_gps(void);

/*!
*   @brief Thread function that directly interfaces with the GPS module
*/
static void gps_thread(void *parameters){
    
    wait_gps_module.signal(THREAD_SIGNAL_0); 
    for(;;){
        os_thread_sleep_ms(100); 
    }
}

/*!
*   @brief Call from main function, starts up all subsystems to handle gps module. 
*/
void start_gps_module(void){
    wait_gps_module.clear(THREAD_SIGNAL_0); 
    os_add_thread(&gps_thread, NULL, sizeof(gps_stack_space), gps_stack_space); 
}

/*!
*   @brief Spinlocks a thread until the gps module has been initiated
*/
void spinlock_gps(void){
    wait_gps_module.wait_notimeout(THREAD_SIGNAL_0); 
}