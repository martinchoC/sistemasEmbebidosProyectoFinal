#include "driver_firebase.h"

#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;

void firebase_driver_init(firebase_config cfg)
{
  config.api_key = cfg.API_KEY;
  config.database_url = cfg.DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("Conectado a Firebase!");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback; 
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void mostrarPostExito()
{
   Serial.println("PASSED");
   Serial.println("PATH: " + fbdo.dataPath());
   Serial.println("TYPE: " + fbdo.dataType());
}

void mostrarPostFallo()
{
  Serial.println("FAILED");
  Serial.println("REASON: " + fbdo.errorReason());
}


void postFirebaseDataInt(char * key,int * value)
{
  if (Firebase.RTDB.setInt(&fbdo, key, *value))
      mostrarPostExito();
    else
      mostrarPostFallo(); 
}

void postFirebaseDataFloat(char * key,float * value)
{
  if (Firebase.RTDB.setFloat(&fbdo, key, *value))
      mostrarPostExito();
    else
      mostrarPostFallo(); 
}

void postFirebaseDataString(char * key,char* value)
{
  if (Firebase.RTDB.setString(&fbdo, key, value))
      mostrarPostExito();
    else
      mostrarPostFallo(); 
}

void postFirebase(char * key,void * value, uint8_t type){
  if (Firebase.ready() && signupOK){
    switch(type)
    {
        case 1: postFirebaseDataInt(key,(int*)value);
                break;
        case 2: postFirebaseDataFloat(key,(float*)value);
                break;
        case 3: postFirebaseDataString(key,(char*)value);
                break;
        default:break;
    }
  }
}

void getFirebaseDataInt(char * key,int &value)
{
  if (Firebase.RTDB.getInt(&fbdo, key))
      if(fbdo.dataType()=="int"){
        mostrarPostExito();
        value = fbdo.intData();
      }
    else
      mostrarPostFallo(); 
}

void getFirebaseDataFloat(char * key,float &value)
{
  if (Firebase.RTDB.getFloat(&fbdo, key))
      if(fbdo.dataType()=="float"){
        mostrarPostExito();
        value = fbdo.floatData();
      }
      else if(fbdo.dataType()=="int"){
        mostrarPostExito();
        value = (float)fbdo.intData();
      }
    else
      mostrarPostFallo(); 
}
