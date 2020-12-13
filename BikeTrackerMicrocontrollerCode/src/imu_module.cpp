#include "imu_module.h"
#include "OS/OSSignalKernel.h"
#include "HAL/MPU6050/mpu6050_imu.h"

/*!
*   @brief Signal that spinlocks threads until the imu module has been initiated. 
*/
static OSSignal wait_imu_module; 

/*!
*   @brief stack space for imu thread
*/
static uint8_t imu_stack_space[512]; 

/*!
*   @brief Function declarations
*/
static void imu_thread(void *parameters); 
void start_imu_module(void);
void spinlock_imu(void); 

/*!
*   @brief Thread function that directly interfaces with the imu module
*/
static void imu_thread(void *parameters){
    mpu_init_status_t init_status = init_mpu6050(MPU6050_DEFAULT_I2C_ADDR, ACCELEROMETER_16G, GYRO_500_DEGREE_SECCOND); 
    
    // If we are able to interface with the device successfully.  
    switch(init_status){
    case(MPU6050_INIT_SUCCESS):
        wait_imu_module.signal(THREAD_SIGNAL_0); 
        break; 
        
    default: 
        break; 
    }

    for(;;){
        os_thread_sleep_ms(100); 
    }
}

/*!
*   @brief Call from main function, starts up all subsystems to handle imu module. 
*/
void start_imu_module(void){
    wait_imu_module.clear(THREAD_SIGNAL_0); 
    os_add_thread(&imu_thread, NULL, sizeof(imu_stack_space), imu_stack_space); 
}

/*!
*   @brief Spinlocks a thread until the imu module has been initiated
*/
void spinlock_imu(void){
    wait_imu_module.wait_notimeout(THREAD_SIGNAL_0); 
}