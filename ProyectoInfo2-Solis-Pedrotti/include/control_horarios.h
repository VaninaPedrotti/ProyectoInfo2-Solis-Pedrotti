
#ifndef CONTROL_HORARIOS_H
#define CONTROL_HORARIOS_H

//Convertir la hora y minuto actual en string
String obtenerHoraActual(int hora, int minuto, int segundo);
//Lectura de horarios programados desde la memoria SD
void leerHorarios();
//Comparar la hora actual con los horarios programados
bool compararHorarios(String horaActual);

#endif