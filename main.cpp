/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "TM1638.h"

// Define los pines que usarás para conectar el TM1638
#define STB_PIN D10  // Pin de STB (Strobe)
#define CLK_PIN D9   // Pin de CLK (Clock)
#define DIO_PIN D8   // Pin de DIO (Data In/Out)

// Prototipos de funciones
void leer_botones(void);

// Variables globales
int conteo = 0;
bool reset_conteo = false;

// Instancia del display TM1638
TM1638 tm1638(STB_PIN, CLK_PIN, DIO_PIN);

// Hilos del sistema operativo
Thread T_leer_botones(osPriorityNormal, 4096, NULL, "Hilo Leer Botones");

int main() {
    // Inicializa el TM1638
    tm1638.init();
    tm1638.setBrightness(7);  // Ajusta el brillo al máximo
    printf("Arranque del programa\n\r");

    // Prueba inicial del display
    tm1638.displayNumber(12345678);  // Muestra un número en el display
    wait_us(2e6);  // Espera 2 segundos
    tm1638.clearDisplay();  // Limpia el display

    // Arranco el hilo para leer botones
    T_leer_botones.start(leer_botones);

    while (true) {
        if (reset_conteo) {
            conteo = 0;  // Reinicia el conteo si se presiona un botón
            reset_conteo = false;
        }
        tm1638.displayNumber(conteo++);  // Muestra el número en el display y lo incrementa
        ThisThread::sleep_for(500ms);    // Pausa de 500 ms
    }
}

void leer_botones(void) {
    uint8_t keys = 0;
    while (true) {
        keys = tm1638.readKeys();  // Lee el estado de las teclas
        if (keys) {
            printf("Keys pressed: %02X\n", keys);  // Muestra las teclas presionadas en la consola
            reset_conteo = true;  // Si se presiona cualquier tecla, reinicia el conteo
        }
        ThisThread::sleep_for(100ms);  // Pausa de 100 ms
    }
}
