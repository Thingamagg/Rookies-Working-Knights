#include <Adafruit_NeoPixel.h>
#include "Timer.h"

// wire no res = 0.32 volt reading
// Current = 4mA
// increments of 183 ohms

#define WPawn 1
#define WRook 2
#define WKnight 3
#define WBishop 4
#define WQueen 5
#define WKing 6
#define BPawn 12
#define BRook 7
#define BKnight 9
#define BBishop 10
#define BQueen 11
#define BKing 8
#define Empty 13 

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

//Chess bot
#define M 0x88
#define S 128
#define I 8000


//Used for LED thingo
const int lightsPin = 28;   
const int numLights = 64;

//buttons
const int onPin = 31; //leftmost button
const int cheatPin = 23; //rightmost button 

const int colourPin1 = 29; // the 3 buttons to change colours
const int colourPin2 = 27;
const int colourPin3 = 25;

//for LED
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(numLights, lightsPin, NEO_GRB + NEO_KHZ800);


bool onButton = 1;
int lastOnState;
int currentOnState;

//Check Function
bool CheckFunction(char colour);
bool PieceMovement(char colour, int piece, int row, int column);

//Checkmate
bool Checkmate();
//EN PASSANT
bool Wpawnstart[8] = {1,1,1,1,1,1,1,1};
bool Wenpassant[8] = {0,0,0,0,0,0,0,0};
bool Bpawnstart[8] = {1,1,1,1,1,1,1,1};
bool Benpassant[8] = {0,0,0,0,0,0,0,0};

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

int lastPieceNumber;
int pieceVoltage;
int pieceNumber;
int ledNumber;

int colour1 = 4;
int colour2 = 3;
int colour3 = 7;

//CASTLING 
bool Wcastlestart[2] = {1,1};
bool Bcastlestart[2] = {1,1};
bool WKingstart = 1;
bool BKingstart = 1; 

void castling();
bool Wkingsidecastle = 0;
bool Wqueensidecastle = 0;
bool Bkingsidecastle = 0;
bool Bqueensidecastle = 0;

char W;
char B;

bool cheatToggle;

bool colour1State = HIGH;
bool colour2State = HIGH;
bool colour3State = HIGH;

bool lastColour1State = HIGH;
bool lastColour2State = HIGH;
bool lastColour3State = HIGH;

int a;    //Where 'a' is collumns (0 to 7 is bottom to top)
int i;    //Where 'i' is rows (0 to 7 is left to right)

int tile[8][8] = {                          //Tiles of the board with numbers representing pieces (13 empty space)
  {13, 13, 13, 13, 13, 13, 13, 13},
  {13, 13, 13, 13, 13, 13, 13, 13},
  {13, 13, 13, 13, 13, 13, 13, 13},
  {13, 13, 13, 13, 13, 13, 13, 13},
  {13, 13, 13, 13, 13, 13, 13, 13},
  {13, 13, 13, 13, 13, 13, 13, 13},
  {13, 13, 13, 13, 13, 13, 13, 13},
  {13, 13, 13, 13, 13, 13, 13, 13}
};

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

//Initial chess bot
char b[]={     
  22, 19, 21, 23, 20, 21, 19, 22, 28, 21, 16, 13, 12, 13, 16, 21,
  18, 18, 18, 18, 18, 18, 18, 18, 22, 15, 10,  7,  6,  7, 10, 15,
   0,  0,  0,  0,  0,  0,  0,  0, 18, 11,  6,  3,  2,  3,  6, 11,
   0,  0,  0,  0,  0,  0,  0,  0, 16,  9,  4,  1,  0,  1,  4,  9,
   0,  0,  0,  0,  0,  0,  0,  0, 16,  9,  4,  1,  0,  1,  4,  9,
   0,  0,  0,  0,  0,  0,  0,  0, 18, 11,  6,  3,  2,  3,  6, 11,
   9,  9,  9,  9,  9,  9,  9,  9, 22, 15, 10,  7,  6,  7, 10, 15,
  14, 11, 13, 15, 12, 13, 11, 14, 28, 21, 16, 13, 12, 13, 16, 21, 0
};

#define MYRAND_MAX 65535     /* 16bit pseudo random generator */
long  N, T;                  /* N=evaluated positions+S, T=recursion limit */
short Q, O, K, R, k=16;      /* k=moving side */
char *p, c[5], Z;            /* p=pointer to c, c=user input, computer output, Z=recursion counter */

char L,
w[]={0,2,2,7,-1,8,12,23},                             /* relative piece values    */
o[]={-16,-15,-17,0,1,16,0,1,16,15,17,0,14,18,31,33,0, /* step-vector lists */
     7,-1,11,6,8,3,6,                                 /* 1st dir. in o[] per piece*/
     6,3,5,7,4,5,3,6};                                /* initial piece setup      */

unsigned int seed=0;
uint32_t  byteBoard[8];

char sym[17] = {".?pnkbrq?P?NKBRQ"};
char lastH[5], lastM[5];
unsigned short ledv=1;

//Functions
void resetCheck();                      //Resets The digital pin outputs to low (for columns) and high (for rows)
void blankLights();                     //Turns off all the lights 
void checkV();                         //Uses the adc value to determine which piece has been picked up 
void checkPickUp();                    //Check if a Piece has been picked up by comparing adc values 
void showMoves(int x);                //Shows moves dependent on what piece it is 
void checkPutDown();                 //Check if the piece has been put back 
void chooseAnalog(int numAnalog);    //Chooses which analog will be run as the loop run throughs the tiles 


//Piece Moves 
bool wPawnMoves();
bool bPawnMoves();
bool wRookMoves();
bool bRookMoves();
bool wKnightMoves();
bool bKnightMoves();
bool wBishopMoves();
bool bBishopMoves();
bool wQueenMoves();
bool bQueenMoves();
bool wKingMoves();
bool bKingMoves();

bool Checkmate();
bool Stalemate();

void enpassant();
void checkColourButton();
void colourswap1();
void colourswap2();
void colourswap3();
short D(short q, short l, short e, unsigned char E, unsigned char z, unsigned char n);
unsigned short myrand(void);
void updateBARray();

bool colourM = 1;

Timer timer;

void setup() {
  timer.init();
  Serial.begin(115200);
  pixels.begin();
    
  pinMode(columnZero, OUTPUT);
  pinMode(columnOne, OUTPUT);
  pinMode(columnTwo, OUTPUT);
  pinMode(columnThree, OUTPUT);
  pinMode(columnFour, OUTPUT);
  pinMode(columnFive, OUTPUT);
  pinMode(columnSix, OUTPUT);
  pinMode(columnSeven, OUTPUT);
  pinMode(rowZero, OUTPUT);
  pinMode(rowOne, OUTPUT);
  pinMode(rowTwo, OUTPUT);
  pinMode(rowThree, OUTPUT);
  pinMode(rowFour, OUTPUT);
  pinMode(rowFive, OUTPUT);
  pinMode(rowSix, OUTPUT);
  pinMode(rowSeven, OUTPUT);
  
  pinMode(testColumnZero, INPUT);    
  pinMode(testColumnOne, INPUT);
  pinMode(testColumnTwo, INPUT);
  pinMode(testColumnThree, INPUT);
  pinMode(testColumnFour, INPUT);  
  pinMode(testColumnFive, INPUT);
  pinMode(testColumnSix, INPUT);  
  pinMode(testColumnSeven, INPUT);

  pinMode(colourPin1, INPUT_PULLUP);    
  pinMode(colourPin2, INPUT_PULLUP);
  pinMode(colourPin3, INPUT_PULLUP);    
  pinMode(onPin, INPUT_PULLUP);  
  pinMode(cheatPin, INPUT_PULLUP); 

  delay(50);   

  for (a = 0 ; a<8; a++) {
    for (i = 0 ; i<8; i++) { 

      resetCheck();
      lastPieceNumber = tile[i][a];
          
      switch (i) {
        case 0:
          digitalWrite(rowZero, LOW);
          break;
        case 1:      
          digitalWrite(rowOne, LOW);
          break;
        case 2:
          digitalWrite(rowTwo, LOW);
          break;
        case 3:
          digitalWrite(rowThree, LOW);
          break;
        case 4:
          digitalWrite(rowFour, LOW);
          break;
        case 5:
          digitalWrite(rowFive, LOW);
          break;
        case 6:
          digitalWrite(rowSix, LOW); 
          break;
        case 7: 
          digitalWrite(rowSeven, LOW);
          break;
        }
        
      chooseAnalog(a); 
      checkV();
      tile[i][a] = pieceNumber;
      
    }
  }
}

void startUp() {
  int z = 0;
  int x = 63;
  int c = 32;
  int v = 32;
  if (onButton == 1){
  for (int q; q < 32; q++) {
    pixels.setPixelColor(z, pixels.Color(10,10,10));
    pixels.setPixelColor(x, pixels.Color(100,100,100));   
    z++;
    x--;
    pixels.show();
    delay(20);
  }
  for (int q = 0; q < 32; q++) {
    pixels.setPixelColor(c, pixels.Color(0,0,0));
    pixels.setPixelColor(v, pixels.Color(0,0,0));   
    c++;
    v--;
    pixels.show();
    delay(20);  
  }  
  }
  else 
     for (int q; q < 32; q++) {
    pixels.setPixelColor(z, pixels.Color(100,0,110));
    pixels.setPixelColor(x, pixels.Color(100,0,110));   
    z++;
    x--;
    pixels.show();
    delay(20);
  }
  for (int q = 0; q < 32; q++) {
    pixels.setPixelColor(c, pixels.Color(0,0,0));
    pixels.setPixelColor(v, pixels.Color(0,0,0));   
    c++;
    v--;
    pixels.show();
    delay(20);  
  }
     //Resetting to Default Once turned off
  colour1 =4; 
  colour2 = 3;
  colour3 = 7;
  for (int i = 0; i < 2; i++){
   Wcastlestart[i] = 1;
 Bcastlestart[i] = 1;
  }
 WKingstart = 1;
 BKingstart = 1;
 for (int i = 0; i < 8 ; i++)
 {
  Wpawnstart[i] = 1;
 Bpawnstart[i] = 1;
 }
}

void cheatButtonCheck() {
  
  bool lastCheatToggle = cheatToggle;
  cheatToggle = digitalRead(cheatPin);
  static bool key = 1;

  if (cheatToggle == LOW && lastCheatToggle == HIGH) {
    Serial.println("button pressed");
    if (colourM == 1 && (key == 1)) {
      Serial.println("white moves");
      key = 0;
      updateBARray();
      K = I;
      N = 0;
      T = 0x3F;                                 /* T=Computer Play strength */
      D(-I, I, Q, O, 1, 3);                 /* Think & do*/            
    }
    else if ((colourM == 0) && (key == 0)) {
      Serial.println("black moves");
      key = 1;
      updateBARray();
      K = I;
      N = 0;
      T = 0x3F;                                 /* T=Computer Play strength */
      D(-I, I, Q, O, 1, 3);                 /* Think & do*/         
    }
    chessMatrixConverter();          
  }
  
  else if (cheatToggle == HIGH && lastCheatToggle == LOW) {    
    Serial.println("off lights");
    blankLights();
  }

  
}
 
void loop() {
  timer.looping();
  lastOnState = currentOnState;
  currentOnState = digitalRead(onPin);

  if (currentOnState == HIGH && lastOnState == LOW) {
    onButton = !onButton;
    startUp();
    blankLights();
  }

if (onButton == 1) {
  // Updates matrix to know what is on each tile
    for (a = 0 ; a<8; a++) {

        if (currentOnState == HIGH && lastOnState == LOW) {
          break;
        }
      
      for (i = 0 ; i<8; i++) { 
      
        if (currentOnState == HIGH && lastOnState == LOW) {
          onButton = !onButton;
          startUp();
          blankLights();
          break;
        }        
        
        checkColourButton();
        resetCheck();
        lastPieceNumber = tile[i][a];
            
        switch (i) {
          case 0:
            digitalWrite(rowZero, LOW);
            break;
          case 1:      
            digitalWrite(rowOne, LOW);
            break;
          case 2:
            digitalWrite(rowTwo, LOW);
            break;
          case 3:
            digitalWrite(rowThree, LOW);
            break;
          case 4:
            digitalWrite(rowFour, LOW);
            break;
          case 5:
            digitalWrite(rowFive, LOW);           
            break;
          case 6:
            digitalWrite(rowSix, LOW);         
            break;
          case 7: 
            digitalWrite(rowSeven, LOW);
            break;
          }
          
        chooseAnalog(a); 
        checkV();
        tile[i][a] = pieceNumber;
        checkPickUp();
        checkPutDown();
        lastPieceNumber = tile[i][a];
        cheatButtonCheck();        
  
    if (CheckFunction('W') == true) {
      if (tile[i][a] == WKing) {
        matrixToArray(i, a);
        colourswap3();
        pixels.show();
      }
    }
  
    if (CheckFunction('B') == true) {
      if (tile[i][a] == BKing) {
        matrixToArray(i, a);
        colourswap3();
        pixels.show();
      }
    }
  }

  Serial.println("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

  for (int j = 0; j < 8; j++)
  {
    for (int k = 0; k < 8; k++)
    {
      
      Serial.print(tile[j][k]);
      Serial.print("\t");
    }
    Serial.println("");
  }
  Serial.println("++++++++++++++++++++++++++++");
  //delay(100);        

    }  

      if (Checkmate() == true){
      Serial.println("Checkmate");
      for(int i=0;i<numLights;i++){
        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        ledNumber = i;
        colourswap3(); // Moderately bright green color.
        pixels.show(); // This sends the updated pixel color to the hardware.
        delay(50);
    }
    blankLights(); 
    }    
  }
}

void resetCheck() {
  digitalWrite(columnZero, LOW);
  digitalWrite(columnOne, LOW);
  digitalWrite(columnTwo, LOW);
  digitalWrite(columnThree, LOW);
  digitalWrite(columnFour, LOW);  
  digitalWrite(columnFive, LOW);
  digitalWrite(columnSix, LOW);
  digitalWrite(columnSeven, LOW);        
  digitalWrite(rowZero, HIGH);
  digitalWrite(rowOne, HIGH);
  digitalWrite(rowTwo, HIGH);
  digitalWrite(rowThree, HIGH);
  digitalWrite(rowFour, HIGH);
  digitalWrite(rowFive, HIGH);
  digitalWrite(rowSix, HIGH);
  digitalWrite(rowSeven, HIGH);      
}

void blankLights() {

    for(int i=0;i<numLights;i++){
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(0,0,0)); // Moderately bright green color.
      pixels.show(); // This sends the updated pixel color to the hardware.
    }
  
}

void checkV() {

  if (pieceVoltage <= 220) {
    pieceNumber = 1;           //White Pawn           
  }
  else if ((220 < pieceVoltage) && (pieceVoltage <= 280)) {
    pieceNumber = 2;        //White Rook
  }
  else if ((280 < pieceVoltage) && (pieceVoltage <= 350)) {
    pieceNumber = 3;       // White Knight
  }
  else if ((350 < pieceVoltage) && (pieceVoltage <= 430)) {
    pieceNumber = 4;    //White Bishop
  }
  else if ((430 < pieceVoltage) && (pieceVoltage <= 510)) {
    pieceNumber = 5;    //White Queen
  }
  else if ((510 < pieceVoltage) && (pieceVoltage <= 570)) {
    pieceNumber = 6;    //White King
  }
  else if ((570 < pieceVoltage) && (pieceVoltage <= 680)) {
    pieceNumber = 7;    //Black pawn
  }
  else if ((680 < pieceVoltage) && (pieceVoltage <= 730)) {
    pieceNumber = 8;    //Black Rook
  }
  else if ((730 < pieceVoltage) && (pieceVoltage <= 800)) {
    pieceNumber = 9;    //Black Knight
  }
  else if ((800 < pieceVoltage) && (pieceVoltage <= 870)) {
    pieceNumber = 10;   //Black Bishop 
  }
  else if ((870 < pieceVoltage) && (pieceVoltage <= 950)) {
    pieceNumber = 11;    //Black Queen
  }
  else if ((950 < pieceVoltage) && (pieceVoltage <= 1010)) {
    pieceNumber = 12;   //Black King 
  }
  else {
    pieceNumber = 13;   //Empty Space 
  }
}

void showMoves(int x) {

  switch (x) {
    case 1:
      wPawnMoves();
      break;
    case 2:
      wRookMoves();
      break;
    case 3:
      wKnightMoves();
      break;
    case 4:
      wBishopMoves();
      break;
    case 5:
      wQueenMoves();
      break;
    case 6:
      wKingMoves();
      break;
    case 12:
      bPawnMoves();
      break;
    case 7:
      bRookMoves();
      break;
    case 9:
      bKnightMoves();
      break;
    case 10:
      bBishopMoves();
      break;
    case 11:
      bQueenMoves();
      break;
    case 8:
      bKingMoves();
      break;
  }

}

void checkPickUp() {
      
  if ((pieceNumber != lastPieceNumber) && pieceNumber == 13) {
    blankLights();
    Serial.print("Piece picked up: ");
    Serial.print(lastPieceNumber);
    Serial.print(" at: ");
    Serial.print(i);
    Serial.print(", ");
    Serial.println(a);
    matrixToArray(i, a);
    colourswap1();
    showMoves(lastPieceNumber);
    pixels.show();
  }
}

void checkPutDown() {
  if ((pieceNumber != lastPieceNumber) && lastPieceNumber == 13) {
    blankLights();
    if (pieceNumber > 6) {
      colourM = 1;
    }
    else {
      colourM = 0;  
    }
  }  
}

void chooseAnalog(int numAnalog) {

  switch (numAnalog) {
    case 0:
      digitalWrite(columnZero, HIGH);
      pieceVoltage = analogRead(testColumnZero);
      break;
    case 1:
      digitalWrite(columnOne, HIGH);
      pieceVoltage = analogRead(testColumnOne);
      break;
    case 2:
      digitalWrite(columnTwo, HIGH);
      pieceVoltage = analogRead(testColumnTwo);
      break;
    case 3:
      digitalWrite(columnThree, HIGH);
      pieceVoltage = analogRead(testColumnThree);
      break;
    case 4:
      digitalWrite(columnFour, HIGH);
      pieceVoltage = analogRead(testColumnFour);
      break; 
    case 5:
      digitalWrite(columnFive, HIGH);
      pieceVoltage = analogRead(testColumnFive);
      break;  
    case 6:
      digitalWrite(columnSix, HIGH);
      pieceVoltage = analogRead(testColumnSix);
      break;        
    case 7:
      digitalWrite(columnSeven, HIGH);
      pieceVoltage = analogRead(testColumnSeven);
      break;
  }
}
  
void matrixToArray(int i, int a) {

  // If i is even
  if (i % 2 == 0){
  ledNumber = (i*8) + a;
  }
  else 
  ledNumber = (i*8) + (7-a); 
}

bool PieceMovement(char colour, int piece, int row, int column){
  
bool possible = 0; 

int originalRow;
int originalColumn; 

if ((row >= 0) && (row <= 7) && (column <= 7) && (column >= 0)){

    int prevpiece = tile[row][column];
      if (CheckFunction(colour) == false){
        
        tile[row][column] = piece;

        if (CheckFunction(colour) == false) {
          possible = true; 
        matrixToArray(row,column);

        if (prevpiece == Empty){
          if (Wenpassant[column] == true){
            colourswap3();
          }
          else if (Benpassant[column] == true){
            colourswap3();
          }
          else {
            colourswap2();
          }
        }
        else {
          colourswap3();
        }
      }
      tile[row][column] = prevpiece;  
    }

    else{

      for (int j = 0; j<8; j++) {
        for (int k = 0; k<8 ; k++) {
          if (tile[j][k] == piece) {
            tile[j][k] = 13;
            originalRow = j;
            originalColumn = k; 
          }
        }
      }
    tile[row][column] = piece;   
                

    if (CheckFunction(colour) == false){
      matrixToArray(row,column);
          possible = true; 

        if (prevpiece == Empty){
          //colourswap2();
        }
      else {
          //colourswap3(); ???

        }
      }
      tile[row][column] = prevpiece;
      tile[originalRow][originalColumn] = piece;
      
    } 
    
  }
  if (possible == true){

    return true;
  } else {
  return false;
  }
}
 
bool CheckFunction(char colour){      //W is white, B is black 

  int KingRow;
  int KingColumn;
  
  //Checking if White is in Check 
  if (colour == 'W'){

//Run through tiles 
  for (int column = 0; column < 8; column++){

    for (int row = 0; row < 8; row++){

    //Find White King
      if (tile[row][column] == WKing){
        
        KingRow = row;
        KingColumn = column;       
      }
      
    //Find Black Pawn 
       if (tile[row][column] == BPawn){
        
         //Check if Black Pawn is Attacking WKing
        if ((tile[row + 1][column + 1] == WKing) || (tile[row + 1][column - 1] == WKing)) {

           return true; 
        }
      }
      
    //Find Black Rook
       if (tile[row][column] == BRook){
        
         int BRookRow = row;
         int BRookColumn = column;
                 //Check if Black Rook is Attacking WKing
          for (int j = BRookRow+1; j < 8; j++)
          {
           //If theres are piece leading up to the rook row from the right,
              if (tile[j][BRookColumn] != Empty)
            {
                if (tile[j][BRookColumn] == WKing){
                  //If king is in line of rook
                     
                  return true; 
                }
              //immediately terminate the for loop
                  break;    
            }  
          }

          //Count from left of the rook to end of board
           for (int j = BRookRow - 1; j >= 0; j--)
           {
           //If theres are piece leading up to the rook row from the right,
              if (tile[j][BRookColumn] != Empty)
             {  
                //Find out if the piece is white or black (1 to 6 is white, 7 to 12 black)
                if (tile[j][BRookColumn] == WKing){
                     
                  return true; 
                }
              //immediately terminate the for loop
                  break;    
             }
           }
          //Count from rook collumn to bottom of board
             for (int j = BRookColumn - 1; j >= 0; j--)
           {
           //If theres are piece below rook
              if (tile[BRookRow][j] != Empty)
             {
                if (tile[BRookRow][j] == WKing){
                     
                  return true; 
                }
              //immediately terminate the for loop
                  break;    
            } 
          }
          //Count from rook column to top of board
             for (int j = BRookColumn +1; j < 8; j++)
           {
           //If theres are piece leading up to the rook row from the right, c
              if (tile[BRookRow][j] != Empty)
             {
                if (tile[BRookRow][j] == WKing){

                 return true; 
                }
                  break;    
            }   
          }   
     }
      
      //Find Black Knight 
             if (tile[row][column] == BKnight){

                      //#1
                if (tile[row+2][column+1] == WKing){
                //Check if move fits on the board 
                  if ((row+2 <= 7) && (column+1 <= 7)){
   
                     return true;    
                                            
                  }
                }
                      //#2  
                        if (tile[row+2][column-1] == WKing)
                        {
                      //Check if move fits on the board 
                        if ((row+2 <= 7) && (column-1 >= 0)){
   
                       return true; 
                       }
                    }
                       //#3
                      if (tile[row-2][column+1] == WKing){
                    //Check if move fits on the board 
                      if ((row-2 >= 0) && (column+1 <= 7)){
                           
                        return true; 
                     }
                    }
                     //#4
                      if (tile[row-2][column-1] == WKing){
                    //Check if move fits on the board 
                      if ((row-2 >= 0) && (column-1 >= 0)){
                           
                        return true; 
                        }
                      }
                       //#5
                        if (tile[row+1][column+2] == WKing){
                      //Check if move fits on the board 
                        if ((row+1 <= 7) && (column+2 <= 7)){
   
                       return true; 
                      }
                        }
                      //#6
                        if (tile[row-1][column+2] == WKing){
                      //Check if move fits on the board 
                        if ((row-1 >= 0) && (column+2 <= 7)){
                             
                          return true; 
                      }
                        }
                      //#7
                      if (tile[row-1][column-2] == WKing){
                      //Check if move fits on the board 
                        if ((row-1 >= 0) && (column-2 >= 0)){
                          
                          return true; 
                      }
                        }
                      //#8
                      if (tile[row+1][column-2] == WKing){
                      //Check if move fits on the board 
                        if ((row+1 <= 7) && (column-2 >= 0)){

                         }
                        }
             }
      //Find Black Bishop
      if (tile[row][column] == BBishop){

        int bishopRow = row;
        int bishopColumn = column; 

              for (int j = bishopRow+1; j <= 7; j++)
              {
                      if (tile[j][bishopColumn +j - bishopRow] != Empty)
                    {
                        if (tile[j][bishopColumn+j - bishopRow] == WKing){            
                           return true; 
                        }
                        break;    
                    }
              }
              for (int j = bishopRow+1; j <= 7; j++){

                      if (bishopColumn - j + bishopRow >= 0){
                      if (tile[j][bishopColumn - j + bishopRow] != Empty)
                    {
                        if (tile[j][bishopColumn - j + bishopRow] == WKing){
                           return true; 
                        }
                      //immediately terminate the for loop
                        break;    
                    }
                  }   
              }  
               for (int j = bishopRow - 1; j >= 0; j--)
               {
                  if (bishopColumn + j - bishopRow >= 0){
                    if (tile[j][bishopColumn +j -bishopRow] != Empty)
                   {  
                      if (tile[j][bishopColumn +j -bishopRow] == WKing){
                       return true;           
                      }
                    //immediately terminate the for loop
                      break;    
                    }
                 }
               }
               for (int j = bishopRow - 1; j >= 0; j--){
                    if (tile[j][bishopColumn- j + bishopRow] != Empty)
                  {
                      if (tile[j][bishopColumn- j + bishopRow] == WKing){
                        return true; 
                        
                      }
                    //immediately terminate the for loop
                      break;    
                  }
                }     
            /**************************** UP TO DOWN*******************/
                 for (int j = bishopColumn - 1; j >= 0; j--)
               {
                 if (bishopRow + j - bishopColumn >= 0){
                    if (tile[bishopRow+j-bishopColumn][j] != Empty)
                   {
                      if (tile[bishopRow+j-bishopColumn][j] == WKing){
                        return true; 
                        
                      }
                    //immediately terminate the for loop
                      break;    
                   }
                }
               }
                 for (int j = bishopColumn - 1; j >= 0; j--){
                    if (tile[bishopRow -j+bishopColumn][j] != Empty)
                  {
                      if (tile[bishopRow -j+bishopColumn][j] == WKing){
                        return true; 
                        
                      }
                    //immediately terminate the for loop
                      break;    
                  }
                }      
                 for (int j = bishopColumn +1; j <= 7; j++)
               {
                  if (tile[bishopRow+j-bishopColumn][j] != Empty)
                 {
                    if (tile[bishopRow+j-bishopColumn][j] == WKing){
                          return true; 
                        
                    }
                      break;    
                }
               } 
               for (int j = bishopColumn +1; j <= 7; j++){
                 if (bishopRow - j + bishopColumn >= 0){
                    if (tile[bishopRow -j+bishopColumn][j] != Empty)
                  {
                      if (tile[bishopRow -j+bishopColumn][j] == WKing){
                        return true;
                      }
                    //immediately terminate the for loop
                      break;    
                  }
                }     
              }
      }
      //Find Black Queen 
      if (tile[row][column] == BQueen){

  int queenRow = row;
  int queenColumn = column;
  
             /*************************ROOK PART******************/

           /****************************LEFT TO RIGHT*******************/

        //Count from right of the rook to end of board
          for (int j = queenRow+1; j < 8; j++)
          {
              if (tile[j][queenColumn] != Empty)
            {
                if (tile[j][queenColumn] == WKing){                
                 return true; 
                }
              //immediately terminate the for loop
                  break;    
            }
          }
          //Count from left of the rook to end of board
           for (int j = queenRow - 1; j >= 0; j--)
           {
           //If theres are piece leading up to the rook row from the right, c
              if (tile[j][queenColumn] != Empty)
             {  

                if (tile[j][queenColumn] == WKing){                  
                  return true; 
                }
              //immediately terminate the for loop
                  break;    
            }   
          }
        /**************************** UP TO DOWN*******************/
             for (int j = queenColumn - 1; j >= 0; j--)
           {
              if (tile[queenRow][j] != Empty)
             {
                if (tile[queenRow][j] == WKing){               
                  return true; 
                }
              //immediately terminate the for loop
                  break;    
            }
          }
             for (int j = queenColumn +1; j < 8; j++)
           {
              if (tile[queenRow][j] != Empty)
             {
                 if (tile[queenRow][j] == WKing){                
                 return true;  
                }
              //immediately terminate the for loop
                  break;    
            }   
          }
            /****************************BISHOP PART*******************/
          /****************************LEFT TO RIGHT*******************/
          for (int j = queenRow+1; j <= 7; j++)
          {
                if (tile[j][queenColumn+j - queenRow] != Empty)
              {
                  if (tile[j][queenColumn+j - queenRow] == WKing){
                      return true; 
                  
                  }
                //immediately terminate the for loop
                  break;    
              }
            }
           for (int j = queenRow+1; j <= 7; j++){
                if (tile[j][queenColumn- j + queenRow] != Empty)
              {
                  if (tile[j][queenColumn- j + queenRow] == WKing){                  
                    return true; 
                    
                  }
                //immediately terminate the for loop
                  break;    
              }
            }     
           for (int j = queenRow - 1; j >= 0; j--)
           {
                if (tile[j][queenColumn+j -queenRow] != Empty)
               {  
                  if (tile[j][queenColumn+j -queenRow] == WKing){                  
                      return true; 
                    
                  }
                //immediately terminate the for loop
                  break;    
                }
           }
           for (int j = queenRow - 1; j >= 0; j--){
                if (tile[j][queenColumn- j + queenRow] != Empty)
              {
                  if (tile[j][queenColumn- j + queenRow] == WKing){               
                    return true; 
                    
                  }
                //immediately terminate the for loop
                  break;    
              }
            }   
        /**************************** UP TO DOWN*******************/
             for (int j = queenColumn - 1; j >= 0; j--)
           {

                if (tile[queenRow+j-queenColumn][j] != Empty)
               {
                    if (tile[queenRow+j-queenColumn][j] == WKing){                  
                    return true; 
                    
                  }
                //immediately terminate the for loop
                  break;    
               }
           }
           for (int j = queenColumn - 1; j >= 0; j--){

                if (tile[queenRow-j+queenColumn][j] != Empty)
              {
                  if (tile[queenRow-j+queenColumn][j] == WKing){                  
                        return true;        
                  }
                //immediately terminate the for loop
                  break;    
              }
            }                
             for (int j = queenColumn +1; j <= 7; j++)
           {
              if (tile[queenRow+j-queenColumn][j] != Empty)
             {
                if (tile[queenRow+j-queenColumn][j] == WKing){                
                      return true; 
                }
                  break;    
            }
           }
             for (int j = queenColumn +1; j <= 7; j++){
                if (tile[queenRow-j+queenColumn][j] != Empty)
              {
                  if (tile[queenRow-j+queenColumn][j] == WKing){                 
                    return true;
                  }
                  break;    
              }
            }            
          }
         
        if (tile[row][column] == BKing) {
          //check can move up
          if (tile[row][column+1] == WKing) {
            return true;
          }
          if (tile[row][column-1] == WKing) {
            return true;
          }
          if (tile[row+1][column] == WKing) {
            return true;
          }          
            if (tile[row-1][column] == WKing) {
            return true;
          }
          if (tile[row+1][column+1] == WKing) {
            return true;
          }
          if (tile[row-1][column+1] == WKing) {
            return true;
          }
          if (tile[row+1][column-1] == WKing) {
            return true;
          }
          if (tile[row-1][column-1] == WKing) {
            return true;
          }
        }                                        
      }
    }  
  }
  
  //If Colour is B
  if (colour == 'B'){
      //Run through tiles 
  for (int column = 0; column < 8; column++){
    for (int row = 0; row < 8; row++){
    //Find White King
      if (tile[row][column] == BKing){
        
        int KingRow = row;
        int KingColumn = column; 
      }
      
    //Find White Pawn 
       if (tile[row][column] == WPawn){
        
         //Check if Black Pawn is Attacking BKing
        if ((tile[row - 1][column - 1] == BKing) || (tile[row - 1][column+ 1] == BKing)) {
        
           return true; 
        }
      }
      
    //Find White Rook
       if (tile[row][column] == WRook){
        
         int BRookRow = row;
         int BRookColumn = column;
                 //Check if Black Rook is Attacking BKing
          for (int j = BRookRow+1; j < 8; j++)
          {
           //If theres are piece leading up to the rook row from the right,
              if (tile[j][BRookColumn] != Empty)
            {
                if (tile[j][BRookColumn] == BKing){
                  //If king is in line of rook
                  return true; 
                }
              //immediately terminate the for loop
                  break;    
            }  
          }

          //Count from left of the rook to end of board
           for (int j = BRookRow - 1; j >= 0; j--)
           {
           //If theres are piece leading up to the rook row from the right,
              if (tile[j][BRookColumn] != Empty)
             {  
                //Find out if the piece is white or black (1 to 6 is white, 7 to 12 black)
                if (tile[j][BRookColumn] == BKing){
                  
                  return true; 
                }
              //immediately terminate the for loop
                  break;    
             }
           }
          //Count from rook collumn to bottom of board
             for (int j = BRookColumn - 1; j >= 0; j--)
           {
           //If theres are piece below rook
              if (tile[BRookRow][j] != Empty)
             {
                if (tile[BRookRow][j] == BKing){
                  return true; 
                }
              //immediately terminate the for loop
                  break;    
            } 
          }
          //Count from rook column to top of board
             for (int j = BRookColumn +1; j < 8; j++)
           {
           //If theres are piece leading up to the rook row from the right, c
              if (tile[BRookRow][j] != Empty)
             {
                if (tile[BRookRow][j] == BKing){
                 return true; 
                }
                  break;    
            }   
          }   
     }
     
      //Find White Knight 
             if (tile[row][column] == WKnight){
                      //#1
                if (tile[row+2][column+1] == BKing){
                //Check if move fits on the board 
                  if ((row+2 <= 7) && (column+1 <= 7)){
                     return true;                                      
                  }
                }
                      //#2  
                        if (tile[row+2][column-1] == BKing)
                        {
                      //Check if move fits on the board 
                        if ((row+2 <= 7) && (column-1 >= 0)){

                       return true; 
                       }
                    }
                       //#3
                      if (tile[row-2][column+1] == BKing){
                    //Check if move fits on the board 
                      if ((row-2 >= 0) && (column+1 <= 7)){
                        return true; 
                     }
                    }
                     //#4
                      if (tile[row-2][column-1] == BKing){
                    //Check if move fits on the board 
                      if ((row-2 >= 0) && (column-1 >= 0)){
                        return true; 
                        }
                      }
                       //#5
                        if (tile[row+1][column+2] == BKing){
                      //Check if move fits on the board 
                        if ((row+1 <= 7) && (column+2 <= 7)){

                       return true; 
                      }
                        }
                      //#6
                        if (tile[row-1][column+2] == BKing){
                      //Check if move fits on the board 
                        if ((row-1 >= 0) && (column+2 <= 7)){
                          return true; 
                      }
                        }
                      //#7
                      if (tile[row-1][column-2] == BKing){
                      //Check if move fits on the board 
                        if ((row-1 >= 0) && (column-2 >= 0)){
                          return true; 
                      }
                        }
                      //#8
                      if (tile[row+1][column-2] == BKing){
                      //Check if move fits on the board 
                        if ((row+1 <= 7) && (column-2 >= 0)){
                          return true;
                         }
                        }
             }
      //Find White Bishop
      if (tile[row][column] == WBishop){

        int bishopRow = row;
        int bishopColumn = column; 
               for (int j = bishopRow+1; j <= 7; j++)
              {
                      if (tile[j][bishopColumn +j - bishopRow] != Empty)
                    {
                        if (tile[j][bishopColumn+j - bishopRow] == BKing){            
                           return true; 
                        }
                        break;    
                    }
              }
              for (int j = bishopRow+1; j <= 7; j++){

                      if (bishopColumn - j + bishopRow >= 0){
                      if (tile[j][bishopColumn - j + bishopRow] != Empty)
                    {
                        if (tile[j][bishopColumn - j + bishopRow] == BKing){
                           return true; 
                        }
                      //immediately terminate the for loop
                        break;    
                    }
                  }   
              }  
               for (int j = bishopRow - 1; j >= 0; j--)
               {
                  if (bishopColumn + j - bishopRow >= 0){
                    if (tile[j][bishopColumn +j -bishopRow] != Empty)
                   {  
                      if (tile[j][bishopColumn +j -bishopRow] == BKing){
                       return true;           
                      }
                    //immediately terminate the for loop
                      break;    
                    }
                 }
               }
               for (int j = bishopRow - 1; j >= 0; j--){
                    if (tile[j][bishopColumn- j + bishopRow] != Empty)
                  {
                      if (tile[j][bishopColumn- j + bishopRow] == BKing){
                        return true; 
                        
                      }
                    //immediately terminate the for loop
                      break;    
                  }
                }     
            /**************************** UP TO DOWN*******************/
                 for (int j = bishopColumn - 1; j >= 0; j--)
               {
                 if (bishopRow + j - bishopColumn >= 0){
                    if (tile[bishopRow+j-bishopColumn][j] != Empty)
                   {
                      if (tile[bishopRow+j-bishopColumn][j] == BKing){
                        return true; 
                        
                      }
                    //immediately terminate the for loop
                      break;    
                   }
                }
               }
                 for (int j = bishopColumn - 1; j >= 0; j--){
                    if (tile[bishopRow -j+bishopColumn][j] != Empty)
                  {
                      if (tile[bishopRow -j+bishopColumn][j] == BKing){
                        return true; 
                        
                      }
                    //immediately terminate the for loop
                      break;    
                  }
                }      
                 for (int j = bishopColumn +1; j <= 7; j++)
               {
                  if (tile[bishopRow+j-bishopColumn][j] != Empty)
                 {
                    if (tile[bishopRow+j-bishopColumn][j] == BKing){
                          return true; 
                        
                    }
                      break;    
                }
               } 
               for (int j = bishopColumn +1; j <= 7; j++){
                 if (bishopRow - j + bishopColumn >= 0){
                    if (tile[bishopRow -j+bishopColumn][j] != Empty)
                  {
                      if (tile[bishopRow -j+bishopColumn][j] == BKing){
                        return true;
                      }
                    //immediately terminate the for loop
                      break;    
                  }
                }     
              }
      }
      //Find White Queen 
      if (tile[row][column] == WQueen){

  int queenRow = row;
  int queenColumn = column;
  
             /*************************ROOK PART******************/

          for (int j = queenRow+1; j < 8; j++)
          {
              if (tile[j][queenColumn] != Empty)
            {
                if (tile[j][queenColumn] == BKing){
                 return true; 
                }
              //immediately terminate the for loop
                  break;    
            }
          }
           for (int j = queenRow - 1; j >= 0; j--)
           {
              if (tile[j][queenColumn] != Empty)
             {  
                if (tile[j][queenColumn] == BKing){
                  return true; 
                }
              //immediately terminate the for loop
                  break;    
            }   
          }
        /**************************** UP TO DOWN*******************/
             for (int j = queenColumn - 1; j >= 0; j--)
           {
              if (tile[queenRow][j] != Empty)
             {
                if (tile[queenRow][j] == BKing){
                  return true; 
                }
              //immediately terminate the for loop
                  break;    
            }

          }
             for (int j = queenColumn +1; j < 8; j++)
           {
              if (tile[queenRow][j] != Empty)
             {
                 if (tile[queenRow][j] == BKing){
                 return true;  
                }
              //immediately terminate the for loop
                  break;    
            }   
          }
            /****************************BISHOP PART*******************/
          for (int j = queenRow+1; j <= 7; j++)
          {
                if (tile[j][queenColumn+j - queenRow] != Empty)
              {
                  if (tile[j][queenColumn+j - queenRow] == BKing){
                    //Turn on led to signify that you can take the piece   
                      return true; 
                  }
                //immediately terminate the for loop
                  break;    
              }
          }
          for (int j = queenRow+1; j <= 7; j++){
                if (tile[j][queenColumn- j + queenRow] != Empty)
              {
                  if (tile[j][queenColumn- j + queenRow] == BKing){
                    return true; 
                    }                  
                //immediately terminate the for loop
                  break;    
              }
            }     
           for (int j = queenRow - 1; j >= 0; j--)
           {
                if (tile[j][queenColumn+j -queenRow] != Empty)
               {  
                  if (tile[j][queenColumn+j -queenRow] == BKing){

                      return true; 
                    
                  }
                //immediately terminate the for loop
                  break;    
                }
           }
           for (int j = queenRow - 1; j >= 0; j--){
                if (tile[j][queenColumn- j + queenRow] != Empty)
              {
                  if (tile[j][queenColumn- j + queenRow] == BKing){

                    return true;    
                  }
                //immediately terminate the for loop
                  break;    
              }
            }   
        /**************************** UP TO DOWN*******************/
             for (int j = queenColumn - 1; j >= 0; j--)
           {
                if (tile[queenRow+j-queenColumn][j] != Empty)
               {
                if (tile[queenRow+j-queenColumn][j] == BKing){
                    return true;     
                  }
                //immediately terminate the for loop
                  break;    
               }
           }
           for (int j = queenColumn - 1; j >= 0; j--){
                if (tile[queenRow-j+queenColumn][j] != Empty)
              {
                  if (tile[queenRow-j+queenColumn][j] == BKing){
                        return true;
                  }
                //immediately terminate the for loop
                  break;    
              }
            }                
             for (int j = queenColumn +1; j <= 7; j++)
           {
              if (tile[queenRow+j-queenColumn][j] != Empty)
             {
                if (tile[queenRow+j-queenColumn][j] == BKing){
                      return true; 
                    }
                }
              //immediately terminate the for loop
                  break;    
            }
           for (int j = queenColumn +1; j <= 7; j++){
                if (tile[queenRow-j+queenColumn][j] != Empty)
              {
                  if (tile[queenRow-j+queenColumn][j] == BKing){
                    return true;
                  }
                //immediately terminate the for loop
                  break;    
              }
            }            
          }
        if (tile[row][column] == WKing) {
          //check can move up
          if (tile[row][column+1] == BKing) {
            return true;
          }
          if (tile[row][column-1] == BKing) {
            return true;
          }
          if (tile[row+1][column] == BKing) {
            return true;
          }          
            if (tile[row-1][column] == BKing) {
            return true;
          }
          if (tile[row+1][column+1] == BKing) {
            return true;
          }
          if (tile[row-1][column+1] == BKing) {
            return true;
          }
          if (tile[row+1][column-1] == BKing) {
            return true;
          }
          if (tile[row-1][column-1] == BKing) {
            return true;
          }
        }
       
      }
    }
  }
 //If Nothing is putting king in check
return false; 
}
bool wPawnMoves() { // moves forward 1 if empty, Move diagonal if enemy, move 2 if start
  /* 'i' is row number, 'a' is column number*/

bool possiblemoves = false;
enpassant();
  
//Simple Move Forward if "13" (Empty Space) 
    if (tile[i-1][a] == Empty) {    
     PieceMovement('W',WPawn,i-1,a);
     if (PieceMovement('W',WPawn,i-1,a) == true){
       possiblemoves = true; 
     }
    }
  //If black pawn diagonal to white pawn 
    if ((tile[i-1][a+1] > 6) && (tile[i-1][a+1] != Empty)) {
     PieceMovement('W',WPawn,i-1,a+1);
       if  (PieceMovement('W',WPawn,i-1,a+1) == true){
       possiblemoves = true; 
     }
      }
   
    if ((tile[i-1][a-1] > 6) && (tile[i-1][a-1] != Empty)) {
       if  (PieceMovement('W',WPawn,i-1,a-1) == true){
       possiblemoves = true; 
     }
    }
    
  //If at the start of game, possible to move twice 
    if (i == 6 && ((tile[i-1][a]) == Empty) && (tile[i-2][a]) == Empty) {
   PieceMovement('W',WPawn,i-2,a);
   if  (PieceMovement('W',WPawn,i-2,a) == true){
       possiblemoves = true; 
     }
    }
  
  //Setting Up En Passant (if pawn has moved twice and its the start, enable en passant 
  if ((i == 3) ){
    
    if  ((Benpassant[a+1] == 1)){
      PieceMovement('W',WPawn,i-1,a+1);
      if  ( PieceMovement('W',WPawn,i-1,a+1) == true){
       possiblemoves = true; 
     }
  }
     if  (Benpassant[a-1] == 1) {
      PieceMovement('W',WPawn,i-1,a-1);
      if  (PieceMovement('W',WPawn,i-1,a-1) == true){
       possiblemoves = true; 
     }
    } 
  }
  //If pawn not at the start, change to 0 (THIS HAS TO BE AFTER THE EN PASSANT CHECK OTHERWISE IT WONT WORK)
  if (i != 6){
  Wpawnstart[a] = 0;
  }
  //Used to see if game is checkmate
  if (possiblemoves == true){
    return true;
  }
  else
  return false; 
}

bool bPawnMoves() { // moves forward 1 if empty, Move diagonal if enemy, move 2 if start Returns true if possible, Returns false if not (USED FOR CHECKMATE)
bool possiblemoves = false;

  enpassant();
  //Simple Move up fowards if empty space
    if ((tile[i+1][a] == Empty)) {
   PieceMovement('B',BPawn,i+1,a);
 if (PieceMovement('B',BPawn,i+1,a) == true){
       possiblemoves = true; 
     }    }
    //If white pawn diagonal to black pawn 
    if (tile[i+1][a-1] <= 6) {
       PieceMovement('B',BPawn,i+1,a-1);
 if (PieceMovement('B',BPawn,i+1,a-1) == true){
       possiblemoves = true; 
     }
   }
    if (tile[i+1][a+1] <= 6) {
   PieceMovement('B',BPawn,i+1,a+1);
 if  (PieceMovement('B',BPawn,i+1,a+1)== true){
       possiblemoves = true; 
     }
    }
    //If at the start of game, possible to move twice 
    if (i == 1 && ((tile[i+1][a]) == Empty) && (tile[i+2][a]) == Empty) {
       PieceMovement('B',BPawn,i+2,a);
 if  (PieceMovement('B',BPawn,i+2,a) == true){
       possiblemoves = true; 
     }
    }
  
   //Setting Up En Passant (if pawn has moved twice and its the start, enable en passant 
  if (i == 4) {
    if (Wenpassant[a + 1] == 1) {
       PieceMovement('B',BPawn,i+1,a+1);
 if  (PieceMovement('B',BPawn,i+1,a+1) == true){
       possiblemoves = true; 
     }
    }
     
    if (Wenpassant[a - 1] == 1){
       PieceMovement('B',BPawn,i+1,a-1);
 if  (PieceMovement('B',BPawn,i+1,a-1) == true){
       possiblemoves = true; 
     }
    }
  }
  //If pawn not at the start, change to 0 (THIS HAS TO BE AFTER THE EN PASSANT CHECK OTHERWISE IT WONT WORK)
  if (i != 1){
  Bpawnstart[a] = 0;
  }
  //Used to see if game is checkmate
  if (possiblemoves == true){
    return true;
  }
  else
  return false;
}

bool wRookMoves() {
   
   bool possiblemoves = 0; 
  int rookrow = i;
  int rookcolumn = a;
  /****************************LEFT TO RIGHT*******************/

//Count from right of the rook to end of board
  for (int j = rookrow+1; j < 8; j++)
  {
   if (tile[j][a] != 13)
     {
        if (tile[j][a] > 6){
          
        PieceMovement('W',WRook,j,a);
if  (PieceMovement('W',WRook,j,a) == true){
       possiblemoves = true; 
     } 
   }
          break;    
    }
    else
      PieceMovement('W',WRook,j,a);
if  (PieceMovement('W',WRook,j,a) == true){
       possiblemoves = true; 
     }       }   
  
  
  //Count from left of the rook to end of board
   for (int j = rookrow - 1; j >= 0; j--)
   {
   if (tile[j][a] != 13)
     {
        if (tile[j][a] > 6){
        PieceMovement('W',WRook,j,a);
if  (PieceMovement('W',WRook,j,a) == true){
       possiblemoves = true; 
     }      
       }
          break;    
    }
    else
      PieceMovement('W',WRook,j,a);
if  (PieceMovement('W',WRook,j,a) == true){
       possiblemoves = true; 
     }
   }     
  
/**************************** UP TO DOWN*******************/
  //Count from rook collumn to bottom of board
     for (int j = rookcolumn - 1; j >= 0; j--)
   {
  if (tile[i][j] != 13)
     {
        if (tile[i][j] > 6){
        PieceMovement('W',WRook,i,j);
if  (PieceMovement('W',WRook,i,j) == true){
       possiblemoves = true; 
     }    
         }
          break;    
    }
    else
      PieceMovement('W',WRook,i,j); 
if  ( PieceMovement('W',WRook,i,j) == true){
       possiblemoves = true; 
     }  }
  //Count from rook column to top of board
     for (int j = rookcolumn +1; j < 8; j++)
   {
   //If theres are piece leading up to the rook row from the right, c
     if (tile[i][j] != 13)
     {
        if (tile[i][j] > 6){
        PieceMovement('W',WRook,i,j);
if  ( PieceMovement('W',WRook,i,j)== true){
       possiblemoves = true; 
     }        }
          break;    
    }
    else
      PieceMovement('W',WRook,i,j);
if  (PieceMovement('W',WRook,i,j) == true){
       possiblemoves = true; 
     }  
   }

if (possiblemoves == 1){

  return true; 
} 
else

  return false; 
}

bool bRookMoves() {
     bool possiblemoves = 0; 
    int rookrow = i;
  int rookcolumn = a;
  /**************************** LEFT TO RIGHT *******************/

//Count from right of the rook to end of board
  for (int j = rookrow+1; j < 8; j++)
  {
   //If theres are piece leading up to the rook row from the right, c
      if (tile[j][a] != 13)
    {
        if (tile[j][a] <= 6){
      PieceMovement('B',BRook,j,a);
if  ( PieceMovement('B',BRook,j,a)== true){
       possiblemoves = true; 
     }         }
      //immediately terminate the for loop
          break;    
    }
    else
   PieceMovement('B',BRook,j,a);
if  ( PieceMovement('B',BRook,j,a)== true){
       possiblemoves = true; 
     }   }
  
  //Count from left of the rook to end of board
   for (int j = rookrow - 1; j >= 0; j--)
   {
   //If theres are piece leading up to the rook row from the right, c
      if (tile[j][a] != 13)
     {
           //Find out if the piece is white or black (1 to 6 is white, 7 to 12 black)
        if (tile[j][a] <= 6){
        PieceMovement('B',BRook,j,a);
if  ( PieceMovement('B',BRook,j,a)== true){
       possiblemoves = true; 
     }         }
      //immediately terminate the for loop
          break;    
    }
    else
      PieceMovement('B',BRook,j,a);
if  ( PieceMovement('B',BRook,j,a)== true){
       possiblemoves = true; 
     }   }
  
/**************************** UP TO DOWN*******************/

  //Count from rook column to bottom of board
     for (int j = rookcolumn - 1; j >= 0; j--)
   {
   //If theres are piece leading up to the rook row from the right, c
      if (tile[i][j] != 13)
    {
        if (tile[i][j] <= 6){
      PieceMovement('B',BRook,i,j);
if  ( PieceMovement('B',BRook,i,j)== true){
       possiblemoves = true; 
     }         }
      //immediately terminate the for loop
          break;    
    }
    else
   PieceMovement('B',BRook,i,j);
if  ( PieceMovement('B',BRook,i,j)== true){
       possiblemoves = true; 
     }   }
  
  //Count from left of the rook to end of board
   for (int j = rookcolumn + 1; j <= 7; j++)
   {
   //If theres are piece leading up to the rook row from the right, c
      if (tile[i][j] != 13)
    {
        if (tile[i][j] <= 6){
      PieceMovement('B',BRook,i,j);
if  ( PieceMovement('B',BRook,i,j)== true){
       possiblemoves = true; 
     }         }
      //immediately terminate the for loop
          break;    
    }
    else
   PieceMovement('B',BRook,i,j);
if  ( PieceMovement('B',BRook,i,j)== true){
       possiblemoves = true; 
     } 
  }
  if (possiblemoves == 1){

  return true; 
  } 
else
  return false; 
}

bool wKnightMoves() {
  
  bool possiblemoves = 0;
//Define the 8 possible moves of knight and determine whether they are possible (e.g, inside the board and not where the pieces are same colour 
  
  //#1
  if (tile[i+2][a+1] > 6){
     PieceMovement('W',WKnight,i+2,a+1);
if  ( PieceMovement('W',WKnight,i+2,a+1) == true){
       possiblemoves = true; 
     }   
    }
  //#2  
    if (tile[i+2][a-1] > 6)
    {
   PieceMovement('W',WKnight,i+2,a-1);
if  (PieceMovement('W',WKnight,i+2,a-1) == true){
       possiblemoves = true; 
     }     
  }
   //#3
    if (tile[i-2][a+1] > 6){
    PieceMovement('W',WKnight,i-2,a+1);    
if  (PieceMovement('W',WKnight,i-2,a+1) == true){
       possiblemoves = true; 
     }  
   }
   //#4
    if (tile[i-2][a-1] > 6){
  //Check if move fits on the board 
    PieceMovement('W',WKnight,i-2,a-1); 
if  (PieceMovement('W',WKnight,i-2,a-1) == true){
       possiblemoves = true; 
     } 
       }
    
   //#5
    if (tile[i+1][a+2] > 6){
   PieceMovement('W',WKnight,i+1,a+2);
if  (PieceMovement('W',WKnight,i+1,a+2) == true){
       possiblemoves = true; 
     } 
  }
  //#6
    if (tile[i-1][a+2] > 6){
  PieceMovement('W',WKnight,i-1,a+2);
if  (PieceMovement('W',WKnight,i-1,a+2) == true){
       possiblemoves = true; 
     } 
       }

  //#7
  if (tile[i-1][a-2] > 6){
   PieceMovement('W',WKnight,i-1,a-2);
if  (PieceMovement('W',WKnight,i-1,a-2) == true){
       possiblemoves = true; 
     } 
       }
    
  //#8
  if (tile[i+1][a-2] > 6){  
  PieceMovement('W',WKnight,i+1,a-2);
if  (PieceMovement('W',WKnight,i+1,a-2) == true){
       possiblemoves = true; 
     } 
       }

  //Used to see if game is checkmate
  if (possiblemoves == true){
    return true;
  }
  else
  return false; 
}

bool bKnightMoves() {
     bool possiblemoves = 0;

//Define the 8 possible moves of knight and determine whether they are possible (e.g, inside the board and not where the pieces are same colour 
  
  //#1
  if ((tile[i+2][a+1]  < 7) || (tile[i+2][a+1]  == 13)){
    PieceMovement('B',BKnight,i+2,a+1);
if  (PieceMovement('B',BKnight,i+2,a+1) == true){
       possiblemoves = true; 
     } 
            }   
  //#2  
    if ((tile[i+2][a-1] < 7) || (tile[i+2][a+1]  == 13))
    {
    PieceMovement('B',BKnight,i+2,a-1);
if  (PieceMovement('B',BKnight,i+2,a-1) == true){
       possiblemoves = true; 
     } 
         }
   //#3
    if ((tile[i-2][a+1] < 7) || (tile[i-2][a+1] == 13))
    { 
   PieceMovement('B',BKnight,i-2,a+1);
if  (PieceMovement('B',BKnight,i-2,a+1) == true){
       possiblemoves = true; 
     } 
         }  
   //#4
    if ((tile[i-2][a-1] <7 ) || (tile[i-2][a-1] == 13)){   
    PieceMovement('B',BKnight,i-2,a-1);
if  (PieceMovement('B',BKnight,i-2,a-1) == true){
       possiblemoves = true; 
     } 
         }
   //#5
    if ((tile[i+1][a+2] < 7) || (tile[i+1][a+2] == 13)){
    PieceMovement('B',BKnight,i+1,a+2);
if  (PieceMovement('B',BKnight,i+1,a+2) == true){
       possiblemoves = true; 
     } 
         }
  //#6
    if ((tile[i-1][a+2] < 7) || (tile[i-1][a+2] == 13)){
    PieceMovement('B',BKnight,i-1,a+2);
if  (PieceMovement('B',BKnight,i-1,a+2) == true){
       possiblemoves = true; 
     } 
         }
  //#7
  if ((tile[i-1][a-2] < 7) || (tile[i-1][a-2] == 13)){
    PieceMovement('B',BKnight,i-1,a-2);
if  ( PieceMovement('B',BKnight,i-1,a-2) == true){
       possiblemoves = true; 
     } 
         }
  //#8
  if ((tile[i+1][a-2] < 7) || (tile[i+1][a-2] == 13)){
   PieceMovement('B',BKnight,i+1,a-2);
if  (PieceMovement('B',BKnight,i+1,a-2) == true){
       possiblemoves = true; 
     } 
         }

      //Used to see if game is checkmate
  if (possiblemoves == true){
    return true;
  }
  else
  return false;      
}

bool wBishopMoves() {
       bool possiblemoves = 0; 
    int bishopRow = i;
  int bishopColumn = a;

  /****************************LEFT TO RIGHT*******************/

//Count from right of the bishop to end of board
  for (int j = bishopRow+1; j <= 7; j++)
  {
 //Find out if the piece is white or black (1 to 6 is white, 7 to 12 black)
    if (tile[j][bishopColumn+j - bishopRow] > 6){

       PieceMovement('W',WBishop,j,bishopColumn+j-bishopRow);
if  ( PieceMovement('W',WBishop,j,bishopColumn+j-bishopRow) == true){
       possiblemoves = true; 
     } 
       if (tile[j][bishopColumn + j - bishopRow] != Empty)
      {
    break;  
      }
    }
    else {
      break;
    }
  }
   for (int j = bishopRow+1; j <= 7; j++)
  { 
    //Other Side
    if (tile[j][bishopColumn - j + bishopRow] > 6){
  PieceMovement('W',WBishop,j,bishopColumn-j+bishopRow);
if  (PieceMovement('W',WBishop,j,bishopColumn-j+bishopRow) == true){
       possiblemoves = true; 
     }       
      if (tile[j][bishopColumn - j + bishopRow] != Empty)
      {
    break;  
      }
    } 
    else {
      break;         
    }
  }
  //Count from left of the bishop to end of board
   for (int j = bishopRow - 1; j >= 0; j--)
   {
 //Find out if the piece is white or black (1 to 6 is white, 7 to 12 black)
    if (tile[j][bishopColumn+j - bishopRow] > 6){
       PieceMovement('W',WBishop,j,bishopColumn+j-bishopRow);
if  (PieceMovement('W',WBishop,j,bishopColumn+j-bishopRow) == true){
       possiblemoves = true; 
     }        if (tile[j][bishopColumn + j - bishopRow] != Empty)
      {
        //immediately terminate the for loop
    break;  
      }
    }
    else {
      break;
    }   
   }
        for (int j = bishopRow - 1; j >= 0; j--){
    //Other Side
    if (tile[j][bishopColumn - j + bishopRow] > 6){

  PieceMovement('W',WBishop,j,bishopColumn-j+bishopRow);
  if  ( PieceMovement('W',WBishop,j,bishopColumn-j+bishopRow) == true){
       possiblemoves = true; 
     } 
      if (tile[j][bishopColumn - j + bishopRow] != Empty)
      {
        //immediately terminate the for loop
    break;  
      }
    }  
    else {
      break;
    }    
   }
   
/**************************** UP TO DOWN*******************/
  //Count from rook collumn to bottom of board
     for (int j = bishopColumn - 1; j >= 0; j--)
   {
       //Find out if the piece is white or black (1 to 6 is white, 7 to 12 black)
    if (tile[bishopRow+j - bishopColumn][j] > 6){
       PieceMovement('W',WBishop,bishopRow+j - bishopColumn,j);
if  ( PieceMovement('W',WBishop,bishopRow+j - bishopColumn,j) == true){
       possiblemoves = true; 
     } 
       if (tile[bishopRow+j - bishopColumn][j] != Empty)
      {
        //immediately terminate the for loop
    break;  
      }
    }
    else {
      break;
    }    
   }
         for (int j = bishopColumn - 1; j >= 0; j--){
    //Other Side
    if (tile[bishopRow -j + bishopColumn][j] > 6){

  PieceMovement('W',WBishop,bishopRow -j + bishopColumn,j);
  if  ( PieceMovement('W',WBishop,bishopRow -j + bishopColumn,j) == true){
       possiblemoves = true; 
     } 
      if (tile[bishopRow -j + bishopColumn][j] != Empty)
      {
        //immediately terminate the for loop
    break;  
      }
    }
    else {
      break;
    }     
   }
  //Count from rook column to top of board
     for (int j = bishopColumn +1; j <= 7; j++)
   {
            //Find out if the piece is white or black (1 to 6 is white, 7 to 12 black)
    if (tile[bishopRow+j - bishopColumn][j] > 6){
       PieceMovement('W',WBishop,bishopRow+j - bishopColumn,j);
if  (PieceMovement('W',WBishop,bishopRow+j - bishopColumn,j) == true){
       possiblemoves = true; 
     } 
       if (tile[bishopRow+j - bishopColumn][j] != Empty)
      {
        //immediately terminate the for loop
    break;  
      }
    }
    else {
      break;
    } 
   }
         for (int j = bishopColumn +1; j <= 7; j++)
{   
    //Other Side
    if (tile[bishopRow -j + bishopColumn][j] > 6){

  PieceMovement('W',WBishop,bishopRow -j + bishopColumn,j);
  if  (PieceMovement('W',WBishop,bishopRow -j + bishopColumn,j)== true){
       possiblemoves = true; 
     } 
      if (tile[bishopRow -j + bishopColumn][j] != Empty)
      {
        //immediately terminate the for loop
    break;  
      }
    }
    else {
      break;
    }      
 }
 if (possiblemoves == true){
    return true;
  }
  else
  return false; 
}

bool bBishopMoves() {
      bool possiblemoves = 0; 

    int bishopRow = i;
  int bishopColumn = a;

  /****************************LEFT TO RIGHT*******************/


//Count from right of the bishop to end of board
  for (int j = bishopRow+1; j <= 7; j++)
  {
 //Find out if the piece is white or black (1 to 6 is white, 7 to 12 black)
    if ((tile[j][bishopColumn+j - bishopRow] <= 6) || (tile[j][bishopColumn+j - bishopRow] == Empty)){
       PieceMovement('B',BBishop,j,bishopColumn+j-bishopRow);
if  (  PieceMovement('B',BBishop,j,bishopColumn+j-bishopRow) == true){
       possiblemoves = true; 
     } 
       if (tile[j][bishopColumn + j - bishopRow] != Empty)
      {
        //immediately terminate the for loop
    break;  
      }
    }
    else {
      break;
    }  
  }  
    for (int j = bishopRow+1; j <= 7; j++){

    //Other Side
    if ((tile[j][bishopColumn - j + bishopRow] <= 6) || (tile[j][bishopColumn-j + bishopRow] == Empty)){

  PieceMovement('B',BBishop,j,bishopColumn-j+bishopRow);
  if  ( PieceMovement('B',BBishop,j,bishopColumn-j+bishopRow) == true){
       possiblemoves = true; 
     } 
      if (tile[j][bishopColumn - j + bishopRow] != Empty)
      {
        //immediately terminate the for loop
    break;  
      }
    }
    else {
      break;
    }              
  }
  //Count from left of the bishop to end of board
   for (int j = bishopRow - 1; j >= 0; j--)
   {
 //Find out if the piece is white or black (1 to 6 is white, 7 to 12 black)
    if ((tile[j][bishopColumn+j - bishopRow] <= 6) || (tile[j][bishopColumn+j - bishopRow] == Empty)){
       PieceMovement('B',BBishop,j,bishopColumn+j-bishopRow);
       if  (PieceMovement('B',BBishop,j,bishopColumn+j-bishopRow) == true){
       possiblemoves = true; 
     } 

       if (tile[j][bishopColumn + j - bishopRow] != Empty)
      {
        //immediately terminate the for loop
    break;  
      }
    }
    else {
      break;
    }   
   }
   
   for (int j = bishopRow - 1; j >= 0; j--){

    //Other Side
    if ((tile[j][bishopColumn - j + bishopRow] <= 6) || (tile[j][bishopColumn - j + bishopRow] == Empty)){

  PieceMovement('B',BBishop,j,bishopColumn-j+bishopRow);
      if  ( PieceMovement('B',BBishop,j,bishopColumn-j+bishopRow) == true){
       possiblemoves = true; 
     } 
      if (tile[j][bishopColumn - j + bishopRow] != Empty)
      {
        //immediately terminate the for loop
    break;  
      }
    }
    else {
      break;
    }      
   }
/**************************** UP TO DOWN*******************/ 
  //Count from bishgop collumn to bottom of board
     for (int j = bishopColumn - 1; j >= 0; j--)
   {
       //Find out if the piece is white or black (1 to 6 is white, 7 to 12 black)
    if ((tile[bishopRow+j - bishopColumn][j] <= 6) || (tile[bishopRow+j - bishopColumn][j] == Empty)){
       PieceMovement('B',BBishop,bishopRow+j - bishopColumn,j);
if  (PieceMovement('B',BBishop,bishopRow+j - bishopColumn,j) == true){
       possiblemoves = true; 
     } 
       if (tile[bishopRow+j - bishopColumn][j] != Empty)
      {
        //immediately terminate the for loop
    break;  
      }
    }
    else {
      break;
    }    
   }
   
        for (int j = bishopColumn - 1; j >= 0; j--){
    //Other Side
    if ((tile[bishopRow -j + bishopColumn][j] <= 6) || (tile[j][bishopColumn - j + bishopRow] == Empty)){

  PieceMovement('B',BBishop,bishopRow -j + bishopColumn,j);
  if  ( PieceMovement('B',BBishop,bishopRow -j + bishopColumn,j) == true){
       possiblemoves = true; 
     } 
      if (tile[bishopRow -j + bishopColumn][j] != Empty)
      {
        //immediately terminate the for loop
    break;  
      }
    }
    else {
      break;
    }      
   }
  //Count from rook column to top of board
     for (int j = bishopColumn +1; j <= 7; j++)
   {
            //Find out if the piece is white or black (1 to 6 is white, 7 to 12 black)
    if ((tile[bishopRow+j - bishopColumn][j] <= 6) || (tile[j][bishopColumn+j - bishopRow] == Empty)){
       PieceMovement('B',BBishop,bishopRow+j - bishopColumn,j);
if  ( PieceMovement('B',BBishop,bishopRow+j - bishopColumn,j) == true){
       possiblemoves = true; 
     } 
       if (tile[bishopRow+j - bishopColumn][j] != Empty)
      {
        //immediately terminate the for loop
    break;  
      }
    }
    else {
      break;
    }    
   } 
        for (int j = bishopColumn +1; j <= 7; j++){
    //Other Side
    if ((tile[bishopRow -j + bishopColumn][j] <= 6) || (tile[j][bishopColumn - j + bishopRow] == Empty)){
     PieceMovement('B',BBishop,bishopRow -j + bishopColumn,j);
      if  ( PieceMovement('B',BBishop,bishopRow -j + bishopColumn,j) == true){
       possiblemoves = true; 
     } 
      if (tile[bishopRow -j + bishopColumn][j] != Empty)
      {
        //immediately terminate the for loop
    break;  
      }
    }
    else {
      break;
    }      
 }
 if (possiblemoves == true){
    return true;
  }
  else
  return false; 
}

bool wQueenMoves() {      /* Queen Code is Almagamation of the bishop and rook  */
       bool possiblemoves = 0; 

      int queenRow = i;
  int queenColumn = a;
  /****************************LEFT TO RIGHT*******************/

//Count from right of the queen to end of board
  for (int j = queenRow+1; j < 8; j++)
  {
   if (tile[j][a] != 13)
     {
        if (tile[j][a] > 6){
        PieceMovement('W',WQueen,j,a);
        if  (PieceMovement('W',WQueen,j,a) == true){
       possiblemoves = true; 
     } 
        }
          break;    
    }
    else
      PieceMovement('W',WQueen,j,a);
      if  (PieceMovement('W',WQueen,j,a) == true){
       possiblemoves = true; 
     } 
       }   
  
  
  //Count from left of the queen to end of board
   for (int j = queenRow - 1; j >= 0; j--)
   {
   if (tile[j][a] != 13)
     {
        if (tile[j][a] > 6){
        PieceMovement('W',WQueen,j,a);
        if  (PieceMovement('W',WQueen,j,a) == true){
       possiblemoves = true; 
     } 
        }
          break;    
    }
    else
      PieceMovement('W',WQueen,j,a);
      if  (PieceMovement('W',WQueen,j,a) == true){
       possiblemoves = true; 
     } 
       }     
  
/**************************** UP TO DOWN*******************/
  //Count from queen collumn to bottom of board
     for (int j = queenColumn - 1; j >= 0; j--)
   {
  if (tile[i][j] != 13)
     {
        if (tile[i][j] > 6){
        PieceMovement('W',WQueen,i,j);
        if  (PieceMovement('W',WQueen,i,j) == true){
       possiblemoves = true; 
     } 
        }
          break;    
    }
    else
      PieceMovement('W',WQueen,i,j);  
      if  (PieceMovement('W',WQueen,i,j) == true){
       possiblemoves = true; 
     }  
  }
  //Count from queen column to top of board
     for (int j = queenColumn +1; j < 8; j++)
   {
   //If theres are piece leading up to the queen row from the right, c
     if (tile[i][j] != 13)
     {
        if (tile[i][j] > 6){
        PieceMovement('W',WQueen,i,j);
        if  ( PieceMovement('W',WQueen,i,j) == true){
       possiblemoves = true; 
     } 
        }
          break;    
    }
    else
      PieceMovement('W',WQueen,i,j);
      if  ( PieceMovement('W',WQueen,i,j) == true){
       possiblemoves = true; 
     } 
  }
  
//Count from right of the queen to end of board
  for (int j = queenRow+1; j <= 7; j++)
  {
 //Find out if the piece is white or black (1 to 6 is white, 7 to 12 black)
    if (tile[j][queenColumn+j - queenRow] > 6){
       PieceMovement('W',WQueen,j,queenColumn+j-queenRow);
       if  (PieceMovement('W',WQueen,j,queenColumn+j-queenRow) == true){
       possiblemoves = true; 
     } 

       if (tile[j][queenColumn + j - queenRow] != Empty)
      {
    break;  
      }
    }
    else {
      break;
    } 
  }
    for (int j = queenRow+1; j <= 7; j++){   
    //Other Side
    if (tile[j][queenColumn - j + queenRow] > 6){
  PieceMovement('W',WQueen,j,queenColumn-j+queenRow);
  if  ( PieceMovement('W',WQueen,j,queenColumn-j+queenRow) == true){
       possiblemoves = true; 
     } 
      
      if (tile[j][queenColumn - j + queenRow] != Empty)
      {
    break;  
      }
    }
    else {
      break;
    }              
  }
  //Count from left of the queen to end of board
   for (int j = queenRow - 1; j >= 0; j--)
   {
 //Find out if the piece is white or black (1 to 6 is white, 7 to 12 black)
    if (tile[j][queenColumn+j - queenRow] > 6){
       PieceMovement('W',WQueen,j,queenColumn+j-queenRow);
       if  (PieceMovement('W',WQueen,j,queenColumn+j-queenRow) == true){
       possiblemoves = true; 
     } 

       if (tile[j][queenColumn + j - queenRow] != Empty)
      {
        //immediately terminate the for loop
    break;  
      }
    }
    else {
      break;
    }  
   }  
       for (int j = queenRow - 1; j >= 0; j--)
  {
    //Other Side
    if (tile[j][queenColumn - j + queenRow] > 6){

  PieceMovement('W',WQueen,j,queenColumn-j+queenRow);
  if  (PieceMovement('W',WQueen,j,queenColumn-j+queenRow) == true){
       possiblemoves = true; 
     } 
      if (tile[j][queenColumn - j + queenRow] != Empty)
      {
        //immediately terminate the for loop
    break;  
      }
    }
    else {
      break;
    }      
   }
/**************************** UP TO DOWN*******************/
  //Count from queen collumn to bottom of board
     for (int j = queenColumn - 1; j >= 0; j--)
   {
       //Find out if the piece is white or black (1 to 6 is white, 7 to 12 black)
    if (tile[queenRow+j - queenColumn][j] > 6){
       PieceMovement('W',WQueen,queenRow+j - queenColumn,j);
if  ( PieceMovement('W',WQueen,queenRow+j - queenColumn,j) == true){
       possiblemoves = true; 
     } 
       if (tile[queenRow+j - queenColumn][j] != Empty)
      {
        //immediately terminate the for loop
    break;  
      }
    }
    else {
      break;
    }   
   }
        for (int j = queenColumn - 1; j >= 0; j--)
  {
    //Other Side
    if (tile[queenRow -j + queenColumn][j] > 6){

  PieceMovement('W',WQueen,queenRow -j + queenColumn,j);
  if  ( PieceMovement('W',WQueen,queenRow -j + queenColumn,j) == true){
       possiblemoves = true; 
     } 
      if (tile[queenRow -j + queenColumn][j] != Empty)
      {
        //immediately terminate the for loop
    break;  
      }
    }
    else {
      break;
    }      
   }
  //Count from queen column to top of board
     for (int j = queenColumn +1; j <= 7; j++)
   {
            //Find out if the piece is white or black (1 to 6 is white, 7 to 12 black)
    if (tile[queenRow+j - queenColumn][j] > 6){
       PieceMovement('W',WQueen,queenRow+j - queenColumn,j);
if  ( PieceMovement('W',WQueen,queenRow+j - queenColumn,j) == true){
       possiblemoves = true; 
     } 
       if (tile[queenRow+j - queenColumn][j] != Empty)
      {
        //immediately terminate the for loop
    break;  
      }
    }
    else {
      break;
    }   
   }
        for (int j = queenColumn +1; j <= 7; j++){ 
    //Other Side
    if (tile[queenRow -j + queenColumn][j] > 6){

  PieceMovement('W',WQueen,queenRow -j + queenColumn,j);
  if  (PieceMovement('W',WQueen,queenRow -j + queenColumn,j) == true){
       possiblemoves = true; 
     } 
      if (tile[queenRow -j + queenColumn][j] != Empty)
      {
        //immediately terminate the for loop
    break;  
      }
    }
    else {
      break;
    }      
 }
 if (possiblemoves == true){
    return true;
  }
  else
  return false; 

}

bool bQueenMoves() {
        bool possiblemoves = 0; 
 
  int queenRow = i;
  int queenColumn = a;
  /****************************LEFT TO RIGHT*******************/

//Count from right of the queen to end of board
  for (int j = queenRow+1; j < 8; j++)
  {
   if (tile[j][a] != 13)
     {
        if (tile[j][a] <= 6){
        PieceMovement('B',BQueen,j,a);
        if  (PieceMovement('B',BQueen,j,a) == true){
       possiblemoves = true; 
     } 
        }
          break;    
    }
    else
      PieceMovement('B',BQueen,j,a);
      if  ( PieceMovement('B',BQueen,j,a) == true){
       possiblemoves = true; 
     } 
       }   
  
  
  //Count from left of the queen to end of board
   for (int j = queenRow - 1; j >= 0; j--)
   {
   if (tile[j][a] != 13)
     {
        if (tile[j][a] <= 6){
        PieceMovement('B',BQueen,j,a);
        if  (PieceMovement('B',BQueen,j,a) == true){
       possiblemoves = true; 
     } 
        }
          break;    
    }
    else
      PieceMovement('B',BQueen,j,a);
      if  (PieceMovement('B',BQueen,j,a) == true){
       possiblemoves = true; 
     } 
       }     
  
/**************************** UP TO DOWN*******************/
  //Count from queen collumn to bottom of board
     for (int j = queenColumn - 1; j >= 0; j--)
   {
  if (tile[i][j] != 13)
     {
        if (tile[i][j] <= 6){
        PieceMovement('B',BQueen,i,j);
        if  ( PieceMovement('B',BQueen,i,j) == true){
       possiblemoves = true; 
     } 
        }
          break;    
    }
    else
      PieceMovement('B',BQueen,i,j);   
      if  ( PieceMovement('B',BQueen,i,j) == true){
       possiblemoves = true; 
     } 
  }
  //Count from queen column to top of board
     for (int j = queenColumn +1; j < 8; j++)
   {
   //If theres are piece leading up to the queen row from the right, c
     if (tile[i][j] != 13)
     {
        if (tile[i][j] <= 6){
        PieceMovement('B',BQueen,i,j);
        if  ( (PieceMovement('B',BQueen,i,j)) == true){
       possiblemoves = true; 
     } 
        }
          break;    
    }
    else
      PieceMovement('B',BQueen,i,j);
      if  ( PieceMovement('B',BQueen,i,j) == true){
       possiblemoves = true; 
     } 
  }
  
//Count from right of the queen to end of board
  for (int j = queenRow+1; j <= 7; j++)
  {
 //Find out if the piece is white or black (1 to 6 is white, 7 to 12 black)
    if ((tile[j][queenColumn +j - queenRow] <= 6) || (tile[j][queenColumn +j - queenRow] == Empty)){
       PieceMovement('B',BQueen,j,queenColumn+j-queenRow);
       if  (PieceMovement('B',BQueen,j,queenColumn+j-queenRow) == true){
       possiblemoves = true; 
     } 

       if (tile[j][queenColumn + j - queenRow] != Empty)
      {
    break;  
      }
    }
    else {
      break;
    }   
  } 
      for (int j = queenRow+1; j <= 7; j++){
    //Other Side
    if ((tile[j][queenColumn - j + queenRow] <= 6) || (tile[j][queenColumn - j + queenRow] == Empty)){
  PieceMovement('B',BQueen,j,queenColumn-j+queenRow);
  if  ( PieceMovement('B',BQueen,j,queenColumn-j+queenRow) == true){
       possiblemoves = true; 
     } 
      
      if (tile[j][queenColumn - j + queenRow] != Empty)
      {
    break;  
      }
    }
    else {
      break;
    }              
  }
  //Count from left of the queen to end of board
   for (int j = queenRow - 1; j >= 0; j--)
   {
 //Find out if the piece is white or black (1 to 6 is white, 7 to 12 black)
    if ((tile[j][queenColumn +j - queenRow] <= 6) || (tile[j][queenColumn+j - queenRow] == Empty)){
       PieceMovement('B',BQueen,j,queenColumn+j-queenRow);
if  (PieceMovement('B',BQueen,j,queenColumn+j-queenRow) == true){
       possiblemoves = true; 
     } 
       if (tile[j][queenColumn + j - queenRow] != Empty)
      {
        //immediately terminate the for loop
    break;  
      }
    }
    else {
      break;
    }    
   }
      for (int j = queenRow - 1; j >= 0; j--)
  {
    //Other Side
    if ((tile[j][queenColumn -j + queenRow] <= 6) || (tile[j][queenColumn -j + queenRow] == Empty)){

  PieceMovement('B',BQueen,j,queenColumn-j+queenRow);
  if  (PieceMovement('B',BQueen,j,queenColumn-j+queenRow) == true){
       possiblemoves = true; 
     } 
      if (tile[j][queenColumn - j + queenRow] != Empty)
      {
        //immediately terminate the for loop
    break;  
      }
    }
    else {
      break;
    }      
   }
/**************************** UP TO DOWN*******************/
  //Count from queen collumn to bottom of board
     for (int j = queenColumn - 1; j >= 0; j--)
   {
       //Find out if the piece is white or black (1 to 6 is white, 7 to 12 black)
    if ((tile[queenRow+j - queenColumn][j] <= 6) || (tile[queenRow+j - queenColumn][j] == Empty)){
       PieceMovement('B',BQueen,queenRow+j - queenColumn,j);
if  (PieceMovement('B',BQueen,queenRow+j - queenColumn,j) == true){
       possiblemoves = true; 
     } 
       if (tile[queenRow+j - queenColumn][j] != Empty)
      {
        //immediately terminate the for loop
    break;  
      }
    }
    else {
      break;
    } 
   }  
         for (int j = queenColumn - 1; j >= 0; j--)
  { 
    //Other Side
    if ((tile[queenRow - j + queenColumn][j] <= 6) || (tile[queenRow- j + queenColumn][j] == Empty)){

  PieceMovement('B',BQueen,queenRow -j + queenColumn,j);
  if  (PieceMovement('B',BQueen,queenRow -j + queenColumn,j) == true){
       possiblemoves = true; 
     } 
      if (tile[queenRow -j + queenColumn][j] != Empty)
      {
        //immediately terminate the for loop
    break;  
      }
    }
    else {
      break;
    }      
   }
  //Count from queen column to top of board
     for (int j = queenColumn +1; j <= 7; j++)
   {
            //Find out if the piece is white or black (1 to 6 is white, 7 to 12 black)
    if ((tile[queenRow+j - queenColumn][j] <= 6) || (tile[queenRow+j - queenColumn][j] == Empty)) {
       PieceMovement('B',BQueen,queenRow+j - queenColumn,j);
if  (PieceMovement('B',BQueen,queenRow+j - queenColumn,j) == true){
       possiblemoves = true; 
     } 
       if (tile[queenRow+j - queenColumn][j] != Empty)
      {
        //immediately terminate the for loop
    break;  
      }
    }
    else {
      break;
    }    
   }
         for (int j = queenColumn +1; j <= 7; j++)
  { 
    //Other Side
    if ((tile[queenRow - j + queenColumn][j] <= 6) || (tile[queenRow -j + queenColumn][j] == Empty)){

  PieceMovement('B',BQueen,queenRow -j + queenColumn,j);
  if  (  PieceMovement('B',BQueen,queenRow -j + queenColumn,j) == true){
       possiblemoves = true; 
     } 
      if (tile[queenRow -j + queenColumn][j] != Empty)
      {
        //immediately terminate the for loop
    break;  
      }
    }
    else {
      break;
    }      
 }
 if (possiblemoves == true){
    return true;
  }
  else
  return false; 
}

bool wKingMoves() {
        bool possiblemoves = 0; 

    //If king not at the start, change to 0 
    if ((a != 4) || (i != 7)){

    WKingstart = 0;
  }
  
  castling();
  
  if (Wkingsidecastle == 1){
    PieceMovement('W',WKing,i,a+2);
    if  (PieceMovement('W',WKing,i,a+2) == true){
       possiblemoves = true; 
     } 
  }
   if (Wqueensidecastle == 1){
    PieceMovement('W',WKing,i,a-2);
    if  (PieceMovement('W',WKing,i,a-2) == true){
       possiblemoves = true; 
     } 
  }
  //check can move up
  if (tile[i][a+1] > 6) {
   PieceMovement('W',WKing,i,a+1);
   if  ( PieceMovement('W',WKing,i,a+1) == true){
       possiblemoves = true; 
     } 
  }
  
//check can move down
  if (tile[i][a-1] > 6)  {
   PieceMovement('W',WKing,i,a-1);
   if  (PieceMovement('W',WKing,i,a-1) == true){
       possiblemoves = true; 
     } 
  }
  
//check can move left
  if (tile[i-1][a] > 6) {
  PieceMovement('W',WKing,i-1,a);
  if  ( PieceMovement('W',WKing,i-1,a) == true){
    Serial.println("can move up");
       possiblemoves = true; 
     } 
  }  
  
//check can move Right
  if (tile[i+1][a] > 6) {
   PieceMovement('W',WKing,i+1,a);
   if  ( PieceMovement('W',WKing,i+1,a) == true){
       possiblemoves = true; 
     } 
  }    

//check can move Top right
  if (tile[i+1][a+1] > 6) { 
   PieceMovement('W',WKing,i+1,a+1);
   if  ( PieceMovement('W',WKing,i+1,a+1) == true){    
       possiblemoves = true; 
     } 
  }

//check can move Bottom right
  if (tile[i-1][a+1] > 6)  {
    PieceMovement('W',WKing,i-1,a+1);
    if  (PieceMovement('W',WKing,i-1,a+1) == true){
    Serial.println("can move up left");      
       possiblemoves = true; 
     } 
  }

//check can move Bottom Left
  if (tile[i-1][a-1] > 6) {
    PieceMovement('W',WKing,i-1,a-1);
    Serial.println("can move up");    
    if  (PieceMovement('W',WKing,i-1,a-1) == true){
    Serial.println("can move up right");      
       possiblemoves = true; 
     } 
  }  

//check can move Top Left
  if (tile[i+1][a-1] > 6) {
  PieceMovement('W',WKing,i+1,a-1);
  if  (PieceMovement('W',WKing,i+1,a-1) == true){
       possiblemoves = true; 
     } 
  }
  if (possiblemoves == true){
    return true;
  }
  else
  return false; 
}

bool bKingMoves() {
        bool possiblemoves = 0; 
  //If king not at the start, change to 0 
  if ((a != 4) || (i != 0)){
    
    BKingstart = 0;
  }
    //Check If castling can occur 
  castling();
  
  if (Bkingsidecastle == 1){
   PieceMovement('B',BKing,i,a+2);
   if  (PieceMovement('B',BKing,i,a+2) == true){
       possiblemoves = true; 
     } 
  }
   if (Bqueensidecastle == 1){
      PieceMovement('B',BKing,i,a-2);
      if  (PieceMovement('B',BKing,i,a-2) == true){
       possiblemoves = true; 
     } 
  }
  
//check can move up
  if ((tile[i][a+1] < 7)  || (tile[i][a+1] == 13)) {
      PieceMovement('B',BKing,i,a+1);
      if  ( PieceMovement('B',BKing,i,a+1) == true){
       possiblemoves = true; 
     } 
  }
  
//check can move down
  if ((tile[i][a-1] < 7) || (tile[i][a-1] == 13)) {
     PieceMovement('B',BKing,i,a-1);
     if  (PieceMovement('B',BKing,i,a-1) == true){
       possiblemoves = true; 
     } 
  }
  
//check can move left
  if ((tile[i-1][a] < 7) || (tile[i-1][a] == 13)) {
      PieceMovement('B',BKing,i-1,a);
      if  (PieceMovement('B',BKing,i-1,a) == true){
       possiblemoves = true; 
     } 
  }  
  
//check can move Right
  if ((tile[i+1][a] < 7)  || (tile[i+1][a] == 13)) {
      PieceMovement('B',BKing,i+1,a);
      if  ( PieceMovement('B',BKing,i+1,a) == true){
       possiblemoves = true; 
     } 
  }    

//check can move Top right
  if ((tile[i+1][a+1] < 7) || (tile[i+1][a+1] == 13)) {
     PieceMovement('B',BKing,i+1,a+1);
     if  (PieceMovement('B',BKing,i+1,a+1) == true){
       possiblemoves = true; 
     } 
  }

//check can move Bottom right
  if ((tile[i-1][a+1] < 7)  || (tile[i-1][a+1] == 13)) {
      PieceMovement('B',BKing,i-1,a+1);
      if  (PieceMovement('B',BKing,i-1,a+1) == true){
       possiblemoves = true; 
     } 
  }

//check can move Bottom Left
  if ((tile[i-1][a-1] < 7)|| (tile[i-1][a-1] == 13)) {
     PieceMovement('B',BKing,i-1,a-1);
     if  ( PieceMovement('B',BKing,i-1,a-1) == true){
       possiblemoves = true; 
     } 
  }  

//check can move Top Left
  if ((tile[i+1][a-1] < 7) || (tile[i+1][a-1] == 13)) {
       PieceMovement('B',BKing,i+1,a-1);
       if  ( PieceMovement('B',BKing,i+1,a-1) == true){
       possiblemoves = true; 
     } 
  } 
   if (possiblemoves == true){
    return true;
  }
  else
  return false; 
}  

void checkColourButton() {

  lastColour1State = colour1State;
  colour1State = digitalRead(colourPin1);

  if (colour1State == HIGH && lastColour1State == LOW) {
    
    colour1++;
    
    if (colour1 == 8) {
      colour1 = 1;
    }

    for (ledNumber = 0; ledNumber < 64; ledNumber++) {
      colourswap1();
      pixels.show();
    }
    delay(5);
    blankLights();     
  }


  lastColour2State = colour2State;
  colour2State = digitalRead(colourPin2);

  if (colour2State == HIGH && lastColour2State == LOW) {
    
    colour2++;
      
    if (colour2 == 8) {
      colour2 = 1;
    }
    for (ledNumber = 0; ledNumber < 64; ledNumber++) {
      colourswap2();
      pixels.show(); 
    }

    delay(5);
    blankLights();       
  }

  lastColour3State = colour3State;
  colour3State = digitalRead(colourPin3);

  if (colour3State == HIGH && lastColour3State == LOW) {
    
    colour3++;
      
    if (colour3 == 8) {
      colour3 = 1;
    }
    for (ledNumber = 0; ledNumber < 64; ledNumber++) {
      colourswap3();
      pixels.show();
    }
    delay(5);
    blankLights();       
  }
}
      
void colourswap1() { // colour of piece picked up
  
  switch(colour1) {
    case 1:
      //white
      pixels.setPixelColor(ledNumber, pixels.Color(50,50,50));
      break;
    case 2:
      //yellow
      pixels.setPixelColor(ledNumber, pixels.Color(50,50,0));
      break;
    case 3:
      //green
      pixels.setPixelColor(ledNumber, pixels.Color(0,50,0)); 
      break;
    case 4:
      //cyan
      pixels.setPixelColor(ledNumber, pixels.Color(0,50,50));    
      break;
    case 5:
      //blue
      pixels.setPixelColor(ledNumber, pixels.Color(0,0,50)); 
      break;
    case 6:
      //magenta
      pixels.setPixelColor(ledNumber, pixels.Color(50,0,50));     
      break;
    case 7:
      //red
      pixels.setPixelColor(ledNumber, pixels.Color(50,0,0));   
      break;
  }
}

void colourswap2() { // colour of piece moves

  switch(colour2) {
    case 1:
      //white
      pixels.setPixelColor(ledNumber, pixels.Color(50,50,50));
      break;
    case 2:
      //yellow
      pixels.setPixelColor(ledNumber, pixels.Color(50,50,0));
      break;
    case 3:
      //green
      pixels.setPixelColor(ledNumber, pixels.Color(0,50,0));
      break;
    case 4:
      //cyan
      pixels.setPixelColor(ledNumber, pixels.Color(0,50,50));    
      break;
    case 5:
      //blue
      pixels.setPixelColor(ledNumber, pixels.Color(0,0,50)); 
      break;
    case 6:
      //magenta
      pixels.setPixelColor(ledNumber, pixels.Color(50,0,50));     
      break;
    case 7:
      //red
      pixels.setPixelColor(ledNumber, pixels.Color(50,0,0)); 
      break;     
  }  
}

void colourswap3() { // colour of piece captures
  
  switch(colour3) {
    case 1:
      //white
      pixels.setPixelColor(ledNumber, pixels.Color(50,50,50));
      break;
    case 2:
      //yellow
      pixels.setPixelColor(ledNumber, pixels.Color(50,50,0));
      break;
    case 3:
      //green
      pixels.setPixelColor(ledNumber, pixels.Color(0,50,0));
      break;
    case 4:
      //cyan
      pixels.setPixelColor(ledNumber, pixels.Color(0,50,50));    
      break;
    case 5:
      //blue
      pixels.setPixelColor(ledNumber, pixels.Color(0,0,50)); 
      break;
    case 6:
      //magenta
      pixels.setPixelColor(ledNumber, pixels.Color(50,0,50));     
      break;
    case 7:
      //red
      pixels.setPixelColor(ledNumber, pixels.Color(50,0,0));  
      break;
  }  
}

void enpassant(){

for ( int s = 0; s < 8; s++){
//scan through rows 3 and 4 to see if there are pawns there and if start is still 1 (meaning they just moved twice)
  //Find if white pawns move twice
  if ((tile[4][s] == WPawn) && (Wpawnstart[s] == 1)){
  Wenpassant[s] = 1;
    
  } else
     {      Wenpassant[s] = 0;
}
  
//Find if black pawns move twice
    if ((tile[3][s] == BPawn) && (Bpawnstart[s] == 1)){
      Benpassant[s] = 1;

    } else
    {      Benpassant[s] = 0;
}
  }
}

void castling(){

if (!(tile[0][0] == BRook)){
Bcastlestart[0] = 0;
}
if (!(tile[0][7] == BRook)){
Bcastlestart[1] = 0;
}
 if (!(tile[7][7] == WRook)){
Wcastlestart[1] = 0;
}
if (!(tile[7][0] == WRook)){
Wcastlestart[0] = 0;
}
  
 
  //If White King Hasnt Moved
if (BKingstart == 1){

      //Look to the right of the king just before the rook 
    for ( int s = 5; s < 7; s++){

      
       if ((tile[0][s] == Empty) && (Bcastlestart[1] == 1)){
      Bkingsidecastle = 1;
    }
      //If clear, and rookstart is there, then castling can occur 
     else {
       
             Bkingsidecastle = 0;
    break;
     }
   
    }
      //Look to the left of the king just before rook             
    for ( int s = 3; s > 0; s--){

      
      if ((tile[0][s] == Empty) && (Bcastlestart[0] == 1)){
      Bqueensidecastle = 1;
    }
      //If clear, and rookstart is there, then castling can occur 
    else
      Bqueensidecastle = 0;
        break;
     } 
      
    }
  else {
  
   Bkingsidecastle = 0;
 Bqueensidecastle = 0;
  }
  
   //If Black King Hasnt Moved
if (WKingstart == 1){

      //Look to the right of the king just before the rook 
    for ( int s = 5; s < 7; s++){

      //If clear, and rookstart is there, then castling can occur 
     if ((tile[7][s] == Empty) && (Wcastlestart[1] == 1)){
      Wkingsidecastle = 1;
        break;
     }
      else {
            Wkingsidecastle = 0;
        break;
      }
    }
      //Look to the left of the king just before rook 
    for ( int s = 3; s > 0; s--){

      //If clear, and rookstart is there, then castling can occur 
     if ((tile[7][s] == Empty) && (Wcastlestart[0] == 1)){
         Wqueensidecastle = 1;
     }
      else {
     Wqueensidecastle = 0;
      break;   
      }
    }
  } else {

 Wkingsidecastle = 0;
 Wqueensidecastle = 0;

}
  
}

bool Checkmate(){

bool mate = 0;

//If White is In check 
if (CheckFunction('W') == true){


//Run through tiles 
  for (int column = 0; column < 8; column++){

    for (int row = 0; row < 8; row++){
      
  //WPawn Moves
      if (tile[row][column] == WPawn){
        i = row;
        a = column;
        //If theres possible moves, return false
        if (wPawnMoves() == true){
          i = 0;
          a = 0;
          mate = 1;          
          return false;
        }
      }
      
  //WRook Moves
      if (tile[row][column] == WRook){
        i = row;
        a = column;
        //If theres possible moves, return false
        if (wRookMoves() == true){
          i = 0;
          a = 0;
          mate = 1;          
          return false;
        }
      }
      

  //WKnight Moves
      if (tile[row][column] == WKnight){
        i = row;
        a = column;        
         if (wKnightMoves() == true){
          i = 0;
          a = 0;
          mate = 1;                     
          return false;
        }
      }

  //WBishop Moves
      if (tile[row][column] == WBishop){
        i = row;
        a = column;        
         if (wBishopMoves() == true){
          i = 0;
          a = 0;
          mate = 1;                     
          return false;
        }
      }
  //WQueen Moves
      if (tile[row][column] == WQueen){
        i = row;
        a = column;        
         if (wQueenMoves() == true){
          i = 0;
          a = 0;
          mate = 1;                     
          return false;
        }
      }
  //WKing Moves 
      if (tile[row][column] == WKing){
        i = row;
        a = column;        
         if (wKingMoves() == true){       
          i = 0;
          a = 0;
          mate = 1;                     
          return false;
        }
      }      
    }
 }

  if (mate == 0) { 
    return true;
  }
  
}
//If Black is In check 
else if (CheckFunction('B') == true){


//Run through tiles 
  for (int column = 0; column < 8; column++){

    for (int row = 0; row < 8; row++){
  //BPawn Moves
  if (tile[row][column] == BPawn){
        i = row;
        a = column;    

        //If theres possible moves, return false
        if (bPawnMoves() == true){
          i = 0;
          a = 0;
          mate = 1;                    
          return false;
        }
      }
  //BRook Moves
  if (tile[row][column] == BRook){
        i = row;
        a = column;    
         if (bRookMoves() == true){
          i = 0;
          a = 0; 
          mate = 1;                   
          return false;
        }
      }
  //BKnight Moves
  if (tile[row][column] == BKnight){
        i = row;
        a = column;    
         if (bKnightMoves() == true){
          i = 0;
          a = 0;      
          mate = 1;              
          return false;
        }
      }      
  //BBishop Moves
 if (tile[row][column] == BBishop){
        i = row;
        a = column;  
         if (bBishopMoves() == true){
          i = 0;
          a = 0;    
          mate = 1;                
          return false;
        }
      }
  //BQueen Moves
 if (tile[row][column] == BQueen){
        i = row;
        a = column;  
         if (bQueenMoves() == true){
          i = 0;
          a = 0;   
          mate = 1;                 
          return false;
        }
      }
  //BKing Moves 
 if (tile[row][column] == BKing){

        i = row;
        a = column;  
         if (bKingMoves() == true){         
          i = 0;
         a = 0;       
         mate = 1;             
          return false;
       }
      }
    }
  }
  if (mate == 0) { 
    return true;
  }
}
  
  else {
      return false;
  }
}

unsigned short myrand(void) {
  unsigned short r = (unsigned short)(seed % MYRAND_MAX);
  return r = ((r << 11) + (r << 7) + r) >> 1;
}

short D(short q, short l, short e, unsigned char E, unsigned char z, unsigned char n) {
  short m, v, i, P, V, s;
  unsigned char t, p, u, x, y, X, Y, H, B, j, d, h, F, G, C;
  signed char r;
  if (++Z > 30) {                                   /* stack underrun check */
    --Z; return e;
  }
  q--;                                          /* adj. window: delay bonus */
  k ^= 24;                                      /* change sides             */
  d = Y = 0;                                    /* start iter. from scratch */
  X = myrand() & ~M;                            /* start at random field    */
  while(d++ < n || d < 3 ||                         /* iterative deepening loop */
    z & K == I && (N < T & d < 98 ||            /* root: deepen upto time   */
                   (K = X, L = Y & ~M, d = 3)))                /* time's up: go do best    */
  { x = B = X;                                   /* start scan at prev. best */
    h = Y & S;                                   /* request try noncastl. 1st*/
    P = d < 3 ? I : D(-l, 1 - l, -e, S, 0, d - 3); /* Search null move         */
    m = -P < l | R > 35 ? d > 2 ? -I : e : -P;   /* Prune or stand-pat       */
    ++N;                                         /* node count (for timing)  */
    do {
      u = b[x];                                   /* scan board looking for   */
      if (u & k) {                                /*  own piece (inefficient!)*/
        r = p = u & 7;                             /* p = piece type (set r>0) */
        j = o[p + 16];                             /* first step vector f.piece*/
        while(r = p > 2 & r < 0 ? -r : -o[++j])        /* loop over directions o[] */
        { A:                                        /* resume normal after best */
          y = x; F = G = S;                         /* (x,y)=move, (F,G)=castl.R*/
          do {                                      /* y traverses ray, or:     */
            H = y = h ? Y ^ h : y + r;               /* sneak in prev. best move */
            if (y & M)break;                         /* board edge hit           */
            m = E - S & b[E] && y - E < 2 & E - y < 2 ? I : m; /* bad castling             */
            if (p < 3 & y == E)H ^= 16;              /* shift capt.sqr. H if e.p.*/
            t = b[H]; if (t & k | p < 3 & !(y - x & 7) - !t)break; /* capt. own, bad pawn mode */
            i = 37 * w[t & 7] + (t & 192);           /* value of capt. piece t   */
            m = i < 0 ? I : m;                       /* K capture                */
            if (m >= l & d > 1)goto C;               /* abort on fail high       */
            v = d - 1 ? e : i - p;                   /* MVV/LVA scoring          */
            if (d - !t > 1)                          /* remaining depth          */
            { v = p < 6 ? b[x + 8] - b[y + 8] : 0;    /* center positional pts.   */
              b[G] = b[H] = b[x] = 0; b[y] = u | 32;  /* do move, set non-virgin  */
              if (!(G & M))b[F] = k + 6, v += 50;     /* castling: put R & score  */
              v -= p - 4 | R > 29 ? 0 : 20;           /* penalize mid-game K move */
              if (p < 3)                              /* pawns:                   */
              { v -= 9 * ((x - 2 & M || b[x - 2] - u) + /* structure, undefended    */
                          (x + 2 & M || b[x + 2] - u) - 1  /*        squares plus bias */
                          + (b[x ^ 16] == k + 36))          /* kling to non-virgin King */
                     - (R >> 2);                       /* end-game Pawn-push bonus */
                V = y + r + 1 & S ? 647 - p : 2 * (u & y + 16 & 32); /* promotion or 6/7th bonus */
                b[y] += V; i += V;                     /* change piece, add score  */
              }
              v += e + i; V = m > q ? m : q;          /* new eval and alpha       */
              C = d - 1 - (d > 5 & p > 2 & !t & !h);
              C = R > 29 | d < 3 | P - I ? C : d;     /* extend 1 ply if in check */
              do
                s = C > 2 | v > V ? -D(-l, -V, -v,     /* recursive eval. of reply */
                                       F, 0, C) : v;    /* or fail low if futile    */
              while(s > q&++C < d); v = s;
              if (z && K - I && v + I && x == K & y == L) /* move pending & in root:  */
              { Q = -e - i; O = F;                     /*   exit if legal & found  */
                R += i >> 7; --Z; return l;            /* captured non-P material  */
              }
              b[G] = k + 6; b[F] = b[y] = 0; b[x] = u; b[H] = t; /* undo move,G can be dummy */
            }
            if (v > m)                               /* new best, update max,best*/
              m = v, X = x, Y = y | S & F;            /* mark double move with S  */
            if (h) {
              h = 0;  /* redo after doing old best*/
              goto A;
            }
            if (x + r - y | u & 32 |                 /* not 1st step,moved before*/
                p > 2 & (p - 4 | j - 7 ||             /* no P & no lateral K move,*/
                         b[G = x + 3 ^ r >> 1 & 7] - k - 6     /* no virgin R in corner G, */
                         || b[G ^ 1] | b[G ^ 2])               /* no 2 empty sq. next to R */
               )t += p < 5;                           /* fake capt. for nonsliding*/
            else F = y;                              /* enable e.p.              */
          } while(!t);                                  /* if not capt. continue ray*/
        }
      }
    } while((x = x + 9 & ~M) - B);                 /* next sqr. of board, wrap */
C: if (m > I - M | m < M - I)d = 98;           /* mate holds to any depth  */
    m = m + I | P == I ? m : 0;                  /* best loses K: (stale)mate*/  
    if (z && d > 2)
    { *c = 'a' + (X & 7); c[1] = '8' - (X >> 4); c[2] = 'a' + (Y & 7); c[3] = '8' - (Y >> 4 & 7); c[4] = 0;
      char buff[150];
    }
  }                                             /*    encoded in X S,8 bits */
  k ^= 24;                                     /* change sides back        */
  --Z; return m += m < e;                       /* delayed-loss bonus       */

}

void updateBARray() {

int counter;

  for (int row = 0; row < 8; row++ ) {
    for (int column = 0; column < 8; column++) {
      if (tile[row][column] == WPawn){      
        counter = row*8 + column;  
        if (row > 0) {
          counter = counter + row*8;
        }
        b[counter] = 9;
      }
      if (tile[row][column] == BPawn){      
        counter = row*8 + column;  
        if (row > 0) {
          counter = counter + row*8;
        }
        b[counter] = 18;
      }      
      if (tile[row][column] == WRook){      
        counter = row*8 + column;  
        if (row > 0) {
          counter = counter + row*8;
        }
        b[counter] = 14;
      }      
      if (tile[row][column] == BRook){      
        counter = row*8 + column;  
        if (row > 0) {
          counter = counter + row*8;
        }
        b[counter] = 22;
      }
      if (tile[row][column] == WKnight){      
        counter = row*8 + column;  
        if (row > 0) {
          counter = counter + row*8;
        }
        b[counter] = 11;
      } 
      if (tile[row][column] == BKnight){      
        counter = row*8 + column;  
        if (row > 0) {
          counter = counter + row*8;
        }
        b[counter] = 19;
      }
      if (tile[row][column] == WBishop){      
        counter = row*8 + column;  
        if (row > 0) {
          counter = counter + row*8;
        }
        b[counter] = 13;
      }
      if (tile[row][column] == BBishop){      
        counter = row*8 + column;  
        if (row > 0) {
          counter = counter + row*8;
        }
        b[counter] = 21;
      } 
      if (tile[row][column] == WQueen){      
        counter = row*8 + column;  
        if (row > 0) {
          counter = counter + row*8;
        }
        b[counter] = 15;
      }
      if (tile[row][column] == BQueen){      
        counter = row*8 + column;  
        if (row > 0) {
          counter = counter + row*8;
        }
        b[counter] = 23;
      }
      if (tile[row][column] == WKing){      
        counter = row*8 + column;  
        if (row > 0) {
          counter = counter + row*8;
        }
        b[counter] = 12;
      }                                              
      if (tile[row][column] == BKing){      
        counter = row*8 + column;  
        if (row > 0) {
          counter = counter + row*8;
        }
        b[counter] = 20;
      }  
      if (tile[row][column] == Empty){      
        counter = row*8 + column;  
        if (row > 0) {
          counter = counter + row*8;
        }
        b[counter] = 0;
      }            
    }
  }  
}

void chessMatrixConverter() {
  
  int prevY = c[0] - 97;
  int prevX = abs(c[1] - '8');
  int postY = c[2] - 97;
  int postX = abs(c[3] - '8'); 
  
  matrixToArray(prevX, prevY);
  colourswap1();
  matrixToArray(postX, postY);
  colourswap1();
  pixels.show();   
  
}
