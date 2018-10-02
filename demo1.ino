// Using: https://www.adafruit.com/product/420
// Github Library: https://github.com/pkourany/RGBmatrixPanel_IDE
// Gibhub Example: https://github.com/pkourany/RGBmatrixPanel_IDE/blob/master/examples/scrolltext_16x32/scrolltext_16x32.ino

// Token = “e94ca7a33c00354cb20b43878bdfc010a3017a42”
// ID = “2d0046000747363339343638”

/******************************************************************************
 *                                                                            *
 *                 ASSUMES WE ONLY HAVE ONE USER AT A TIME                    *
 *               ASSUMES A USER WILL ALWAYS ACTIVATE THE SENSOR.              *
 *                                                                            *
 *  Look into Particle.subscribe() for PHP w/ status website                  *
 *  Track # in and # out                                                      *
 *          --> Check logic on I/O                                            *
 *  Group by integers rather than booleans to keep track of multiple in / out *
 *                                                                            *   
 ******************************************************************************/

#include "Adafruit_mfGFX.h"   // Core graphics library
#include "RGBmatrixPanel.h"   // Hardware-specific library
#include "math.h"
// RGB Shield Version
#define RGBSHIELDVERSION 4

// Pin layout for our sensor
#define TRIG D5
#define ECHO D4

// Pin layout for LED matrix
#define CLK	D6
#define OE	D7
#define LAT	TX
#define A  	A0
#define B  	A1
#define C  	A2
#define D	RX

RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, true);     // Defining the matrix

bool occupied = false;                      // Whether the room is occupied or not
int textX = matrix.width();                 // The width of the LED matrix
int textY = matrix.height();                // The height of the LED matrix
char status[] = "Starting up";              // Will be published to the Particle website + used for LED text

static unit16_t boardColor = 0xFFFF;        // Color of text on the board

Particle.variable("Status=", status);       // Status of the room.
Particle.variable("Color=", boardColor);    // Color of the board.
Particle.variable("Number passed=", numPassed);

void setup() {
    // Sensor
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);

    // LED Matrix
    matrix.fillScreen(0);                   // Clears the setting.
    matrix.setTextSize(2);                  // Look into appropriate size...
    matrix.print(str);                      // Writes str to board
}


void loop() {
    // Checks the activity of the doorway.

    // Doesn't necessarily work due to only finding that the door is open
    // If occupied and there is no longer someone in there, it remains occupied
    if(!occupied && check())
        full();
    // If occupied and there is someone at the door, we assume they leave?
    else(occupied && check())
        available();

    // Refreshes every loop...
    boardWrite(boardColor);
    delay(50);                              // Doesn't need to accomodate sensors as pulseIn() does it automatically.
}

/* Checks if there is an object closer to the sensor than the door frame.
 * Returns true if the object is in the way, false if there is no interference. */
void check() {
    unsigned long duration;                 // Keeps track of the length of the pulse
    unsigned long distance;                 // Distance that the sensor captured in meters.
    int across = 2;                         // Distance from sensor to doorframe (SHORTER RATHER THAN LONGER)

    status = "Checking";                    // Updating the status that the Photon sends to the cloud

    // Checking the sensor for activity
    duration = pulseIn(TRIG, HIGH);         // Gets the pulse length in ms
    distance = duration * 0.343;            // ms -> m using 0.343 as m/ms speed of sound

    if(distance < across)
        return true;

    return false;
}

// Not sure if color is an option, but board is RGB so it should have access
void boardWrite(unit16_t color){
    matrix.setCursor(textX / 2, textY / 2);
    matrix.print(status);
    matrix.setTextColor(color);
}


void full() {
    status = "Occupied";                // Changing the status of the board & particle.var
    boardColor = 0x1000;                // Changing the color of the board to...
    occupied = true;
}


void available() {
    status = "Vacant";                  // Changing the status of the board & particle.var
    boardColor = 0x0080;                // Changing the color of the board to...
    occupied = false;
}
