#include <xc.h>

#include "debug_utils.h"
#include "utils.h"

void sendIntToMCP(unsigned int pin, unsigned int data){
    const int init_pulse = 20;
    const int high_pulse = 10;
    const int low_pulse = 5;
    const int delay_between_pulses = 2;
    
    setPin(pin);
    __delay_ms(init_pulse);
    resetPin(pin);
    __delay_ms(delay_between_pulses);
    
    for (int i=0; i<16; i++){
        setPin(pin)               ;
        
        if (data & 1<<i) {
          __delay_ms(high_pulse);
        } else {
           __delay_ms(low_pulse);
        }
        
        resetPin(pin);
        __delay_ms(delay_between_pulses);
    }
}