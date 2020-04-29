#ifndef UTILS_H
#define	UTILS_H
#define	_XTAL_FREQ 4000000 // 4 khz cycles

const int INPUT = 1;
const int OUTPUT = 0;
const int HIGH = 1;
const int LOW = 0;
void setPin(int pin);
void resetPin(int pin);
void pinMode(unsigned int pin_num, unsigned int pin_mode);
void digitalWrite(unsigned int pin_num, unsigned int state);
void setupAnalog(unsigned pin_num);
void initADC(unsigned int pin_num);
unsigned int analogRead(unsigned pin_num);
unsigned int map(unsigned int value,
        unsigned int min,
        unsigned int max,
        unsigned int new_min,
        unsigned int new_max );
void mem_write(unsigned char addr, unsigned int value);
unsigned int mem_read(unsigned char addr);
#endif	/* UTILS_H */


