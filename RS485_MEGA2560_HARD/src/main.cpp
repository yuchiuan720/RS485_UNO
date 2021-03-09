#include <Arduino.h>

#define SSerialTxControl 41  //2.0 2
#define RS485Transmit    HIGH
#define RS485Receive     LOW

#define RS485Serial Serial1
String byteReceived;
String byteSend;

int ID_num;
String ID_A;
String ID_B;
String ID_C;
String ID_D;

unsigned long new_time;
unsigned long old_time;
unsigned long msg_time;

byte srPin1 = A1; // s1
byte srPin2 = A2; // s2
byte srPin3 = A3; // s3
byte srPin4 = A4; // s4

int sr1_value_in = 0;
int sr2_value_in = 0;
int sr3_value_in = 0;
int sr4_value_in = 0;

void SendData(String str){
   digitalWrite(SSerialTxControl, RS485Transmit);  // Enable RS485 Transmit
   digitalWrite(13, HIGH);
   RS485Serial.print(str);
   RS485Serial.flush();// Wait for everything to be transmitted, before disabling transmitter.
   digitalWrite(SSerialTxControl, RS485Receive);  // Disable RS485 Transmit
   digitalWrite(13, LOW);
}

void SendData(int val){
   digitalWrite(SSerialTxControl, RS485Transmit);  // Enable RS485 Transmit
   digitalWrite(13, HIGH);
   RS485Serial.print(val);
   RS485Serial.flush();// Wait for everything to be transmitted, before disabling transmitter.
   digitalWrite(SSerialTxControl, RS485Receive);  // Disable RS485 Transmit
   digitalWrite(13, LOW);
}

void serialEvent1(){
   String str;
   while(RS485Serial.available()){
      str = RS485Serial.readString();
      if(str.equals(ID_A)){
         SendData(sr1_value_in);
      }else if(str.equals(ID_B)){
         SendData(sr2_value_in);
      }else if(str.equals(ID_C)){
         SendData(sr3_value_in);
      }else if(str.equals(ID_D)){
         SendData(sr4_value_in);
      }else{
         SendData(str);
      }
   }
}

void setup(){
   pinMode(13, OUTPUT);
   pinMode(SSerialTxControl, OUTPUT);    
   digitalWrite(SSerialTxControl, RS485Transmit); // Init Transceiver
   delay(1);
   RS485Serial.setTimeout(2);                     // Start the software serial port, to another device
   RS485Serial.begin(19200);                      // set the data rate 
   while(!RS485Serial){;/* wait Serial prot connected*/ }                          
   RS485Serial.print("Hello, I am Echo. ");
   RS485Serial.print("Tell me what you need. ");
   RS485Serial.flush();                           // Wait for everything to be transmitted, before disabling transmitter.
   digitalWrite(SSerialTxControl, RS485Receive);  // Disable RS485 Transmit

   pinMode(srPin1,INPUT);
   digitalWrite(srPin1,HIGH);
   pinMode(srPin2,INPUT);  
   digitalWrite(srPin2,HIGH);
   pinMode(srPin3,INPUT);
   digitalWrite(srPin3,HIGH);
   pinMode(srPin4,INPUT);  
   digitalWrite(srPin4,HIGH);
   ID_num=1;
   ID_A="#"+String(ID_num);
   ID_B="#"+String(ID_num+1);
   ID_C="#"+String(ID_num+2);
   ID_D="#"+String(ID_num+3);
}

void loop(){
   sr1_value_in=analogRead(srPin1);
   sr2_value_in=analogRead(srPin2);
   sr3_value_in=analogRead(srPin3);
   sr4_value_in=analogRead(srPin4);
}