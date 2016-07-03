//# include <Trellis.h>
// CHECK CONTRAST!!!
#include <LiquidCrystal.h>
#include <Wire.h>
#include "Adafruit_Trellis.h"
#include <StackArray.h>

#define NUMTRELLIS 1
#define numKeys (NUMTRELLIS * 16)
#define INTPIN A2
int scr = 0;
Adafruit_Trellis matrix0 = Adafruit_Trellis();
Adafruit_TrellisSet trellis =  Adafruit_TrellisSet(&matrix0);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
StackArray <double> stack;
String input;

void setup() {
  Serial.begin(9600);
  pinMode(INTPIN, INPUT);
  digitalWrite(INTPIN, HIGH);

  trellis.begin(0x70);

  lcd.begin(16, 2);
  lcd.clear();
}


char readBoard() {
  char keys[] = {
    '7', '8', '9', '+',
    '4', '5', '6', '-',
    '1', '2', '3', '*',
    '=', '0', '.', '/'
  };
  char readChar = ' ';
  if (trellis.readSwitches()) {
    for (uint8_t i = 0; i < numKeys; i++) {
      if (trellis.justPressed(i)) {
        trellis.setLED(i);
      }
      if (trellis.justReleased(i)) {
        readChar = keys[i];
        trellis.clrLED(i);
      }
    }
    trellis.writeDisplay();
  }
  return readChar;
}

void basic(char key) {
    if (stack.count() < 2) {
      return;
    }
    switch (key) {
      case '+':
        stack.push(stack.pop() + stack.pop());
        break;
      case '-':
        stack.push(-(stack.pop() - stack.pop()));
        break;
      case '*':
        stack.push(stack.pop() * stack.pop());
        break;
      case '/': {
        double div1 = stack.pop();
        double div2 = stack.pop();
        stack.push(div2 / div1);
        break;
      }
    }
}

void inputToStack() {
  if (input.length() > 0) {
    stack.push(input.toFloat());
    input = "";
  }
}

void calculator(char key) {
  switch (key) {
    case '.':
      if (input.indexOf('.') == -1) {
        input.concat(key);
      }
      break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      input.concat(key);
      break;
    case '=':
      inputToStack();
      break;
    case '+':
    case '-':
    case '*':
    case '/':
      inputToStack();
      basic(key);
      break;
    default:
      break;
  }
}

void loop() {
  delay(30);

  char key = readBoard();
  if (key != ' ') {
    lcd.clear();
    calculator(key);
    if (!stack.isEmpty()) {
      lcd.setCursor(0, 1);
      lcd.print(stack.peek());
    }
    lcd.setCursor(0, 0);
    lcd.print(input);
    Serial.println(input);
  }
}
