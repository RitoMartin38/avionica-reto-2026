
#include <Arduino.h>

// Configuracion inicial del sistema
// Configuracion de lectura de boton usando PULLUP
// Implementacion de sistema de antirrebote
// Declaracion de tipos de datos que emitira el boton
// Declaracion de los estados del sistema

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

// Funcion para clasificar los estados del sistema
enum EstadoSistema{
    IDLE,
    ARMED,
    ASCENT,
    DEPLOYMENT,
    LANDED
};

EstadoSistema estadoActual = IDLE;

void clasificarEvento(EventoBoton evento){
    switch(estadoActual){
        case IDLE:
        switch(evento){
            case PULSACION_LARGA:
            estadoActual = ARMED;
            break;

            default: break;
        } break;

        case ARMED:
        switch(evento){
            case PULSACION_LARGA:
            estadoActual= IDLE;
            break;
            case PULSACION_CORTA:
            estadoActual=ASCENT;
            break;
            default: break;
        }break;

        case ASCENT:
        switch(evento){
            case PULSACION_CORTA:
            estadoActual=DEPLOYMENT;
            break;
            default: break;
        }break;

        case DEPLOYMENT:
        switch(evento){
            case PULSACION_CORTA:
            estadoActual  = LANDED;
            break;
            default: break;
        }break;

        case LANDED:
        break;
        default: break;
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
                
                if(evento != NINGUNO){
                    clasificarEvento(evento);
                    Serial.println("Estado Actual: ");
                    Serial.println((int)estadoActual);
                }
            }
        }
    }

    
    ultimalectura = lectura;


}
