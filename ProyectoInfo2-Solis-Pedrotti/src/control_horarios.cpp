
#include <Arduino.h>
#include <control_horarios.h>
#include <memoria_SD.h>

#include <RTClib.h> //reloj
#include <SPI.h>

RTC_DS3231 RTC; // objeto del modulo Reloj

const int totalHorarios = 10; 
int cantidadHorarios = 0;
String horarios[totalHorarios]; 

void inicializaControlHorarios(){
    RTC.begin(); 
    RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

DateTime horaRTC(){
    DateTime now = RTC.now();
    return now;
}
String obtenerHoraActual(int hora, int minuto) {//convertir a string
    char resultado[6];
    sprintf(resultado, "%02d:%02d", hora, minuto);
    return String(resultado);
}
void leerHorarios() {
    File archivo2 = SD.open("HORARIOS.TXT", FILE_READ);
    if (!archivo2) {
        Serial.println("Error leer horarios");
        return;
    }
    int index = 0;
    while (archivo2.available() && index < totalHorarios) { 
        horarios[index] = archivo2.readStringUntil('\n');
        horarios[index].trim(); // Limpia la cadena
        index++;
    }
    cantidadHorarios = index;
    archivo2.close();
}

bool compararHorarios(String horaActual) {
    for (int i = 0; i < totalHorarios; i++) {
        if (horaActual == horarios[i]) {
            return true; // Coincidencia encontrada
        }
    }
    return false;
}


