# DAQ-IOT_2018_Display
Particle Photon microprocessor project for a Fall 2018 Case Western Reserve University SAGES group assignment

GOAL: Use sensors to detect activity in and out of a bathroom to notify others if the bathroom is occupied/available from outside of the bathroom, and evntually, via the web

Created for an Adafruit 16x32 RGB LED Matrix display (http://www.adafruit.com/products/420).
Runs on a Particle Photon microprocessor.

Version 1.0 -- Runs basic functions of display and communications.

To-Do:
  - Change visual fx to add flair to sign
  - Add web implementation
  
Version 1.0.1 -- Debugging from Ver 1.0 and added two files for testing hardware:
  analongPing.ino -- Checks that the ultrasonic sensor is working in correspondence with Analog pins on the Photon board
  boardTest.ino   -- Checks that the LED matrix works with the Photon board by displaying Occupied/Vacant in a similar fashion                      to how the display will work in the actual process.
