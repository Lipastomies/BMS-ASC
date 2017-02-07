# BMS-ASC
BMS (BeatMania Simulator) (Arcade Style) Controller, powered by Arduino Leonardo.

## Dependencies
The controller uses [NicoHood's HID library](https://github.com/NicoHood/HID) for Arduino.

## Usage


## Controller Structure
BMS gameplay consists of the player hitting keys and turning a turntable according to the notes the game presents. This controller is designed to be semi-accurate to its arcade counterpart (Beatmania IIDX series by Konami). It has the same button placement and the same distance from the buttons to the turntable, which makes it similar to play either with the controller or in an arcade. Since the real arcade cabinet has two different sides to play on, with the turntable on the left or on the right, it's possible to turn the buttons upside down to change the playside. This does not affect the relative distances between the turntable and buttons.
The controller can be divided into four main subcomponents: The actual controller case, including the top and bottom plates, the button assembly, the turntable assembly, and the electronics. This project includes the design files to each of the subcomponents, a parts list, as well as general instructions for the building of this controller.

## The case
The case holds all of the electronics of the controller and provides a base for the mounting of the buttons and the turntable. The case consists of a top plate, the frame, and a removable bottom plate.

## Dimensions
The case has a top plate that is 528X270mm big. The top plate is sunk into the frame, so the case itself has a width of the top plate, as well as the frame material's width twice. In my case, I use 16mm thick MDF, which means the case is 560X302 mm big. Depending on the type and thickness of your case, this may vary. 

### Case materials
I made the case from MDF, but any kind of wood or plywood is acceptable. What matters is that the material is easy to work with.

### Frame
The frame consists of five pieces of 16mm MDF:
2X 560X80mm pieces for the upper and lower sides
2X 270X80mm pieces for the left and right sides
1X 270X60mm piece for a middle support.
These pieces are joined together using glue and buttjoints. The frame also has wooden blocks for threaded inserts to be inserted in it, so that the turntable and button assemblies can be attached to it.

### Top plate
The controller top plate acts as a base for our turntable and button assemblies. The top plate 

### Bottom Plate
The bottom plate keeps our controller's insides from harm's way. The bottom plate is attached to the rest of the case with machine screws and threaded inserts, so that it can be easily mounted and removed during building and assembly.

## Button assembly


## Turntable assembly

### Structure

## Electronics And wiring up the controller
The electronics for the controller are rather simple. The Arduino Leonardo board feeds power and reads the states of each of the buttons and the turntable, and translates these into keyboard button presses for the computer.
### Optical encoder 

### Buttons
The microswitches in the buttons have standard male 4.8mm spade connectors. Connect each microswitch to the Arduino ground pin and a button-specific pin on the arduino.
The buttons also have LEDs and light holders. The light holders have standard male 6.3mm spade connectors. Connect each LED to the Arduino ground pin and a LED-specific digital pin on the Arduino.

## License
The project is licensed under the MIT license.

