#include <Firebase_ESP_Client.h>

#define FIREBASE_TYPE_INT 1
#define FIREBASE_TYPE_FLOAT 2
#define FIREBASE_TYPE_STRING 3

struct firebase_config{
    char * API_KEY;
    char * DATABASE_URL;
};

void firebase_driver_init(firebase_config cfg);
void mostrarPostExito();
void mostrarPostFallo();
void postFirebaseDataInt(char * key,int * value);
void postFirebaseDataFloat(char * key,float * value);
void postFirebaseDataString(char * key,char* value);
void postFirebase(char * key,void * value, uint8_t type);
void getFirebaseDataInt(char * key,int &value);
void getFirebaseDataFloat(char * key,float &value);
//void getFirebaseDataString(char * key,char **value);
