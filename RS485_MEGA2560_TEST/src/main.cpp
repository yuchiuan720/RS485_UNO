#include <Arduino.h>
#include <SoftwareSerial.h>

/* SoftSerial RX Support Pin 
 * OK:50,51,52,53,  10, 11, 12, 13, A8, A9, A10, A11, A12, A13, A14, A15   
 * NG:14, 15, 
 */
#define SSerialRX        10 //2.0 0 
#define SSerialTX        18  //2.0 1
#define SSerialTxControl 41  //2.0 2
#define RS485Transmit    HIGH
#define RS485Receive     LOW
#define DebugBaud 9600
/*-------+------------+------------+------------+------
 * baud  | 9600       | 19200      | 115200     |
 * ------+------------+------------+------------+-------
 * 1s    | good(46ms) | good(32ms) | good(50ms) |
 * 0.5s  | good(48ms) | good(47ms) | good(30ms) |
 * 0.1s  | good(60ms) | good(45ms) | good(47ms) |
 * 0.09s | good(60ms) | good(56ms) | good(34ms) |
 * 0.08s | good(63ms) | good(63ms) | good(32ms) |
 * 0.07s | good(60ms) | good(61ms) | good(43ms) |
 * 0.06s | miss(58ms) | miss(60ms) | good(30ms) |
 * 0.05s | miss(59ms) | miss(57ms) | good(29ms) |
 * 0.04s | Unstable   | miss(27ms) | good(29ms) |
 * 0.03s | lost       | Unstable   | Unstable   |
 * 0.02s | lost       | lost       | Unstable   |
 * 0.01s | lost       | lost       | lost       |
 * ------+------------+------------+------------+-------- 
 */
#define RS485Baud 19200
//SoftwareSerial RS485Serial(SSerialRX, SSerialTX); // RX, TX
#define RS485Serial Serial1
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

void SendData(int val){
   digitalWrite(SSerialTxControl, RS485Transmit);  // Enable RS485 Transmit
   RS485Serial.println(val);
   RS485Serial.flush();// Wait for everything to be transmitted, before disabling transmitter.
   digitalWrite(SSerialTxControl, RS485Receive);  // Disable RS485 Transmit
}

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
      }
      DEBUGSerial.println(str);
      
   }
}

void setup(){
   DEBUGSerial.begin(DebugBaud);
   while (!DEBUGSerial) {;/* wait for Serial port connected.*/}
   DEBUGSerial.println("Hello, Programmer!");
   
   pinMode(SSerialTxControl, OUTPUT);    
   digitalWrite(SSerialTxControl, RS485Transmit); // Init Transceiver
   RS485Serial.setTimeout(1);                     // Start the software serial port, to another device
   RS485Serial.begin(RS485Baud);                      // set the data rate 
   while(!RS485Serial) {;/* wait Serial prot connected. */}                          
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
   
}