#include <xc.h>
#include "utils.h"
#include "xc8debug.h"

unsigned char S_TRISIO = 0;
unsigned char S_GPIO;

void setPin(int pin){
    S_GPIO = S_GPIO | (1 << pin);
    GPIO = S_GPIO;
}

void resetPin(int pin){
    S_GPIO = S_GPIO & ~(1 << pin);
    GPIO = S_GPIO;
}

void pinMode(unsigned int pin_num, unsigned int pin_mode) {
    // 5 (00000001)
    unsigned int mask = 1 << pin_num;

    if (pin_mode) {
        // set to INPUT the GPIO port
        S_TRISIO = S_TRISIO | mask;
    } else {
        // set to OUTPPUT the GPIO port
        S_TRISIO = S_TRISIO & ~mask;
    }

    TRISIO = S_TRISIO;
}

void digitalWrite(unsigned int pin_num, unsigned int state) {
    unsigned int mask = 1 << pin_num;

    if (pin_num == 3) {
        // pin 3 is read only
        return;
    }

    if (state) {
        GPIO = GPIO | mask;
    } else {
        GPIO = GPIO & ~mask;
    }
}

unsigned int analogRead(unsigned int pin_num) {
    unsigned int high = ADRESH;
    unsigned int low = ADRESL;
    unsigned int ad_value = low + (high << 8);
    return ad_value;

}

void setupAnalog(unsigned int pin_num) {
    if (pin_num == 4) {
        --pin_num;
    }

    ANSEL = 0b01110000; //use Fosc/16 since 4 MHZ osc, set analog for gpio0
    unsigned int mask = 1 << pin_num;
    ANSEL = ANSEL | mask; // turn on the pin number - analog    
    __delay_ms(1000);
}

void initADC(unsigned int pin_num) {
    ADCON0bits.ADFM = 1; //right justify
    ADCON0bits.VCFG = 0; //VDD reference
    ADCON0bits.CHS1 = 1; //combine with line below,,
    ADCON0bits.CHS0 = 1; //this makes AN3 input
    ADCON0bits.ADON = 1; //enable the a2d
    __delay_ms(1000);
}

unsigned int map(unsigned int value,
        unsigned int min,
        unsigned int max,
        unsigned int new_min,
        unsigned int new_max) {

    if (value < min) {
        return new_min;
    }

    if (value > max) {
        return new_max;
    }

    return ((value - min) *(new_max - new_min)) / (max - min) + new_min;
}

void mem_write(unsigned char addr, unsigned int value) {
    eeprom_write((unsigned char) addr, value >> 8 & 0xFF);
    __delay_ms(200);
    
    eeprom_write((unsigned char) addr+1, value    & 0xFF);
    __delay_ms(200);
}

unsigned int mem_read(unsigned char addr) {
    unsigned char chars[2];
    
    chars[0] = eeprom_read(addr);
    chars[1] = eeprom_read(addr+1);
    
    return *(unsigned int *) chars;
}
