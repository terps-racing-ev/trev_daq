# DAQ

[Sensor Organization](https://umd0.sharepoint.com/:x:/s/TeamsTerpsRacingEV/Ef79KA5FaFhJh21jqTwfWeUBTny6eTJU5965LJCH_cuU6w?e=4OiMr1)

## Using Platformio
- Platformio is essentially the Arduino interface but allows for much better file organization and version control
- Install VSCode and the Platformio extension


## Uploading Code
- In the ```platformio.ini``` file, change ```src_dir = src/CoolingBoard``` to the name of the folder of the board you want to upload. Use the Testing folder for testing

## Code Structure
- Each sensor is an object and has its own class
- All sensors either inherit the abstract class ```AnalogSensor``` or ```DigitalSensor``` and have their own type
  - Analog
    - ```update()``` just updates a moving average for filtered adc
    - ```calculate(&result)``` needs to be overwritten
  - Digital
    - ```update()``` updates the moving average for the last pulse time
    - ```calculate(&result)``` needs to be overwritten. Calling ```get_digital_average()``` isn't very intuitive so check it out
    - Interrupt Handlers must be static functions, so you will have to declare a static wrapper function in ```main.cpp``` that simply calls the object's interrupt handler (see wheelspeed)
- Each board's ```main.cpp``` is responsible for  organizing can messages

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

### DBC File
- This repo also containts a DBC file to be uploaded to race capture pro
