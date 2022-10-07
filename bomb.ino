#include <LiquidCrystal.h>
#include <Keypad.h>

const int MAX_PASSWORD_LENGTH = 4;

const byte ROWS = 4;
const byte COLS = 3;

const char HEXA_KEYS[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}  
};

const byte ROW_PINS[ROWS] = {13, 12, 11, 10};
const byte COL_PINS[COLS] = {9, 8, 7};

LiquidCrystal lcd(1, 2, 3, 4, 5, 6);
Keypad keypad = Keypad(makeKeymap(HEXA_KEYS), ROW_PINS, COL_PINS, ROWS, COLS);

void setup() {
  lcd.begin(16, 2);
}

int passwordLength = 0;
bool passwordRegistered = false;
char password[MAX_PASSWORD_LENGTH];

void loop() {
  char input = keypad.getKey();
  if(input) {
    if(passwordLength < MAX_PASSWORD_LENGTH) {
      if(input == '*') {
        clearPassword();
      } else if(input != '#') {
        password[passwordLength] = input;
        passwordLength++;        
      }
    } else if(!passwordRegistered) {
      if(input == '#') {
        passwordRegistered = true;
      } else if(input == '*') {
        clearPassword();
      }
    }
  }

  // Print
  if(!passwordRegistered) {
    lcd.print("Password:");
    lcd.setCursor(0, 1);
    for(int i = 0; i < passwordLength; i++) {
      lcd.print(password[i]);
    }
  }
  delay(100);
  lcd.clear();
}

void clearPassword() {
  for(int i = 0; i < passwordLength; i++) {
    password[i] = NULL;          
  }
  passwordLength = 0;
}