#include <math.h>
#define uchar unsigned char
#define LED 5
uchar temp;             //define temperature parameter
uchar humi;               //define humidity parameter
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
      bgn:
      delay(200);
      pinMode(LED, OUTPUT);
      digitalWrite(LED, LOW);
      delay(20);
      digitalWrite(LED, HIGH);
      delayMicroseconds(40);
      digitalWrite(LED, LOW);
      pinMode(LED, INPUT);
      loopCnt = 10000;
      while(digitalRead(LED)!=HIGH)
      {
        if(loopCnt-- == 0)
        {
          Serial.println("HIGH");
          goto bgn;  
        } 
      }
      loopCnt = 10000;
      while(digitalRead(LED)!=LOW)
      {
        if(loopCnt-- == 0)
        {
          Serial.println("LOW");
          goto bgn;  
        } 
      }
      for(int i=0; i<40; i++)
      {
        while(digitalRead(LED)==LOW)
        {}
        times = micros();
        while(digitalRead(LED)==HIGH)
        {}
        if(micros()- times > 50)
        {
          chr[i] = 1;
        }  
        else
        {
          chr[i] = 0;  
        }
//        Serial.print(chr[i]);
      }
//      Serial.print("\n");
      humi = chr[0]*128+chr[1]*64+chr[2]*32+chr[3]*16+chr[4]*8+chr[5]*4+chr[6]*2+chr[7];
      temp = chr[16]*128+chr[17]*64+chr[18]*32+chr[19]*16+chr[20]*8+chr[21]*4+chr[22]*2+chr[23];
      Serial.print(temp);
      Serial.print(' ');
      Serial.print(humi);
      Serial.print("\n");
}
