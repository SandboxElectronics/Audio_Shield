/* 
Description:
This is a demo code for Sandbox Electronics' WT2000 audio play/record shield.
You can get one of those products on 
http://sandboxelectronics.com

This code demonstrate that
You can get a sim900 shield on 
http://sandboxelectronics.com

This code demonstrates the playing and recording ability of the shield. 
Version:
V0.1

Release Date:
2014-02-21

Author:
Tiequan Shao          info@sandboxelectronics.com

Lisence:
CC BY-NC-SA 3.0

Please keep the above information when you use this code in your project. 
*/


#include <SPI.h>
#include <Wire.h>
#include <SC16IS750.h>
#include <string.h>
#include <WT2000.h>


SC16IS750 i2cuart = SC16IS750(SC16IS750_PROTOCOL_I2C,SC16IS750_ADDRESS_BA);
WT2000 myaudioshield = WT2000(&i2cuart);



#define         PIN_AUDIO_SWITCH                        (5)



void setup() 
{
    Serial.begin(9600);
    
    // UART to Serial Bridge Initialization
    i2cuart.begin(9600);               //baudrate setting
    i2cuart.pinMode(PIN_AUDIO_SWITCH,OUTPUT);  //Audio channel selection output. 0: Earphone R - WT2000 DACR ,
                                               //                                   Earphone L -WT2000 DACL
                                               //                                   Microphone - WT2000 MIC in
                                               //                                   WT2000 DACR - SIM900 Audio Input       
                                               //                                1: Earphone R - SIM900 Audio Ouput
                                               //                                   Earphone L - SIM900 Audio Ouput
                                               //                                   Microphone - SIM900 Audio Input
    i2cuart.digitalWrite(PIN_AUDIO_SWITCH,0);  //Set audio channel selection to 0, so sound played by WT2000 will be fed to earphone 
    myaudioshield.mode(WT2000_MODE_ALL_CYCLE);    //Set play mode to single shot mode
    myaudioshield.channel(WT2000_CHANNEL_MIC);    //Set MIC input as the recording channel; 

    Serial.println("Sandbox Electronics' Audio/DTMF Shield Demo Code");
    Serial.println("================================================");
    Serial.println("PLAY ------------------------------ Play a track");
    Serial.println("STOP ------------------------------ Stop playing");
    Serial.println("PREVIOUS ------------------------- Play previous");
    Serial.println("RECORD ---------------------------- Record track");
    Serial.println("FINISH ------------------------ Finish recording");
    Serial.println("================================================");    

	if (i2cuart.ping() !=1) {
		Serial.println("Error1: Can not connnect to SC16IS750");
		Serial.println("Please check the connectivity of SDA-A4, and SCL-A5 if you are a Uno Board.");
		Serial.println("You may need to connect A4 to SDA and A5 to SCL with wires if your board does not have SCL and SDA broke out.");
		while(1);
	} else {
		// Serial.println("Connected to SC16IS750");
       
	} 	
	
	
};

void loop() 
{
  
  SerialReadCommand();
  
}


void SerialReadCommand()
{

    char command_buffer[100];
    uint8_t index = 0;
  
    while((Serial.available() > 0) && (index<99)) {
        command_buffer[index++] = Serial.read();
	delay(50);
    }
	
    command_buffer[index++] = 0;
	
    if (!strcmp(command_buffer,"PLAY")) {
        Serial.println("Playing tracks...");
        myaudioshield.play("MENU","TRACK");
    }

    if (!strcmp(command_buffer,"NEXT")) {
        Serial.println("Playing next track...");
        myaudioshield.next();
    }

    if (!strcmp(command_buffer,"STOP")) {
        Serial.println("Stop playing...");
        myaudioshield.stop();
    }

    if (!strcmp(command_buffer,"PREVIOUS")) {
        Serial.println("Playing previous track...");
        myaudioshield.previous();
    }
    
    if (!strcmp(command_buffer,"RECORD")) {
        Serial.println("Recording...");  
        myaudioshield.remove(0x0001,"REC00");
        myaudioshield.record(0x0001,"REC00");  
        
    } 
 
    if (!strcmp(command_buffer,"FINISH")) {
        Serial.println("Finish recording...");
        myaudioshield.finish();

    }
    
    command_buffer[0] = 0;
    	
	
	
}


