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
void guardarHistorial(int hora, int minuto, int dia, int mes, int year){
    archivo = SD.open("his.txt", FILE_WRITE);
    char fecha[20];  //  "HH:MM DD/MM/YYYY\0"
    sprintf(fecha, "%02d:%02d %02d/%02d/%04d\n", 
            hora, minuto, 
            dia, mes, year);
    if (archivo) {
        archivo.print(fecha);  
        archivo.close();  
    } else {
        Serial.println("Error guardando historial");
    }
}
void enviarHorario() {
    archivo = SD.open("horarios.txt"); 
    if (archivo) {
        Serial.println("Listado");  // Envia un indicador de inicio
        while (archivo.available()) {
            String linea = archivo.readStringUntil('\n');  // Lee hasta el salto de línea
            linea.trim();  // Elimina espacios y saltos de línea extra
            Serial.println(linea);  // Envia cada línea leída
        }
        //Serial.println(".");  // Envia un indicador de fin
        archivo.close();
    } else {
        Serial.println("Error enviando archivo");
    }
}
void enviarHistorial() {
    archivo = SD.open("his.txt", FILE_READ);
    if (!archivo) {
        Serial.println("Error leyendo historial");
        return;
    }

    // Paso 1: Contar el número de líneas en el archivo
    int totalLineas = 0;
    while (archivo.available()) {
        if (archivo.read() == '\n') {
            totalLineas++;
        }
    }
    archivo.close();

    // Paso 2: Reabrir el archivo y enviar solo las últimas 10 líneas
    archivo = SD.open("his.txt", FILE_READ);
    if (!archivo) {
        Serial.println("Error reabriendo archivo");
        return;
    }

    int saltarLineas = max(0, totalLineas - 10); // Número de líneas a ignorar
    int lineaActual = 0;
    Serial.println("Listado");
    while (archivo.available()) {
        String linea = archivo.readStringUntil('\n');
        if (lineaActual >= saltarLineas) {
            Serial.println(linea); // Enviar al monitor serial
            // BluetoothSerial.print(linea); // Enviar vía Bluetooth si usas Bluetooth
        }
        lineaActual++;
    }

    archivo.close();
}



