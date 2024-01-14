*Note: Any time I say MAD, I am refering to the memory address.

# Overview

Magnetic-core memory was an early form of non-volatile random access memory (RAM) used in early computing, notably the Apollo Guidance Computers. I am working with a friend to create our own magnetic core memory to interface with an 8-bit computer. This repository contains the code, schematics, and PCBs we make. Currently, we have made the schematic and PCB for the logic component of the memory as well as code to emulate the rest of the memory and interface with computers through serial. This is still a work in progress and has a long way to go but I thought I would share what I have.

# Schematics and PCBs

I used EasyEDA to design the schematics and PCBs so the .json files located in the [CAD](./CAD/) folder can be imported into EasyEDA if you want to edit them. If you just want to order the PCB, you can find the files in the [order](./CAD/order/) folder. I used JLCPCB so the BOM and Pick and Place files may not work with other services but you should be able to get a bare PCB from most places with the Gerber zip.

# Code

Because I only have the logic board right now, I coded an emulator to run on an MSP430F5529 LaunchPad to fill in for the rest of the memory. If you would like to do the same, the code is in the [Memory Emulator](./Memory%20Emulator/) folder as a Platformio project. You can compile it for different boards by editing the [platformio.ini](/Memory%20Emulator/platformio.ini) file but make sure to update the pin definitions at the top of [main.cpp](/Memory%20Emulator/src/main.cpp).

I also included emulator code that interfaces with the memory and provides a serial interface that can be controlled through [code](./Test%20Scripts/mem_tester.py) or by a terminal. The code is in the [Computer Emulator](./Computer%20Emulator/) folder and can be compiled and uploaded using Platformio as well. 

The delays in both projects are calibrated for using an Arduino Uno with a LaunchPad. If the boards you are using are faster or slower than mine, you may need to edit some delays. The delays you need to edit will have comments next to them that end with OK. Both emulators also have a fast and regular mode with different delays. The regular mode is more verbose so if you need help getting your memory working, I would recommend tuning in regular mode first.

# Building the Memory

Since I only have the logic board right now, there is not much assembly required. Once the PCB is assembled, connect everything on the left side (the one with the header labeled bus) to the Arduino (computer emulator) and everything else to the LaunchPad (memory emulator). I only connected half of the MADX and MADY pins (the first half of the top and bottom headers) because I didn't have enough IO and it made commands only 1 byte. If you wish to use the full 256 bytes instead of only 64, you will need to edit the code a little.

TIP: You can use ethernet cables to carry bus data as it has 8 conductors in it.

# Using the Memory

Once it is built, you can connect it to any computer and use a command line-like interface through serial to manually read and write to the memory. The commands are:
```
r <MAD> (Reads from the specified MAD)
w <MAD> <DATA> (Writes DATA to the specified MAD)
d <FUNCTION> <DISABLE> (Disables or enables the specified FUNCTION (0 for MAD or 1 for Write))
f (Switch to fast mode for faster memeory access and script interface)
t (Switch to test mode to auto test RAM)
```
The disable commands are not too intuitive but in a nutshell:

* ```d 0 0```: Enables MADX and MADY and allows you to read and write from the RAM.
* ```d 0 1```: Disables MADX and MADY meaning you can not address any memory location to read or write.
* ```d 1 0```: Enables the write bus which allows the ram to write data (function normally)
* ```d 1 1```: Disables the write bus which means any address you write to or even read from will become 0. 

## Fast Mode
IMPORTANT: If you are using fast mode and my memory emulator, make sure you switch the memory emulator to fast mode as well. (by default on the launchpad, it is controlled by button 1 and active when the red LED is on)

Fast Mode allows for a faster and more script-friendly interface where you use raw bytes to communicate with the RAM. Every command is 1 byte that takes the form 0bDDDDDDCC. The C bits are the command and the D bits are the data. Some commands such as write may also need a second byte for additional data. The commands are:
* ```00```: The read command. Put the target MAD in the 6 data bits. (this format will only work for 64 bytes and less) It will return a byte with the data at the MAD
* ```01```: The write command. Put the target MAD in the 6 data bits and send another byte with the data you want written. It should return a 0 byte indicating it is finished.
* ```10```: The disable command. Use the first data bit (least significant) to specify the FUNCTION and the second bit to specify whether to disable it. Similar to the d command. It should return a 0 byte.
*```11```: Fast Mode command. If the first data bit is set, it will exit fast mode. If it is not set, it will return the magic number (10) validating it is in fast mode. This is mostly for scripts to ensure it is in fast mode.

You can find an example of a script interfacing with the RAM in this [code](./Test%20Scripts/mem_tester.py)

## Test Mode
Test mode will continually generate a random array of bytes to write and read from memory to ensure it is working. This mode helps calibrate the delays as well as gives you useful statistics such as error rate and bandwidth. It can be run in both regular and fast mode but I would suggest tuning everything and getting a low error rate in regular mode first. Also note, that to exit test mode, you need to restart the microcontroller.

# Future
I will continue to work on this project with my friend but any feedback or contributions from the community are welcome. My goals going forward are:
* Improve the build guide and make some more code examples
* Work on the rest of the RAM
* Build a working stand-alone board with magnetic cores (replace memory emulator)