#include <Arduino.h>
#include <sensor.h>

#define trigPin  4 
#define echoPin  5

void inicializaSensor(){
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    digitalWrite(trigPin, LOW);
}

float medidadeSensor(){
    // Genera un pulso de 10 μs en el pin TRIG
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2); 
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Mide la duración del pulso HIGH en el pin ECHO
    long tiempo = pulseIn(echoPin, HIGH);
    // Calcula la distancia en cm
    float distancia = (0.034 * tiempo) /2;
    return distancia;
}


