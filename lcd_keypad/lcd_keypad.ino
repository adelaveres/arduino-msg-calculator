/*
   << 4x4 matrix keypad with LCD >>
   
   The  Arduino circuit connection for LCD:
     LCD RS  pin to analog pin A0
     LCD  Enable pin to analog pin A1
     LCD D4  pin to analog pin A2
     LCD D5  pin to analog pin A3
     LCD D6  pin to analog pin A4
     LCD D7  pin to analog pin A5
   The  Arduino circuit connection for MAtrix Key Pad:
     ROW1 pin  to digital pin 9
     ROW2 pin  to digital pin 8
     ROW3 pin  to digital pin 7
     ROW4 pin  to digital pin 6
     COLUMN1  pin to digital pin 5
     COLUMN2  pin to digital pin 4
     COLUMN3  pin to digital pin 3
     COLUMN4  pin to digital pin 2
*/


#include <LiquidCrystal.h>
#include <Key.h>
#include <Keypad.h> 

//initialize the library with the numbers of the interface pins
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

//4x4 Matrix key pad
const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns

// Define the Keymap
char keys[ROWS][COLS] =
{
  {'1', '2', '3', 'E'},
  {'4', '5', '6', '<'},
  {'7', '8', '9', 'X'},
  {'-', '0', '+', '='}
};

//Code that shows the keypad connections to the arduino terminals
byte rowPins[ROWS] = { 9, 8, 7, 6 }; //Rows 0 to 3
byte colPins[COLS] = { 5, 4, 3, 2 }; //Columns 0 to 3

// Create the Keypad
//initializes an instance of the Keypad class
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

char lastKeyPressed = 'z';
int noTimesPressed = 1 ;
char charToPrint;
int letterPos = 0;
int arrayLength = 4; //key's no of letters
char messageToPrint[32]= "";
char messageLine1[16];
char messageLine2[16];

char letters_plus[2] = {'+', '*'};
char letters_minus[2] = {'-', '/'};
char letters_0[3] = {' ', '0', '#'}; // #-verify eq;
char letters_1[5] = {'.', ',', '?', '!', '1'};
char letters_2[4] = {'a', 'b', 'c', '2'};
char letters_3[4] = {'d', 'e', 'f', '3'};
char letters_4[4] = {'g', 'h', 'i', '4'};
char letters_5[4] = {'j', 'k', 'l', '5'};
char letters_6[4] = {'m', 'n', 'o', '6'};
char letters_7[5] = {'p', 'q', 'r', 's', '7'};
char letters_8[4] = {'t', 'u', 'v', '8'};
char letters_9[5] = {'w', 'x', 'y', 'z', '9'};

//-----------------------------------------------------------------------------------------------------------------------------
//                                                   FUNCTIONS

void positionLetter()
{
  if(letterPos<32)
    letterPos++;
  else{
    letterPos=0;
    clearMessage();
  }
}

void parseMessageToDisplay()
{
  //Serial.println(messageToPrint);
  for(int i=0; i<16; i++){
    messageLine1[i]=messageToPrint[i];
  }
  
  for(int i=16; i<32; i++){
    messageLine2[i-16]=messageToPrint[i];
  }
}

char chooseChar(char key)
{
  switch(key){
    case '+': return letters_plus[noTimesPressed-1];
    case '-': return letters_minus[noTimesPressed-1];
    case '0': return letters_0[noTimesPressed-1];
    case '1': return letters_1[noTimesPressed-1];
    case '2': return letters_2[noTimesPressed-1];
    case '3': return letters_3[noTimesPressed-1];
    case '4': return letters_4[noTimesPressed-1];
    case '5': return letters_5[noTimesPressed-1];
    case '6': return letters_6[noTimesPressed-1];
    case '7': return letters_7[noTimesPressed-1];
    case '8': return letters_8[noTimesPressed-1];
    case '9': return letters_9[noTimesPressed-1];
  }
  return key;
}

void timesPressed(char key)
{
  if(key==lastKeyPressed){ 
    if(noTimesPressed < arrayLength)
      noTimesPressed++;  
    else
      noTimesPressed=1;   
  }
  else{
    noTimesPressed=1; 
    lastKeyPressed=key;
  }
}

void clearMessage()
{
  for(int i=0; i<32; i++)
    messageToPrint[i]=' ';
}

void printResToMessage(int res)
{
  if(res > 9){
    char units= res % 10;
    char dozens= res / 10;
    messageToPrint[letterPos] = '=';
    messageToPrint[letterPos+1] = dozens + '0';
    messageToPrint[letterPos+2] = units + '0';
    letterPos += 3;
  }
  else{
    messageToPrint[letterPos] = '=';
    messageToPrint[letterPos+1] = res + '0';
    letterPos += 2;
  }
}

int calculate(char operation, int operand1, int operand2)
{
    switch(operation){
      case '+': return operand1 + operand2;
      case '-': {
              if(operand1 >= operand2)
                return operand1 - operand2;
              else
                return operand2 - operand1;
              }
      case '/': return operand1 / operand2;
      case '*': return operand1 * operand2;
      default: return 0;
    }
}

int equalsTo()
{
  char operation = messageToPrint[letterPos-2]; // can be + * - /
  int operand1 = messageToPrint[letterPos-3] - '0';
  int operand2 = messageToPrint[letterPos-1] - '0';
  int res = calculate(operation, operand1, operand2);
  
  return res;
}

bool evaluateEquation()
{  
  int operand1 = messageToPrint[letterPos-5] - '0';
  int operand2 = messageToPrint[letterPos-3] - '0';
  int res = messageToPrint[letterPos-1] - '0';
  char operation = messageToPrint[letterPos-4];
  int expectedRes = calculate(operation, operand1, operand2);

  if(res == expectedRes)
    return true;
  return false;
}

void corectEq()
{
 // congrats !
    digitalWrite(12,HIGH);
    delay(500);
    digitalWrite(12, LOW);
    delay(500);
    
    digitalWrite(12,HIGH);
    delay(500);
    digitalWrite(12, LOW);

    messageToPrint[21]='c';
    messageToPrint[22]='o';
    messageToPrint[23]='r';
    messageToPrint[24]='e';
    messageToPrint[25]='c';
    messageToPrint[26]='t';  

    Serial.println(messageToPrint);
}

void wrongEq()
{
      // wrong !
    for(int i=0; i<6; i++){
      digitalWrite(12,HIGH);
      delay(100);
      digitalWrite(12, LOW);
      delay(100);
    }
    digitalWrite(12,HIGH);
    delay(2000);
    digitalWrite(12, LOW);
    
    messageToPrint[21]='w';
    messageToPrint[22]='r';
    messageToPrint[23]='o';
    messageToPrint[24]='n';
    messageToPrint[25]='g';
    messageToPrint[26]='!';
}

void verifyEquation()
{
  
   if(evaluateEquation()==true)
     corectEq();
   else
     wrongEq();
}

int findNoLetters(char key)
{
  if((key=='7')||(key=='9')||(key=='1'))
    return 5;
  if((key=='+')||(key=='-')||(key=='E'))
    return 2;
  if(key=='0')
    return 3;

  return 4;
}

void backspace()
{
    messageToPrint[letterPos]=' ';
    if(letterPos > 0)
      letterPos--;
}

void chooseFunctionality(char key)
{
      switch(key){
      case 'X':{
            clearMessage();
            letterPos=0;
           };break;
      case 'E':
            positionLetter(); break;
      case '<':{
            backspace();
           };break;
      case '=':{
            int res = equalsTo();
            printResToMessage(res);
           };break;
      default: {
            charToPrint = chooseChar(key);

            if(charToPrint=='#'){
              messageToPrint[letterPos] = 'e'; // evaluate
              verifyEquation();
            }else
              messageToPrint[letterPos] = charToPrint;
           };break;
      }
}

void setup()
{
  pinMode(12, OUTPUT);
  clearMessage();
}


  void loop()
{
  
  char key = keypad.getKey();
  arrayLength = 4;

  
  // Check for a valid key
  if(key!=NO_KEY && keypad.keyStateChanged())
  {
    
    // key's no. of letters/chars
    arrayLength = findNoLetters(key);
    
    // no. times pressed button
    timesPressed(key);
  
    // choose functionality
    chooseFunctionality(key);

    parseMessageToDisplay();
    
    lcd.setCursor(0,0);
    lcd.print(messageLine1);
    lcd.setCursor(0,1);
    lcd.print(messageLine2);
    
  }
  

}

