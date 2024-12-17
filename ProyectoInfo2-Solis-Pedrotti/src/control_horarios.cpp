#include <Arduino.h>

#include <SD.h> //memoria_SD
#include <SPI.h> //comunicacion SPI para el modulo sd

#include <control_horarios.h>
#include <memoria_SD.h>

const int limiteHorarios = 10; //Mismo valor que la de memoria_SD
int cantidadHorarios = 0; //Ingresados al archivo
String horarios[limiteHorarios]; //Arreglo para almacenar los horarios programados y poder compararlos con el horario actual

String obtenerHoraActual(int hora, int minuto, int segundo) {
    char resultado[9]; // HH:MM:SS\0
    sprintf(resultado, "%02d:%02d:%02d", hora, minuto, segundo); //unir hora:minuto:segundo
    return String(resultado); //convertir a String
}
void leerHorarios() {
    File archivo2 = SD.open("horarios.txt", FILE_READ); // Modo lectura
    if (!archivo2) {
        // Crear el archivo vacío si no existe
        archivo2 = SD.open("horarios.txy", FILE_WRITE); // Modo escritura
        if (!archivo2) {
            Serial.println("Error al crear el archivo HORARIOS.TXT");
            return;
        }
        archivo2.close(); 
        return;
    }

    int index = 0;
    while (archivo2.available() && index < limiteHorarios) { 
        horarios[index] = archivo2.readStringUntil('\n'); // Leer datos hasta \n
        horarios[index].trim(); // Limpiar la cadena
        index++;
    }
    cantidadHorarios = index;
    archivo2.close();
}


bool compararHorarios(String horaActual) {
    for (int i = 0; i < cantidadHorarios; i++) {
        if (horaActual == horarios[i]) {//Comparar cada hora programada con la hora actual
            return true; 
        }
    }
    return false;
}


