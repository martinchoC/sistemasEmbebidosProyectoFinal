#include "dht11.h"

// Inicializacion del sensor
DHT *dht;

void dht_init(volatile uint8_t pin)
{
  dht = new DHT(pin,DHT11);
  dht->begin();

}

float leer_sensacion_termica(){
  float temperatura = leer_temperatura_ambiente();
  float humedad = leer_porcentaje_humedad_ambiente();
  if (isnan(temperatura) || isnan(humedad)) {
    Serial.println(F("Falla en la lectura del dht11"));
    return -150;
  }

  // Calculo de la temperatura en grados Celsius
  float hic = dht->computeHeatIndex(temperatura, humedad, false);
  return hic;
}

float leer_porcentaje_humedad_ambiente(){
  float humedad = dht->readHumidity();
  if (isnan(humedad)) {
    Serial.println(F("Falla en la lectura del dht11"));
    return -1;
  }
  return humedad;
}

float leer_temperatura_ambiente(){
  float temperatura = dht->readTemperature();
  if (isnan(temperatura)) {
    Serial.println(F("Falla en la lectura del dht11"));
    return -150;
  }

  return temperatura;
}
