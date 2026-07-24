# Multi-Hazard Adaptive Window Protection System for Arid and Dust-Prone Regions
An intelligent, climate-responsive window automation system engineered specifically to combat local environmental challenges such as severe dust storms and extreme high ambient temperatures (exceeding $42^\circ\text{C}$). Designed with strict contest compliance in mind, the system operates on a single microcontroller and a unified power architecture.

---

## Key Features

* **Climate-Adaptive Automation:** Automatically detects particulate spikes (dust storms) via an MQ air quality sensor to seal the window and protect indoor spaces.
* **Contest Rule Compliant:** Built strictly around a **single microcontroller** (ESP32) and a **single power source** (3-cell 18650 battery pack) to satisfy all technical inspection requirements.
* **Optimized Mechanical Drive:** Utilizes an **Ultra-Short Crank Linkage** designed in Fusion 360, paired with a JGA25-370 gear motor.
* **Software Safety Safeguards:** Implements ESP32 PWM (Pulse Width Modulation) control to cap motor speed, preventing sudden physical shock loads and protecting the internal gearbox.

---

## Materials

| Component | Description | Function |
| --- | --- | --- |
| **Microcontroller** | ESP32 Development Board | Central processing unit and logic controller |
| **Motor Driver** | L298N Dual H-Bridge Driver | Handles motor power switching and onboard 5V regulation |
| **Actuator** | JGA25-370 DC Gear Motor (620 RPM) | Provides mechanical torque for window actuation |
| **Air Quality Sensor** | MQ Sensor (e.g., MQ-135) | Detects dust particles and environmental pollution |
| **Power Source** | 3x 18650 Li-Ion Cells (in a 4-cell holder) | Unified power supply providing ~11.1V nominal |

---

## Power Architecture

To maintain strict compliance with single-power-supply rules while protecting the microcontroller from voltage sags, the system uses a regulated shared-rail topology:

1. **Power Input:** The 3-cell 18650 battery pack connects directly to the **12V** and **GND** screw terminals on the L298N motor driver.
2. **Logic & Sensor Supply:** The L298N's onboard voltage regulator steps down the battery input, supplying a clean **5V** from its output terminal to the ESP32 and the MQ sensor VCC.
3. **Common Ground:** A dedicated ground bridge connects the L298N `GND` terminal back to the ESP32 `GND` pin.


## Mechanical Design 

The physical actuation relies on a **Direct-Drive Ultra-Short Crank Linkage**:

* **Motor Cradle:** A custom 3D-printed clamshell mount rigidly secures the 25mm JGA25-370 motor cylinder to the fixed window frame.
* **Crank Arm:** A minimized 1cm radius arm locks onto the motor's D-shaped output shaft, maximizing mechanical advantage and preventing motor stalling under load.
* **Push Rod:** Connects the crank pin to a pivoting bracket mounted on the moving window sash.
