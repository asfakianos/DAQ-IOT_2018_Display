// Testing Analog-based I/O ultrasonic usage
#include "math.h"

// Pin layout for our sensor. These should work despite the analog I/O as we let them function as on/off only
#define TRIG A5
#define ECHO A4

bool blocked = false;
int status = 0;

void setup() {
    // Sensor
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
    
    Particle.variable("Status=", status);       // Status of the room.
    Particle.variable("Blocked=", blocked);     // Whether or not the sensor has an object within range.
}


/* Checks if there is an object closer to the sensor than the door frame.
 * Returns true if the object is in the way, false if there is no interference. */
bool check() {
    unsigned long duration;                 // Keeps track of the length of the pulse
    unsigned long distance;                 // Distance that the sensor captured in meters.
    int across = 2;                         // Distance from sensor to doorframe (SHORTER RATHER THAN LONGER)

    // Checking the sensor for activity
    duration = pulseIn(TRIG, HIGH);         // Gets the pulse length in ms
    distance = duration * 0.343;            // ms -> m using 0.343 as m/ms speed of sound

    if(distance < across) {
        status++;
        blocked = true;
        return true;
    }

    blocked = false;
    return false;
}


void loop() {
    check();
    delay(50);                              // Doesn't need to accomodate sensors as pulseIn() does it automatically.
}

