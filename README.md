# Bad Smells Detector
<img src="/demo/1.jpeg" height="100px"/> <img src="/demo/2.jpeg" height="100px"/> <img src="/demo/3.jpeg" height="100px"/> <img src="/demo/4.jpeg" height="100px"/> <img src="/demo/5.jpeg" height="100px"/>

### Description:
A real-time indicator for bed smells which has 3 states:
1. ![#90EE90](https://placehold.it/15/90EE90/000000?text=+) `green`
2. ![#FFA500](https://placehold.it/15/FFA500/000000?text=+) `orange`
2. ![#FF0000](https://placehold.it/15/FF0000/000000?text=+) `red`


When it's Red, you don't want to go to the bathroom...

### Calibration:
1. **short press** on the button defines the value for a "good" smell
2. **long press** defines "bad" smell.
3. **10 sec press** will go back to defaults.

### Consists of:
- 1 Micro-controller: [PIC12F683](https://ww1.microchip.com/downloads/en/devicedoc/41211d_.pdf)
- 1 Methane gas sensor: [MQ4](https://www.sparkfun.com/datasheets/Sensors/Biometric/MQ-4.pdf)
- 1 Push Button
- 1 Regulator: [L7805](https://www.st.com/resource/en/datasheet/l78.pdf)
- 1 Matrix
- 3 Leds (Green, Orange, Red)
- capacitors, resistors and wires
- programmer: [PIC Programmer K150](https://www.ebay.com/itm/PIC-Programmer-K150-USB-Automatic-Microchip-Develop-Microcontroller-ICSP-Cable/252710962515?hash=item3ad6bf4553:g:rG4AAOSw2xRYbh9x) 
- Software:
  - C language
  - programmer software: `DIY K150 PICmicro Programmer v150807` 
  - IDE: `MPLAB X IDE V5.25 by Microchip (XC8)`

<img src="/demo/5.jpeg" height="600px"/>

