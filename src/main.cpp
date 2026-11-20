
#include <Arduino.h>

// Configuracion inicial del sistema
// Configuracion de lectura de boton usando PULLUP
// Implementacion de sistema de antirrebote
// Declaracion de tipos de datos que emitira el boton

int const boton = 2;

int estadoBoton = HIGH;
int ultimalectura = HIGH;
int lectura = HIGH;

unsigned long tiempoinicial = 0;
unsigned long tiempofinal = 0;
unsigned long duracion = 0;
unsigned long tiempoRebote = 0;
const unsigned long antirrebote=50;

enum EventoBoton {
    NINGUNO,
    PULSACION_CORTA,
    PULSACION_LARGA
};

// funcion para clasificar la pulsacion del boton (mas adelante se llamará a la funcion
EventoBoton clasificarPulsacion (unsigned long duracion){
    if (duracion < 1000){
        return PULSACION_CORTA;
    }
    if (duracion >= 2000){
        return PULSACION_LARGA;
    }
    else{
        return NINGUNO;
    }
}

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
                
                EventoBoton evento = clasificarPulsacion(duracion);
                switch(evento){

                    case PULSACION_CORTA:
                    Serial.println("PULSACION CORTA");
                    break;

                    case PULSACION_LARGA:
                    Serial.println("PULSACION LARGA");
                    break;

                    case NINGUNO:
                    Serial.println("EVENTO IGNORADO");
                    break;
                }
            }
        }
    }

    
    ultimalectura = lectura;


}
