#include <LiquidCrystal.h>
#include <Keypad.h>

enum Mode {
  PASSWORD_INPUT,
  TIMER_RUNNING,
  DETONATED,
  DEFUSED
};

const int PASSWORD_LENGTH = 4;
const int DETONATION_TIME = 60;     // In seconds

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

Mode mode = PASSWORD_INPUT;

int passwordLength = 0;
char password[PASSWORD_LENGTH];
char defuseInput[PASSWORD_LENGTH];

int timer = DETONATION_TIME;
unsigned long lastTick;

void loop() {
  char input = keypad.getKey();
  if(input) {
    if(mode == PASSWORD_INPUT) {
      if(passwordLength < PASSWORD_LENGTH) {
        if(input == '*') {
          clearPassword();
        } else if(input != '#') {
          password[passwordLength] = input;
          passwordLength++;        
        }
      } else {
        if(input == '#') {
          mode = TIMER_RUNNING;
          lastTick = millis();
          passwordLength = 0;
        } else if(input == '*') {
          clearPassword();
        }
      }
    } else if(mode == TIMER_RUNNING) {
      if(input == '*') {
        passwordLength = 0;
      } else if(input == '#' && passwordLength == PASSWORD_LENGTH) {
        bool allEqual = true;

        for(int i = 0; i < passwordLength; i++) {
          if(password[i] != defuseInput[i]) {
            allEqual = false;
          }
        }
        
        if(allEqual) {
          mode = DEFUSED;
        } else {
          mode = DETONATED;
        }
      } else {
        defuseInput[passwordLength] = input;   
        passwordLength++;
      }
    }
  }

  if(mode == TIMER_RUNNING) {
    if(timer > 0 && lastTick + 1000 <= millis()) {
      timer--;            
      lastTick = millis();              
    }
  }

  if(timer == 0) {
    mode = DETONATED;
  }

  // Render to the display
  if(mode == PASSWORD_INPUT) {
    lcd.print("Password:");
    lcd.setCursor(0, 1);
    for(int i = 0; i < passwordLength; i++) {
      lcd.print(password[i]);
    }
  } else if(mode == TIMER_RUNNING) {
    lcd.print("Detonating in ");
    lcd.print(timer);
    lcd.setCursor(0, 1);
    for(int i = 0; i < passwordLength; i++) {
      lcd.print(defuseInput[i]);
    }
  } else if(mode == DETONATED) {
    lcd.print("Detonated");
  } else if(mode == DEFUSED) {
    lcd.print("Defused");
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
