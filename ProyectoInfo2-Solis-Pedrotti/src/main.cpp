/**
 * @file main.cpp
 * @author Pedrotti Vanina - Solis Milagros 
 * @brief 
 * @version 0.1
 * @date 2024-07-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <Arduino.h>
#include <dispenser.h>
#include <sensor.h>
#include <memoria_SD.h>

void setup() {
    Serial.begin(9600);
    InicializaDispenser();
    InicializaSensor();
    InicializaMemoriaSD();
}

void loop() {
    Dispenser();
}