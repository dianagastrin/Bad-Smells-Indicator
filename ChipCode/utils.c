#include <xc.h>
#include "utils.h"
#include "xc8debug.h"


void setPin(unsigned int pin){
    GPIO = GPIO | (1 << pin);
}

void resetPin(unsigned int pin){
    GPIO = GPIO & ~(1 << pin);
}

void pinMode(unsigned int pin_num, unsigned int pin_mode) {
    unsigned char S_TRISIO = TRISIO;
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


unsigned int analogRead(unsigned int pin_num) {
    unsigned int high = ADRESH;
    unsigned int low = ADRESL;
    unsigned int ad_value = low + (high << 8);
    return ad_value;

}

void enableAnalogToDigital(unsigned int an_pin) {
    ANSEL = 0b01110000; // bit 4-6, x11 = FRC (clock derived from a dedicated internal oscillator = 500 kHz max)
    unsigned int mask = 1 << an_pin;
    ANSEL = ANSEL | mask; // turn on the pin number - analog    
    __delay_ms(1000);
    
    ADCON0bits.ADFM = 1; //right justify
    ADCON0bits.VCFG = 0; //VDD reference
    ADCON0bits.CHS1 = 1; //combine with line below,,
    ADCON0bits.CHS0 = 1; //this makes AN3 input
    ADCON0bits.ADON = 1; //enable the a2d
    
    __delay_ms(1000);
    ADCON0bits.GO_nDONE = 1;
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

    return ((value - min) * (new_max - new_min)) / (max - min) + new_min;
}

void mem_write(unsigned char addr, unsigned int value) {
    eeprom_write((unsigned char) addr,  (unsigned char) ((value >> 8) & 0xFFFF)); // MSB
    __delay_ms(200);
    
    eeprom_write((unsigned char) addr+1, (unsigned char) (value & 0xFF)); // LSB
    __delay_ms(200);
}

unsigned int mem_read(unsigned char addr) {
    unsigned char calibration[2];
    unsigned int val = 0xFF; // 0000000011111111
    
    
    calibration[0] = eeprom_read(addr); // MSB
    calibration[1] = eeprom_read(addr+1); // LSB
    
    val = (calibration[0] & val) << 8;  // xxxxxxxx00000000
    val = val | calibration[1];
    
    return val;
}