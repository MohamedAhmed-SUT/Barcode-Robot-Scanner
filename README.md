# Barcode-Robot-Scanner

# Autonomous Robot with Barcode Scanner and Obstacle Avoidance

This Arduino-based project is a **smart autonomous robot** designed for **retail and warehouse assistance**. It combines **ultrasonic obstacle avoidance** with a **barcode scanner** and **LCD display** to identify products and show their details in real-time.

---

## 📦 Features

### 🤖 Autonomous Navigation
- Uses **3 ultrasonic sensors** (Front, Left, Right) to detect obstacles.
- Makes smart decisions to:
  - Move forward when the path is clear.
  - Turn left or right if a person is detected.
  - Stop when an obstacle is very close in front.

### 🧠 Intelligent Movement Logic
- Front obstacle detection between **30cm and 200cm**.
- Side detection range between **15cm and 40cm** with smart **side preference margin**.
- Prevents collisions and adjusts movement based on sensor inputs.

### 📷 Barcode Product Scanner
- Integrates **GM65 barcode scanner** (via Serial1).
- Reads EAN/UPC barcodes or custom strings.
- Looks up product in a predefined local database.

### 💡 LCD Display (I2C)
- Shows product **name** and **price** upon scanning.
- Displays “Product not found” if the barcode is not in the database.
- Clears the screen after a few seconds of inactivity (default: 3 seconds).

### 🧾 Product Database
- Local database of up to 14 sample products:
  - Chips, beverages, chocolate, dairy, etc.
- Easily extendable with more items.

---

## 🛠️ Components Used

| Component               | Description                        |
|------------------------|------------------------------------|
| Arduino Uno / Mega     | Main controller                    |
| GM65 Barcode Scanner   | Connected to `Serial1` (TX/RX)     |
| LCD 16x2 I2C           | For displaying product info        |
| 3x HC-SR04 Sensors     | Obstacle detection (Front, Left, Right) |
| L298N Motor Driver     | Motor control                      |
| DC Motors (x2)         | For movement                       |
| Jumper Wires, Battery  | Power and connections              |

---

## 🔌 Pin Configuration

### Ultrasonic Sensors:
| Sensor Position | Trigger Pin | Echo Pin |
|------------------|-------------|----------|
| Front (S1)       | D2          | D3       |
| Right (S2)       | D4          | D5       |
| Left (S3)        | D6          | D7       |

### Motors:
| Motor         | IN1 | IN2 |
|---------------|-----|-----|
| Left Motor    | D8  | D9  |
| Right Motor   | D10 | D11 |

### LCD:
- I2C Address: `0x27`
- Size: `16x2`

---

## 🧠 Code Behavior Summary

### Main Functions:
- `handleRobotMovement()`  
  Decides the robot’s movement based on ultrasonic sensor data.

- `handleBarcodeScanner()`  
  Reads the barcode from the GM65 and displays product info.

- `showProduct(barcode)`  
  Searches the barcode in the product array and displays the data.

- `resetDisplay()`  
  Clears the screen and resets message after timeout.

---

## 🚀 Getting Started

1. Connect all hardware as per the pin configuration.
2. Upload the Arduino sketch via Arduino IDE.
3. Power the Arduino and GM65 scanner.
4. Scan any barcode and observe product info on LCD.
5. Place obstacles to test robot navigation.


## 📸 Demo 

<img width="1080" height="1080" alt="image" src="https://github.com/user-attachments/assets/77d08014-0991-4d2a-8ab4-ef3e98ca1c0e" />

---

## 👨‍💻 Author

**Project by: Yousef Nasser**  
University Graduation Project | 2025

---

## 👨‍💻 Contributors

- **Mohamed Ahmed (Me)** – Contributed to software logic and implementation  
  *(Project 2 course, Graduation Team under Yousef Nasser – 2025)*

---

## 📃 License

This project is licensed under the MIT License – feel free to modify, enhance, and use it for learning or academic purposes.
