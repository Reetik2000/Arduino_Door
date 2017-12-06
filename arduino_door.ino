#include "Password.h" //http://playground.arduino.cc/uploads/Code/Password.zip //tells to use password library
#include <Keypad.h>
// #include <LiquidCrystal.h>

const bool PRIVATE_MODE = false;
Password password = Password("4192A"); //Password object

const byte ROWS = 4; // Four rows
const byte COLS = 4; // columns
// Define the Keymap
char keys[ROWS][COLS] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};
// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = {9,8,7,6}; // Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = {5,4,3,2};

// Create the Keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// LiquidCrystal lcd();

int sensor = 0;
bool armed = 0;
String tempPass = "";

bool checkPassword() {
    if (password.evaluate()) { //if password is right 
        return true;
    } else {
        return false;
    }
}

void arm() {
    armed = true;
}

void disarm() {
    armed = false;
}

void clear() {
    password.reset();
    tempPass = "";
}

// void outputLCD() {
//     lcd.clear();
//     if(armed) {
//         lcd.setCursor(0,0);
//         lcd.print("***ARMED***");
//         lcd.setCursor(0,1);
//         lcd.print(tempPass);
//     } else {
//         lcd.setCursor(0,0);
//         lcd.print("***DISARMED***");
//         lcd.setCursor(0,1);
//         lcd.print(tempPass);
//     }
// }
void beep() {
    digitalWrite(12, HIGH);
    delay(10);
    digitalWrite(12, LOW);
}

void boop() {
    digitalWrite(12, HIGH);
    delay(1000);
    digitalWrite(12, LOW);
}

void accepted() {
    // lcd.clear();
    // lcd.print("***Accepted***");
    boop();
    delay(2000);
    // outputLCD();
}

void invalid() {
    // lcd.clear();
    // lcd.print("***Invalid***");
    boop();
    boop();
    boop();
    delay(2000);
    // outputLCD();
}

void addChar(char key) {
    if(PRIVATE_MODE) {
        tempPass += "*";
    } else {
        tempPass += key;
    }
    // outputLCD();
}


//take care of some special events
void keypadEvent(KeypadEvent eKey) {
    switch (keypad.getState()) {
        case PRESSED:
            beep();
            delay(10);

            switch (eKey) {
                case '*':
                    if(checkPassword()) {
                        accepted();
                        arm();
                    } else {
                        invalid();
                    }
                    delay(1);
                    clear();
                    break;
                
                case '#':
                    if(checkPassword()) {
                        accepted();
                        disarm();
                    } else {
                        invalid();
                    }
                    delay(1);
                    clear();
                    break;
                
                case 'D':
                    clear();
                    delay(1);
                    break;

                default:
                    password.append(eKey);
                    addChar(eKey);
                    delay(1);
            }
    }
}

void setup() {
    Serial.begin(9600);
    delay(3000);
    // lcd.begin(16,2);
    // lcd.print("Hello World!");
    pinMode(11, INPUT); //sensor
    pinMode(12, OUTPUT); //red light  
    keypad.addEventListener(keypadEvent); //add an event listener for this keypad

}

void loop() {
    sensor = digitalRead(11);
    if (sensor == HIGH && armed) {
        digitalWrite(12, HIGH);
    }
    keypad.getKey();
}