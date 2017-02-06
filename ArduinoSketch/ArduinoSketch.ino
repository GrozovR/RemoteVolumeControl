#include "IRremote.h"
 
int receiver = 11; // pin 1 of IR receiver to Arduino digital pin 11

IRrecv irrecv(receiver);           // create instance of 'irrecv'
decode_results results;            // create instance of 'decode_results'


 
void setup()  
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver 
}
 
 
void loop() 
{
  if (irrecv.decode(&results)) // have we received an IR signal?
  {
    Serial.println(results.value, HEX);  //UN Comment to see raw values
    irrecv.resume(); // receive the next value
  }  
}
 
 
