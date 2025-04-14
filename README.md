# DAQ

[Sensor Organization](https://umd0.sharepoint.com/:x:/s/TeamsTerpsRacingEV/Ef79KA5FaFhJh21jqTwfWeUBTny6eTJU5965LJCH_cuU6w?e=4OiMr1)

## Using Platformio
- Platformio is essentially the Arduino interface but allows for much better file organization and version control
- Install VSCode and the Platformio extension


## Uploading Code
- In the ```platformio.ini``` file, change ```src_dir = src/CoolingBoard``` to the name of the folder of the board you want to upload. Use the Testing folder for testing

## Code Structure
- Each sensor is an object and has its own class
- All sensors inherit the abstract class ```sensor.h```
  - void init(pin, Optional interrupt function, number of readings for moving averages)
  - bool calculate(void* result)
    - Returns true for success, false for error
    - Both sensor and main must agree on type by using ```sensortypes.h```
- Interrupts use ```EIMSK &= ~(1 << digitalPinToInterrupt(pin));``` instead of ```sei``` and ```cli``` to avoid affecting all interrupts
  - Interrupt Handlers must be static functions, so you will have to declare a static wrapper function in ```main.cpp``` that simply calls the object's interrupt handler (see wheelspeed)
- Each board's ```main.cpp``` is responsible for calling organizing can messages

Also, ```pins_arduino.h``` has been modified due to not being able to use the .zip file that you would normally link in Arduino. Everything works so far with the at90can128 preset platformio provides, but there may be issues/other functions that don't work.
If you want to build your own project, it's important to have the pins_arduino.h file in variants/can485 and use this in the .ini file
```
  [env:can485]
platform = atmelavr
board = at90can128
framework = arduino
upload_flags =
    -xbootsize=2048  ; Neccessary for Sparkfun AST-CAN485
build_flags = -I variants/Can485  ; pin definitions
monitor_speed = 9600
```