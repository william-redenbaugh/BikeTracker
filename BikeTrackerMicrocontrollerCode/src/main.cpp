#include <Arduino.h>
#include "OS/OSThreadKernel.h"

#include "gps_module.h"
#include "gsm_module.h"
#include "location_module.h"

void setup() {
  os_init();

  _os_yield();
}

// We Generally don't use this all too often
void loop() {}