#include <Arduino.h>
#include <memoria_SD.h>
#include <control_horarios.h>

#include <SPI.h>
#include <SD.h>

#define pinCS 10 // Pin de CS del modulo SD

File archivo;
void inicializaMemoriaSD(){
    SPI.begin();
    pinMode(pinCS, OUTPUT); // Configura el pin de CS
    if (SD.begin(pinCS)) {
        Serial.println("Tarjeta SD inicializada correctamente.");
    } else {
    Serial.println("Error al inicializar la tarjeta SD.");
    }
}

void guardarHorario(byte hora, byte minuto, byte segundo = 00) {
    archivo = SD.open("horarios.txt", FILE_WRITE);
    if (archivo) {
        char resultado[9]; 
        sprintf(resultado, "%02d:%02d:%02d", hora, minuto, segundo); 
        archivo.println(resultado); //guardar en el archivo
        archivo.close();
    } else {
        Serial.println("Error guardando horario");
    }
}
void guardarHistorial(){
    archivo = SD.open("his.txt", FILE_WRITE);
    DateTime now = horaRTC();
    char fecha[20];  //  "HH:MM DD/MM/YYYY\0"
    sprintf(fecha, "%02d:%02d %02d/%02d/%04d\n", 
            now.hour(), now.minute(), 
            now.day(), now.month(), now.year());
    if (archivo) {
        archivo.print(fecha);  
        archivo.close();  
    } else {
        Serial.println("Error guardando historial");
    }
}
void enviarArchivo(const char* nombreArchivo) {
    archivo = SD.open(nombreArchivo); 
    if (archivo) {
        Serial.println("Inicio");  // Envia un indicador de inicio
        while (archivo.available()) {
            String linea = archivo.readStringUntil('\n');  // Lee hasta el salto de línea
            linea.trim();  // Elimina espacios y saltos de línea extra
            Serial.println(linea);  // Envia cada línea leída
        }
        Serial.println("Fin");  // Envia un indicador de fin
        archivo.close();
    } else {
        Serial.println("Error enviando archivo");
    }
}



