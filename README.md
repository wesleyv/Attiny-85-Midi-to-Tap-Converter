This project consists of code and a circuit for expanding the MIDI functionality of the BOSS RC-5 looper station specifically, however this unit can be used to add MIDI control of any button that is actuated by being shorted to ground.
The Boss RC-5 has certain basic features which have been excluded from its MIDI functionality, and can only be executed by physically pressing the buttons on the unit. This modification allows for any function assigned to the physical buttons to be executed via MIDI.

The following youtube video contains detailed instructions for installation of the MIDI upgrade circuit:
https://youtu.be/7GXlwmM-9fs
The Microcontroller used in this project is an ATTINY85, which can be programmed using a hardware programmer or a repurposed Arduino Uno. There are no libraries needed, however there is a core that must be added to the board manager, which can be downloaded from the following URL:
https://raw.githubusercontent.com/damellis/attiny/ide-1.6.x-boards-manager/package_damellis_attiny_index.json
See my tutorial video for more detailed installation instructions:

[![Video Thumbnail](Reference%20Photos/RC-5Videothumbnail.jpg)](https://youtu.be/7GXlwmM-9fs)

This is a good resource for using an UNO to program a Tiny85:

https://www.youtube.com/watch?v=30rPt802n1k

This is an intermediate project, and not recommended for full-on beginners. 

This project can be completed with a custom PCB (gerber files included here) or with a strip-board version of the circuit. A diagram for a strip-board layout is included in the files for this project. If you want to save some time and money, this option should be perfectly functional if you use a high quality strip-board. 

There are a number of solder lugs on the Boss RC-5 that must be connected to their corresponding solder points on the Midi upgrade circuit. The mechanical switch that disconnects the battery harness from the power supply must be permenantly closed using a piece of jumper wire. This allows the battery harness wires to remain connected to power and ground when a DC power supply is inserted. The battery harness wires will then power the upgrade circuit.

Modification Circuit:

![Mod Circuit Photo](Reference%20Photos/Color%20Coded%20Connection%20to%20PCB.png)

Power Modification and Midi Data Connections on the RC-5:

![Midi and Power Lugs](Reference%20Photos/Midi%20and%20Power%20Lugs.png)

Solder lug on RC-5 for the main switch:

![Main Switch Lug](Reference%20Photos/Main%20Switch%20Lug.png)

Unfortunately I was not able to get good photos of the solder lugs for the Tip/Ring of the auxiliary button port, but they can be found relatively easily using a multimeter in continuity mode, by plugging in a TRS cable and testing one end of the cable while probing the internal jack socket.

Keep in mind that the stripboard layout shows the components and copper trace cuts together in one image, but in real life the copper strips and corresponding trace cuts will be on the back of the board, meaning that the trace cut pattern will be mirrored in relation to the diagram.

![Midi and Power Lugs](Reference%20Photos/Attiny85%20Midi%20to%20Tap%20-%20stripboard%20layout.png)

28 guage stranded core wire recommended. Removal of a plastic tab in the battery compartment is recommended to allow wires to pass from the compartment to their various solder points in the RC-5.

Tab before removal:
![Tab before removal](Reference%20Photos/Tab%20to%20be%20Removed.png)

Tab after removal:
![Tab after removal](Reference%20Photos/Tab%20Removed.png)

To access the code in the Arduino IDE, download the folder labeled "attinymidi" and keep all the files in a folder with that specific name. Open "attinymidi.ino" in the Arduino IDE and the the other files will be automatically referenced by the project. Use the IDE to set the midi channel of the upgrade circuit to match the midi channel that the RC-5 is configured to listen to.

To upload Gerber files to your board manufacturer of choice, make sure that the contents of the Gerber File folder are contained in Zip file. The files must be uploaded to the board manufacturer in a single Zip file.
