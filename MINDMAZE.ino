#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

// Define the size of the keypad
const byte ROWS = 4;
const byte COLS = 4;

// Define the keymap
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Define the row and column pins
byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3}; 

// Define LED pins
int redLED = 13;
int yellowLED = 10;
int blueLED = 2;
int greenLED = 12;
int button1 = A1;
int button2 = A0;
int button3 = A3;
int button4 = A2;
int buzzerPin = 11;


//Define LCD parameters 
LiquidCrystal_I2C lcd(0x27,16,2);
//keypad object
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Function prototypes
void mode1();
void mode2();

void setup() {
  
  Wire.begin();

  // Initialize LCD
    lcd.init();
 lcd.clear();         
  lcd.backlight(); 
 
  // LED pins are outputs
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);

  // push button are inputs with pull-up resistors
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(button4, INPUT_PULLUP);

  pinMode(buzzerPin, OUTPUT);

  // main screen on LCD
  lcd.setCursor(2, 0);
  lcd.print("Select mode");
  lcd.setCursor(1, 1);
  lcd.print("Mode 1 Mode 2");
 
}

void loop() {
  char key = keypad.getKey();
  if (key != NO_KEY) {
    if (key == '1') {
      mode1();
    } else if (key == '2') {
      mode2();
    }
    // Reset LCD to main screen after finish any of 2 modes
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("Select mode");
    lcd.setCursor(1, 1);
    lcd.print("Mode 1  Mode 2 ");
    
  }
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, LOW);
  digitalWrite(blueLED, LOW);
  digitalWrite(yellowLED, LOW);
}

void mode1() {
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("Mode 1");
  lcd.setCursor(1, 1);
  lcd.print("Response Time");
  delay(1000);

  int responseTimes[5];
  for (int i = 0; i < 5; i++) {
    int colorToTurnOn = random(4); 
    switch (colorToTurnOn) {
      case 0:
        analogWrite(redLED, 255); 
        delay(1000); 
        analogWrite(redLED, 0); 
        break;
      case 1:
        analogWrite(greenLED, 255); 
        delay(1000); 
        analogWrite(greenLED, 0); 
        break;
      case 2:
        analogWrite(blueLED, 255); 
        delay(1000); 
        analogWrite(blueLED, 0); 
        break;
      case 3:
        analogWrite(yellowLED, 255); 
        delay(1000); 
        analogWrite(yellowLED, 0); 
        break;
    }
    delay(50);
    unsigned long startTime = millis();
    int chosenButton = -1;
    while (chosenButton == -1) {
      if (analogRead(button1) == LOW) {
        chosenButton = button1;
      } else if (analogRead(button2) == LOW) {
        chosenButton = button2;
      } else if (analogRead(button3) == LOW) {
        chosenButton = button3;
      } else if (analogRead(button4) == LOW) {
        chosenButton = button4;
      } else {
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("Press Button");
      }
    }

    unsigned long endTime = millis();
    responseTimes[i] = (endTime - startTime);
    delay(2000);
  }
 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Avg Time in ms:");
  float sum = 0;
  for (int i = 0; i < 5; i++) {
    sum += responseTimes[i];
  }
  
  float avgResponse = (sum / 5.0);
 
  lcd.setCursor(4, 1);
  lcd.print(avgResponse); 
  delay(5000);
}

void mode2() {
  int score = 0;
  while (true) {
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("Mode 2");
    lcd.setCursor(2, 1);
    lcd.print("Memory Game");
    delay(1000);

    // to generate random number of LEDs for each color
    int numRedLEDs = random(5);
    int numGreenLEDs =random(5);
    int numBlueLEDs = random(5);
    int numYellowLEDs = random(5);

    // to generate LED sequence for each color
    for (int i = 0; i < max(max(numRedLEDs, numGreenLEDs), max(numBlueLEDs, numYellowLEDs)); i++) {
      if (i < numRedLEDs) {
        analogWrite(redLED, 255);
      }
      if (i < numGreenLEDs) {
        analogWrite(greenLED, 255);
      }
      if (i < numBlueLEDs) {
        analogWrite(blueLED, 255);
      }
      if (i < numYellowLEDs) {
        analogWrite(yellowLED, 255);
      }
      delay(500);
      analogWrite(redLED, 0);
      analogWrite(greenLED, 0);
      analogWrite(blueLED, 0);
      analogWrite(yellowLED, 0);
      delay(500);
    }

    lcd.clear();
    lcd.print("Count");
    
    // to select a random color to ask the user for
    int selectedColor = random(4); 
    switch (selectedColor) {
      case 0:
        lcd.setCursor(6, 0);
        lcd.print("Red LEDs");
        break;
      case 1:
        lcd.setCursor(6, 0);
        lcd.print("Green LEDs");
        break;
case 2:
        lcd.setCursor(6, 0);
        lcd.print("Blue LEDs");
        break;
      case 3:
        lcd.setCursor(6, 0);
        lcd.print("Yellow LEDs");
        break;
    }

    int answer = 0;
    char key;
    while (true) {
      key = keypad.getKey();
      if (key >= '0' && key <= '9') {
        lcd.setCursor(7, 1);
        lcd.print(key);
        answer = answer * 10 + (key - '0');
      } else if (key == '#') {
        break; // press ‘#’ key on keypad to enter the answer 
      }
    }

    // Check if user's answer matches the number of LEDs for the selected color
    int correctAnswer = 0;
    switch (selectedColor) {
      case 0:
        correctAnswer = numRedLEDs;
        break;
      case 1:
        correctAnswer = numGreenLEDs;
        break;
      case 2:
        correctAnswer = numBlueLEDs;
        break;
      case 3:
        correctAnswer = numYellowLEDs;
        break;
    }

    if (answer == correctAnswer) {
      score++;
    } else {
      lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("Wrong Answer!");
    lcd.setCursor(5, 1);
    lcd.print("Score");
      lcd.print(score);
   // Turn on the buzzer for wrong answer
  digitalWrite(buzzerPin, HIGH);
  delay(1000);
  digitalWrite(buzzerPin, LOW); 
  delay(2000);
      

      break; // go back to main lcd screen if wrong answer
    }
  }
}