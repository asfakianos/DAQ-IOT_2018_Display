
#include "Adafruit_mfGFX.h"   // Core graphics library
#include "RGBmatrixPanel.h"   // Hardware-specific library
#include "math.h"

// RGB Shield Version
#define RGBSHIELDVERSION 4

// Pin layout for LED matrix
#define CLK	D6
#define OE	D7
#define LAT	TX
#define A  	A0
#define B  	A1
#define C  	A2
#define D	RX


RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, true);     // Defining the matrix


char occupied[] = "Occupied";
char vacant[] = "Vacant";
int textX;
int textY;

uint16_t boardColor = 0xFFFF;        // Color of text on the board

void setup() {
	matrix.begin();
	matrix.setTextSize(2);
}


// Not sure if color is an option, but board is RGB so it should have access
void boardWrite(uint16_t color, int status) {
    matrix.setCursor(textX / 2, textY / 2);
    matrix.setTextColor(color);
    
    if (status == 0)
        matrix.print(occupied);
        
    if (status == 1)
        matrix.print(vacant);
}


void loop() {
	boardWrite(boardColor, 0);
    delay(500);

    boardWrite(boardColor, 1);
    delay(500);
}


