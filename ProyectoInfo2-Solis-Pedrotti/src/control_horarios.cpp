
#include <Arduino.h>
#include <control_horarios.h>
#include <memoria_SD.h>

//#include <Wire.h>  // comunicacion I2C con el rtc



#include <SD.h> //memoria_SD
#include <SPI.h> //comunicacion para el modulo sd



const int totalHorarios = 10; //se verifica que no se pase ese limite desde la app
int cantidadHorarios = 0; //Ingresados al archivo
String horarios[totalHorarios]; //Arreglo para almacenar los horarios programados y poder compararlos con el horario actual


String obtenerHoraActual(int hora, int minuto, int segundo) {
    char resultado[9]; // HH:MM:SS\0
    sprintf(resultado, "%02d:%02d:%02d", hora, minuto, segundo); //unir hora:minuto
    return String(resultado); //convertir a String
}
void leerHorarios() {
    File archivo2 = SD.open("HORARIOS.TXT", FILE_READ); // Modo lectura
    if (!archivo2) {
        // Crear el archivo vacío si no existe
        archivo2 = SD.open("HORARIOS.TXT", FILE_WRITE); // Modo escritura
        if (!archivo2) {
            Serial.println("Error al crear el archivo HORARIOS.TXT");
            return;
        }
        archivo2.close(); 
        return;
    }

    int index = 0;
    while (archivo2.available() && index < totalHorarios) { 
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


