// Using:          https://www.adafruit.com/product/420
// Github Library: https://github.com/pkourany/RGBmatrixPanel_IDE

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
#include <math.h>

// Pin layout for our sensor.
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

uint16_t red =  0x1000;                     // Green color for the board.
uint16_t green = 0x0080;                    // Red color for the board.
uint16_t boardColor;

void setup() {
    matrix.begin();
    // Sensor
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);

    // Filling the matrix with an empty background (no light bg)
    matrix.fillRect(0, 0, 32, 16, matrix.Color333(0, 0, 0));
    // Setting the Cursor to the top left with 1 LED of padding.
    matrix.setCursor(1, 0);
    matrix.setTextSize(1);
    // Sets the color to yellow for testing purposes.
    matrix.setTextColor(0xAFE5);
}


/* Checks if there is an object closer to the sensor than the door frame.
 * Returns true if the object is in the way, false if there is no interference.
 * Takes maxDist in inches as the maximum distance that a player can be at. */
bool refresh(uint32_t maxDist) {

    uint32_t duration, inches;

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
    matrix.setTextColor(color);
    matrix.fillScreen(0x9471);
    matrix.setCursor(1, 0);
    
    if (occupied)
        matrix.print("Occupied");
        
    if (!occupied)
        matrix.print("Vacant");
}


void fill() {
    boardColor = 0x1000;       // Changing the color of the board to red (I think)
    occupied = true;
    boardWrite(boardColor);
}


void available() {
    boardColor = 0x0080;        // Changing the color of the board to green (I think)
    occupied = false;
    boardWrite(boardColor);
}


void loop() {
    // Checks the activity of the doorway.
    bool result = refresh(24);
    // Doesn't necessarily work due to only finding that the door is open
    // If occupied and there is someone at the door, we assume they leave?
    if (occupied && result){
        available();
        delay(1000);
        }
    
    // If occupied and there is no longer someone in there, it remains occupied
    else if (!occupied && result){
        fill();
        delay(1000);
    }
    
    delay(10);
}

