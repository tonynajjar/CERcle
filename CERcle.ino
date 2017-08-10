#include "FastLED.h"

// How many leds are in the strip?
#define NUM_LEDS_BOOSTER 2
#define NUM_LEDS_PS 16
#define NUM_LEDS_SPS 24
#define NUM_LEDS_LHC 60
#define NUM_LEDS_TUNNELS 112
#define NUM_LEDS_CONNECTIONS 8
#define NUM_LEDS_TOTAL 112
// Data pin that led data will be written out over
#define DATA_PIN_LOOPS 2
#define DATA_PIN_CONNECTIONS 3
// Clock pin only needed for SPI based chipsets when not using hardware SPI

#define BOOSTER_HIGHEST_SPEED 75
#define BOOSTER_LOWEST_SPEED 150

#include <WiFi.h>
#include <WiFiUdp.h>


int status = WL_IDLE_STATUS;
int base;
int starts[6]= {0,11,27,51,111};
int index=0;
int sensitivity=30;
unsigned int localPort = 4390;
unsigned long previousMillis = 0;
char packetBuffer[255]; //buffer to hold incoming packet
int variableStart= starts[0];
int variableEnd= starts[1] ;
int startLed;
int counter=0;
WiFiUDP Udp;
int nextLowSpeed= BOOSTER_LOWEST_SPEED;
int nextHighSpeed= BOOSTER_HIGHEST_SPEED;
int prevSpeed=0;
int currentSpeed=0;

int circumference[4]= {157,628,7000, 27000};
int energy[4]= {2, 25, 450, 6500 };
int electromagnets[4]= {66,277,1317,1624};
String names[4]= {"Proton Synchrotron Booster", "Proton Synchrotron", "Super Proton Synchrotron", "Large Hadron Collider"};
// This is an array of leds.  One item for each led in your strip.
CRGB tunnels[NUM_LEDS_TUNNELS];
CRGB connections[NUM_LEDS_CONNECTIONS];

// This function sets up the ledsand tells the controller about them
void setup() {
 

  Serial.begin(9600);
  
  FastLED.addLeds<NEOPIXEL, DATA_PIN_LOOPS>(tunnels, NUM_LEDS_TUNNELS);
  FastLED.addLeds<NEOPIXEL, DATA_PIN_CONNECTIONS>(connections, NUM_LEDS_CONNECTIONS);
  FastLED.setBrightness(255);
  turnOff();
 
  
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println("f@");
    //Sunrise_2.4GHz_6F1520
    //52vw22ruhxFb
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin("f@");
    // wait 10 seconds for connection:
   
  // sanity check delay - allows reprogramming if accidently blowing power w/leds
    delay(2000);
  }
   
  Serial.println("\nConnected to Wifi");
  printWifiStatus();

  Serial.println("\nStarting connection to UDP server...");
  Udp.begin(localPort);
  delay(40);
  AskForConnection();
  

  Serial.print("Get ready to enter the ");
        Serial.print(names[index]);
        Serial.println();
        Serial.print("Electromagnets Nb: ");
        Serial.println(electromagnets[index]);
        Serial.print("Ultimate Energy (GeV): ");
        Serial.println(energy[index]);
        Serial.print("Circumference (m): ");
        Serial.println(circumference[index]);
        Serial.println();
        Serial.println();
  
 
  for(int count=5; count>0; count--){
        for(int all=variableStart; all <= variableEnd; all++){
          tunnels[all] = CRGB::Red;
        }
      FastLED.show();
       
      
      delay(500);  
  
      turnOff();
      
      delay(500);
      }

      calibrate();

   

}


void loop() {                         

   delay(40);
    AskForConnection();
    
   // Move a single white led 
  for(startLed = variableEnd; startLed >= variableStart; ) {
   
    int gyro_x= constrain(getRelativeGyro_x(),10,300) ;
    
   if(gyro_x==0){
      continue;
    }

    
  
    currentSpeed= map(gyro_x,10,300, nextLowSpeed, nextHighSpeed);
       
      //Serial.print(gyro_x);
      //Serial.print(" ---- ");
      //Serial.println(currentSpeed);

      //So many conditions due to hardware irregularities
    if(abs(currentSpeed-prevSpeed) > sensitivity && abs(currentSpeed-prevSpeed) < 150){
      //gameOver();
      //return;
      
      }
    
    prevSpeed= currentSpeed;
    
    if(currentSpeed == nextHighSpeed){
      counter++;
      if(counter > 350 && (startLed==6 || startLed==16 || startLed==29 || startLed==95)){
        turnOff();
        if(startLed == 95){
          lightShow();
        }
        nextLowSpeed= nextHighSpeed;
        nextHighSpeed= nextHighSpeed-20;
        sensitivity= sensitivity-1;
        index++;
        variableStart= starts[index]+1;
        startLed= variableStart;
        variableEnd= starts[index+1];
        counter=0;
        
        Serial.print("Get ready to enter the ");
        Serial.print(names[index]);
        Serial.println();
        Serial.print("Electromagnets Nb: ");
        Serial.println(electromagnets[index]);
        Serial.print("Ultimate Energy (GeV): ");
        Serial.println(energy[index]);
        Serial.print("Circumference (m): ");
        Serial.println(circumference[index]);
        Serial.println();
        Serial.println();
                         
        
        
switch(variableEnd){

  
 case 27:   for(int i=0; i<5; i++){
  
         for(int j=6; j>2; j--){
            tunnels[j].setRGB(255,255/(j+1),255/(j+1));
            FastLED.show();
            delay(100);
         }

         for(int k=6; k>2; k--){
            tunnels[k]= CRGB::Black;
            
         }
         FastLED.show();
         
         delay(400);
}
      break;

case 51: 

for(int i=0; i<5; i++){
  
         for(int j=0; j<6; j++){
            connections[j].setRGB(255,255/(j+1),255/(j+1));
            FastLED.show();
            delay(75);
         }

         for(int k=0; k<6; k++){
            connections[k]= CRGB::Black;
            
         }
         FastLED.show();
         
         delay(400);
}
break;

case 111:

for(int i=0; i<5; i++){
  
         for(int j=6; j<8; j++){
            connections[j].setRGB(255,255/(j+1),255/(j+1));
            FastLED.show();
            delay(200);
         }

         for(int k=6; k<8; k++){
            connections[k]= CRGB::Black;
            
         }
         FastLED.show();
         
         delay(400);
}
break;


}   
        
        calibrate();

      }
    }
    else {
      counter=0;
    } 
    
    rotate();
          
  }

  
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void AskForConnection(){
  if(!Udp.available()){
    
    Serial.println("Connect mobile phone...");
     while(!Udp.available()){
      ;
    }
    for(int i=0; i<10; i++){
      delay(20);
     Udp.parsePacket();
     Udp.read(packetBuffer, 255);
     Udp.flush();
    }
     prevSpeed=0; //to not be mistaken as too fast change
     calibrate();
     
  }
 
}


void calibrate(){

  for(int i=0; i<30; i++){
    getAbsoluteGyro_x();
  }


  base= getAbsoluteGyro_x();
    
}


float getAbsoluteGyro_x(){
    
    int packetSize = Udp.parsePacket();
    // read the packet into packetBufffer
    
    int len = Udp.read(packetBuffer, 255);
    Udp.flush();
     
    if (len > 0) {  
      packetBuffer[len] = 0;
    }
       //Serial.println(packetBuffer);  
    char gyro_x_char[5];
      
    for (int i = 0; i < 5; i++){
      gyro_x_char[i] = packetBuffer[i];
    }
    gyro_x_char[5]='\0';
      
    int gyro_x= atof(gyro_x_char)*1000;
    

    return gyro_x;

}


int getRelativeGyro_x(){

  if(base-getAbsoluteGyro_x() >0){
    return base-getAbsoluteGyro_x();
  }

  else return 10;
}

void gameOver(){
       

  prevSpeed=currentSpeed=0;
  index=0;
  sensitivity=30;
  startLed=0;
  variableStart= starts[0];
  variableEnd= starts[1];
  counter=0;
  nextHighSpeed= BOOSTER_HIGHEST_SPEED;
        
      Serial.println("GAME OVER - Restarting in: ");
     
      for(int count=3; count>0; count--){
        for(int all=0; all < NUM_LEDS_TOTAL; all++){
          tunnels[all] = CRGB::Red;
        }

        for(int all=0; all < 8; all++){
          connections[all] = CRGB::Red;
        }
      FastLED.show();
       
      Serial.println(count);
      delay(500);  
  
      turnOff();
      
      delay(500);
      }
 
 
   delay(1000);
    calibrate();
    
      return;  
      

}

void rotate(){
  
      unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= currentSpeed) {

      // save the last time you blinked the LED
      previousMillis = currentMillis;

      if(variableEnd==51){
          // Turn our current led on to white, then show the leds
        tunnels[79-startLed].setRGB(255,map(currentSpeed,15,150,0,255),map(currentSpeed,15,150,0,255));
    
        // Show the leds (only one of which is set to white, from above)
       
        // Turn our current led back to black for the next loop around
         if(79-startLed != variableStart){
            tunnels[(79-startLed-1)] = CRGB::Black;
         }
         else{
          tunnels[51] = CRGB::Black;
         }
         
         FastLED.show();
  
         startLed = startLed - 1;
          
      }

      else if(variableEnd==111){

        int x;
        int diff= 30;
        
        if(startLed < 82){
          x= startLed+60;
        }

        else{
          x= startLed;
        }
         tunnels[startLed].setRGB(255,map(currentSpeed,15,150,0,255),map(currentSpeed,15,150,0,255));
         tunnels[163-(x-diff)].setRGB(map(currentSpeed,15,150,0,255),map(currentSpeed,15,150,0,255),255);
    
        // Show the leds (only one of which is set to white, from above)
       
        // Turn our current led back to black for the next loop around
         if(startLed != variableEnd){
            tunnels[(startLed+1)] = CRGB::Black;
            
         }
         else{
          tunnels[(variableStart)] = CRGB::Black;
          
          
         }


         if( 163-(x-diff) != variableStart){
          tunnels[163-(x-diff)-1] = CRGB::Black;
         }

         else{
          tunnels[111] = CRGB::Black;
          tunnels[110] = CRGB::Black;
         }
        
        FastLED.show();
  
         startLed = startLed - 1;
        
      }
      
      else{
        // Turn our current led on to white, then show the leds
        tunnels[startLed].setRGB(255,map(currentSpeed,15,150,0,255),map(currentSpeed,15,150,0,255));
    
        // Show the leds (only one of which is set to white, from above)
       
        // Turn our current led back to black for the next loop around
         if(startLed != variableEnd){
            tunnels[(startLed+1)] = CRGB::Black;
         }
         else{
          tunnels[(variableStart)] = CRGB::Black;
         }
        
        FastLED.show();
  
         startLed = startLed - 1;
      }
    }
}

void turnOff(){
  
   for(int all=0; all < NUM_LEDS_TUNNELS; all++){
    tunnels[all] = CRGB::Black;
   }

   for(int all=0; all < NUM_LEDS_CONNECTIONS; all++){
    connections[all] = CRGB::Black;
   }

        
   FastLED.show();
  
}

void lightShow(){

  Serial.println("Congratulations! You successfully collided at 13 TeV and discovered a new particule! What will you call it?");
        
  FastLED.setBrightness(50);
        while(1){
        rainbow(5);
        }
      
}

void rainbow(uint8_t wait) 
{

  uint16_t hue;
  FastLED.clear();

  for(hue=10; hue<255*3; hue++) 
  {

    fill_rainbow( &(tunnels[0]), NUM_LEDS_TOTAL, hue);
    fill_rainbow( &(connections[0]),8, hue);      
    FastLED.show();
    delay(wait);
  }
  return;
}

void(* resetFunc) (void) = 0; 

