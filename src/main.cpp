
#include <Arduino.h>

// Configuracion inicial del sistema
// Configuracion de lectura de boton usando PULLUP

int const boton = 2;

int estadoBoton = HIGH;
int estadoAnterior = HIGH;

unsigned long tiempoinicial = 0;
unsigned long tiempofinal = 0;
unsigned long duracion = 0;
void setup() {

    Serial.begin(9600);

    Serial.println("Sistema de recuperacion AAIAQ");
    Serial.println("Inicializando firmware...");

    pinMode(boton,INPUT_PULLUP);
}


void loop() {
    estadoBoton = digitalRead(boton);
    if(estadoAnterior == HIGH && estadoBoton == LOW){
        tiempoinicial = millis();
        Serial.println("Boton presionado");
    }

    if(estadoAnterior == LOW && estadoBoton == HIGH){
        tiempofinal = millis();
        duracion = tiempofinal-tiempoinicial;

        Serial.println(duracion);
    }

    // Se actualiza la lectura anterior

    estadoAnterior = estadoBoton;

    

}
