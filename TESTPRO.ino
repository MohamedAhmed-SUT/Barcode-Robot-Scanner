#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ========== Ultrasonic Sensor Pins ==========
#define S1Trig 2  // Front
#define S2Trig 4  // Right
#define S3Trig 6  // Left
#define S1Echo 3
#define S2Echo 5
#define S3Echo 7

// ========== Motor Control Pins ==========
#define LEFT_MOTOR_PIN1 8
#define LEFT_MOTOR_PIN2 9
#define RIGHT_MOTOR_PIN1 10
#define RIGHT_MOTOR_PIN2 11

#define MAX_SPEED 150

// ========== Distance Limits (in cm) ==========
#define FRONT_MIN_DISTANCE 30
#define FRONT_MAX_DISTANCE 200
#define SIDE_MIN_DISTANCE 15
#define SIDE_MAX_DISTANCE 40
#define SIDE_MARGIN 10

// ========== LCD I2C ==========
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C Scanner

// ========== Product Definition ==========
struct Product {
  String barcode;
  String name;
  float price;
};

// ========== Product Database ==========
Product products[] = {
  {"6224007537923", "Pepsi 500ml", 7.50},
  {"https://123456", "Chipsy 25g", 5.00},
  {"https://123457", "Galaxy Choco", 10.00},
  {"https://123458", "Water 1L", 4.00},
  {"https://123459", "Juice Mango", 8.00},
  {"https://123460", "Milk 1L", 12.00},
  {"https://123461", "Bread Small", 3.00},
  {"https://123462", "Chocolate", 9.50},
  {"https://123463", "Snack Bar", 6.00},
  {"https://123464", "Yogurt Cup", 4.50},
  {"https://123465", "Coffee Sachet", 2.50},
  {"https://123466", "Tea Bag Box", 15.00},
  {"https://123467", "lcd 25g", 5.00},
  {"https://123462", "Chipsy 25g", 5.00},
};

const int numProducts = sizeof(products) / sizeof(Product);
String incomingBarcode = "";
unsigned long lastBarcodeTime = 0;
const unsigned long barcodeDisplayTime = 3000; 

void setup() {
  // ========== Initialize Ultrasonic Sensors ==========
  pinMode(S1Trig, OUTPUT); pinMode(S1Echo, INPUT);
  pinMode(S2Trig, OUTPUT); pinMode(S2Echo, INPUT);
  pinMode(S3Trig, OUTPUT); pinMode(S3Echo, INPUT);

  // ========== Initialize Motors ==========
  pinMode(LEFT_MOTOR_PIN1, OUTPUT); pinMode(LEFT_MOTOR_PIN2, OUTPUT);
  pinMode(RIGHT_MOTOR_PIN1, OUTPUT); pinMode(RIGHT_MOTOR_PIN2, OUTPUT);

  // ========== Initialize Serial Ports ==========
  Serial.begin(115200);      
  Serial1.begin(115200);     

  // ========== Initialize LCD ==========
  lcd.init();
  lcd.backlight();
  resetDisplay();

  Serial.println("System Initialized - Robot and Barcode Scanner Ready");
}

void loop() {
  // ========== Part 1: Handle Robot Movement ==========
  handleRobotMovement();

  // ========== Part 2: Handle Barcode Scanner ==========
  handleBarcodeScanner();

  // ========== Part 3: Handle Display Timeout ==========
  if (millis() - lastBarcodeTime > barcodeDisplayTime) {
    resetDisplay();
  }
}

void handleRobotMovement() {
  int front = readUltrasonic(S1Trig, S1Echo);
  int right = readUltrasonic(S2Trig, S2Echo);
  int left = readUltrasonic(S3Trig, S3Echo);

  Serial.print("Front: "); Serial.print(front);
  Serial.print(" cm | Right: "); Serial.print(right);
  Serial.print(" cm | Left: "); Serial.print(left);
  Serial.println(" cm");

  if (front >= FRONT_MIN_DISTANCE && front <= FRONT_MAX_DISTANCE) {
    if (left >= SIDE_MIN_DISTANCE && left <= SIDE_MAX_DISTANCE && (left + SIDE_MARGIN) < right) {
      Serial.println("Turning Left (person on left)");
      turnLeft();
      delay(500);
    } 
    else if (right >= SIDE_MIN_DISTANCE && right <= SIDE_MAX_DISTANCE) {
      Serial.println("Turning Right (person on right)");
      turnRight();
      delay(500);
    } 
    else {
      Serial.println("Moving Forward (no person on sides)");
      moveForward();
    }
  } 
  else {
    Serial.println("Stopping (front obstacle)");
    stopMotors();
  }
}

void handleBarcodeScanner() {
  if (Serial1.available()) {
    incomingBarcode = Serial1.readStringUntil('\n');
    incomingBarcode.trim();

    if (incomingBarcode.length() > 0) {
      Serial.println("Scanned: " + incomingBarcode);
      showProduct(incomingBarcode);
      lastBarcodeTime = millis();
    }
  }
}

void showProduct(String barcode) {
  lcd.clear();

  for (int i = 0; i < numProducts; i++) {
    if (products[i].barcode == barcode) {
      lcd.setCursor(0, 0);
      lcd.print(products[i].name);
      lcd.setCursor(0, 1);
      lcd.print("Price: ");
      lcd.print(products[i].price, 2);
      lcd.print(" LE");
      return;
    }
  }

  lcd.setCursor(0, 0);
  lcd.print("Product not found");
}

void resetDisplay() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scan product ");
  lcd.setCursor(0, 1);
}

// ========== Ultrasonic Sensor Functions ==========
int readUltrasonic(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 15000); // timeout = 15ms
  return (duration <= 0) ? 999 : duration * 0.034 / 2;
}

// ========== Motor Control Functions ==========
void moveForward() {
  analogWrite(LEFT_MOTOR_PIN1, MAX_SPEED);
  digitalWrite(LEFT_MOTOR_PIN2, LOW);
  analogWrite(RIGHT_MOTOR_PIN1, MAX_SPEED);
  digitalWrite(RIGHT_MOTOR_PIN2, LOW);
}

void turnLeft() {
  analogWrite(LEFT_MOTOR_PIN1, LOW);
  analogWrite(LEFT_MOTOR_PIN2, MAX_SPEED);
  analogWrite(RIGHT_MOTOR_PIN1, MAX_SPEED);
  digitalWrite(RIGHT_MOTOR_PIN2, LOW);
}

void turnRight() {
  analogWrite(LEFT_MOTOR_PIN1, MAX_SPEED);
  digitalWrite(LEFT_MOTOR_PIN2, LOW);
  analogWrite(RIGHT_MOTOR_PIN1, LOW);
  analogWrite(RIGHT_MOTOR_PIN2, MAX_SPEED);
}

void stopMotors() {
  digitalWrite(LEFT_MOTOR_PIN1, LOW);
  digitalWrite(LEFT_MOTOR_PIN2, LOW);
  digitalWrite(RIGHT_MOTOR_PIN1, LOW);
  digitalWrite(RIGHT_MOTOR_PIN2, LOW);
}