This project consists of code and a circuit for expanding the MIDI functionality of the BOSS RC-5 looper station specifically, however this unit can be used to add MIDI control of any button that is actuated by being shorted to ground.
The Boss RC-5 has certain basic features which have been excluded from its MIDI functionality, and can only be executed by physically pressing the buttons on the unit. This modification allows for any function assigned to the physical buttons to be executed via MIDI.

The following youtube video contains detailed instructions for installation:

The Microcontroller used in this project is an ATTINY85, which can be programmed using a hardware programmer or a repurposed Arduino Uno. There are no libraries included, however there is a core that must be added to the board manager, which can be downloaded from the following URL:
https://raw.githubusercontent.com/damellis/attiny/ide-1.6.x-boards-manager/package_damellis_attiny_index.json

This is a good resource for using an UNO to program a Tiny85:
https://www.youtube.com/watch?v=30rPt802n1k

This is an intermediate project, and not recommended for full-on beginners. 

This project can be completed with a custom PCB (gerber files included here) or with a strip-board version of the circuit. A diagram for a strip-board layout is included in the files for this project. If you want to save some time and money, this option should be perfectly functional if you use a high quality strip-board. 

There are a number of solder lugs on the Boss RC-5 that must be connected to their corresponding solder points on the Midi upgrade circuit. The mechanical switch that disconnects the battery harness from the power supply must be permenantly closed using a peice of jumper wire. This allows the battery harness wires to remain connection to power and ground when a DC power supply is inserted. The battery harness wires will then power the upgrade circuit.

Midi Upgrade Circuit:

![Midi and Power Lugs](Reference%20Photos/Midi%20and%20Power%20Lugs.png)
