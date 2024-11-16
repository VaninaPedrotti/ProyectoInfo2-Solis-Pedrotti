/**
 * @file dispenser.cpp
 * @author Pedrotti Vanina - Solis Milagros 
 * @brief 
 * @version 0.1
 * @date 2024-09-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <Arduino.h>
#include <dispenser.h>

#include <RTClib.h> //reloj
#include <Servo.h> //motor

RTC_DS3231 RTC; // objeto del modulo Reloj
Servo servo; //objeto del motor
int hora = 18;
int minuto = 34;
unsigned long millisPrevio = 0;
const unsigned long Intervalo = 4000;  // Tiempo del servo abierto

void InicializaDispenser(){
    RTC.begin(); 
    servo.attach(8);//motor pin 8
    pinMode(2, INPUT); //boton
    pinMode(3, INPUT);//sensor IR 
    RTC.adjust(DateTime(__DATE__, __TIME__));
}

void activaServo(){
    servo.write(90);
    millisPrevio = millis();
    // Aquí debemos guardar la fecha y hora del establecimiento en la memoria SD
    Serial.println("Activa servo");
}

void desactivaServo() {
    servo.write(0);
    Serial.println("desactivo servo");
}

void Dispenser() {
    DateTime now = RTC.now();
    bool dispenseEstablecido = (hora == now.hour() && minuto == now.minute());
    bool dispenseManual = (digitalRead(2) == HIGH); //boton 
    bool dispenseControl = (digitalRead(3) == LOW); //si

    if (dispenseEstablecido || dispenseManual || dispenseControl) {
        if (servo.read() != 90) {
            activaServo();
        }
    }
    
    // Control del tiempo del servo
    if (servo.read() == 90 && (millis() - millisPrevio) >= Intervalo) {
        desactivaServo();
    }
}
