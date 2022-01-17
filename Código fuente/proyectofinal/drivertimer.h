#include <stdint.h>

void timer_init(void (*function)(), volatile uint32_t microsegundos);
void timer_start(void);
void timer_stop(void);
