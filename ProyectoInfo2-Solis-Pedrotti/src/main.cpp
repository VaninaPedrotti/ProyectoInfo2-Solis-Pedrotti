#include <Arduino.h>
#include <control_horarios.h>
#include <sensor.h>
#include <memoria_SD.h>

#include <Servo.h>

#define botonPin 6
#define servoPin 8

Servo servo; //objeto del motor

String datos;
int hora, minuto;
const unsigned int Intervalo = 2000;  // Tiempo del servo abierto (2s)

void setup() {
    Serial.begin(9600);
    inicializaControlHorarios();
    inicializaMemoriaSD();    
    inicializaSensor();

    servo.attach(servoPin);//inicializar pin del servo
    servo.write(0);//inicializar el servo en posicion 0
    pinMode(botonPin, INPUT); // inicializar el boton
}

void loop() {
    usodeSensor();
    DateTime now = horaRTC();
    String horaActual = obtenerHoraActual(now.hour(), now.minute());
    leerHorarios();
    
    bool dispenseProgramado = compararHorarios(horaActual);
    bool dispenseManual = (digitalRead(botonPin) == HIGH);

    if (dispenseProgramado || dispenseManual) {
        servo.write(90);
        delay(Intervalo);
        servo.write(0);
        guardarHistorial();
        delay(2000);
    } else if (Serial.available()>0) { //hay datos en el puerto serial para ser leidos
        datos = Serial.readString(); //lee y los guarda en datos
        if (datos == "1"){ //desde la app manda un 1 cuando se acciona el boton dispensar
            servo.write(90);
            delay(Intervalo);
            servo.write(0);
            guardarHistorial();
        } else if(datos.startsWith("<")){
            datos.remove(0, 1); // Elimina el primer carácter '<'
            hora = (datos.toInt()); // Convierte el valor antes de la coma a un entero
            datos.remove(0, (datos.indexOf(",")+1)); // Elimina todo hasta y incluyendo la coma
            minuto = (datos.toInt());
            datos.remove(0, (datos.indexOf(">")+1));
            guardarHorario(hora, minuto); 
        } else if (datos == "TXT1") {
            enviarArchivo("horarios.txt"); 
        } else if (datos == "TXT2") {
            enviarArchivo("his.txt"); 
        }
    }
}