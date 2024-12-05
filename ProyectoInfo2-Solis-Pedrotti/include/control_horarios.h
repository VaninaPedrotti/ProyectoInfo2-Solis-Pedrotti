
#ifndef CONTROL_HORARIOS_H
#define CONTROL_HORARIOS_H

#include <RTClib.h> //reloj
#include <Servo.h> //motor

void inicializaControlHorarios();
DateTime horaRTC();
String obtenerHoraActual(int hora, int minuto);
void leerHorarios();
bool compararHorarios(String horaActual);


#endif