/*
  Arduino Code: Zonnekoffer Beroepenhuis SUN ENERGY LAB
  Extracted from manual pages 9-22 [cite: 209]

  This work is licensed under a Creative Commons Attribution 4.0 International License.
  http://creativecommons.org/licenses/by/4.0/
*/

#include <Wire.h> 
#include <LCD-I2C.h> // [cite: 211]

// Initialization of LCD
LCD_I2C lcd(0x27, 20, 4); // SCL and SDA on A4 and A5 [cite: 212]

// --- Pin Definitions ---
// Outputs for MOSFET gates controlling halogen lamps
#define Halogeen1 2  // [cite: 213]
#define Halogeen2 3  // [cite: 214]
#define Halogeen3 4  // [cite: 215]

// Inputs for time-of-day buttons
#define OCHTEND 5    // [cite: 216]
#define VOORMIDDAG 6 // [cite: 217]
#define MIDDAG 7     // [cite: 218]
#define NAMIDDAG 8   // [cite: 219]
#define AVOND 9      // [cite: 220]

// Inputs for load switches
#define SWITCHGSM 10    // GSM Charger [cite: 221]
#define SWITCHBOX 11    // Bluetooth Box [cite: 222]
#define SWITCHLAMP 12   // Incandescent Lamp [cite: 223]
#define SWITCHLAPTOP 13 // Laptop [cite: 224]

// --- Global Variables ---
bool powerSwitch1 = false;
bool powerSwitch2 = false;
bool powerSwitch3 = false;
bool powerSwitch4 = false; // [cite: 228]

bool ochtendAan = false;
bool voormiddagAan = false;
bool middagAan = false;
bool namiddagAan = false;
bool avondAan = false; // [cite: 233]

float spanning = 0;
float spanningOud; // Used to retain raw voltage for calculations [cite: 235]
int teller = 0;
float vermogen;
float vermogenOud;
float stroom;
int vermogenSwitch; // Switch to toggle between real and simulated values [cite: 240]

// Scaling factors for "simulated" mode
int factorspanning = 20; 
int factorstroom = 50; // [cite: 243, 244]

// --- Function: Read Inputs and Control Lamps ---
void ingangenLezen() { // 
  
  // 1. Morning Button
  if (!digitalRead(OCHTEND)) {
    delay(10);
    while (!digitalRead(OCHTEND)) delay(15); // Debounce
    
    if (!ochtendAan) {
      digitalWrite(Halogeen3, HIGH);
      digitalWrite(Halogeen2, LOW);
      digitalWrite(Halogeen1, LOW);
      ochtendAan = true;
      voormiddagAan = false; middagAan = false; namiddagAan = false; avondAan = false;
      delay(350);
    } else {
      digitalWrite(Halogeen3, LOW);
      digitalWrite(Halogeen2, LOW);
      digitalWrite(Halogeen1, LOW);
      ochtendAan = false;
    }
  }

  // 2. Late Morning Button
  if (!digitalRead(VOORMIDDAG)) { // [cite: 274]
    delay(10);
    while (!digitalRead(VOORMIDDAG)) delay(25);
    
    if (!voormiddagAan) {
      digitalWrite(Halogeen3, HIGH);
      digitalWrite(Halogeen2, HIGH);
      digitalWrite(Halogeen1, LOW);
      ochtendAan = false; voormiddagAan = true; middagAan = false; namiddagAan = false; avondAan = false;
      delay(350);
    } else {
      digitalWrite(Halogeen3, LOW);
      digitalWrite(Halogeen2, LOW);
      digitalWrite(Halogeen1, LOW);
      voormiddagAan = false; // Correction from source logic
    }
  }

  // 3. Noon Button
  if (!digitalRead(MIDDAG)) { // [cite: 299]
    delay(10);
    while (!digitalRead(MIDDAG)) delay(25);
    
    if (!middagAan) {
      digitalWrite(Halogeen3, HIGH);
      digitalWrite(Halogeen2, HIGH);
      digitalWrite(Halogeen1, HIGH);
      delay(300);
      ochtendAan = false; voormiddagAan = false; middagAan = true; namiddagAan = false; avondAan = false;
    } else {
      digitalWrite(Halogeen3, LOW);
      digitalWrite(Halogeen2, LOW);
      digitalWrite(Halogeen1, LOW);
      middagAan = false;
    }
  }

  // 4. Afternoon Button
  if (!digitalRead(NAMIDDAG)) { // [cite: 323]
    delay(10);
    while (!digitalRead(NAMIDDAG)) delay(25);
    
    if (!namiddagAan) {
      digitalWrite(Halogeen3, LOW);
      digitalWrite(Halogeen2, HIGH);
      digitalWrite(Halogeen1, HIGH);
      delay(300);
      ochtendAan = false; voormiddagAan = false; middagAan = false; namiddagAan = true; avondAan = false;
    } else {
      digitalWrite(Halogeen3, LOW);
      digitalWrite(Halogeen2, LOW);
      digitalWrite(Halogeen1, LOW);
      namiddagAan = false;
    }
  }

  // 5. Evening Button
  if (!digitalRead(AVOND)) { // [cite: 347]
    delay(10);
    while (!digitalRead(AVOND)) delay(25);
    
    if (!avondAan) {
      digitalWrite(Halogeen3, LOW);
      digitalWrite(Halogeen2, LOW);
      digitalWrite(Halogeen1, HIGH);
      delay(300);
      ochtendAan = false; voormiddagAan = false; middagAan = false; namiddagAan = false; avondAan = true;
    } else {
      digitalWrite(Halogeen3, LOW);
      digitalWrite(Halogeen2, LOW);
      digitalWrite(Halogeen1, LOW);
      avondAan = false;
    }
  }

  // Read Load Switches
  // Logic inverted: if High (5V), load is ON. Source uses '!powerSwitch' logic later.
  if (digitalRead(SWITCHGSM)) powerSwitch1 = false; else powerSwitch1 = true;     // [cite: 373]
  if (digitalRead(SWITCHBOX)) powerSwitch2 = false; else powerSwitch2 = true;     // [cite: 377]
  if (digitalRead(SWITCHLAMP)) powerSwitch3 = false; else powerSwitch3 = true;    // [cite: 380]
  if (digitalRead(SWITCHLAPTOP)) powerSwitch4 = false; else powerSwitch4 = true;  // [cite: 383]

  // Read Voltage from Solar Panels
  spanning = analogRead(A0); // [cite: 386]
  vermogenSwitch = analogRead(A1); // Read mode switch [cite: 372]
}

// --- Function: Calculate Current ---
// Calculates current based on voltage drop logic over specific resistors
float stroomMeting() { // [cite: 390]
  float drempelSpanning; 
  
  // Determine LED threshold voltage based on total voltage
  if (spanning > 3) drempelSpanning = 2;
  else if (spanning > 2.5) drempelSpanning = 1.9;
  else if (spanning > 2) drempelSpanning = 1.8;
  else if (spanning >= 1.7) drempelSpanning = 1.7; // [cite: 394-401]
  
  if (spanning < 1.7) drempelSpanning = spanning; // No current through LED if below threshold [cite: 402]

  // Calculate total current by summing current through active loads
  // Formula: (V - Vled)/R_series + V/R_parallel
  // Multiplied by 1000 to get mA
  return ((((spanning - drempelSpanning)/100) + spanning/1000) * 1000) * !powerSwitch1 + 
         ((((spanning - drempelSpanning)/100) + spanning/510) * 1000) * !powerSwitch2 + 
         ((((spanning - drempelSpanning)/100) + spanning/150) * 1000) * !powerSwitch3 + 
         ((((spanning - drempelSpanning)/100) + spanning/82) * 1000) * !powerSwitch4; // [cite: 426-427]
}

// --- Setup ---
void setup() { // [cite: 429]
  Wire.begin();
  lcd.begin(&Wire);
  lcd.cursorOff();
  lcd.blinkOff();
  lcd.display();
  lcd.backlight();
  
  pinMode(Halogeen1, OUTPUT);
  pinMode(Halogeen2, OUTPUT);
  pinMode(Halogeen3, OUTPUT);
  
  pinMode(OCHTEND, INPUT_PULLUP);
  pinMode(VOORMIDDAG, INPUT_PULLUP);
  pinMode(MIDDAG, INPUT_PULLUP);
  pinMode(NAMIDDAG, INPUT_PULLUP);
  pinMode(AVOND, INPUT_PULLUP);
  
  pinMode(SWITCHGSM, INPUT_PULLUP);
  pinMode(SWITCHBOX, INPUT_PULLUP);
  pinMode(SWITCHLAMP, INPUT_PULLUP);
  pinMode(SWITCHLAPTOP, INPUT_PULLUP);
  
  // Initialize lamps to OFF
  digitalWrite(Halogeen1, LOW);
  digitalWrite(Halogeen2, LOW);
  digitalWrite(Halogeen3, LOW); // [cite: 449-451]
}

// --- Main Loop ---
void loop() { // [cite: 453]
  ingangenLezen();
  
  // Convert analog reading to voltage (assuming 5V ref and voltage divider)
  spanning = (spanning * 5) / 1024; // [cite: 455]
  
  // Noise filtering for low voltage
  if (spanning < 0.15) spanning = 0; // [cite: 456]
  
  spanningOud = spanning; // Store raw voltage

  // MODE A: Simulated/Scaled Values (High Wattage display)
  if (vermogenSwitch < 50) { // [cite: 459]
    spanning = spanning * factorspanning;
    
    if (teller > 2000) { // Update display every 2000 cycles
      teller = 0;
      
      // 1. Display Voltage
      lcd.setCursor(0, 0);
      lcd.print("Spanning = ");
      lcd.print(spanning);
      if (spanning >= 10) lcd.print(" V"); else lcd.print(" V ");
      
      // Reset voltage to raw for current calculation
      spanning = spanningOud; 
      stroom = stroomMeting();
      stroom = (stroom * factorstroom) / 1000; // Convert to Amps
      
      // 2. Display Current
      lcd.setCursor(0, 1);
      lcd.print("Stroom = ");
      lcd.print(stroom);
      if (stroom >= 100) lcd.print(" A ");
      else if (stroom >= 10) lcd.print(" A  ");
      else lcd.print(" A   ");
      
      // 3. Display Power (P = U * I)
      spanning = spanning * factorspanning; // Restore scaled voltage
      vermogen = (spanning * stroom);
      
      lcd.setCursor(0, 2);
      lcd.print("Vermogen = ");
      lcd.print(vermogen, 1);
      if (vermogen < 10) lcd.print(" W  ");
      else if (vermogen < 100) lcd.print(" W ");
      else lcd.print(" W");
      
      // 4. Power Bar Graph
      int vermogenBlok;
      vermogen = vermogen / 5; // Scale for display blocks
      if (vermogen > 20) vermogen = 20; // Cap at 20 blocks
      vermogenBlok = int(vermogen);
      
      for (int i = 0; i < vermogenBlok; i++) {
        lcd.setCursor(i, 3);
        lcd.write(255); // Block character
      }
      for (int i = vermogenBlok; i < 20; i++) {
        lcd.setCursor(i, 3);
        lcd.print(" ");
      }
    }
  } 
  // MODE B: Real/Measured Values (milliWatts/milliAmps display)
  else { // [cite: 518]
    if (teller > 2000) {
      teller = 0;
      
      // 1. Display Voltage
      lcd.setCursor(0, 0);
      lcd.print("Spanning = ");
      lcd.print(spanning);
      if (spanning < 10) lcd.print(" V "); else lcd.print(" V");
      
      // 2. Display Current
      stroom = stroomMeting();
      lcd.setCursor(0, 1);
      lcd.print("Stroom = ");
      lcd.print(stroom);
      lcd.print(" mA ");
      
      // 3. Display Power
      vermogen = spanning * stroom;
      lcd.setCursor(0, 2);
      lcd.print("Vermogen = ");
      lcd.print(vermogen);
      if (vermogen < 10) lcd.print(" mW ");
      else if (vermogen < 100) lcd.print(" mW ");
      else lcd.print(" mW");
      
      // 4. Power Bar Graph
      int vermogenBlok;
      vermogen = vermogen / 5;
      if (vermogen > 20) vermogen = 20;
      vermogenBlok = int(vermogen);
      
      for (int i = 0; i < vermogenBlok; i++) {
        lcd.setCursor(i, 3);
        lcd.write(255);
      }
      for (int i = vermogenBlok; i < 20; i++) {
        lcd.setCursor(i, 3);
        lcd.print(" ");
      }
    }
  }
  teller++;
}