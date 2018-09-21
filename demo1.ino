/* Demonstrating a few basic uses of the sensors we have available */

/* Be sure to:
     Attach the "long" leg of the LED to D7
     Attach one end of the Photoresistor to A0, and the other leg can be anywhere
     Attach the other end of the Photoresistor to A5 to supply some amount of power */
int LED = D7;
int photoresistor = A0;
int power_photo = A5;
int analogReading;


void setup(){

  pinMode(LED, OUTPUT);
  pinMode(photoresistor, INPUT);
  pinMode(power_photo, OUTPUT);

  Particle.function("flicker", flickerThatShit);
  Particle.variable("Photo_demo", &analogReading, INT);
}


void loop(){

  /* Checks the reading of the photoresistor every 100ms. */
  analogReading = analogRead(photoresistor);
  delay(100);
}

// Must take a String and return an int
// Turns an LED on for 500ms, but then turns it back off.
int flickerThatShit(String args){
  digitalWrite(LED, HIGH);

  delay(500); // Half-second delay

  digitalWrite(LED, LOW);

}

/* Demonstrates usage of the photoresistor by altering an LED based off of the light given */
int photo_demo(String args){

}
