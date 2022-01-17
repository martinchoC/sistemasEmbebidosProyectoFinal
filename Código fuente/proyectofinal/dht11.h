#include "DHT.h"

#define DHTTYPE DHT11   // DHT 11

void dht_init(volatile uint8_t pin);

float leer_sensacion_termica();
float leer_porcentaje_humedad_ambiente();
float leer_temperatura_ambiente();
