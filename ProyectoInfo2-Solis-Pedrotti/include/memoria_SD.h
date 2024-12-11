
#ifndef MEMORIA_SD_H
#define MEMORIA_SD_H

void inicializaMemoriaSD();
//Guardar la hora y el minuto recibidos mediante la app en el archivo "horarios.txt"
void guardarHorario(byte hora, byte minuto, byte segundo = 00);
// Guardar los horarios cuando se dispensa en el archivo "his.txt"
void guardarHistorial();
// Enviar los archivos para poder visualizarlos en la app
void enviarArchivo(const char* nombreArchivo);

#endif