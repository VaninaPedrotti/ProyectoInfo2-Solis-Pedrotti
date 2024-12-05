<<<<<<< HEAD
/**
 * @file memoria_SD.h
 * @author Pedrotti Vanina - Solis Milagros 
 * @brief 
 * @version 0.1
 * @date 2024-09-12
 * 
 * @copyright Copyright (c) 2024
 * MEMORIA
 */
#ifndef MEMORIA_SD_H
#define MEMORIA_SD_H

#include <SPI.h>
#include <SD.h>

/**
 * @brief Función que inicializa.
 */
void inicializaMemoriaSD();
void guardarHorario(byte hora, byte minuto);
void guardarHistorial();
void enviarArchivo(const char* nombreArchivo);


=======
/**
 * @file memoria_SD.h
 * @author Pedrotti Vanina - Solis Milagros 
 * @brief 
 * @version 0.1
 * @date 2024-09-12
 * 
 * @copyright Copyright (c) 2024
 * MEMORIA
 */
#ifndef MEMORIA_SD_H
#define MEMORIA_SD_H

/**
 * @brief Función que inicializa.
 */
void InicializaMemoriaSD();



>>>>>>> 6cba66d9048db2b20a1351033f5fc103c3c277ac
#endif