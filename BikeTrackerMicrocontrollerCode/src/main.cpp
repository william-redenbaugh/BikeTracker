#include <Arduino.h>
#include "OS/OSThreadKernel.h"

#include "gps_module.h"
#include "gsm_module.h"
#include "location_module.h"
#include "imu_module.h"

void setup() {
  os_init();
  //start_gps_module(); 
  start_imu_module(); 
  //start_gsm_module(); 
  start_location_module(); 

  _os_yield();
}

// We Generally don't use this all too often
void loop() {}