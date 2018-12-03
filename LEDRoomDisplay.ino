// Using:          https://www.adafruit.com/product/420

/******************************************************************************
 *                                                                            *
 *                 ASSUMES WE ONLY HAVE ONE USER AT A TIME                    *
 *               ASSUMES A USER WILL ALWAYS ACTIVATE THE SENSOR.              *
 *                                                                            *
 *  Look into Particle.subscribe() for PHP w/ status website                  *
 *  Track # in and # out                                                      *
 *          --> Check logic on I/O                                            *
 *                                                                            *   
 ******************************************************************************/

#include <Adafruit_mfGFX.h>   // Core graphics library
#include <RGBmatrixPanel.h>   // Hardware-specific library
#include "math.h"

// Pin layout for our sensor. These should work despite the analog I/O as we let them function as on/off only
#define TRIG A5
#define ECHO A4

// Pin layout for LED matrix
#define CLK	D6
#define OE	D7
#define LAT	A3
#define A  	A0
#define B  	A1
#define C  	A2

RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);     // Defining the matrix

bool occupied = false;                      // Whether the room is occupied or not
int textX = matrix.width();                 // The width of the LED matrix
int textY = matrix.height();                // The height of the LED matrix
int status = 0;                             // Will be published to the Particle website + used for LED text: 0 = vacant, 1 = occupied
int numPassed = 0;                          // Tracking the total number passed (potentially deprecated?)
uint16_t red =  0x1000;
uint16_t green = 0x0080;

static uint16_t boardColor = 0xFFFF;        // Color of text on the board

void setup() {
    // Sensor
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);

    // LED Matrix
    matrix.fillScreen(matrix.Color333(0, 0, 0));
    matrix.setCursor(1, 0);
    matrix.setTextSize(1);
    matrix.setTextColor(green);
    matrix.print("Occupied");
    
    Particle.variable("Occupied= ", occupied);                 // Status of the room.
    Particle.variable("Num pass=", numPassed);
}


/* Checks if there is an object closer to the sensor than the door frame.
 * Returns true if the object is in the way, false if there is no interference.
 * Takes maxDist in inches as the maximum distance that a player can be at. */
bool check(uint32_t maxDist) {

    uint32_t duration, inches, cm;

    digitalWriteFast(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWriteFast(TRIG, LOW);

    duration = pulseIn(A4, HIGH);

    // Speed of Sound = 1130 ft/s (~ 74 in/s).
    inches = duration / 74 / 2;

    if (inches < maxDist)
        return true;

    if (inches >= maxDist)
        return false;


}


void boardWrite(uint16_t color) {
    matrix.setCursor(textX / 2, textY / 2);
    matrix.setTextColor(color);
    
    if (occupied)
        matrix.print("Occupied");
        
    if (!occupied)
        matrix.print("Vacant");
}


void fill() {
    status = 1;                // Changing the status of the board & particle.var
    boardColor = 0x1000;       // Changing the color of the board to red (I think)
    occupied = true;
}


void available() {
    status = 0;                  // Changing the status of the board & particle.var
    boardColor = 0x0080;         // Changing the color of the board to green (I think)
    occupied = false;
}


void loop() {
    // Checks the activity of the doorway.
    bool result = check();
    // Doesn't necessarily work due to only finding that the door is open
    // If occupied and there is someone at the door, we assume they leave?
    if (occupied && result)
        available();
    
    
    // If occupied and there is no longer someone in there, it remains occupied
    else if (!occupied && result)
        fill();

    delay(10);
}


