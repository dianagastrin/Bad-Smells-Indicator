/*
 * File:   main.c
 * Author: Diana Gastrin
 *
 */
#include <xc.h>
#include "utils.h"

// Configuration bits
#define	_XTAL_FREQ 4000000 // 4 khz cycles
#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select bit (MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Detect (BOR enabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal External Switchover mode is enabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
// end of configuration

// GPIO pins
#define METHAN_PIN 4
#define RED_PIN  0
#define YELLOW_PIN  1
#define GREEN_PIN  2
#define PUSH_BTN_PIN  5


const int DELAY_MS = 100;
const int MAX_PRESSED_MS = 1000;
const int MAX_PRESSED_RESET_MS = 10000;
const int HIGH_THRESHOLD = 7;
const int LOW_THRESHOLD = 4;

const unsigned char MIN_ADDR_START = 0;
const unsigned char MAX_ADDR_START = 2;

unsigned int value = 0;

// initialize eeprom once
__eeprom char calData[4] = {0x00, 0x00, 0x03, 0xFF};

unsigned int min;
unsigned int max;

unsigned int pressed_counter = 0;
unsigned int mapped_value;

int low_threshold = 4;
int high_threshold = 7;

void setAllLeds() {
    setPin(GREEN_PIN);
    setPin(YELLOW_PIN);
    setPin(RED_PIN);
}

void resetAllLeds() {
    resetPin(GREEN_PIN);
    resetPin(YELLOW_PIN);
    resetPin(RED_PIN);
}

void init() {
    min = mem_read(MIN_ADDR_START);
    max = mem_read(MAX_ADDR_START);

    pinMode(PUSH_BTN_PIN, INPUT);
    pinMode(RED_PIN, OUTPUT);
    pinMode(YELLOW_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(METHAN_PIN, INPUT);

    OPTION_REGbits.nGPPU = 0;
    WPUbits.WPU5 = 1;

    setupAnalog(METHAN_PIN);
    initADC(METHAN_PIN);
    ADCON0bits.GO_nDONE = 1;
}

void blinkLed(unsigned int led_pin, unsigned int duration) {
    for (int i = 0; i < duration; i++) {
        setPin(led_pin);
        __delay_ms(DELAY_MS);
        resetPin(led_pin);
        __delay_ms(DELAY_MS);
    }
}

void blinkAllLeds(unsigned int duration) {
    for (int i = 0; i < duration; i++) {
        setAllLeds();
        __delay_ms(DELAY_MS);
        resetAllLeds();
        __delay_ms(DELAY_MS);
    }
}

void loop() {
    while (1) {
        if (ADCON0bits.GO_nDONE) {
            continue;
        }

        value = analogRead(METHAN_PIN);

        if (((unsigned int) GPIObits.GP5) == 0) {
            pressed_counter = pressed_counter + 1;
        } else {
            if (pressed_counter > 0) {
                resetAllLeds();
                if (pressed_counter * DELAY_MS > MAX_PRESSED_RESET_MS) {
                    min = 0;
                    max = 1023;
                    blinkAllLeds(3);
                    mem_write(MIN_ADDR_START, min);
                    mem_write(MAX_ADDR_START, max);
                } else if (pressed_counter * DELAY_MS > MAX_PRESSED_MS) {
                    // configure max!!!
                    if (value <= min) {
                        blinkLed(YELLOW_PIN, 3);
                    } else {
                        max = value;
                        mem_write(MAX_ADDR_START, value);
                        blinkLed(RED_PIN, 3);
                    }
                } else {
                    // configure min!!!
                    if (value >= max) {
                        blinkLed(YELLOW_PIN, 3);
                    } else {
                        min = value;
                        mem_write(MIN_ADDR_START, value);
                        blinkLed(GREEN_PIN, 3);
                    }
                }

                pressed_counter = 0;
                __delay_ms(DELAY_MS);
            }
        }

        resetAllLeds();
        mapped_value = map(value, min, max, 1, 9);

        if (mapped_value < low_threshold) {
            setPin(GREEN_PIN);
            low_threshold = LOW_THRESHOLD;
            high_threshold = HIGH_THRESHOLD;
        } else if (mapped_value >= low_threshold && mapped_value < high_threshold) {
            setPin(YELLOW_PIN);
            low_threshold = LOW_THRESHOLD - 1;
            high_threshold = HIGH_THRESHOLD;
        } else {
            setPin(RED_PIN);
            low_threshold = LOW_THRESHOLD;
            high_threshold = HIGH_THRESHOLD - 1;
        }

        __delay_ms(DELAY_MS);
        ADCON0bits.GO_nDONE = 1;
    }
}

void main(void) {
    init();
    loop();
}