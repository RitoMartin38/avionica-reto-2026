
#include <Arduino.h>

// Configuracion inicial del sistema
// Configuracion de lectura de boton usando PULLUP
// Implementacion de sistema de antirrebote

int const boton = 2;

int estadoBoton = HIGH;
int ultimalectura = HIGH;
int lectura = HIGH;

unsigned long tiempoinicial = 0;
unsigned long tiempofinal = 0;
unsigned long duracion = 0;
unsigned long tiempoRebote = 0;
const unsigned long antirrebote=50;

void setup() {

    Serial.begin(9600);

    Serial.println("Sistema de recuperacion AAIAQ");
    Serial.println("Inicializando firmware...");

    pinMode(boton,INPUT_PULLUP);
}


void loop() {
    lectura = digitalRead(boton);

    if(lectura != ultimalectura){
        tiempoRebote  = millis();
    }
    if(millis()-tiempoRebote >= antirrebote){
        if(lectura != estadoBoton){
            estadoBoton = lectura;
            if (estadoBoton == LOW){
                tiempoinicial = millis();
            }
            else{
                tiempofinal = millis();
                duracion = tiempofinal-tiempoinicial;
                Serial.println(duracion);
            }
        }
    }

    
    ultimalectura = lectura;


}
