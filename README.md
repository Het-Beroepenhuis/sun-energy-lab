# SUN ENERGY LAB

**Manufacturer:** Schokarts BV  
**Model:** SUN ENERGY LAB  
**Date:** 17/12/2025

## License

This work is licensed under a [Creative Commons Attribution 4.0 International License](http://creativecommons.org/licenses/by/4.0/).

## 📖 Overview

The **Sun Energy Lab** is a demonstration kit designed to teach students the principles of solar energy. It allows users to investigate how solar panels function under different lighting conditions and electrical loads.

The unit features an internal **Arduino Uno** that controls the simulation lights (halogen), measures voltage from the panels and calculates the current and power drawn by the connected loads.

## 🛠 Hardware Specifications

* **Solar Array:** 4 small solar panels capable of Series and Parallel configurations
* **Sun Simulator:** 3x Halogen lamps (10W/12V) mounted on an aluminum stand to simulate sunlight intensity
* **Controller:** Arduino Uno (internal)
* **Power Supply:** Internal 230VAC to 12VDC (4.2A) power supply
* **Display:** LCD screen showing Voltage ($V$), Current ($A/mA$) and Power ($W/mW$)

### Simulated Loads
The suitcase includes 4 switchable dummy loads to simulate real-world usage :
1.  **GSM Charger:** ~10W
2.  **Bluetooth Box:** ~20W
3.  **Incandescent Lamp:** ~100W
4.  **Laptop:** ~200W

> **Note:** These loads are simulated using LEDs and resistors (100Ω, 510Ω, 150Ω, 82Ω) to teach power concepts safely .

## 💻 Software & Firmware

The system runs on an Arduino Sketch (`Zonnekoffer.ino`). The code manages the state machine for the "time of day" lights and performs the math for power calculations.

### Dependencies
* `Wire.h` (Standard Arduino Library) 
* `LCD-I2C.h` (Display driver) 

### Pin Configuration
The Arduino is wired as follows based on the internal source code :

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
Use the push buttons to simulate the sun's position. The Arduino controls the halogen lamps to vary intensity.

### 2. Panel Configuration
Connect the banana plugs in the cable tray to configure the 4 panels:
* **Series:** Increases Voltage ($V_{total} = V1 + V2 + ...$)
* **Parallel:** Increases Current ($I_{total} = I1 + I2 + ...$)

![Circuit Diagram](path/to/circuit_diagram.png)

### 3. Display Modes
A switch next to the display toggles between two measurement modes :
* **Correct Measurement:** Shows the actual, small measurements from the micro-panels.
* **Converted Measurement:** Displays scaled values (multiplied by factors) to simulate the output of a full-sized roof installation (e.g., displaying 200W instead of milliwatts).

## ⚠️ Safety & Compliance

* **Heat Warning:** The halogen lamps produce heat. Experiments should be limited to **max 15 minutes** to prevent overheating
* **Voltage:** The device connects to **230V mains**. The internal supply is shielded, and components are covered by plexiglass to prevent electric shock
* **Supervision:** Not intended as a toy. Use under adult supervision

### EU Conformity
This product complies with the following directives :
* **2014/35/EU:** Low Voltage Directive (LVD)
* **2014/30/EU:** EMC Directive
* **2011/65/EU:** RoHS Directive
