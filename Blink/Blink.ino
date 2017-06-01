#include <math.h>
#define uchar unsigned char
#define LED 5
uchar temp;             //define temperature parameter
uchar humi;             //define humidity parameter
int tol;                //define jiaoxian 
unsigned int loopCnt;   //loop counter
int chr[40] = {0};      //define a array to store 40 bits
unsigned int times;     //define times parameter
uchar inByte = 10;      //receive data from python script

void setup() {
  Serial.begin(9600);               //Set Baud Rate
  pinMode(LED, OUTPUT);             // Initialize the LED_BUILTIN pin as an output
}

// the loop function runs over and over again forever
void loop() {
  if( Serial.available() > 0)               //receive buffer data more than zero
  {             
      inByte = Serial.read();               //receive serial port data
      Serial.write(inByte);
      delay(100);
  }
      bgn:                                  //define a label
      delay(200);
      pinMode(LED, OUTPUT);                 //output mode
      digitalWrite(LED, LOW);               //output low level 20ms
      delay(20);
      digitalWrite(LED, HIGH);              //output high level 40us
      delayMicroseconds(40);                
      pinMode(LED, INPUT);                  //input mode
      loopCnt = 10000;                      //DHT response signal counter
      while(digitalRead(LED)!=HIGH)         //judge DHT low response signal
      {
        if(loopCnt-- == 0)
        {
          Serial.println("HIGH");
          goto bgn;  
        } 
      }
      loopCnt = 10000;
      while(digitalRead(LED)!=LOW)          //judge DHT high response signal
      {
        if(loopCnt-- == 0)
        {
          Serial.println("LOW");
          goto bgn;  
        } 
      }
      for(int i=0; i<40; i++)               //receive 40 characters data
      {
        while(digitalRead(LED)==LOW)        //judge high level width
        {}
        times = micros();
        while(digitalRead(LED)==HIGH)
        {}
        if(micros()- times > 50)            //high level = 70us denotes "1".
        {
          chr[i] = 1;
        }  
        else                                //high level = 26-28us denotes "0"
        {
          chr[i] = 0;  
        }
      }
      humi = chr[0]*128+chr[1]*64+chr[2]*32+chr[3]*16+chr[4]*8+chr[5]*4+chr[6]*2+chr[7];
      temp = chr[16]*128+chr[17]*64+chr[18]*32+chr[19]*16+chr[20]*8+chr[21]*4+chr[22]*2+chr[23];
      Serial.write(temp);                   //return temperature data to python script
      Serial.write(humi);                   //return humidity data to python script
}
