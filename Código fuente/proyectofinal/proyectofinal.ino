#include <Arduino.h>
#include <WiFi.h>
#include "time.h"
#include "driver_firebase.h"
#include "drivertimer.h"
#include "dht11.h"
#include "fnqueue.h"

// Credenciales para la conexion a internet
#define WIFI_SSID "INSERTAR NOMBRE DE LA RED WIFI"
#define WIFI_PASSWORD "INSERTAR LA CONTRASEÑA"

// Pines para los sensores ANALOGICOS
#define PIN_LUMINOSIDAD_AMBIENTE 35
#define PIN_HUMEDAD_SUELO 34

// Pines para los relays
#define PIN_RELAY_LUZ 25
#define PIN_RELAY_BOMBA 26

// Pines para los sensores DIGITALES
#define PIN_DTH11 4

// Parámetros para obtener la fecha y hora
const char* ntp_server = "south-america.pool.ntp.org";
const long  gmt_offset_sec = -10800;
const int   daylight_offset_sec = 3600;
char dia[35];
struct tm timeinfo; 

// Parametros del DHT11
uint8_t porcentaje_humedad_ambiente;
float temperatura_ambiente;
float sensacion_termica;

volatile uint8_t segundos = 0;
volatile uint8_t segundos_riego = 0;

// Riego del sistema
// Se setean valores por defecto en caso que la conexion 
// con firebase para obtenerlos falle
int8_t porcentaje_humedad_suelo_minima = 10;
int8_t porcentaje_humedad_aire_minima = 30;
int8_t porcentaje_humedad_suelo_maxima = 60;
float temperatura_maxima = 35;
volatile bool esta_regando = false;

// Parámetro para lámpara
volatile bool encender_lampara;

// Parámetro para el sensor de humedad del suelo
uint8_t porcentaje_humedad_suelo;

// Configuracion de Firebase
struct firebase_config firebase_cfg;

// Declaración de funciones
void iniciar_relays();
void recuperar_parametros_desde_firebase();
void funcion_timer();
void setLocalTime();
bool controlar_luminosidad_ambiente();
void leer_humedad_suelo();
void controlar_riego();
void regar_falta_humedad_suelo();
void regar_falta_humedad_aire();
void regar_mucha_temperatura();
void finalizar_riego();

void conectar_red_wifi(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Conectando al Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.println("Aguarda un momento...");
    delay(300);
  }
  Serial.println();
  Serial.print("Conectado con la IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void iniciar_conexion_firebase(){
   firebase_cfg.API_KEY="AIzaSyAW4KUXA99rgutn9Li0VIULRyrd9MEOUAg";
   firebase_cfg.DATABASE_URL="https://proyectofinalembebidos-3b631-default-rtdb.firebaseio.com/";
   firebase_driver_init(firebase_cfg);
   
}

void iniciar_timer() {
  timer_init(&funcion_timer,1000000);
  timer_start();
}

void setup(){
  Serial.begin(115200);
  fnqueue_init();
  conectar_red_wifi();
  iniciar_conexion_firebase();
  recuperar_parametros_desde_firebase();
  iniciar_relays();
  dht_init(PIN_DTH11);
  iniciar_timer();
}

void loop(){
  fnqueue_run(); 
}

void mostrar_metricas()
{
  Serial.print("humedad aire " );
  Serial.println(porcentaje_humedad_aire_minima);
  Serial.print("humedad suelo minima: ");
  Serial.println(porcentaje_humedad_suelo_minima);
  Serial.print("Humedad suelo maxima: ");
  Serial.println(porcentaje_humedad_suelo_maxima);
  Serial.print("temperatura maxima: ");
  Serial.println(temperatura_maxima);
}

void recuperar_parametros_desde_firebase(){
  int aux_porcentaje_humedad_aire_minima = porcentaje_humedad_aire_minima;
  int aux_porcentaje_humedad_suelo_minima = porcentaje_humedad_suelo_minima;
  int aux_porcentaje_humedad_suelo_maxima = porcentaje_humedad_suelo_maxima;
  getFirebaseDataInt("parametros/humedad_ambiente_minima",aux_porcentaje_humedad_aire_minima);
  getFirebaseDataInt("parametros/humedad_suelo_minima",aux_porcentaje_humedad_suelo_minima);
  getFirebaseDataInt("parametros/humedad_suelo_maxima",aux_porcentaje_humedad_suelo_maxima);
  getFirebaseDataFloat("parametros/temperatura_maxima",temperatura_maxima);
  porcentaje_humedad_aire_minima = (uint8_t)aux_porcentaje_humedad_aire_minima;
  porcentaje_humedad_suelo_minima = (uint8_t)aux_porcentaje_humedad_suelo_minima;
  porcentaje_humedad_suelo_maxima = (uint8_t)aux_porcentaje_humedad_suelo_maxima;
  mostrar_metricas();
}

void iniciar_relays() {
  pinMode(PIN_RELAY_LUZ, OUTPUT);
  pinMode(PIN_RELAY_BOMBA, OUTPUT);
}


char * recuperar_estado_lampara()
{
  if(encender_lampara)
     return "ENCENDIDA";
  else
    return "APAGADA";
}

char * recuperar_estado_bomba()
{
   if(esta_regando)
     return "ENCENDIDA";
  else
    return "APAGADA";
}

void enviar_informacion_a_firebase(){
  int porcentaje_humedad_suelo_aux = porcentaje_humedad_suelo;
  char* estado_lampara = recuperar_estado_lampara();
  char* estado_bomba = recuperar_estado_bomba();   
  postFirebase("data/porcentaje_humedad_ambiente",&porcentaje_humedad_ambiente,FIREBASE_TYPE_INT); 
  postFirebase("data/temperatura_ambiente",&temperatura_ambiente,FIREBASE_TYPE_FLOAT); 
  postFirebase("data/sensacion_termica",&sensacion_termica,FIREBASE_TYPE_FLOAT); 
  postFirebase("data/humedad_suelo",&porcentaje_humedad_suelo_aux,FIREBASE_TYPE_INT); 
  postFirebase("data/diahora",dia,FIREBASE_TYPE_STRING);
  postFirebase("data/luz",estado_lampara,FIREBASE_TYPE_STRING);
  postFirebase("data/bomba",estado_bomba,FIREBASE_TYPE_STRING);
}

void tomar_medidas()
{
  tomar_dht11();
  tomar_luminosidad();
  tomar_humedad_suelo();
}

void controlar_segundos_riego()
{
   if (segundos_riego > 0)
    segundos_riego --;
  else
    controlar_riego();
}

void funcion_timer(){
  Serial.println("Iniciando funcion timer");
  tomar_medidas();
  controlar_segundos_riego();
  segundos++;
  if(segundos>=5){
   set_dia_hora();
   enviar_informacion_a_firebase();
   recuperar_parametros_desde_firebase();
   segundos=0;
  }
  Serial.println("Finalizando funcion timer");
}

void tomar_humedad_ambiente()
{
  uint8_t humedad_ambiente_aux = (uint8_t) leer_porcentaje_humedad_ambiente();
  if(humedad_ambiente_aux>0 && humedad_ambiente_aux<=100)
    porcentaje_humedad_ambiente = humedad_ambiente_aux;
}

void tomar_temperatura_ambiente()
{
  float temperatura_ambiente_aux = leer_temperatura_ambiente();
  if(temperatura_ambiente_aux>=0 && temperatura_ambiente_aux<=50)
    temperatura_ambiente = temperatura_ambiente_aux;
}

void tomar_sensacion_termica()
{
  float sensacion_termica_aux = leer_sensacion_termica();
  if(sensacion_termica_aux>=0 && sensacion_termica_aux<=60)
    sensacion_termica = sensacion_termica_aux;
}

void tomar_dht11(){
  tomar_humedad_ambiente();
  tomar_temperatura_ambiente();
  tomar_sensacion_termica();
  Serial.print(F("Humidity: "));
  Serial.print(porcentaje_humedad_ambiente);
  Serial.print(F("%  Temperature: "));
  Serial.print(temperatura_ambiente);
  Serial.print(F("°C "));
  Serial.print(F("%  Sensacion termica: "));
  Serial.print(sensacion_termica);
  Serial.println(F("°C "));
}

void tomar_luminosidad(){
  encender_lampara = controlar_luminosidad_ambiente();
  if(encender_lampara)
  Serial.println("Encender lampara");
  else
    Serial.println("Apagar lampara");
}

void tomar_humedad_suelo(){
  leer_humedad_suelo();
  Serial.print("Humedad del suelo: ");
  Serial.print(porcentaje_humedad_suelo);
  Serial.println("%");
}

void set_dia_hora(){
   setLocalTime();
   strftime(dia,35,"%d/%m/%Y %T",&timeinfo);
}

void setLocalTime(){
  
  configTime(gmt_offset_sec, daylight_offset_sec, ntp_server);
  if(!getLocalTime(&timeinfo)){
    Serial.println("Error para obtener la fecha/hora");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

bool controlar_luminosidad_ambiente() {
  int16_t lectura_luminosidad = analogRead(PIN_LUMINOSIDAD_AMBIENTE);
  Serial.print("LUMINOSIDAD: ");
  Serial.println(lectura_luminosidad);
  if (lectura_luminosidad < 2800) {
    digitalWrite(PIN_RELAY_LUZ,HIGH);
    return true;
  }else{
    digitalWrite(PIN_RELAY_LUZ,LOW);
    return false;
  }
}

void leer_humedad_suelo() {
  uint16_t valor_analogico= analogRead(PIN_HUMEDAD_SUELO);
  porcentaje_humedad_suelo = map(valor_analogico,1620,4095,100,0);
  if(porcentaje_humedad_suelo > 100){
    porcentaje_humedad_suelo = 100;
  }
}

void regar(uint8_t segundos)
{
   segundos_riego = segundos;
   digitalWrite(PIN_RELAY_BOMBA,HIGH);
   esta_regando = true;
   Serial.println("Regando...");
}

bool hay_que_regar()
{
  if (porcentaje_humedad_suelo < porcentaje_humedad_suelo_minima)
    return true;
  if(temperatura_ambiente > temperatura_maxima && porcentaje_humedad_suelo < porcentaje_humedad_suelo_maxima)
    return true;
  if(temperatura_ambiente > temperatura_maxima && porcentaje_humedad_suelo < porcentaje_humedad_suelo_maxima)
    return true;
  return false;
}

void controlar_riego() {
  if(hay_que_regar())
    regar(4);
  else
    finalizar_riego();
}

void finalizar_riego() {
  if (segundos_riego == 0 && esta_regando) {
    digitalWrite(PIN_RELAY_BOMBA,LOW);
    esta_regando = false;
    Serial.println("Dejando de regar...");
  }
}
