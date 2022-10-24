/*
 * File:   main.c
 * Author: dianagastrin
 */

#include <xc.h>
#include "debug_utils.h"
#include "utils.h"

#define DEBUG 0

// Configuration bits
#define	_XTAL_FREQ 4000000 // 4 mhz cycles
#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF       // Enable / Disable Pull Up in GPIO3
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF       // Brown Out Detect (BOR enabled)
#pragma config IESO = OFF        // Internal External Switchover bit (Internal External Switchover mode is enabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
// end of configuration

// Initialize GPIO pins
#define RED_PIN  0 // GP0
#define YELLOW_PIN  1 // GP1
#define GREEN_PIN  2 // GP2
#define METHANE_PIN 4 // GP4
#define PUSH_BTN_PIN  5 // GP5

#define MOVING_AVERAGE_WINDOW_SIZE 5

// Initialize eeprom once
// min:0 (0x0000) max:1023 (0x03FF)
__eeprom char calibrationData[4] = {0x00, 0x00, 0x03, 0xFF};
const unsigned char EEPROM_MIN_ADDR_START = 0;
const unsigned char EEPROM_MAX_ADDR_START = 2;

// Initialize button
unsigned int calibration_counter = 0;
const unsigned int MIN_CALIBRATION_BUTTON_PRESSED_SECONDS = 1000; // 1 sec
unsigned int pressed_counter = 0;
unsigned char calibration_mode = 0;


const unsigned int DELAY_MS = 100;


// Initialize moving average
unsigned int MOVING_AVERGAE_WINDOW[MOVING_AVERAGE_WINDOW_SIZE] = {0};
unsigned int moving_index = 0;


// max that ADC can be (10 bits)
const unsigned int GLOBAL_MAX = 1023;
const unsigned int GLOBAL_MIN = 0;

// Initialize thresholds
const unsigned int HIGH_THRESHOLD = 7;
const unsigned int LOW_THRESHOLD = 4;

// Calibration function min-max defaults
unsigned int min_calibrated = 1023;
unsigned int max_calibrated = 0;

// Calibration global min max
unsigned int min =0;
unsigned int max = 1023;

// Hysteresis
unsigned int low_threshold;
unsigned int high_threshold;

unsigned int methane_value = 0;
unsigned int mapped_value;

// INIT FUNCTIONS

void initLeds() {
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(YELLOW_PIN, OUTPUT);
    pinMode(RED_PIN, OUTPUT);
        
    resetPin(GREEN_PIN);
    resetPin(YELLOW_PIN);
    resetPin(RED_PIN);
}

void initPushButton() {
    pinMode(PUSH_BTN_PIN, INPUT);

    // enable pull-up
    OPTION_REGbits.nGPPU = 0; // GPIO pull-ups are enabled by individual PORT latch values in WPU register
    WPUbits.WPU5 = 1; // GP5 Weak Pull-up Control bits, 1 enabled
}

void initGasSensor() {
    // INPUT mode
    pinMode(METHANE_PIN, INPUT);

    // bit 4-6: x11 = FRC (clock derived from a dedicated internal oscillator = 500 kHz max)
    // bit 3: AN3 pin
    ANSEL = 0b01111000;
    __delay_ms(1000);

    ADCON0bits.ADFM = 1; //right justify
    ADCON0bits.VCFG = 0; //VDD reference

    // Enable the AN3 input "11"
    ADCON0bits.CHS1 = 1;
    ADCON0bits.CHS0 = 1;
    // enable the analog-to-digital
    ADCON0bits.ADON = 1;

    __delay_ms(1000);
    ADCON0bits.GO_nDONE = 1;

}

void initMinMax() {
    min = mem_read(EEPROM_MIN_ADDR_START);
    max = mem_read(EEPROM_MAX_ADDR_START);
}

void setup() {
    initMinMax();
    initLeds();

    initPushButton();
    initGasSensor();
    enableAnalogToDigital(3);

    low_threshold = LOW_THRESHOLD;
    high_threshold = HIGH_THRESHOLD;

}

void blinkLed(unsigned int led_pin, unsigned int duration) {
    for (int i = 0; i < duration; i++) {
        setPin(led_pin);
        __delay_ms(DELAY_MS);
        resetPin(led_pin);
        __delay_ms(DELAY_MS);
    }
}

void calibrate() {
    if (methane_value < min_calibrated) {
        min_calibrated = methane_value;
    }

    if (methane_value > max_calibrated) {
        max_calibrated = methane_value;
    }

    if (calibration_counter % 5 == 0) {
        resetPin(RED_PIN);
        resetPin(GREEN_PIN);
        blinkLed(YELLOW_PIN, 1);
    }

    calibration_counter++;
}

unsigned int apply_moving_average(unsigned int curr_methane_value) {
    moving_index = moving_index % MOVING_AVERAGE_WINDOW_SIZE;
    unsigned int sum = 0;
    MOVING_AVERGAE_WINDOW[moving_index] = curr_methane_value;
    moving_index++;

    for (int i = 0; i < MOVING_AVERAGE_WINDOW_SIZE; i++) {
        if (MOVING_AVERGAE_WINDOW[i] == 0) {
            // we must have at  least 5 values
            return 0;
        }
        sum += MOVING_AVERGAE_WINDOW[i];
    }

    return sum / MOVING_AVERAGE_WINDOW_SIZE;
}

void loop() {
    // Counter how long the button is pressed
    if ((unsigned int) GPIObits.GP5 == 0) {
        pressed_counter = pressed_counter + 1;
        __delay_ms(DELAY_MS);
        return;
    }
    
    // check if the analog-to-digital is ready
    if (ADCON0bits.GO_nDONE == 0) {
        // read the value
        methane_value = analogRead(METHANE_PIN);
    }

    // Check if the button was pressed for <MIN_CALIBRATION_BUTTON_PRESSED_SECONDS> seconds, if yes - calibrate
    if (pressed_counter * DELAY_MS  > MIN_CALIBRATION_BUTTON_PRESSED_SECONDS) {
        if (calibration_mode > 0) {
            calibration_mode = 0;

            if (min_calibrated < max_calibrated && (max_calibrated - min_calibrated) > 15 ) {
                resetPin(RED_PIN);
                resetPin(YELLOW_PIN);
                blinkLed(GREEN_PIN, 3);

                min = min_calibrated;
                max = max_calibrated;
            } else {
                resetPin(GREEN_PIN);
                resetPin(YELLOW_PIN);
                blinkLed(RED_PIN, 3);
            }
        } else {
            calibration_mode = 1;
            calibration_counter = 0;
            min = GLOBAL_MIN;
            max = GLOBAL_MAX;
            min_calibrated = GLOBAL_MAX;
            max_calibrated = GLOBAL_MIN;
        }

        mem_write(EEPROM_MIN_ADDR_START, min);
        mem_write(EEPROM_MAX_ADDR_START, max);

    } else {
        if (calibration_mode > 0) {
            calibrate();
        } else {
            methane_value = apply_moving_average(methane_value);

            mapped_value = map(methane_value, min, max, 0, 9);

            if (DEBUG > 0) {
                // Debug code
                sendIntToMCP(0, 12345); // start signature
                sendIntToMCP(0, min);
                sendIntToMCP(0, max);
                sendIntToMCP(0, methane_value);
                sendIntToMCP(0, mapped_value);
                sendIntToMCP(0, low_threshold);
                sendIntToMCP(0, high_threshold);
                sendIntToMCP(0, 56789); // end signature
            }

            if (mapped_value < low_threshold) {
                resetPin(YELLOW_PIN);
                resetPin(RED_PIN);
                setPin(GREEN_PIN);

                // hysteresis
                low_threshold = LOW_THRESHOLD;
                high_threshold = HIGH_THRESHOLD;
            } else if (mapped_value >= low_threshold && mapped_value < high_threshold) {
                resetPin(GREEN_PIN);
                resetPin(RED_PIN);
                setPin(YELLOW_PIN);
                // hysteresis
                low_threshold = LOW_THRESHOLD - 1;
                high_threshold = HIGH_THRESHOLD;
            } else {
                resetPin(GREEN_PIN);
                resetPin(YELLOW_PIN);
                setPin(RED_PIN);
                // hysteresis
                low_threshold = LOW_THRESHOLD;
                high_threshold = HIGH_THRESHOLD - 1;
            }
            
        }
    }

    pressed_counter = 0;
    ADCON0bits.GO_nDONE = 1;
    __delay_ms(DELAY_MS);
}

void main(void) {
    setup();

    while (1) {
        loop();
    }
}
