// Using:          https://www.adafruit.com/product/420
// Github Library: https://github.com/pkourany/RGBmatrixPanel_IDE
// Gibhub Example: https://github.com/pkourany/RGBmatrixPanel_IDE/blob/master/examples/scrolltext_16x32/scrolltext_16x32.ino
// File API:       https://developer.mozilla.org/en-US/docs/Web/API/File/Using_files_from_web_applications
// ^ ^ Use with:   https://reactjs.org/docs/higher-order-components.html

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
 *                                                                            *   
 ******************************************************************************/

#include <Adafruit_mfGFX.h>   // Core graphics library
#include <RGBmatrixPanel.h>   // Hardware-specific library
#include "math.h"
// RGB Shield Version
#define RGBSHIELDVERSION 4

// Pin layout for our sensor. These should work despite the analog I/O as we let them function as on/off only
#define TRIG A5
#define ECHO A4

// Pin layout for LED matrix
#define CLK D6
#define OE  D7
#define LAT TX
#define A   A0
#define B   A1
#define C   A2
#define D   RX

RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, true);     // Defining the matrix

bool occupied = false;                      // Whether the room is occupied or not
int textX = matrix.width();                 // The width of the LED matrix
int textY = matrix.height();                // The height of the LED matrix
int status = 0;                             // Will be published to the Particle website + used for LED text: 0 = vacant, 1 = occupied
int numPassed = 0;                          // Tracking the total number passed (potentially deprecated?)

static uint16_t boardColor = 0xFFFF;        // Color of text on the board

void setup() {
    // Sensor
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);

    // LED Matrix
    matrix.fillScreen(0);                   // Clears the setting.
    matrix.setTextSize(2);                  // Look into appropriate size...
    matrix.print("Setting up");          // Writes str to board
    
    Particle.variable("Status = ", status);       // Status of the room.
    Particle.variable("Color=", boardColor);                             // Color of the board.
    Particle.variable("Num pass=", numPassed);
}


/* Checks if there is an object closer to the sensor than the door frame.
 * Returns true if the object is in the way, false if there is no interference. */
bool check() {
    unsigned long duration;                 // Keeps track of the length of the pulse
    unsigned long distance;                 // Distance that the sensor captured in meters.
    unsigned int across = 2;                // Distance from sensor to doorframe (SHORTER RATHER THAN LONGER)

    // Checking the sensor for activity
    duration = pulseIn(TRIG, HIGH);         // Gets the pulse length in ms
    distance = duration * 0.343;            // ms -> m using 0.343 as m/ms speed of sound

    if(distance < across)
        return true;

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


void full() {
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
        full();


    // Refreshes every loop...
    boardWrite(boardColor);
    delay(50);                              // Doesn't need to accomodate sensors as pulseIn() does it automatically.
}


