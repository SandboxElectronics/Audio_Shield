/* 
Description:
This is a demo code to test Sandbox Electronics' WT2000 audio play/record shield.
You can get one of those products on 
http://sandboxelectronics.com


Please check your SDA and SCL wiring before you run this sketch. 
Please confirm that you have connected the lower pink and the lower lime connector with the wire provided with the shield.
Version:
V0.1

Release Date:
2014-04-06

Author:
Tiequan Shao          info@sandboxelectronics.com

Please keep the above information when you use this code in your project. 
*/


#include <Wire.h>
#include <SC16IS750.h>
#include <string.h>
#include <WT2000.h>



SC16IS750 i2cuart = SC16IS750(SC16IS750_PROTOCOL_I2C,SC16IS750_ADDRESS_BA);
WT2000 myaudioshield = WT2000(&i2cuart);


#define         PIN_Q1                                  (0)
#define         PIN_Q2                                  (1)
#define         PIN_Q3                                  (2)
#define         PIN_Q4                                  (3)
#define         PIN_STD                                 (4)
#define         PIN_AUDIO_SWITCH                        (5)


#define       DTMF_CONTEXT_IDLE               (0) 
#define       DTMF_CONTEXT_MAIN               (1)
#define       DTMF_CONTEXT_DECODING_CHECK     (2) 
#define       DTMF_CONTEXT_RECORDING          (3)
#define       DTMF_CONTEXT_PLAY_BACK          (4)
#define       DTMF_CONTEXT_REPRESENTATIVE     (5)

#define       DTMF_KEY_0                     (10)
#define       DTMF_KEY_1                     (1)
#define       DTMF_KEY_2                     (2)
#define       DTMF_KEY_3                     (3)
#define       DTMF_KEY_4                     (4)
#define       DTMF_KEY_5                     (5)
#define       DTMF_KEY_6                     (6)
#define       DTMF_KEY_7                     (7)
#define       DTMF_KEY_8                     (8)
#define       DTMF_KEY_9                     (9)
#define       DTMF_KEY_ASTERISK              (11)
#define       DTMF_KEY_HASH                  (12)
#define       DTMF_KEY_A                     (13)
#define       DTMF_KEY_B                     (14)
#define       DTMF_KEY_C                     (15)
#define       DTMF_KEY_D                     (0)
#define       DTMF_KEY_NULL                  (16)


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
    i2cuart.ModemPin(1);                       //Set GPIO4-GPIO7 as GPIO pins
    i2cuart.InterruptControl(0);               //
    i2cuart.SetPinInterrupt(0x10);             //Set Pin change Interrupt; GPIO0-GPIO3 pin change interrupt enabled;  
    i2cuart.GPIOLatch(1);
    
    
    myaudioshield.mode(WT2000_MODE_SINGLE);    //Set play mode to single shot mode
 
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
	
    if (!strcmp(command_buffer,"TEST")) {


		if (i2cuart.ping() !=1) {
			Serial.println("Error1: Can not connnect to SC16IS750");
			Serial.println("Please check the connectivity of SDA-A4, and SCL-A5 if you are a Uno Board.");
			Serial.println("You may need to connect A4 to SDA and A5 to SCL with wires if your board does not have SCL and SDA broke out.");
			while(1);
		} else {
			// Serial.println("Connected to SC16IS750");
       
		} 
		
        myaudioshield.play("1","TONES");
        delay(1200);
        if (DTMFGetKey() != DTMF_KEY_1) {
            Serial.println("Error2: DTMF decoding error!");
            Serial.println("Please check the connectivity of the lower pink and lower lime connector");
            while(1);
        }
        
        myaudioshield.play("2","TONES");
        delay(1200);
        if (DTMFGetKey() != DTMF_KEY_2) {
            Serial.println("Error2: DTMF decoding error!");
            Serial.println("Please check the connectivity of the lower pink and lower lime connector");
            while(1);
        }

        myaudioshield.play("4","TONES");
        delay(1200);
        if (DTMFGetKey() != DTMF_KEY_4) {
            Serial.println("Error2: DTMF decoding error!");
            Serial.println("Please check the connectivity of the lower pink and lower lime connector");
            while(1);
        }        
        
        myaudioshield.play("8","TONES");
        delay(1200);
        if (DTMFGetKey() != DTMF_KEY_8) {
            Serial.println("Error2: DTMF decoding error!");
            Serial.println("Please check the connectivity of the lower pink and lower lime connector");
            while(1);
        }

        myaudioshield.play("D","TONES");
        delay(1200);
        if (DTMFGetKey() != DTMF_KEY_D) {
            Serial.println("Error2: DTMF decoding error!");
            Serial.println("Please check the connectivity of the lower pink and lower lime connector");
            while(1);
        }
        
        Serial.println("The shield is OK");
   
    }
   
    command_buffer[0] = 0;

}





uint8_t DTMFGetKey(void)
{
   uint8_t key_value;
   uint8_t irq_id;
   irq_id = i2cuart.InterruptPendingTest();
   if ( (irq_id&0x01) != 0 ) { //no valid data
     return DTMF_KEY_NULL;  
   } else {
    key_value = i2cuart.GPIOGetPortState();
    key_value &= 0x0F;
    return key_value;
   }
     
   
}



