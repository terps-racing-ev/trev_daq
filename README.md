# DAQ

[Sensor Organization](https://umd0.sharepoint.com/:x:/s/TeamsTerpsRacingEV/Ef79KA5FaFhJh21jqTwfWeUBTny6eTJU5965LJCH_cuU6w?e=4OiMr1)

## Using Platformio
- Platformio is essentially the Arduino interface but allows for much better file organization and version control
- Install VSCode and the Platformio extension

## Uploading Code
- In the ```platformio.ini``` file, change ```src_dir = src/FRboard``` to the name of the folder of the board you want to upload. Use the misc folder for testing

## Code Structure
- Each sensor is an object and has its own class
- All sensors inherit the abstract class ```sensor.h```
  - void init(pin, Optional interrupt function)
  - int16_t calculate()
    - Result will be scaled by some factor of 10
    - Currently trying to optimize calculations to use ints for everything (lookup table for logarithms?)
- Interrupts use ```EIMSK &= ~(1 << digitalPinToInterrupt(pin));``` instead of ```sei``` and ```cli``` to avoid affecting all interrupts
  - Interrupt Handlers must be static functions, so you will have to declare a static wrapper function in ```main.cpp``` that simply calls the object's interrupt handler (see wheelspeed)
- Each board's ```main.cpp``` is responsible for calling tx(id, data1, data2(optional), data3(optional), data4(optional)
    - Instead of canInit(500000), call canManagerInit(500000)

Also, ```pins_arduino.h``` has been modified due to not being able to use the .zip file that you would normally link in Arduino. Everything works so far with the at90can128 preset platformio provides, but there may be issues/other functions that don't work.
