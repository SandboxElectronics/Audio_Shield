Arduino Library for Sandbox Electronics MP3 Audio Shield [SLD-000022]
=====================================================================

Installation Instruction (For Arduino IDE version 1.0.5 and above):

1. Download the library as an Arduino IDE compatible zip file at https://github.com/SandboxElectronics/Audio_Shield/blob/master/Audio_Shield.zip?raw=true. (Do not unzip it. leave it as is)

2. In the Arduino IDE, select Sketch -> Import Library -> Add Library

3. Navigate to the zip file that was just downloaded - Audio_Shield.zip, and Click Open.

4. Return to the Sketch -> Import Library menu. You should now see the library at the bottom of the drop-down menu. It is ready to be used in your sketch.

5. As this library have a dependency on another Sandbox Electronics Library - UART_Bridge. The same steps 1-4 should be used to install that library if it is not already installed. The library can be downloaded at https://github.com/SandboxElectronics/UART_Bridge/blob/master/UART_Bridge.zip?raw=true.

The installed library should be under:
 - My Documents\Arduino\libraries\ (on Windows)
 - Documents/Arduino/libraries/ (on Mac or Linux)

Note: the above instruction is for automatic installation of 3rd party libraries that is supported starting at Arduino IDE 1.0.5. For users running earlier versions, manual installation is required. For manual installation instruction, please visit http://arduino.cc/en/Guide/Libraries. Please remember to close all opened Arduino IDE windows and restart the Arduino IDE if manual installation was used. Please make sure the new library appears in the Sketch -> Import Library menu item of the software.
