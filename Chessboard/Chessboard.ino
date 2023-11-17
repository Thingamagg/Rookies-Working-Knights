#include <Adafruit_NeoPixel.h>
#include "Timer.h"
#include "Chess.h"

// wire no res = 0.32 volt reading
// Current = 4mA
// increments of 183 ohms

//list what type of piece it is 
enum PIECETYPE{
  NONE = 0,
  BLACK_PAWN,
  BLACK_ROOK,
  BLACK_BISHOP,
  BLACK_KNIGHT,
  BLACK_QUEEN,
  BLACK_KING,
  WHITE_PAWN,
  WHITE_ROOK,
  WHITE_BISHOP,
  WHITE_KNIGHT,
  WHITE_QUEEN,
  WHITE_KING,
};




//Used for LED thingo
const int lightsPin = 28;   
const int numLights = 64;

//buttons
const int onPin = 31; //leftmost button
const int cheatPin = 23; //rightmost button 

const int colourPin1 = 29; // the 3 buttons to change colours
const int colourPin2 = 27;
const int colourPin3 = 25;



//Digital Pins
const int columnZero = A15;
const int columnOne = A13;
const int columnTwo = A11;
const int columnThree = A9;
const int columnFour = A7;
const int columnFive = A5;
const int columnSix = A3;
const int columnSeven = 46;

const int rowZero = 53;
const int rowOne = 51;
const int rowTwo = 49;
const int rowThree = 47;
const int rowFour = 39;
const int rowFive = 41;
const int rowSix = 43;
const int rowSeven = 45;

//Voltage Readers (A0 -A7)          /* Re did the function so you dont have the analog inside of it (so its easier to debug and we can put in any number we want)*/
const int testColumnZero = A14;       //   analogRead(A0)
const int testColumnOne = A12;          // analogRead(A1)
const int testColumnTwo = A10;           // analogRead(A2)
const int testColumnThree = A8;       // analogRead(A3)
const int testColumnFour = A6;        //analogRead(A4)
const int testColumnFive = A4;        //analogRead(A5)
const int testColumnSix = A2;         //analogRead(A6)
const int testColumnSeven = A0;       //analogRead(A7)


//timer variables
const int buttonWhite = 8;
const int buttonBlack = 9;
// const int pinBuzzer = 10;
const int analogPin = A1;

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

PIECETYPE tiled [8][8] ={
  {BLACK_ROOK, BLACK_KNIGHT, BLACK_BISHOP, BLACK_QUEEN, BLACK_KING, BLACK_BISHOP, BLACK_KNIGHT, BLACK_ROOK},
  {BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN},
  {NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE},
  {NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE},
  {NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE},
  {NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE},
  {WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN},
  {WHITE_ROOK, WHITE_KNIGHT, WHITE_BISHOP, WHITE_QUEEN, WHITE_KING, WHITE_BISHOP, WHITE_KNIGHT, WHITE_ROOK},
};

// timer stuff (lcd/buttons)
// (rs, en, d4, d5, d6, d7, white button, black button, analog pin)
Timer timer(rs, en, d4, d5, d6, d7, buttonWhite, buttonBlack, analogPin);

Chess chess(lightsPin, numLights, onPin, cheatPin, colourPin1, colourPin2, colourPin3,
          columnZero, columnOne, columnTwo, columnThree, columnFour, columnFive, columnSix, columnSeven,
          rowZero, rowOne, rowTwo, rowThree, rowFour, rowFive, rowSix,  rowSeven,
          testColumnZero, testColumnOne, testColumnTwo, testColumnThree, testColumnFour, testColumnFive, testColumnSix, testColumnSeven);


void setup() {
  chess.initChess();
  timer.initTimer();

}

 
void loop() {
    timer.loopTimer();
    chess.loopChess();
}
