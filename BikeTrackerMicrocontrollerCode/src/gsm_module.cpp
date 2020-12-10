#include "gsm_module.h"
#include "SIM800L.h"
#include "OS/OSSignalKernel.h"

// SIM Module handler
static SIM800L *gsm_module; 

// Allocating our stack space for the thread. 
static uint8_t gsm_thread_stack_space[2048];

// Reset pin for our gsm module
#define SIM800_RST_PIN 6

// Signal that helps with waiting for GSM module to start up!
static OSSignal wait_gsm_ready; 

/*!
*   @brief Function declarations
*/
static void gsm_thread(void *parameters); 
void start_gsm_module(void); 
void spinlock_gsm(void); 
void sleep_gsm_module(void); 
void wake_gsm_module(void); 

/*!
*   @brief Thread function that handles all of our gsm processing stuff. 
*/
static void gsm_thread(void *parameters){
    wait_gsm_ready.clear(THREAD_SIGNAL_0); 
    Serial1.begin(9600);
    gsm_module = new SIM800L((Stream *)&Serial1, SIM800_RST_PIN, 200, 512);

    // Wait until the module is ready to accept AT commands
    // Checks every second and sleeps until it's ready. 
    while(!gsm_module->isReady())
        os_thread_sleep_ms(1000);

    // Once the gsm module is ready, we signal the flag
    wait_gsm_ready.signal(THREAD_SIGNAL_0);

    for(;;){
        os_thread_sleep_ms(100);
    }
}

/*!
*   @brief Call from main function, starts up gsm module. 
*/
void start_gsm_module(void){
    // Generate gsm thread
    os_add_thread(&gsm_thread, NULL, sizeof(gsm_thread_stack_space), gsm_thread_stack_space); 
}

/*!
*   @brief Blocks a thread until the gsm module is ready. 
*/
void spinlock_gsm(void){
    wait_gsm_ready.wait_notimeout(THREAD_SIGNAL_0); 
} 