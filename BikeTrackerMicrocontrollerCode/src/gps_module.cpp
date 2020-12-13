#include "gps_module.h"
#include "OS/OSSignalKernel.h"
#include <TinyGPS.h>

/*!
*   @brief For Whatever Reason UART2 isn't properly defined, so I needed to create an instance of it here. 
*/

/*!
*   @brief Signal that spinlocks threads until the gps module has been initiated. 
*/
static OSSignal wait_gps_module; 

/*!
*   @brief stack space for gps thread
*/
static uint8_t gps_stack_space[512]; 

/*!
*   @brief Manipulation that handles all of our GPS stuff
*/
TinyGPS gps;

/*!
*   @brief Configuration and object for Serial3 object, so we can use the 3rd Serial peripheral on the device
*/
HardwareSerial Serial3(USART3);

/*!
*   @brief Function declarations
*/
static void gps_thread(void *parameters); 
static void gps_loop(void); 
void start_gps_module(void);
void spinlock_gps(void); 
void sleep_gps(void); 
void wake_gps(void);

/*!
*   @brief Thread function that directly interfaces with the GPS module
*/
static void gps_thread(void *parameters){
    Serial3.begin(115200);

    wait_gps_module.signal(THREAD_SIGNAL_0); 
    for(;;){
        gps_loop(); 
    }
}

static void gps_loop(void){
    bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (Serial3.available())
    {
      char c = Serial3.read();
      // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }

  if (newData)
  {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    Serial.print("LAT=");
    Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    Serial.print(" LON=");
    Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    Serial.print(" SAT=");
    Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    Serial.print(" PREC=");
    Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
  }
  
  gps.stats(&chars, &sentences, &failed);
  Serial.print(" CHARS=");
  Serial.print(chars);
  Serial.print(" SENTENCES=");
  Serial.print(sentences);
  Serial.print(" CSUM ERR=");
  Serial.println(failed);
  if (chars == 0)
    Serial.println("** No characters received from GPS: check wiring **");
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