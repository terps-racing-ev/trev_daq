# DAQ
## Using Platformio
- Platformio is essentially the Arduino interface but allows for much better file organization and version control
- Install VSCode and the Platformio extension

## Uploading Code
- In the ```platformio.ini``` file, change ```src_dir = src/FRboard``` to the name of the folder of the board you want to upload. Use the misc folder for testing

## Code Structure
- Each sensor has it's on header file that contains the code to interpret the signals
- All sensors inherit the abstract class ```sensor.h```
  - void init(pin, CAN ID, Optional interrupt function)
  - float calculate()
    - All sensors call tx(pointer to data, size of data) which automatically wraps the data in the CAN struct and sends it
  - use the '''mapFloat''' utility function for analog mappings since Arduino's map only supports ints
- Interrupts use ```EIMSK &= ~(1 << digitalPinToInterrupt(pin));``` instead of ```sei``` and ```cli``` to avoid affecting all interrupts
  - Interrupt Handlers must be static functions, so you will have to declare a static wrapper function in ```main.cpp``` that simply calls the object's interrupt handler (see wheelspeed)

Also, ```pins_arduino.h``` has been modified due to not being able to use the .zip file that you would normally link in Arduino. Everything works so far with the at90can128 preset platformio provides, but there may be issues/other functions that don't work.
