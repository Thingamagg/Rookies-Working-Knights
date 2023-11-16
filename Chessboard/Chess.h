#ifndef CHESS_H
#define CHESS_H

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

//Chess bot
#define M 0x88
#define S 128
#define I 8000

#include <Adafruit_NeoPixel.h>

class Chess
{
  public:
    Chess(int lightsPin, int numLights, int onPin, int cheatPin, int colourPin1, int colourPin2, int colourPin3,
    int columnZero, int columnOne, int columnTwo, int columnThree, int columnFour, int columnFive, int columnSix, int columnSeven,
    int rowZero, int rowOne, int rowTwo, int rowThree, int rowFour, int rowFive, int rowSix, int rowSeven,
    int testColumnZero, int testColumnOne, int testColumnTwo, int testColumnThree, int testColumnFour, int testColumnFive, int testColumnSix, int testColumnSeven);


    //Check Function
    bool CheckFunction(char colour);
    bool PieceMovement(char colour, int piece, int row, int column);

    //Checkmate
    bool Checkmate();

    
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

    bool Stalemate();

    void enpassant();
    void checkColourButton();
    void colourswap1();
    void colourswap2();
    void colourswap3();
    short D(short q, short l, short e, unsigned char E, unsigned char z, unsigned char n);
    unsigned short myrand(void);
    void updateBARray();
    void initChess();
    void loopChess();
    void chessStartUp();

    void cheatButtonCheck(); 
    void chessMatrixConverter(); 
    void matrixToArray(int i, int a);


  private:
    bool onButton = 1;
    int lastOnState;
    int currentOnState;

    int _lightsPin, _numLights, _onPin, _cheatPin, _colourPin1, _colourPin2, _colourPin3;
    int _columnZero, _columnOne, _columnTwo, _columnThree, _columnFour, _columnFive, _columnSix, _columnSeven;
    int _rowZero, _rowOne, _rowTwo, _rowThree, _rowFour, _rowFive, _rowSix, _rowSeven;
    int _testColumnZero, _testColumnOne, _testColumnTwo, _testColumnThree, _testColumnFour, _testColumnFive, _testColumnSix, _testColumnSeven;

    //EN PASSANT
    bool Wpawnstart[8] = {1,1,1,1,1,1,1,1};
    bool Wenpassant[8] = {0,0,0,0,0,0,0,0};
    bool Bpawnstart[8] = {1,1,1,1,1,1,1,1};
    bool Benpassant[8] = {0,0,0,0,0,0,0,0};

    bool colourM = 1;

  
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

    Adafruit_NeoPixel pixels;

    //Initial chess bot


    #define MYRAND_MAX 65535    /* 16bit pseudo random generator */
    long  N, T;                  /* N=evaluated positions+S, T=recursion limit */
    short Q, O, K, R, k=16;      /* k=moving side */
    char *p, c[5], Z;            /* p=pointer to c, c=user input, computer output, Z=recursion counter */


    unsigned int seed=0;
    uint32_t  byteBoard[8];

    char sym[17] = {".?pnkbrq?P?NKBRQ"};
    char lastH[5], lastM[5];
    unsigned short ledv=1;
    char L;
    char w[8]={0,2,2,7,-1,8,12,23};                             /* relative piece values    */
    char o[32]={-16,-15,-17,0,1,16,0,1,16,15,17,0,14,18,31,33,0, /* step-vector lists */
        7,-1,11,6,8,3,6,                                 /* 1st dir. in o[] per piece*/
        6,3,5,7,4,5,3,6};                                /* initial piece setup      */
    char b[129]={     
      22, 19, 21, 23, 20, 21, 19, 22, 28, 21, 16, 13, 12, 13, 16, 21,
      18, 18, 18, 18, 18, 18, 18, 18, 22, 15, 10,  7,  6,  7, 10, 15,
      0,  0,  0,  0,  0,  0,  0,  0, 18, 11,  6,  3,  2,  3,  6, 11,
      0,  0,  0,  0,  0,  0,  0,  0, 16,  9,  4,  1,  0,  1,  4,  9,
      0,  0,  0,  0,  0,  0,  0,  0, 16,  9,  4,  1,  0,  1,  4,  9,
      0,  0,  0,  0,  0,  0,  0,  0, 18, 11,  6,  3,  2,  3,  6, 11,
      9,  9,  9,  9,  9,  9,  9,  9, 22, 15, 10,  7,  6,  7, 10, 15,
      14, 11, 13, 15, 12, 13, 11, 14, 28, 21, 16, 13, 12, 13, 16, 21, 0
    };
};


#endif