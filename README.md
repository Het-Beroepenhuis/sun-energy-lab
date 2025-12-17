# SUN ENERGY LAB - Didactische Zonnekoffer

**Manufacturer:** Schokarts BV  
**Model:** SUN ENERGY LAB  
**Date:** 25/05/2025

## License

This work is licensed under a [Creative Commons Attribution 4.0 International License](http://creativecommons.org/licenses/by/4.0/).

## 📖 Overview

[cite_start]The **Didactische Zonnekoffer** (Educational Solar Suitcase) is a demonstration kit designed to teach students the principles of solar energy[cite: 10, 61]. [cite_start]It allows users to investigate how solar panels function under different lighting conditions and electrical loads[cite: 61, 62].

[cite_start]The unit features an internal **Arduino Uno** that controls the simulation lights (halogen), measures voltage from the panels, and calculates the current and power drawn by the connected loads[cite: 10, 161].

## 🛠 Hardware Specifications

* [cite_start]**Solar Array:** 4 small solar panels capable of Series and Parallel configurations[cite: 10, 77].
* [cite_start]**Sun Simulator:** 3x Halogen lamps (10W/12V) mounted on an aluminum stand to simulate sunlight intensity[cite: 10, 69].
* [cite_start]**Controller:** Arduino Uno (internal)[cite: 10].
* [cite_start]**Power Supply:** Internal 230VAC to 12VDC (4.2A) power supply[cite: 10].
* [cite_start]**Display:** LCD screen showing Voltage ($V$), Current ($A/mA$), and Power ($W/mW$)[cite: 74].

![Arduino Pinout Diagram](path/to/arduino_diagram.png)

### Simulated Loads
[cite_start]The suitcase includes 4 switchable dummy loads to simulate real-world usage [cite: 113-117]:
1.  **GSM Charger:** ~10W
2.  **Bluetooth Box:** ~20W
3.  **Incandescent Lamp:** ~100W
4.  **Laptop:** ~200W

> [cite_start]**Note:** These loads are simulated using LEDs and resistors (100Ω, 510Ω, 150Ω, 82Ω) to teach power concepts safely [cite: 150-160].

## 💻 Software & Firmware

The system runs on an Arduino Sketch (`Zonnekoffer.ino`). The code manages the state machine for the "time of day" lights and performs the math for power calculations.

### Dependencies
* [cite_start]`Wire.h` (Standard Arduino Library) [cite: 431]
* [cite_start]`LCD-I2C.h` (Display driver) [cite: 211]

### Pin Configuration
[cite_start]The Arduino is wired as follows based on the internal source code [cite: 213-224]:

| Pin | Type | Function |
| :--- | :--- | :--- |
| **D2** | Output | Halogen Lamp 1 Control |
| **D3** | Output | Halogen Lamp 2 Control |
| **D4** | Output | Halogen Lamp 3 Control |
| **D5** | Input | Button: Morning (Ochtend) |
| **D6** | Input | Button: Late Morning (Voormiddag) |
| **D7** | Input | Button: Noon (Middag) |
| **D8** | Input | Button: Afternoon (Namiddag) |
| **D9** | Input | Button: Evening (Avond) |
| **D10** | Input | Switch: GSM Charger |
| **D11** | Input | Switch: Bluetooth Box |
| **D12** | Input | Switch: Lamp |
| **D13** | Input | Switch: Laptop |
| **A0** | Analog | Solar Panel Voltage Measurement |
| **A1** | Analog | Measurement Mode Switch (Real vs. Scaled) |

## ⚙️ Usage Instructions

### 1. Simulating Sunlight
Use the push buttons to simulate the sun's position. The Arduino controls the halogen lamps to vary intensity:
* [cite_start]**Morning / Evening:** Low light intensity[cite: 257, 356].
* [cite_start]**Noon (Middag):** Maximum light intensity (all 3 spots on) [cite: 305-307].

### 2. Panel Configuration
Connect the banana plugs in the cable tray to configure the 4 panels:
* [cite_start]**Series:** Increases Voltage ($V_{total} = V1 + V2 + ...$)[cite: 99].
* [cite_start]**Parallel:** Increases Current ($I_{total} = I1 + I2 + ...$)[cite: 104].

![Circuit Diagram](path/to/circuit_diagram.png)

### 3. Display Modes
[cite_start]A switch next to the display toggles between two measurement modes [cite: 163-165]:
* **Correct Measurement:** Shows the actual, small measurements from the micro-panels.
* **Converted Measurement:** Displays scaled values (multiplied by factors) to simulate the output of a full-sized roof installation (e.g., displaying 200W instead of milliwatts).

## ⚠️ Safety & Compliance

* **Heat Warning:** The halogen lamps produce heat. [cite_start]Experiments should be limited to **max 15 minutes** to prevent overheating[cite: 39].
* **Voltage:** The device connects to **230V mains**. [cite_start]The internal supply is shielded, and components are covered by plexiglass to prevent electric shock[cite: 11, 26].
* **Supervision:** Not intended as a toy. [cite_start]Use under adult supervision[cite: 38].

### EU Conformity
[cite_start]This product complies with the following directives [cite: 12-15]:
* **2014/35/EU:** Low Voltage Directive (LVD)
* **2014/30/EU:** EMC Directive
* **2011/65/EU:** RoHS Directive