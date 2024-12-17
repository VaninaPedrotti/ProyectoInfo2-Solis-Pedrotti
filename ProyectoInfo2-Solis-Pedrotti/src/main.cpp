#include <Arduino.h>

#include <Servo.h> 
//reloj
#include <RTClib.h> 
#include <SPI.h>
#include <Wire.h>

#include <control_horarios.h>
#include <sensor.h>
#include <memoria_SD.h>

#define botonPin 6
#define servoPin 8

Servo servo; // objeto del motor
RTC_DS3231 RTC; // Objeto del modulo Reloj

String datos;
int hora, minuto;

const unsigned long IntervaloServo = 2000;  // Tiempo del servo abierto (2s)
const unsigned long IntervaloMensaje = 60000; // Intervalo para entre cada mensaje "vacio" Si sigue vacio el dispenser
const unsigned long IntervaloSensor = 500; // Intervalo para medir el sensor

unsigned long tiempoAnteriorSensor = 0;
unsigned long tiempoAnteriorMensaje = 0;
unsigned long tiempoInicioServo = 0;

void setup() {
    Serial.begin(9600);

    inicializaMemoriaSD();
    inicializaSensor();
    
    RTC.begin(); //reloj
    Wire.begin(); //comunicacion I2C del reloj
    RTC.adjust(DateTime(F(__DATE__), F(__TIME__))); //ajusta la hora 

    servo.attach(servoPin);  // asigna pin del servo
    servo.write(0);          // inicializar el servo en posición 0
    pinMode(botonPin, INPUT); // inicializar el botón
}

// Función para activar el servo
void activarServo() {
    servo.write(90);  // Abre el servo
    tiempoInicioServo = millis();

    guardarHistorial(RTC.now().hour(), RTC.now().minute(), 
                    RTC.now().day(), RTC.now().month(), RTC.now().year());
}
void loop() {
    bool dispenseManual = (digitalRead(botonPin) == HIGH);
    
    String horaActual = obtenerHoraActual(RTC.now().hour(), RTC.now().minute(), RTC.now().second());
    leerHorarios();
    // --Dispensacion manual
    if (dispenseManual && servo.read() == 0) { // Si el botón está presionado y el servo está cerrado
        activarServo();
    }
    
    if (compararHorarios(horaActual) && servo.read() == 0) { // Si es hora programada y el servo está cerrado
        activarServo();
    }
    
    if (Serial.available() > 0) {
        datos = Serial.readString(); // Lee los datos enviados por Serial

        if (datos == "1" && servo.read() == 0) { // Desde la app manda un 1(boton dispensar) y el servo está cerrado
            activarServo();
        } else if (datos.startsWith("<")) { 
            datos.remove(0, 1); // Elimina el primer carácter '<'
            hora = (datos.toInt()); // Convierte el valor antes de la coma a un entero
            datos.remove(0, (datos.indexOf(",") + 1)); // Elimina todo hasta, incluyendo la coma
            minuto = (datos.toInt());
            datos.remove(0, (datos.indexOf(">") + 1));
            guardarHorario(hora, minuto);
        } else if (datos == "TXT1") {
            enviarHorario();
        } else if (datos == "TXT2") {
            enviarHistorial();
        }
    }

    // -- Control del servo -- 
    unsigned long tiempoActual = millis();
    if (servo.read() == 90 && (tiempoActual - tiempoInicioServo >= IntervaloServo)) {
        servo.write(0);  // Cerrar el servo después del intervalo
    }

    // --- Sensor  ---
    if (tiempoActual - tiempoAnteriorSensor >= IntervaloSensor) {
        tiempoAnteriorSensor = tiempoActual; // Actualiza el tiempo del ultimo sensor
        float distancia = medidadeSensor();
        if (distancia >= 15.0) { //Distancia al fondo del deposito de alimento
            if (tiempoAnteriorMensaje == 0 || tiempoActual - tiempoAnteriorMensaje >= IntervaloMensaje) { // 1 minuto o primer mensaje
                Serial.println("vacio");
                tiempoAnteriorMensaje = tiempoActual; // Actualiza el tiempo del último mensaje 
            }
        } else {
            tiempoAnteriorMensaje = 0; // Reinicia para permitir el primer mensaje inmediatamente
        }
    }
}



