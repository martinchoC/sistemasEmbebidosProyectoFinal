#include <Arduino.h>
#include "drivertimer.h"
#include "fnqueue.h"

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void (*timer_function)();

void IRAM_ATTR onTimer() 
{
  fnqueue_add(timer_function);
}

 void timer_init(void (*function)(), volatile uint32_t microsegundos)
{	
  timer = timerBegin(1, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, microsegundos, true);
  timer_function = function;
}
 
 void timer_start()
{
  timerAlarmEnable(timer);
}

void timer_stop()
{
    timerAlarmDisable(timer);    
}
