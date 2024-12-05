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
    inicializaSensor();
    inicializaMemoriaSD();
    
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
    } else if (Serial.available()>0) { //hay datos en el puerto serial para ser leidos
        datos = Serial.readString(); //lee y os guarda en datos
        if (datos == "1"){ //desde la app manda un 1 cuando se acciona el boton dispensar
            servo.write(90);
            delay(Intervalo);
            servo.write(0);
            guardarHistorial();
        } else if(datos.startsWith("<")){
            datos.remove(0, 1);
            hora = (datos.toInt());
            datos.remove(0, (datos.indexOf(",")+1));
            minuto = (datos.toInt());
            datos.remove(0, (datos.indexOf(">")+1));
            guardarHorario(hora, minuto); // Guardar en la SD
        } else if (datos == "TXT1") {
            enviarArchivo("horarios.txt"); // Enviar el primer archivo
        } else if (datos == "TXT2") {
            enviarArchivo("historial.txt"); // Enviar el segundo archivo
        }
    }
}