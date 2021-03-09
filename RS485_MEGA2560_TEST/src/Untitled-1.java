#include <Arduino.h>
//Sample using LiquidCrystal library
//#include <Wire.h>  
#include <SoftwareSerial.h>


//#define RS485Serial Serial
//#define USBSerial Serial
/* SoftSerial RX Support Pin 
 * OK:50,51,52,53,  10, 11, 12, 13, A8, A9, A10, A11, A12, A13, A14, A15   
 * NG:14, 15, 
 */
#define SSerialRX        53 //2.0 0 
#define SSerialTX        52  //2.0 1
#define SSerialTxControl 41  //2.0 2
#define RS485Transmit    HIGH
#define RS485Receive     LOW

SoftwareSerial RS485Serial(SSerialRX, SSerialTX); // RX, TX
//#define RS485Serial Serial3
#define DEBUGSerial Serial
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
void SendMsg(String str){
   digitalWrite(SSerialTxControl, RS485Transmit);  // Enable RS485 Transmit
   //delayMicroseconds(10);
   DEBUGSerial.println(str);
   DEBUGSerial.flush();// Wait for everything to be transmitted, before disabling transmitter.
   digitalWrite(SSerialTxControl, RS485Receive);  // Disable RS485 Transmit
   //delayMicroseconds(10);
}
void bugMsg(String str){
   str.trim();
   if (str == "")
   {
      return;
   }
   
   //digitalWrite(SSerialTxControl, RS485Transmit);  // Enable RS485 Transmit
   //delayMicroseconds(10);
   DEBUGSerial.print("str:'");
   DEBUGSerial.print(str);
   DEBUGSerial.println("'");
   DEBUGSerial.flush();// Wait for everything to be transmitted, before disabling transmitter.
   //digitalWrite(SSerialTxControl, RS485Receive);  // Disable RS485 Transmit
   //delayMicroseconds(10);
   delay(100);
}

void SendData(int val){
   digitalWrite(SSerialTxControl, RS485Transmit);  // Enable RS485 Transmit
   //delayMicroseconds(10);
   RS485Serial.println(val);
   RS485Serial.flush();// Wait for everything to be transmitted, before disabling transmitter.
   digitalWrite(SSerialTxControl, RS485Receive);  // Disable RS485 Transmit
   //delayMicroseconds(10);
}
/*
void serialEvent0(){
   String str;
   while(RS485Serial.available()){

      str = str + RS485Serial.readString();
      delayMicroseconds(10);
      Serial.println(str);
      while(1)
      {  
         int index = str.indexOf("#");
         if(index > 0){
            String substr = str.substring(index,index+1 );
            str.remove(index, index+1);
            if(substr.equals(ID_A)){
               SendData(sr1_value_in);
            }else if(substr.equals(ID_B)){
               SendData(sr2_value_in);
            }else if(substr.equals(ID_C)){
               SendData(sr3_value_in);
            }else if(substr.equals(ID_D)){
               SendData(sr4_value_in);
            }else{
               SendMsg("");
            }
         }else{
            Serial.println("err");
            break;
         }
      }
   }
}
*/

void serialEvent(){
   String str;
   while(RS485Serial.available()){
      str = RS485Serial.readString();
      delayMicroseconds(10);
      if(str.equals(ID_A)){
         SendData(sr1_value_in);
      }else if(str.equals(ID_B)){
         SendData(sr2_value_in);
      }else if(str.equals(ID_C)){
         SendData(sr3_value_in);
      }else if(str.equals(ID_D)){
         SendData(sr4_value_in);
      }else{
         //SendMsg("Err1");
      }
   }
   
      //Serial.print("str:'"); Serial.print(str); Serial.println("'");
      //Serial.print("ID_A:'"); Serial.print(ID_A); Serial.println("'");
      //Serial.print("ID_B:'"); Serial.print(ID_B); Serial.println("'");
      //Serial.print("ID_C:'"); Serial.print(ID_C); Serial.println("'");
      //Serial.print("ID_D:'"); Serial.print(ID_D); Serial.println("'");
}

/*
void serialEvent(){
   //Look for data from other Arduino 
   String str;
   if (!RS485Serial.available())
   {
      return;
   }
   
   char preview = RS485Serial.peek();
   if(preview == '#'){
      //char c;
      //Serial.println("Now i try read serial.available");
      //int count = 0;
      while(RS485Serial.available()){
         //count++;
         //c = RS485Serial.read();
         //str.concat(c);
         //delay(1);
         //if(str.length() >= 3){
         //   break;
         //}
         //count++;
         str = RS485Serial.readString();
         //str = RS485Serial.readStringUntil(1);
         delayMicroseconds(1);
      }
      //Serial.println(count);
      //Serial.println("Now i finish read serial.available");
      //str.trim();
      //Serial.print("str:'"); Serial.print(str); Serial.println("'");
      if(str.equals(ID_A)){
         SendData(sr1_value_in);
         //Serial.println("you need #1?");
      }else if(str.equals(ID_B)){
         SendData(sr2_value_in);
         //Serial.println("you need #2?");
      }else if(str.equals(ID_C)){
         SendData(sr3_value_in);
         //Serial.println("you need #3?");
      }else if(str.equals(ID_D)){
         SendData(sr4_value_in);
         //Serial.println("you need #4?");
      }else{
         SendMsg("Err1");
      }
      Serial.print("str:'"); Serial.print(str); Serial.println("'");
      Serial.print("ID_A:'"); Serial.print(ID_A); Serial.println("'");
      Serial.print("ID_B:'"); Serial.print(ID_B); Serial.println("'");
      Serial.print("ID_C:'"); Serial.print(ID_C); Serial.println("'");
      Serial.print("ID_D:'"); Serial.print(ID_D); Serial.println("'");
   }else{
      SendMsg("Err2");
   }
}
*/
void setup(){
   DEBUGSerial.begin(9600);
   while (!DEBUGSerial)
   {
      ;/* code */
   }
   DEBUGSerial.println("Hello, Programmer!");
   
   pinMode(SSerialTxControl, OUTPUT);    
   digitalWrite(SSerialTxControl, RS485Transmit); // Init Transceiver
   delay(1);
   RS485Serial.setTimeout(1);                     // Start the software serial port, to another device
   RS485Serial.begin(19200);                      // set the data rate 
   while(!RS485Serial){
      ;// wait Serial prot connected
   }                          
   RS485Serial.println("Hello, I am Echo. ");
   RS485Serial.println("Tell me what you need. ");
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
   serialEvent();
   
   //if (millis() - old_time > 1000){
   //   digitalWrite(13, HIGH);
   //   old_time = millis();
   //}else if(millis() - new_time > 1000){
   //   digitalWrite(13, LOW);
   //   new_time = millis();
   //}
}//