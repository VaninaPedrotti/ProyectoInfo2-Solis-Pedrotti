#include <Arduino.h>
#include <memoria_SD.h>
#include <control_horarios.h>
#include <SPI.h>
#include <SD.h>

#define pinCS 10 // Pin de CS del modulo SD


File archivo;
void inicializaMemoriaSD(){
    while (!Serial) {
    ; 
    }
    SPI.begin();
    pinMode(pinCS, OUTPUT); // Configura el pin de CS
    if (SD.begin(pinCS)) {
        Serial.println("Tarjeta SD inicializada correctamente.");
    } else {
    Serial.println("Error al inicializar la tarjeta SD.");
    }
}

void guardarHorario(byte hora, byte minuto) {
    archivo = SD.open("horarios.txt", FILE_WRITE);
    if (archivo) {
        if (hora < 10) archivo.print("0"); // Agregar un "0" si la hora es menor a 10
        archivo.print(hora);
        archivo.print(":");
        if (minuto < 10) archivo.print("0"); // Agregar un "0" si el minuto es menor a 10
        archivo.println(minuto);
        archivo.close();
    } else {
        Serial.println("Error guardando horario");
    }
}
void guardarHistorial(){
    archivo = SD.open("his.txt", FILE_WRITE);
    DateTime now = horaRTC();
    String fecha =  (now.hour() < 10 ? "0" : "") +  String(now.hour()) + ":" +
                    (now.minute() < 10 ? "0" : "")+ String(now.minute()) + " " +
                    (now.day() < 10 ? "0" : "") +   String(now.day()) + "/" +
                    (now.month() < 10 ? "0" : "") + String(now.month()) + "/" +
                                                    String(now.year()) + "\n";
    if (archivo) {
        archivo.print(fecha);  // Guardar el registro en el archivo
        archivo.close();          // Cerrar el archivo
    } else {
        Serial.println("Error guardando historial");
    }
}
void enviarArchivo(const char* nombreArchivo) {
    archivo = SD.open(nombreArchivo); // Abrir el archivo en la tarjeta SD

    if (archivo) {
        Serial.println("Lista:");
        while (archivo.available()) {
            String linea = archivo.readStringUntil('\n');  // Lee hasta el salto de línea
            Serial.println(linea);  // Envia cada línea leída
        }
        archivo.close();
    } else {
        Serial.println("Error enviando archivo");
        
    }
}


