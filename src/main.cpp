#include <Arduino.h>

// Configuracion inicial del sistema
// Configuracion de lectura de boton usando PULLUP
// Implementacion de sistema de antirrebote
// Declaracion de tipos de datos que emitira el boton
// Declaracion de los estados del sistema
// Registro serial de cada transicion
// Temporizador de respaldo de 8 s
// Ignicion de 1000 ms
// Mejorar interaccion con monitor serie

int const boton = 2;

int estadoBoton = HIGH;
int ultimalectura = HIGH;
int lectura = HIGH;
const int ignicion = 4;

unsigned long tiempoinicial = 0;
unsigned long tiempofinal = 0;
unsigned long duracion = 0;
unsigned long tiempoRebote = 0;
const unsigned long antirrebote=50;
unsigned long tiempoInicioAscenso = 0;

//variables para el LED de ignicion

unsigned long tiempoInicioIgnicion = 0;
bool ignicionactiva = false;
const unsigned long tiempoIgnicion = 1000;

// Declaracion de los LEDS para cada estado
const int ledIdle = 5;
const int ledArmed = 6;
const int ledAscent = 7;
const int ledDeployment = 8;
const int ledLanded = 9;

// Funcion para clasificar los estados del sistema
enum EstadoSistema{
    IDLE,
    ARMED,
    ASCENT,
    DEPLOYMENT,
    LANDED
};

EstadoSistema estadoActual = IDLE;


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



// Funcion para registro de eventos y transiciones
const char* nombreEstado(EstadoSistema estado){
    switch (estado){
        case IDLE:
        return "IDLE";
        
        case ARMED:
        return "ARMED";
    
        case ASCENT:
        return "ASCENT";
        
        case DEPLOYMENT:
        return "DEPLOYMENT";
    
        case LANDED:
        return "LANDED";
        
        default:
        return "DESCONOCIDO";
    
    }
}

// funcion para hacer el seguimiento adecuado en monitor serial
// actualiza el estado
void cambiarEstado (EstadoSistema nuevoEstado){
    EstadoSistema estadoAnterior = estadoActual;
    estadoActual = nuevoEstado;

    if(estadoActual == ASCENT){
        tiempoInicioAscenso = millis();
    }

    if (estadoActual == DEPLOYMENT){
        tiempoInicioIgnicion = millis();
        digitalWrite(ignicion,HIGH);
        ignicionactiva  =true;
    }

    Serial.print("[");
    Serial.print(millis());
    Serial.print("ms]");

    Serial.print(nombreEstado (estadoAnterior));
    Serial.print("->");
    Serial.println(nombreEstado (estadoActual));
}

const char* nombreEvento(EventoBoton evento){
    switch (evento){
        case PULSACION_CORTA:
        return "Pulsacion corta";

        case PULSACION_LARGA:
        return "Pulsacion larga";

        case NINGUNO:
        return("Pulsacion no clasificada");

        default:
        return "DESCONOCIDO";
    }
}

// clasificamos los eventos y definimos el estado
void clasificarEvento(EventoBoton evento){
    Serial.print("[");
    Serial.print(millis());
    Serial.print("ms] Evento recibido: ");
    Serial.println(nombreEvento(evento));

    bool eventoProcesado = false;
    switch(estadoActual){
        case IDLE:
        switch(evento){
            case PULSACION_LARGA:
            cambiarEstado(ARMED);
            eventoProcesado = true;
            break;

            default: break;
        } break;

        case ARMED:
        switch(evento){
            case PULSACION_LARGA:
            cambiarEstado(IDLE);
            eventoProcesado = true;
            break;
            case PULSACION_CORTA:
            cambiarEstado(ASCENT);
            eventoProcesado = true;
            break;
            default: break;
        }break;

        case ASCENT:
        switch(evento){
            case PULSACION_CORTA:
            cambiarEstado(DEPLOYMENT);
            eventoProcesado = true;
            break;
            default: break;
        }break;

        case DEPLOYMENT:
        switch(evento){
            case PULSACION_CORTA:
            cambiarEstado(LANDED);
            eventoProcesado = true;
            break;
            default: break;
        }break;

        case LANDED:
        break;
        default: break;
    }

    if(eventoProcesado== false){
        Serial.print("[");
        Serial.print(millis());
        Serial.print("ms] Evento ignorado: ");
        Serial.print(nombreEvento(evento));
        Serial.print(" en");
        Serial.println(nombreEstado(estadoActual));
    }
}

void actulizarIgnicion (){
    if(ignicionactiva == true){
        if(millis()-tiempoInicioIgnicion >= tiempoIgnicion){
            digitalWrite(ignicion,LOW);
            ignicionactiva = false;
        }
    }

}

//funcion de actualizacion de los LEDS
void apagarLed (){
digitalWrite(ledIdle,LOW);
digitalWrite(ledArmed,LOW);
digitalWrite(ledAscent,LOW);
digitalWrite(ledDeployment,LOW);
digitalWrite(ledLanded,LOW);

}

void actualizarLed(){
    apagarLed();
    switch(estadoActual){
        case IDLE:
        digitalWrite(ledIdle,HIGH);
        break;

        case ARMED:
        digitalWrite(ledArmed,HIGH);
        break;

        case ASCENT:
        digitalWrite(ledAscent,HIGH);
        break;

        case DEPLOYMENT:
        digitalWrite(ledDeployment,HIGH);
        break;
        
        case LANDED:
        digitalWrite(ledLanded,HIGH);
        break;

        default: break;
}
}



void setup() {

    Serial.begin(9600);

    Serial.println("Sistema de recuperacion AAIAQ");
    Serial.println("Inicializando firmware...");

    pinMode(boton,INPUT_PULLUP); // Declaramos el boton como entrada y en configuracion Pull Up
    pinMode(ignicion,OUTPUT); // Led indicador de ignicion
    digitalWrite(ignicion,LOW);

    // configuracion de las salidas de los LEDS
    pinMode(ledIdle,OUTPUT);
    pinMode(ledArmed,OUTPUT);
    pinMode(ledAscent,OUTPUT);
    pinMode(ledDeployment,OUTPUT);
    pinMode(ledLanded,OUTPUT);
    actualizarLed();
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
                
                clasificarEvento(evento);
            }
        }
    }

    
    ultimalectura = lectura;
    // comprobacion de que hayan transcurridpo 8s de ASCENT
    if(estadoActual == ASCENT){ 
        if(millis()-tiempoInicioAscenso >= 8000){
            cambiarEstado(DEPLOYMENT);
        }
    }
    actulizarIgnicion();
    actualizarLed();


}