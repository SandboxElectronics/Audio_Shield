/* 
Description:
This is a demo code for Sandbox Electronics' WT2000 audio play/record shield.
You can get one of those products on 
http://sandboxelectronics.com

This demo code requires a sim900 module/shield connect to the Arduino to process 
the incoming phone call. 
You can get a sim900 shield on 
http://sandboxelectronics.com

This code demonstrates the DTMF decoding and audio playing/recording ability of the shield. 
Version:
V0.1

Release Date:
2014-02-16

Author:
Tiequan Shao          info@sandboxelectronics.com

Lisence:
CC BY-NC-SA 3.0

Please keep the above information when you use this code in your project. 
*/


#include "SIM900.h"
#include <SoftwareSerial.h>
#include "sms.h"
#include "call.h"
#include <Wire.h>
#include <SC16IS750.h>
#include <string.h>
#include <WT2000.h>


CallGSM call;
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



byte stat=0;
uint8_t DTMFContext = DTMF_CONTEXT_IDLE;
uint8_t idle_flag = 1; 
void setup() 
{
    Serial.begin(9600);
    
    //SIM900 GSM Initialization
    if (gsm.begin(2400)) {
        Serial.println("GSM status=READY");
    } else {
        Serial.println("GSM status=IDLE");
    }
    // UART to Serial Bridge Initialization
    i2cuart.begin(9600);               //baudrate setting
    i2cuart.pinMode(PIN_Q1,INPUT);     //DTMF decoding input 1
    i2cuart.pinMode(PIN_Q2,INPUT);     //DTMF decoding input 2
    i2cuart.pinMode(PIN_Q3,INPUT);     //DTMF decoding input 3
    i2cuart.pinMode(PIN_Q4,INPUT);     //DTMF decoding input 4
    i2cuart.pinMode(PIN_STD,INPUT);    //DTMF data valid input
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
    i2cuart.SetPinInterrupt(0x10);             //Set Pin change Interrupt; GPIO4 pin change interrupt enabled; GPIO4 is connected to STD of the DTMF decoder  
    i2cuart.GPIOLatch(1);
    myaudioshield.mode(WT2000_MODE_SINGLE);    //Set play mode to single shot mode
    myaudioshield.channel(WT2000_CHANNEL_AUX);    //Set Aux input as the recording channel; AUX R - Microphone (if audio channel selection == 1) 
                                                  //                                        AUX R - 2.5V (if audio channel selection == 0) 
                                                  //                                        AUX L - SIM900 Audio Output

};

void loop() 
{
  
  stat=call.CallStatus(); 

  if(stat==CALL_INCOM_VOICE){                 
    
    call.PickUp();  //pick up if there is any incoming call
    delay(1500);
    
  } else if (stat==CALL_ACTIVE_VOICE)  {
    DTMFParser();
  } else if (stat==CALL_NONE) {
      if (idle_flag == 0) {
          DTMFContext = DTMF_CONTEXT_IDLE;
          myaudioshield.stop();
          i2cuart.digitalWrite(PIN_AUDIO_SWITCH,0);
      }
      idle_flag = 1;
  }
  
};




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

void DTMFParser(void)
{
  uint8_t key_value;  
  switch(DTMFContext) {
        case DTMF_CONTEXT_IDLE:
            idle_flag = 0;
            myaudioshield.play("MENU","TRACK");
            DTMFContext = DTMF_CONTEXT_MAIN;
            break;
        case DTMF_CONTEXT_MAIN:
            key_value = DTMFGetKey();
            switch (key_value) {
                case DTMF_KEY_NULL:
                
                    break;
                case DTMF_KEY_1:
                    myaudioshield.play("DTMF","TRACK");
                    DTMFContext = DTMF_CONTEXT_DECODING_CHECK;
                    break;
                case DTMF_KEY_2:
                    
                    DTMFContext = DTMF_CONTEXT_RECORDING;
                    break;
                case DTMF_KEY_3:
                    DTMFContext = DTMF_CONTEXT_PLAY_BACK;
                    break;
                case DTMF_KEY_4: 
                    myaudioshield.play("REPR","TRACK");
                    delay(3000);
                    DTMFContext = DTMF_CONTEXT_REPRESENTATIVE;
                    i2cuart.digitalWrite(PIN_AUDIO_SWITCH,1); 
                    break;
                case DTMF_KEY_HASH:
                    call.HangUp(); 
                default:
                    break;  
            }
            break;
       case DTMF_CONTEXT_DECODING_CHECK:
       
            key_value = DTMFGetKey();
            switch (key_value) {
                case DTMF_KEY_NULL:
                    Serial.println("No key pressed");
                    break;
                case DTMF_KEY_1:
                    Serial.println("Key 1 pressed");
                    myaudioshield.play("1","TRACK");
                    break;
                case DTMF_KEY_2:
                    Serial.println("Key 2 pressed");
                    myaudioshield.play("2","TRACK");
                    break;
                case DTMF_KEY_3:
                    Serial.println("Key 3 pressed");
                    myaudioshield.play("3","TRACK");
                    break;
                case DTMF_KEY_4:
                    Serial.println("Key 4 pressed");                
                    myaudioshield.play("4","TRACK");
                    break;
                case DTMF_KEY_5:
                    Serial.println("Key 5 pressed");
                    myaudioshield.play("5","TRACK");
                    break;
                case DTMF_KEY_6:
                    Serial.println("Key 6 pressed");
                    myaudioshield.play("6","TRACK");
                    break;
                case DTMF_KEY_7:
                    Serial.println("Key 7 pressed");
                    myaudioshield.play("7","TRACK");
                    break;
                case DTMF_KEY_8:
                    Serial.println("Key 8 pressed");
                    myaudioshield.play("8","TRACK");
                    break;
                case DTMF_KEY_9:
                    Serial.println("Key 9 pressed");                
                    myaudioshield.play("9","TRACK");
                    break;
                case DTMF_KEY_0:
                    Serial.println("Key 0 pressed");
                    myaudioshield.play("0","TRACK");
                    break;
                case DTMF_KEY_ASTERISK:
                    Serial.println("Key * pressed");
                    myaudioshield.play("ASTER","TRACK");
                    break;
                case DTMF_KEY_HASH:
                    Serial.println("Key # pressed"); 
                    myaudioshield.play("HASH","TRACK");
                    delay(1000);
                    DTMFContext = DTMF_CONTEXT_IDLE;
                    break;                   
                default:
                    break;  
            };
            break;
       case DTMF_CONTEXT_RECORDING:
           myaudioshield.remove(0x0001,"REC00");
           myaudioshield.play("RECRD","TRACK");
           delay(2400);
           myaudioshield.play("BEEP","TRACK");
           delay(2000);
           myaudioshield.record(0x0001,"REC00");
           delay(10000);
           myaudioshield.finish();
           myaudioshield.play("BEEP","TRACK");
           delay(2000);
           key_value = DTMFGetKey();     //clear any key during the recording.
           DTMFContext = DTMF_CONTEXT_IDLE;
           break;
       case DTMF_CONTEXT_PLAY_BACK:
           myaudioshield.play(0X0001,"REC00");
           delay(10000);
           myaudioshield.stop();
           DTMFContext = DTMF_CONTEXT_IDLE;
           break;
       case DTMF_CONTEXT_REPRESENTATIVE:
           i2cuart.digitalWrite(PIN_AUDIO_SWITCH,1);
           break;
           
       default:
           break;
  }
 
        
                          
            
            
            
                

            



}

