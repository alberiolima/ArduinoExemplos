/*
First stab at a auto baud rate detection function. This uses the pulseIn command
to determine what speed an unknown serial link is running at. Detects and sets the standard baud 
rates supported by the Arduino IDE serial monitor. Uses character "U" as a test character

I'm sure characters with multiple zero bits in a row may fake out proper detection. If data is
garbled in the serial monitor then the auto baud rate function failed.

This is just a demo sketch to show concept. After uploading the sketch, open the serial monitor
and pick a random baud rate and then start sending U characters. The monitor will print out
what baud rate it detected. It will default to 9600 baud if found a bit
width too long for a standard rate used by the serial monitor. Not that as written, this is a 
'blocking' function that will wait forever if no character are being sent.

Note that while the serial monitor has a 300 baud option, the Arduino hardware serial library 
does not seem to support that baud rate, at least for version 22, in my testing.

By "retrolefty" 1/22/11
*/

int recPin = 0;  //the pin receiving the serial input data
long baudRate;   // global in case useful elsewhere in a sketch
long rate;



void setup()
{
 pinMode(recPin, INPUT);      // make sure serial in is a input pin
 digitalWrite(recPin, HIGH); // pull up enabled just for noise protection
 
 baudRate = detRate(recPin);  // Function finds a standard baudrate of either
                              // 1200,2400,4800,9600,14400,19200,28800,38400,57600,115200 
                              // by having sending circuit send "U" characters.
                              // Returns 0 if none or under 1200 baud  
 Serial.begin(baudRate);
 Serial.println();
 Serial.print("Detected baudrate at ");
 Serial.print(baudRate);
 Serial.print("\t rate at ");
 Serial.println(rate);
}

void loop()
{
}

long detRate(int recpin){  // function to return valid received baud rate
                         // Note that the serial monitor has no 600 baud option and 300 baud
                         // doesn't seem to work with version 22 hardware serial library

  while(digitalRead(recpin) == 1){} // wait for low bit to start
  rate = pulseIn(recpin,LOW);   // measure zero bit width from character 'U'
  long baud;  
  if (rate < 10)
    baud = 115200;
  else if (rate < 12)
    baud = 74880;
  else if (rate < 20)
    baud = 57600;
  else if (rate < 29)
    baud = 38400;
  else if (rate < 40)
    baud = 28800;
  else if (rate < 60)
    baud = 19200;
  else if (rate < 80)
    baud = 14400;
  else if (rate < 150)  //99-104
    baud = 9600;
  else if (rate < 300)  //209
    baud = 4800;
  else if (rate < 600)
    baud = 2400;
  else if (rate < 1200) //826
    baud = 1200;
  else if (rate < 2400) //410
    baud = 600;
  else if (rate < 4800) //3309
    baud = 300;     
  else if (rate < 9600) 
    baud = 150;         
  else if (rate < 7040) 
    baud = 110;
  else if (rate < 19200) 
    baud = 75;    
  else 
    baud = 0;  
  return baud; 
 }

