# Bill of Materials — Nano ESP32 Wordclock small PCB (V03)


| Qty | Component | Value / Spec | Designators |
|---|---|---|---|
| 1 | Arduino Nano ESP32 | — | main board |
| 1 | LDR | GL5516 | LDR |
| 1 | Rotary Encoder or Membrane keyboard | KY-034 | Rotary |
| 1 | RTC-module | DS3231 | RTC-clok |
| 1 | RGB(W) LED-strip | ES2812 or SK6812 | LED-strip |
| 1 | Resistor | 470 Ω, ±5% | RGB(W) LED-strip |
| 1 | Resistor | 10 kΩ, ±5% | LDR |
| 1 | Electrolytic capacitor | 1000 µF (1 mF), 16 V | Near Power |
| 1 | Diode | 1N5817 | near Power |
| 1 | IC | 74AHCT125 | Level shifter |
| 1 | Screw terminal, 2-pin, 3.5 mm pitch | — | Power |
| 2 | Female pin header, 15-pin, 0.1" | Connector for Arduino Nano ESP32 | Nano ESP32 |
| 1 | Female pin header, 6-pin, 0.1" | DS3231 RTC module (GND, 5V, SDA, SCL wired to Arduino I2C) | RTC-clock |
| 1 | Female pin header, 5-pin, 0.1" | Rotary encoder connector | Rotary |
| 1 | Female pin header, 2-pin, 0.1" | LDR (light sensor) connector | LDR |


## Notes 

- **Diodes** are labeled "1N5817" on the board, but the underlying Fritzing generic part's "part number" metadata field says "1N4001". These are different diode types (1N5817 is a Schottky, 1N4001 is a standard rectifier) 
— "1N4001" has a voltage drop op 0.7V, "1N5817" of 0.45V. The "1N5817" is preferred

