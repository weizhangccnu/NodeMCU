#include <math.h>
#include <ESP8266WiFi.h>

#define uchar unsigned char
#define DATA  5
#define LED   16
uchar temp;             //define temperature parameter
uchar humi;             //define humidity parameter
int tol;                //define jiaoxian 
unsigned int loopCnt;   //loop counter
int chr[40] = {0};      //define a array to store 40 bits
unsigned int times;     //define times parameter
uchar inByte = 10;      //receive data from python script

//====================================================================//
const char* ssid = "CHI NA";                        //your network SSID (name)
const char* password = "hjk397124600HJK@qq.com";                  //you network password

//const char* host = "www.hao123.com";             //host ip address
const char* host = "192.168.0.100";             //host ip address
const char* streamId   = "....................";
const char* privateKey = "....................";

int value = 0;
//====================================================================//
void setup() {
  Serial.begin(9600);                   // Set Baud Rate
  pinMode(DATA, OUTPUT);                 // Initialize the LED_BUILTIN pin as an output
  pinMode(LED, OUTPUT);                    // Initial RXD0 pin

//  Serial.println();                   //we start by connecting to a WiFi network
//  Serial.println();     
  Serial.print("Connecting to ");

  WiFi.begin(ssid,password);

  while(WiFi.status() != WL_CONNECTED)
  {
      delay(500);
      Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

}
// the loop function runs over and over again forever
void loop() {
  if( Serial.available() > 0)               //receive buffer data more than zero
  {             
      inByte = Serial.read();               //receive serial port data
      Serial.write(inByte);
//      Serial.print(inByte);
//      Serial.print('\n');
//      digitalWrite(LED, LOW);
//      delay(1);
//      digitalWrite(LED, HIGH);
//      delay(1);
  }

//====================================================================//
//WiFi connection section
//====================================================================// 
  delay(1000);
  ++value;
  Serial.print("Connecting to ");
  Serial.println(host);
  WiFiClient client;                            //Use WiFiClient class to creat TCP connections
  const int httpPort = 8080;
  if(!client.connect(host, httpPort))
  {
    Serial.println("Connection failed");
    return;  
  }
  
//  String url = "/input/";                       //Create a URL for the request
//  url += streamId;
//  url += "?private_key=";
//  url += 
//====================================================================//
//DHT11 fetch data code section
//====================================================================// 
    bgn:                                  //define a label
    delay(200);
    pinMode(DATA, OUTPUT);                 //output mode
    digitalWrite(DATA, LOW);               //output low level 20ms
    delay(20);
    digitalWrite(DATA, HIGH);              //output high level 40us
    delayMicroseconds(40);                
    pinMode(DATA, INPUT);                  //input mode
    loopCnt = 10000;                      //DHT response signal counter
    while(digitalRead(DATA)!=HIGH)         //judge DHT low response signal
    {
      if(loopCnt-- == 0)
      {
        Serial.println("HIGH");
        goto bgn;  
      } 
    }
    loopCnt = 10000;
    while(digitalRead(DATA)!=LOW)          //judge DHT high response signal
    {
      if(loopCnt-- == 0)
      {
        Serial.println("LOW");
        goto bgn;  
      } 
    }
    for(int i=0; i<40; i++)               //receive 40 characters data
    {
      while(digitalRead(DATA)==LOW)        //judge high level width
      {}
      times = micros();
      while(digitalRead(DATA)==HIGH)
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
    Serial.print("Temperature:");
    Serial.print(temp);                   //return temperature data to python script
    Serial.print(' ');
    Serial.print("Humidity:");
    Serial.print(humi);                   //return humidity data to python script
    Serial.print('\n');
    String chr = String(temp)+String('|')+String(humi); 
    client.print(chr);
//    client.print();
//      uart.alt(1);
}
