#include <Arduino.h>
#include <control_horarios.h>
#include <sensor.h>
#include <memoria_SD.h>
#include <Servo.h>
#include <RTClib.h> //reloj
#include <SPI.h>
#include <Wire.h>

#define botonPin 6
#define servoPin 8

Servo servo; // objeto del motor
RTC_DS3231 RTC; // Objeto del modulo Reloj
String datos;
int hora, minuto;

const unsigned long IntervaloServo = 2000;  // Tiempo del servo abierto (2s)
const unsigned long IntervaloMensaje = 2000; // Intervalo para enviar "vacio"
const unsigned long IntervaloSensor = 500; // Intervalo para medir el sensor

unsigned long tiempoAnteriorSensor = 0;
unsigned long tiempoAnteriorMensaje = 0;
unsigned long tiempoInicioServo = 0;

void setup() {
    Serial.begin(9600);
    inicializaMemoriaSD();
    inicializaSensor();
    Wire.begin();
    RTC.begin(); 
    RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
    servo.attach(servoPin);  // inicializar pin del servo
    servo.write(0);          // inicializar el servo en posición 0
    pinMode(botonPin, INPUT); // inicializar el botón
}
DateTime horaRTC(){ 
    DateTime now = RTC.now();
    return now;
}
// Función para activar el servo
void activarServo() {
    servo.write(90);  // Abre el servo
    tiempoInicioServo = millis();

    DateTime now = horaRTC();
    guardarHistorial(now.hour(), now.minute(), now.day(), now.month(), now.year());
}
void loop() {
    bool dispenseManual = (digitalRead(botonPin) == HIGH);
    
    DateTime now = horaRTC();
    String horaActual = obtenerHoraActual(now.hour(), now.minute(), now.second());
    leerHorarios();

    if (dispenseManual && servo.read() == 0) { // Si el botón está presionado y el servo está cerrado
        activarServo();
        Serial.println(horaActual);
    }
    
    if (compararHorarios(horaActual) && servo.read() == 0) { // Si es hora programada y el servo está cerrado
        activarServo();
    }
    
    if (Serial.available() > 0) {
        datos = Serial.readString(); // Lee los datos enviados por Serial

        if (datos == "1" && servo.read() == 0) { // Desde la app manda un 1 y el servo está cerrado
            activarServo();
        } else if (datos.startsWith("<")) {
            datos.remove(0, 1); // Elimina el primer carácter '<'
            hora = (datos.toInt()); // Convierte el valor antes de la coma a un entero
            datos.remove(0, (datos.indexOf(",") + 1)); // Elimina todo hasta y incluyendo la coma
            minuto = (datos.toInt());
            datos.remove(0, (datos.indexOf(">") + 1));
            guardarHorario(hora, minuto);
        } else if (datos == "TXT1") {
            enviarHorario();
        } else if (datos == "TXT2") {
            enviarHistorial();
        }
    }

    // Control del servo (cerrarlo después de cierto tiempo)
    unsigned long tiempoActual = millis();
    if (servo.read() == 90 && (tiempoActual - tiempoInicioServo >= IntervaloServo)) {
        servo.write(0);  // Cerrar el servo después del intervalo
    }

    // --- Lógica del sensor al final del loop ---
    if (tiempoActual - tiempoAnteriorSensor >= IntervaloSensor) {
        tiempoAnteriorSensor = tiempoActual;

        float distancia = medidadeSensor();
        // Enviar mensaje "vacio" si es necesario
        if (distancia >= 25.0) { // Si la distancia es mayor o igual a 25 cm
            if (tiempoAnteriorMensaje == 0 || tiempoActual - tiempoAnteriorMensaje >= 60000) { // 1 minuto o primer mensaje
                Serial.println("vacio");
                tiempoAnteriorMensaje = tiempoActual; // Actualiza el tiempo del último mensaje "vacio"
            }
        } else {
            // Reinicia el temporizador si la distancia es menor a 25 cm
            tiempoAnteriorMensaje = 0; // Reinicia para permitir el primer mensaje inmediatamente
        }

    }
}



