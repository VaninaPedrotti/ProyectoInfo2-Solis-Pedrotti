/**
 * @file fdispenser.h
 * @author Pedrotti Vanina - Solis Milagros 
 * @brief 
 * @version 0.1
 * @date 2024-09-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef DISPENSER_H
#define DISPENSER_H

/**
 * @brief Función que inicializa.
 */
void InicializaDispenser();

/**
 * @brief Función de loop.
 */
void Dispenser();

/**
 * @brief Función que activa el servo.
 */
void activaServo();

/**
 * @brief Función que desactiva el servo.
 */
void desactivaServo();

#endif