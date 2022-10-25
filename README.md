# Bad Smells Indicator AKA Pookimeter
#### Pookimeter V1
<img src="/Resources/v1.jpg" height="100px"/> 

#### Pookimeter V2
<img src="/Resources/v2_1.jpeg" height="100px"/> <img src="/Resources/v2_2.jpeg" height="100px"/> <img src="/Resources/v2_3.jpg" height="100px"/> <img src="/Resources/v2_4.jpg" height="100px"/> <img src="/Resources/v2_5.jpeg" height="100px"/>

#### Pookimeter V3
<img src="/Resources/v3_1.png" height="100px"/><img src="/Resources/v3_2.png" height="100px"/><img src="/Resources/v3_3.png" height="100px"/>

## Description:
A real-time indicator for bad smells which has 3 states:
```diff
- RED
! ORANGE
+ GREEN

```
When it's Red, you don't want to go into the bathroom...

## Calibration:

Press on the button and it will enter calibration mode. 
It will try to find `min` and `max` values for sensor signal during the calibration. 
Exit the calibration mode by pressing on the button again.

### Code parts
  - IDE: [MPLAB X IDE](https://www.microchip.com/en-us/tools-resources/develop/mplab-x-ide)
  - Compiler: [MPLAB XC8 Compiler](https://www.microchip.com/en-us/tools-resources/develop/mplab-xc-compilers)
  - SRC code in [ChipCode](ChipCode) folder
  
### Hardware parts
  - [PIC12F683 Microcontroller](https://ww1.microchip.com/downloads/en/devicedoc/41211d_.pdf)
  - [Push Button](https://www.aliexpress.com/item/1005003400929705.html?spm=a2g0o.order_list.0.0.79b31802wwf9ee)
  - [USB 2.0 connector](https://www.aliexpress.com/item/32265708803.html?spm=a2g0o.order_list.0.0.79b31802wwf9ee)
  - [Methane gas sensor MQ4](https://www.sparkfun.com/datasheets/Sensors/Biometric/MQ-4.pdf)
  - Breadboard
  - 3 Leds (Green, Orange, Red) and resistors (220 Ohm)
  - Wires

### Burn the code
- Programmer [PIC Programmer K150](https://www.ebay.com/itm/PIC-Programmer-K150-USB-Automatic-Microchip-Develop-Microcontroller-ICSP-Cable/252710962515?hash=item3ad6bf4553:g:rG4AAOSw2xRYbh9x) 
- Programmer driver [DIY K150 PICmicro Programmer v150807](https://buyhere22.com/components/k150/) 

### Debug 
- General Purpose USB to GPIO [MCP221A](https://www.adafruit.com/product/4471])
- Must set `DEBUG=1` in [main.c](ChipCode/main.c)
- Python debugging code [main.py](DebugPython/main.py)

### Resources
- [Presentation](Resources/SmellsLikeChips.pdf)
- Printed Circuit Board file [Gerber File](Resources/GerberFile.zip)

### Burn code
- programmer: [PIC Programmer K150](https://www.ebay.com/itm/PIC-Programmer-K150-USB-Automatic-Microchip-Develop-Microcontroller-ICSP-Cable/252710962515?hash=item3ad6bf4553:g:rG4AAOSw2xRYbh9x) 
- Software:
  - C language
  - programmer software: `DIY K150 PICmicro Programmer v150807` 
  - IDE: `MPLAB X IDE V5.25 by Microchip (XC8)`
